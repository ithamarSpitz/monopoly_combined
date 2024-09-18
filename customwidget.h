// customwidget.h
#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>
#include <QMap>
#include <QColor>
#include <QStringList>

class CustomWidget : public QWidget {
    Q_OBJECT

public:
    explicit CustomWidget(QWidget* parent = nullptr);

    void updatePlayerPositions(const QMap<int, QList<int>>& positions);
    void setSquareCount(int topBottom, int leftRight);
    void setTitles(const QStringList& titles);
    void setSquareColors(const QMap<int, QColor>& colors);
    void setSquareLetters(const QMap<int, QList<int>>& letterPlayerMap);
    void setPlayerDetails(const QString& name, const QString& color, int money, int cards);
    void setAllPlayersProperties(const QMap<QString, QStringList>& properties);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QMap<int, QList<int>> playerPositions;
    QStringList squareTitles;
    QMap<int, QColor> squareColors;
    QMap<int, QList<int>> letterPlayerMap;
    int topBottomSquares;
    int leftRightSquares;
    int squareSize;
    QString playerName;
    QString playerColor;
    int playerMoney;
    int playerCards;
    void drawPlayerDetails(QPainter& painter);
    void updateWidgetSize();
    QMap<QString, QStringList> allPlayersProperties;
    void drawAllPlayersProperties(QPainter& painter);
    void drawSquareWithBorder(QPainter& painter, const QRect& square, int index, const QColor& fillColor);
    QColor getPropertyOwnerColor(const QString& propertyName) const;

};

#endif // CUSTOMWIDGET_H
