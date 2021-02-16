#include "mytextedit.h"

MyTextEdit::MyTextEdit(QWidget *parent) : QTextEdit(parent)
{

}

void MyTextEdit::focusOutEvent(QFocusEvent *e){
    emit focusOut();
    QTextEdit::focusOutEvent(e);
}

void MyTextEdit::focusInEvent(QFocusEvent *e){
    emit focusIn();
    QTextEdit::focusInEvent(e);
}
