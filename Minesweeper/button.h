/*
Author: Anna Ma
Description: Button header file that holds properties of each individual cell in the minesweeper grid. 
Properties include row/col number, whether cells have bombs, flags, question marks, have been uncovered, and number of adjacent bombs. 
Date: Feb. 16, 2024
Class: CS3307 Individual Assignment
*/

#ifndef Button_H
#define Button_H

#include <QPushButton>
#include <QMouseEvent>

class Button : public QPushButton {
    Q_OBJECT
public:
    // constructor
    explicit Button(int row, int col, QWidget *parent = nullptr);

    // getters
    int row() const;
    int col() const;
    bool isMineSpace() const;
    bool isUncovered() const;
    bool isFlagged() const;
    bool isQuestionMark() const;
    int getNumAdjMines() const;  // to store information on how many adjacent cells have mines 
    
    // setters
    void setMineSpace(bool value);
    void setUncovered(bool value);
    void setFlag(bool value);
    void setQuestionMark(bool value);
    void setNumAdjMines(int count); // to set information on how many adjacent cells have mines 

signals:
    void leftClicked(int row, int col);
    void rightClicked(int row, int col);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void handleLeftClick(int row, int col);
    void handleRightClick(int row, int col);

private:
    int m_row;
    int m_col;
    bool m_hasMine; 
    bool m_uncovered;
    bool m_flag;
    bool m_questionMark;
    int numAdjMines;
};

#endif // Button_H