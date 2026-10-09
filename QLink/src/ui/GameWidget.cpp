#include "GameWidget.h"

#include <QPainter>

namespace {
constexpr int kDefaultRows = 10;
constexpr int kDefaultCols = 12;
constexpr int kCellSize = 48;
}

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent),
      board_(kDefaultRows, kDefaultCols),
      cellSize_(kCellSize) {
    board_.generateRandom();
    setFixedSize(board_.cols() * cellSize_, board_.rows() * cellSize_);
}

QSize GameWidget::sizeHint() const {
    return QSize(board_.cols() * cellSize_, board_.rows() * cellSize_);
}

void GameWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    for (int row = 0; row < board_.rows(); ++row) {
        for (int col = 0; col < board_.cols(); ++col) {
            drawTile(painter, row, col, board_.at(row, col));
        }
    }
}

void GameWidget::drawTile(QPainter &painter, int row, int col, TileType type) const {
    const QRect cellRect(col * cellSize_, row * cellSize_, cellSize_, cellSize_);

    QColor color;
    switch (type) {
    case TileType::Ruby:      color = QColor(220, 60, 60); break;
    case TileType::Sapphire:  color = QColor(60, 120, 220); break;
    case TileType::Emerald:   color = QColor(60, 180, 90); break;
    case TileType::Topaz:     color = QColor(230, 200, 60); break;
    case TileType::Empty:
    default:                  color = QColor(40, 40, 40); break;
    }

    painter.fillRect(cellRect.adjusted(2, 2, -2, -2), color);
}
