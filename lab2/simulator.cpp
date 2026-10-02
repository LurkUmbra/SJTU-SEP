#include "simulator.h"

Simulator::Simulator() : pc(0) {
}

void Simulator::run(const int max_steps) {
    int step = 0;
    Status status = Status::AOK;
    for (step = 0; step < max_steps && status == Status::AOK; step++) {
        status = next_instruction();
    }
    report_stopped(step, status);
}

static uint8_t get_hi4(const uint8_t pack) { return (pack >> 4) & 0xF; }
static uint8_t get_lo4(const uint8_t pack) { return pack & 0xF; }

Status Simulator::next_instruction() {
    uint64_t next_pc = pc;

    // get code and function (1 byte)
    const std::optional<uint8_t> codefun = memory.get_byte(next_pc);
    if (!codefun) {
        report_bad_inst_addr();
        return Status::ADR;
    }
    const auto icode = static_cast<InstructionCode>(get_hi4(codefun.value()));
    uint8_t ifun = get_lo4(codefun.value());
    next_pc++;

    // execute the instruction
    switch (icode) {
        case InstructionCode::HALT: // 0:0
        {
            return Status::HLT;
        }
        case InstructionCode::NOP: // 1:0
        {
            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::RRMOVQ: // 2:x regA:regB
        {
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (error_invalid_reg(reg_b)) return Status::INS;
            next_pc++;

            if (cc.satisfy(static_cast<Condition>(ifun))) {
                registers[reg_b] = registers[reg_a];
            }
            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::IRMOVQ: // 3:0 F:regB imm
        {
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (reg_a != 0xF || error_invalid_reg(reg_b)) return Status::INS;
            next_pc++;

            const std::optional<uint64_t> imm = memory.get_long(next_pc);
            if (!imm.has_value()) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            registers[reg_b] = imm.value();
            pc = next_pc + 8;
            return Status::AOK;
        }
        case InstructionCode::RMMOVQ: // 4:0 regA:regB imm
        {
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (error_invalid_reg(reg_b)) return Status::INS;
            next_pc++;

            const std::optional<uint64_t> imm = memory.get_long(next_pc);
            if (!imm.has_value()) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            uint64_t addr = registers[reg_b] + imm.value();
            if (!memory.set_long(addr, registers[reg_a])) {
                report_bad_data_addr(addr);
                return Status::ADR;
            }
            pc = next_pc + 8;
            return Status::AOK;
        }
        case InstructionCode::MRMOVQ: // 5:0 regA:regB imm
        {
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if(!regs.has_value()) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (error_invalid_reg(reg_b)) return Status::INS;
            next_pc++;

            const std::optional<uint64_t> imm = memory.get_long(next_pc);
            if (!imm.has_value()) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            const uint64_t addr = registers[reg_b] + imm.value();
            auto val = memory.get_long(addr);
            if (!val.has_value()) {
                report_bad_data_addr(addr);
                return Status::ADR;
            }
            registers[reg_a] = val.value();
            pc = next_pc + 8;
            return Status::AOK;
        }
        case InstructionCode::ALU: // 6:x regA:regB
        {
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs.has_value()) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (error_invalid_reg(reg_b)) return Status::INS;
            next_pc++;
            
            const auto old_reg_a = registers[reg_a];
            const auto old_reg_b = registers[reg_b];
            const auto op = static_cast<AluOp>(ifun);
            switch (op) {
                case AluOp::ADD:
                    registers[reg_b] += registers[reg_a];
                    break;
                case AluOp::SUB:
                    registers[reg_b] -= registers[reg_a];
                    break;
                case AluOp::AND:
                    registers[reg_b] &= registers[reg_a];
                    break;
                case AluOp::XOR:
                    registers[reg_b] ^= registers[reg_a];
                    break;
                default:
                report_bad_inst(codefun.value());
                return Status::INS;
            }
            
            cc = ConditionCodes::compute(op, old_reg_a, old_reg_b, registers[reg_b]);
            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::JMP: // 7:x imm
        {
            if(cc.satisfy(static_cast<Condition>(ifun))) {
                const std::optional<uint64_t> dest = memory.get_long(next_pc);
                if (!dest.has_value()) {
                    report_bad_inst_addr();
                    return Status::ADR;
                }
                pc = dest.value();
                return Status::AOK;
            }
            pc = next_pc + 8;
            return Status::AOK;
        }
        case InstructionCode::CALL: // 8:0 imm
        {
            const std::optional<uint64_t> dest = memory.get_long(next_pc);
            if (!dest.has_value()) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            next_pc += 8;
            if (!memory.set_long(registers[RegId::RSP] - 8, next_pc)) {
                report_bad_stack_addr(registers[RegId::RSP] - 8);
                return Status::ADR;
            }
            registers[RegId::RSP] -= 8;
            pc = dest.value();
            return Status::AOK;
        }
        case InstructionCode::RET: // 9:0
        {
            auto val = memory.get_long(registers[RegId::RSP]);
            if (!val.has_value()) {
                report_bad_stack_addr(registers[RegId::RSP]);
                return Status::ADR;
            }
            pc = val.value();
            registers[RegId::RSP] += 8;
            return Status::AOK;
        }
        case InstructionCode::PUSHQ: // A:0 regA:F
        {
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs.has_value()) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (reg_b != 0xF) return Status::INS;
            next_pc++;

            if (Registers::name(reg_a) == "%rsp") {
                if (!memory.set_long(registers[RegId::RSP] - 8, registers[RegId::RSP])) {
                    report_bad_stack_addr(registers[RegId::RSP] - 8);
                    return Status::ADR;
                }
                registers[RegId::RSP] -= 8;
            } else {
                registers[RegId::RSP] -= 8;
                if (!memory.set_long(registers[RegId::RSP], registers[reg_a])) {
                    report_bad_stack_addr(registers[RegId::RSP]);
                    return Status::ADR;
                }
            }
            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::POPQ: // B:0 regA:F
        {
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs.has_value()) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (reg_b != 0xF) return Status::INS;
            next_pc++;

            const std::optional<uint64_t> val = memory.get_long(registers[RegId::RSP]);
            if (!val.has_value()) {
                report_bad_stack_addr(registers[RegId::RSP]);
                return Status::ADR;
            }
            registers[reg_a] = val.value();
            if (Registers::name(reg_a) != "%rsp") {
                registers[RegId::RSP] += 8;
            }
            
            pc = next_pc;
            return Status::AOK;
        }
        default:
            report_bad_inst(codefun.value());
            return Status::INS;
    }
}
