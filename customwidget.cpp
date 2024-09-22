// customwidget.cpp
#include "customwidget.h"
#include <QPainter>
#include <QTextOption>
#include <QDebug>

// Constructor initializes the widget with default values
CustomWidget::CustomWidget(int topbottom, int leftright, QWidget* parent)
    : QWidget(parent), topBottomSquares(topbottom), leftRightSquares(leftright), squareSize(2*(topbottom+leftright)) {
    updateWidgetSize();
}

// Updates the widget size based on the number of squares and square size
void CustomWidget::updateWidgetSize() {
    int boardWidth = topBottomSquares * squareSize * 1.20;
    int boardHeight = (leftRightSquares + 2) * squareSize * 1.20;
    int playerDetailsWidth = 200; // Adjust this value as needed
    setFixedSize(boardWidth + playerDetailsWidth, boardHeight);
}

// Sets the number of squares on each side of the board
void CustomWidget::setSquareCount(int topBottom, int leftRight) {
    topBottomSquares = topBottom;
    leftRightSquares = leftRight;
    updateWidgetSize();
    update();
}

// Sets the titles for each square on the board
void CustomWidget::setTitles(const QStringList& titles) {
    squareTitles = titles;
    update();
}

// Sets the colors for specific squares on the board
void CustomWidget::setSquareColors(const QMap<int, QColor>& colors) {
    squareColors = colors;
    update();
}

// Sets the letters and player information for specific squares
void CustomWidget::setSquareLetters(const QMap<int, QList<int>>& letterPlayerMap) {
    this->letterPlayerMap = letterPlayerMap;
    update();
}

// Updates the positions of players on the board
void CustomWidget::updatePlayerPositions(const QMap<int, QList<int>>& positions) {
    playerPositions = positions;
    update();
}

// Sets the details for the current player
void CustomWidget::setPlayerDetails(const QString& name, const QString& color, int money, int cards) {
    playerName = name;
    playerColor = color;
    playerMoney = money;
    playerCards = cards;
    update();
}

// Draws the current player's details on the board
void CustomWidget::drawPlayerDetails(QPainter& painter) {
    int boardWidth = topBottomSquares * squareSize * 1.20;
    int leftMargin = boardWidth + 10;
    int topMargin = 10;
    int lineHeight = 20;

    painter.save();

    // Set up the painter for drawing player details
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 10));

    int detailsWidth = width() - boardWidth - 20;

    // Draw a semi-transparent background for player details
    painter.fillRect(leftMargin, topMargin, 
                     detailsWidth, 4 * lineHeight + 10, 
                     QColor(255, 255, 255, 200));

    // Draw player details text
    painter.drawText(leftMargin, topMargin + lineHeight, "Player: " + playerName);
    painter.drawText(leftMargin, topMargin + 2*lineHeight, "Color: " + playerColor);
    painter.drawText(leftMargin, topMargin + 3*lineHeight, "Money: $" + QString::number(playerMoney));
    painter.drawText(leftMargin, topMargin + 4*lineHeight, "Get out of jail cards: " + QString::number(playerCards));

    painter.restore();
}

// Sets the properties owned by all players
void CustomWidget::setAllPlayersProperties(const QMap<QString, QStringList>& properties) {
    allPlayersProperties = properties;
    update();
}

// Draws the properties owned by all players
void CustomWidget::drawAllPlayersProperties(QPainter& painter) {
    int boardWidth = topBottomSquares * squareSize * 1.20;
    int leftMargin = boardWidth + 10;
    int topMargin = 130; // Position below player details
    int lineHeight = 20;
    int propertyWidth = 200;

    painter.save();

    // Set up the painter for drawing property details
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 10));

    int yOffset = topMargin;

    // Only draw properties for the current player
    if (allPlayersProperties.contains(playerName)) {
        painter.drawText(leftMargin, yOffset, playerName + "'s Properties:");
        yOffset += lineHeight;

        const QStringList& properties = allPlayersProperties[playerName];
        
        // Display properties or "No properties" message
        if (properties.isEmpty()) {
            painter.drawText(leftMargin, yOffset, "No properties");
            yOffset += lineHeight;
        } else {
            for (const QString& property : properties) {
                painter.drawText(leftMargin, yOffset, propertyWidth, lineHeight, Qt::AlignLeft | Qt::AlignVCenter, property);
                yOffset += lineHeight;
            }
        }
    } else {
        painter.drawText(leftMargin, yOffset, "No player properties to display");
    }

    painter.restore();
}

// Returns the color associated with the owner of a property
QColor CustomWidget::getPropertyOwnerColor(const QString& propertyName) const {
    static const QColor playerColors[8] = {
        Qt::red, Qt::blue, Qt::yellow, Qt::cyan,
        Qt::magenta, Qt::gray, Qt::darkGreen, Qt::darkRed
    };

    // Search for the property owner and return their color
    for (auto it = allPlayersProperties.constBegin(); it != allPlayersProperties.constEnd(); ++it) {
        if (it.value().contains(propertyName)) {
            // Find the player's index based on their name
            int playerIndex = it.key().back().toLatin1()-'0';
            if (playerIndex != -1) {
                return playerColors[playerIndex];
            }
        }
    }
    return Qt::black;  // Default color if the property is not owned
}

// Draws a square with a colored border based on property ownership
void CustomWidget::drawSquareWithBorder(QPainter& painter, const QRect& square, int index, const QColor& fillColor) {
    // Draw the fill
    painter.setBrush(fillColor);
    painter.setPen(Qt::NoPen);
    painter.drawRect(square);

    // Draw the border
    QString propertyName = (index < squareTitles.size()) ? squareTitles[index] : QString();
    QColor borderColor = getPropertyOwnerColor(propertyName);
    
    painter.setPen(QPen(borderColor, 2));  // Set border width to 2
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(square);
}

// Main painting event for the widget
void CustomWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    // Define colors for player tokens
    QColor circleColors[8] = {
        Qt::red, Qt::blue, Qt::yellow, Qt::cyan,
        Qt::magenta, Qt::gray, Qt::darkGreen, Qt::darkRed
    };

    int circleRadius = 4;
    QFont font = painter.font();
    font.setPointSize(7);
    painter.setFont(font);
        
    QTextOption textOption;
    textOption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    textOption.setAlignment(Qt::AlignLeft);

    // Draw top and bottom rows of squares
    for (int i = 0; i < topBottomSquares; ++i) {
        int xOffset = i * squareSize * 1.20;
        int yOffset = 0;
        QRect topSquare(xOffset, yOffset, squareSize * 1.20, squareSize * 1.20);
        drawSquareWithBorder(painter, topSquare, i, squareColors.value(i, Qt::white));

        // Draw square title
        if (i < squareTitles.size()) {
            QRect titleRect(xOffset + 1, yOffset + 1, squareSize * 1.20 - 2, squareSize * 1.20 - 2);
            painter.drawText(titleRect, squareTitles[i], textOption);
        }

        // Draw bottom row square
        yOffset = (leftRightSquares + 1) * squareSize * 1.20;
        QRect bottomSquare(xOffset, yOffset, squareSize * 1.20, squareSize * 1.20);
        int bottomSquareIndex = 2*topBottomSquares + leftRightSquares - i - 1;
        drawSquareWithBorder(painter, bottomSquare, bottomSquareIndex, squareColors.value(bottomSquareIndex, Qt::white));

        // Draw bottom square title
        if (bottomSquareIndex < squareTitles.size()) {
            QRect titleRect(xOffset + 1, yOffset + 1, squareSize * 1.20 - 2, squareSize * 1.20 - 2);
            painter.drawText(titleRect, squareTitles[bottomSquareIndex], textOption);
        }
    }

    // Draw left and right columns of squares
    for (int i = 0; i < leftRightSquares; ++i) {
        int xOffset = 0;
        int yOffset = (i + 1) * squareSize * 1.20;
        QRect leftSquare(xOffset, yOffset, squareSize * 1.20, squareSize * 1.20);
        int leftSquareIndex = 2 * topBottomSquares + 2 * leftRightSquares - i - 1;
        drawSquareWithBorder(painter, leftSquare, leftSquareIndex, squareColors.value(leftSquareIndex, Qt::white));

        // Draw left square title
        if (leftSquareIndex < squareTitles.size()) {
            QRect titleRect(xOffset + 1, yOffset + 1, squareSize * 1.20 - 2, squareSize * 1.20 - 2);
            painter.drawText(titleRect, squareTitles[leftSquareIndex], textOption);
        }

        // Draw right column square
        int xOffsetRight = (topBottomSquares - 1) * squareSize * 1.20;
        QRect rightSquare(xOffsetRight, yOffset, squareSize * 1.20, squareSize * 1.20);
        int rightSquareIndex = topBottomSquares + i;
        drawSquareWithBorder(painter, rightSquare, rightSquareIndex, squareColors.value(rightSquareIndex, Qt::white));

        // Draw right square title
        if (rightSquareIndex < squareTitles.size()) {
            QRect titleRect(xOffsetRight + 1, yOffset + 1, squareSize * 1.20 - 2, squareSize * 1.20 - 2);
            painter.drawText(titleRect, squareTitles[rightSquareIndex], textOption);
        }
    }

    // Draw player tokens on the board
    for (auto it = playerPositions.begin(); it != playerPositions.end(); ++it) {
        int squareIndex = it.key();
        QList<int> players = it.value();
        int numPlayers = players.size();
        int xOffset, yOffset;
        bool isBottomRow = false;

        // Skip invalid square indices
        if (squareIndex < 0 || squareIndex >= (2 * topBottomSquares + 2 * leftRightSquares)) {
            qDebug() << "Invalid square index:" << squareIndex;
            continue;
        }

        // Calculate token position based on square index
        if (squareIndex < topBottomSquares) {
            // Top row
            xOffset = squareIndex * squareSize * 1.20;
            yOffset = 0;
        } else if (squareIndex < topBottomSquares + leftRightSquares) {
            // Right column
            xOffset = (topBottomSquares - 1) * squareSize * 1.20;
            yOffset = (squareIndex - topBottomSquares + 1) * squareSize * 1.20;
        } else if (squareIndex < 2 * topBottomSquares + leftRightSquares) {
            // Bottom row
            isBottomRow = true;
            xOffset = (2 * topBottomSquares + leftRightSquares - squareIndex - 1) * squareSize * 1.20;
            yOffset = (leftRightSquares + 1) * squareSize * 1.20;
        } else {
            // Left column
            xOffset = 0;
            yOffset = (2 * topBottomSquares + 2 * leftRightSquares - squareIndex) * squareSize * 1.20;
        }

        int circleXOffset = xOffset + 5 * 1.20;
        int circleYOffset = yOffset + 26 * 1.20;

        // Draw player tokens
        for (int j = 0; j < numPlayers; ++j) {
            int playerID = players[j];
            if (playerID >= 0 && playerID < 8) {
                painter.setBrush(circleColors[playerID]);
                painter.setPen(Qt::black);

                int row = j % 2;
                int col = j / 2;
                int x, y;

                // Adjust token position for bottom row
                if (isBottomRow) {
                    x = circleXOffset + (1 - col) * (circleRadius + 4) * 1.20;
                } else {
                    x = circleXOffset + col * (circleRadius + 4) * 1.20;
                }

                y = circleYOffset + row * (circleRadius * 2 + 1) * 1.20;
                painter.drawEllipse(QPoint(x, y), circleRadius, circleRadius);
            }
        }
    }

    // Draw letters in the bottom-right corners of the squares
    QFont letterFont = painter.font();
    letterFont.setPointSize(10);
    painter.setFont(letterFont);
    QColor black(Qt::black);

    // Lambda function to draw square information (houses/hotels)
    auto drawSquareInfo = [&](const QRect& square, int index) {
        if (letterPlayerMap.contains(index)) {
            int colornum = letterPlayerMap[index][0];
            int numHouses = letterPlayerMap[index][1];
            QColor playerColor = colornum == -1 ? black : circleColors[colornum+1];
            painter.setPen(playerColor);

            if (numHouses != 0) {
                QString displayText = (numHouses == 5) ? "H" : QString::number(numHouses);
                painter.drawText(square.bottomRight() - QPoint(10, 10), displayText);
            }
        }
    };

    // Draw top and bottom rows square information
    for (int i = 0; i < topBottomSquares; ++i) {
        // Top row
        int xOffset = i * squareSize * 1.20;
        int yOffset = 0;
        QRect topSquare(xOffset, yOffset, squareSize * 1.20, squareSize * 1.20);
        drawSquareInfo(topSquare, i);

        // Bottom row
        yOffset = (leftRightSquares + 1) * squareSize * 1.20;
        QRect bottomSquare(xOffset, yOffset, squareSize * 1.20, squareSize * 1.20);
        int bottomSquareIndex = 2*topBottomSquares + leftRightSquares - i - 1;
        drawSquareInfo(bottomSquare, bottomSquareIndex);
    }

    // Draw left and right columns square information
    for (int i = 0; i < leftRightSquares; ++i) {
        // Left column
        int xOffset = 0;
        int yOffset = (i + 1) * squareSize * 1.20;
        QRect leftSquare(xOffset, yOffset, squareSize * 1.20, squareSize * 1.20);
        int leftSquareIndex = 2 * topBottomSquares + 2 * leftRightSquares - i - 1;
        drawSquareInfo(leftSquare, leftSquareIndex);

        // Right column
        int xOffsetRight = (topBottomSquares - 1) * squareSize * 1.20;
        QRect rightSquare(xOffsetRight, yOffset, squareSize * 1.20, squareSize * 1.20);
        int rightSquareIndex = topBottomSquares + i;
        drawSquareInfo(rightSquare, rightSquareIndex);
    }

    // Draw player details
    drawPlayerDetails(painter);

    // Draw all players' properties
    drawAllPlayersProperties(painter);
}