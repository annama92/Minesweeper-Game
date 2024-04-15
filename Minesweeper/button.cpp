
/*
Author: Anna Ma
Description: Button class that holds properties of each individual cell in the minesweeper grid. 
Properties include row/col number, whether cells have bombs, flags, question marks, have been uncovered, and number of adjacent bombs. 
Date: Feb. 16, 2024
Class: CS3307 Individual Assignment
*/

#include <QApplication>
#include <QDebug>

#include "button.h"

/*
Name: Button
Description: Constructor method for each button object in the overall grid.
Intializes all properties accordingly (with corresponding row/col number of its position in the 2D array grid, no mines/adjacent mines, not uncovered, no flags/question marks)
*/
Button::Button(int row, int col, QWidget *parent) : QPushButton(parent), m_row(row), m_col(col), m_hasMine(false), m_uncovered(false), m_flag(false), m_questionMark(false), numAdjMines(0) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
   
    setMinimumSize(35, 35); // set size of buttons to be exactly 35, 35
    setMaximumSize(35, 35);

    connect(this, &Button::leftClicked, this, &Button::handleLeftClick); // connect buttons to correct functions that handle them
    connect(this, &Button::rightClicked, this, &Button::handleRightClick);
}

/*
Name: row
Description: getter method for row number of this button object in the overall 2D array game grid
Return: m_row - returns an integer representing the row number of this button object in the overall 2D array game grid
*/
int Button::row() const { return m_row; }

/*
Name: col
Description: getter method for column number of this button object in the overall 2D array game grid
Return: m_col - returns an integer representing the column number of this button object in the overall 2D array game grid
*/
int Button::col() const { return m_col; }

/*
Name: isMineSpace
Description: getter method to check if button object has mine 
Return: m_hasMine - boolean value which is true if button has mine, false if it doesn't
*/
bool Button::isMineSpace() const { return m_hasMine; }

/*
Name: isUncovered
Description: getter method to check if button object has been left-clicked on, used to check if user has clicked on all non-mine spaces 
Return: m_uncovered - boolean value which is true if button has been left-clicked, false otherwise
*/
bool Button::isUncovered() const { return m_uncovered; } // used to check if user has won yet

/*
Name: isFlagged
Description: getter method to check if button object has been marked with flag by right-click
Return: m_isFlag - boolean value which is true if button has been flagged, false otherwise
*/
bool Button::isFlagged() const { return m_flag; }

/*
Name: isQuestionMark
Description: getter method to check if button object has been marked with question mark by right-click
Return: m_questionMark - boolean value which is true if button has been question marked, false otherwise
*/
bool Button::isQuestionMark() const { return m_questionMark; }

/*
Name: getNumAdjMines
Description: getter method to check number of adjacent spaces that have mines
Return: numAdjMines - number of adjacent spaces that have mines
*/
int Button::getNumAdjMines() const { return numAdjMines; }

// SETTER METHODS THAT CORRRESPOND TO ABOVE GETTER METHODS
void Button::setMineSpace(bool value) { m_hasMine = value; }
void Button::setUncovered(bool value) { m_uncovered = value; }
void Button::setFlag(bool value) { m_flag = value; }
void Button::setQuestionMark(bool value) { m_questionMark = value; }
void Button::setNumAdjMines(int count) { numAdjMines = count; }

/*
Name: mousePressEvent
Description: function for emitting left and right click signals when a mouse event happens
*/
void Button::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(m_row, m_col);
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(m_row, m_col);
    }
}

/*
Name: mouseReleaseEvent
Description: handles when mouse is released
*/
void Button::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event) // unused parameter warnings are supressed
}

/*
Name: handleLeftClick
Description: handles left clicks and prints row number and col number in terminal to give confirmation to player that action has been taken after click
Parameter: row and col number of button that is pressed
*/
void Button::handleLeftClick(int row, int col) {
    qDebug() << "Left clicked on button at row:" << row << "col:" << col;
}

/*
Name: handleRightClick
Description: handles right clicks and prints row number and col number in terminal to give confirmation to player that action has been taken after click
Parameter: row and col number of button that is pressed
*/
void Button::handleRightClick(int row, int col) {
    qDebug() << "Right clicked on button at row:" << row << "col:" << col;
}






