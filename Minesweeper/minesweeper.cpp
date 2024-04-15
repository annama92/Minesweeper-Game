/*
Author: Anna Ma
Description: Minesweeper grid class that uses instances of Button class to create a game grid and handle user input and game logic.
Date: Feb. 16, 2024
Class: CS3307 Individual Assignment
*/

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QIcon>
#include <iostream>
#include <QDebug>
#include <QRandomGenerator>
#include <QDateTime>
#include <QMessageBox>
#include <QString>
#include <QMessageBox>
#include <QProcess>

#include "minesweeper.h"

/*
Name: MinesweeperGrid
Description: Constructor method for the grid that calls 2 other methods. 
createGrid() adds button instances to the overall grid layout.
setMines() srandomly sets 99 mines in the grid in 99 unique button instances.
*/
MinesweeperGrid::MinesweeperGrid(QWidget *parent) : QWidget(parent) {
    gameGrid = new QGridLayout(this);
    createGrid();
    setMines();
}

/*
Name: handleLeftClick
Description: when the user left clicks a button, the button object is acted open accordingly (either reveal hint or hit bomb)
Parameters: row is the row number of the button that is clicked on, col is the column number
*/
void MinesweeperGrid::handleLeftClick(int row, int col)
{
    Button *currentButton = gameGridArray[row][col];
    bool hasUserWon = true; // used to check if user clicked all non-bomb buttons in grid

    if (!currentButton->isMineSpace()) // if the button that is clicked on does not have a mine
    {
        revealButton(row, col); // calls function that either reveals hint if there are adjacent bombs, or makes button invisible if there are no adj mines
        
        if (currentButton->getNumAdjMines() == 0) {  // If the button has no adjacent mines, recursively reveal adjacent buttons (which are either hints or also invisible buttons with no adj mines)
            for (int changeRowIndexBy = -1; changeRowIndexBy <= 1; ++changeRowIndexBy) { // we want to check indices [row-1][current col] and [row+1][current col]
                for (int changeColIndexBy = -1; changeColIndexBy <= 1; ++changeColIndexBy) { // we want to check indices [current row][col-1] and [current row][col+1] 
                    int adjRowIndex = changeRowIndexBy + row; // check rows 1 above and 1 below current row
                    int adjColIndex = changeColIndexBy + col; // check column 1 above and 1 below current column
                    
                    // check to make sure we're not checking adjacent buttons that are out of bounds of gameGridArray 2D array
                    if (adjRowIndex < TOTALROWS && adjRowIndex > -1) { // check that new row we're trying to access is not below 0 or above 16
                        if (adjColIndex < TOTALCOLS && adjColIndex > -1) {
                            Button *adjacentButton = gameGridArray[adjRowIndex][adjColIndex];
                            if (!adjacentButton->isUncovered()) { // if the adjacent button is not already left-clicked on, which ensures that it's not a bomb, left-click it again automatically to reveal hint
                                handleLeftClick(adjRowIndex, adjColIndex); // recursive call
                            }
                        }
                    } 
                }
            }
        }

        for(int i = 0; i < TOTALROWS; i++){ // iterate through all buttons in grid
            for(int j = 0; j < TOTALCOLS; j++){
                if(!gameGridArray[i][j]->isUncovered() && !gameGridArray[i][j]->isMineSpace()) { // if there are still buttons without bombs that haven't been left-clicked by user, user has not yet won
                    hasUserWon = false;
                }
            }
        }

        if (hasUserWon == true) { // if user won, call function that will display winner pop-up
            userWonPopup();
        }
        
    } else { // if the user clicked on a button with a mine
        for (int i=0; i < TOTALROWS; i++) { // reveal all bombs on the grid
            for (int j=0; j < TOTALCOLS; j++) {
                Button *otherButtons = gameGridArray[i][j];
                if (otherButtons->isMineSpace()) {
                    otherButtons->setText(""); // clear previous text
                    otherButtons->setIcon(QIcon(":/minesweeper_icons/bomb.png")); // set grey bomb image
                    otherButtons->setIconSize(QSize(35, 35)); // resize so it's the same size as the buttons
                }
            }
        }
        currentButton->setText(""); //set the bomb that was clicked on to be the red bomb_explode.png image
        currentButton->setIcon(QIcon(":/minesweeper_icons/bomb_explode.png"));
        currentButton->setIconSize(QSize(35, 35)); 
        userLostPopup(); //player lost, ask player in a pop up if they want to play again
    }
}

/*
Name: revealButton
Description: when the user left clicks a button, this function either reveals hint of # of adjacent bombs, or makes button invisible 
Parameters: row is the row number of the button that is clicked on, col is the column number
*/
void MinesweeperGrid::revealButton(int row, int col) {
    Button *currentButton = gameGridArray[row][col];
    currentButton->setUncovered(true); // make sure current button is uncovered in its properties

    if (currentButton->getNumAdjMines() >= 1) { // if the cell has adjacent mines, reveal hint (i.e. count of adjacent mines)
        currentButton->setIcon(QIcon()); // clear previous icons (eg. if it was marked with a flag)
        currentButton->setText(QString::number(currentButton->getNumAdjMines())); // set text to be # of adjacent mines to that button
    } else { // if the cell has no adjacent mines, recursively reveal adjacent cells with hints
        currentButton->setText(""); // clear text
        currentButton->setIcon(QIcon()); // clear icons
        currentButton->setFlat(true); // set button to be invisible without resizing buttons/grid
    }
}

/*
Name: handleRightClick
Description: when the user right clicks a button, the button is either flagged, marked with question mark, or reset
Parameters: row is the row number of the button that is clicked on, col is the column number
*/
void MinesweeperGrid::handleRightClick(int row, int col)
{
    Button *currentButton = gameGridArray[row][col];

    if (!currentButton) {
        qDebug() << "Error: Sender is not a Button object";
        return;
    }

    if (!currentButton->isFlagged() && !currentButton->isQuestionMark()) // if the button has not been right-clicked yet and has no flag or question mark
    {
        currentButton->setText(""); // clear previous text such as # of adjacent bombs text
        currentButton->setIcon(QIcon(":/minesweeper_icons/mine_flag.png"));
        currentButton->setIconSize(QSize(35, 35)); 
        currentButton->setFlag(true); // mark with flag
    } else if (currentButton->isFlagged()) { // if the button is already flagged
        currentButton->setIcon(QIcon()); // clear flag icon
        currentButton->setText("?"); // mark with ?
        currentButton->setFlag(false);
        currentButton->setQuestionMark(true); // mark with question mark
    } else if (currentButton->isQuestionMark()) { // if the button is already question marked
        currentButton->setText(""); // clear previous text aka question mark
        currentButton->setQuestionMark(false); // reset button
    }
}

/*
Name: createGrid
Description: creates 2D array grid for minesweeper game with 16 rows, 30 columns, and a button for each index of the grid
*/
void MinesweeperGrid::createGrid() {
    gameGridArray.resize(TOTALROWS);
    for (int i = 0; i < TOTALROWS; ++i) {
        gameGridArray[i].resize(TOTALCOLS);
        for (int j = 0; j < TOTALCOLS; ++j) {
            Button *button = new Button(i, j, this);
            gameGrid->addWidget(button, i, j);
            gameGridArray[i][j] = button; // add new button to the current index of the 2D array game grid
            connect(button, &Button::leftClicked, this, &MinesweeperGrid::handleLeftClick); // if button recieves left click signal, call handleLeftClick function
            connect(button, &Button::rightClicked, this, &MinesweeperGrid::handleRightClick);
        } 
    }
}

/*
Name: setMines
Description: assigns 99 mines to random, unique buttons within the grid, and changes button property to reflect it.
*/
void MinesweeperGrid::setMines() {
    QRandomGenerator generator;
    generator.seed(QDateTime::currentMSecsSinceEpoch()); // seed with current time so that different buttons are randomly assigned bombs each time the game restarts

    // place 99 mines
    int mineCounter = 0; 
    while (mineCounter < 99) { // keep adding mines until there are 99 mines placed
        int row = generator.bounded(TOTALROWS); // random row index
        int col = generator.bounded(TOTALCOLS); // random column index

        Button *button = gameGridArray[row][col]; // get button at position generated
        if (!button->isMineSpace()) { // if button doesn't have mine
            mineCounter++; // increase mine counter
            button->setMineSpace(true); // set button as having a mine
        }
    }

    // calculates number of mines that are adjacent to each button cell and store as button property
    for (int i = 0; i < TOTALROWS; ++i) {
        for (int j = 0; j < TOTALCOLS; ++j) {
            Button *currentButton = gameGridArray[i][j];
            int count = 0;

            for (int changeRowIndexBy = -1; changeRowIndexBy <= 1; ++changeRowIndexBy) {
                for (int changeColIndexBy = -1; changeColIndexBy <= 1; ++changeColIndexBy) {
                    int adjRowIndex = i + changeRowIndexBy;
                    int adjColIndex = j + changeColIndexBy;
                    if (adjRowIndex >= 0 && adjRowIndex < TOTALROWS && adjColIndex >= 0 && adjColIndex < TOTALCOLS) {
                        if (gameGridArray[adjRowIndex][adjColIndex]->isMineSpace()) {
                            count++; // if adjacent cells have mines, increment count
                        }
                    }
                }
            }
            currentButton->setNumAdjMines(count); // current button has the number of adjacment mines counted earlier
        }
    }
}

/*
Name: userWonPopup
Description: if the user wins, they are shown a window that congratulates them and asks them if they want to play again
*/
void MinesweeperGrid::userWonPopup() {
    QMessageBox::StandardButton isPlayAgain;
    isPlayAgain = QMessageBox::question(nullptr, "Winner!", "You win! Do you want to play again?", QMessageBox::Yes | QMessageBox::No);
    if (isPlayAgain == QMessageBox::No) { // if user presses no, exit game
        QApplication::quit(); 
        exit(EXIT_SUCCESS);
    } else { // if user presses yes, restart game
        QApplication::quit(); 
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments()); 
    } 
}

/*
Name: userLostPopup
Description: if the user clicks on a mine, they are shown a window that notifies them and asks them if they want to play again
*/
void MinesweeperGrid::userLostPopup() {
    QMessageBox::StandardButton isPlayAgain;
    isPlayAgain = QMessageBox::question(nullptr, "Uh Oh!", "You hit a mine! Do you want to play again?", QMessageBox::Yes | QMessageBox::No);
    if (isPlayAgain == QMessageBox::No) {
        QApplication::quit();
        exit(EXIT_SUCCESS);
    } else {
        QApplication::quit(); 
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments()); 
    } 
}

/*
Name: main
Description: main function that creates a minesweeper grid game and runs game
*/
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MinesweeperGrid buttonGrid; // create instance of MinesweeperGrid object
    buttonGrid.resize(800, 600); // set initial size of game grid (game window)
    buttonGrid.show();
    return app.exec();
}

