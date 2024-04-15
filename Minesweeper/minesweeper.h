/*
Author: Anna Ma
Description: Minesweeper grid header file that uses instances of Button class to create a 2D array game grid and handle user input and game logic.
Date: Feb. 16, 2024
Class: CS3307 Individual Assignment
*/

#ifndef MINESWEEPERGRID_H
#define MINESWEEPERGRID_H

#include <QWidget>
#include <QGridLayout>
#include <QVector>
#include "button.h"

class MinesweeperGrid : public QWidget {
    Q_OBJECT
public:
    MinesweeperGrid(QWidget *parent = nullptr);

private slots:
    void handleLeftClick(int row, int col);
    void handleRightClick(int row, int col);

private:
    const int TOTALROWS = 16;
    const int TOTALCOLS = 30;
    QGridLayout *gameGrid;
    QVector<QVector<Button*>> gameGridArray; // 2D array of buttons for minesweeper grid 
    void revealButton(int row, int col); // reveal left-clicked button if there is no bomb 
    void createGrid(); // create game grid
    void setMines(); // assign mines to 99 buttons in grid
    void userWonPopup(); // window shown to winner
    void userLostPopup(); // winner shown to loser

};

#endif // MINESWEEPERGRID_H