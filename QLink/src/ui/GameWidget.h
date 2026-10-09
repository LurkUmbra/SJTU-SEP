#pragma once

#include <QWidget>

#include "Board.h"

class GameWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Board board_;
    int cellSize_;

    void drawTile(QPainter &painter, int row, int col, TileType type) const;
};
