#ifndef COMPILERMAINWINDOW_H
#define COMPILERMAINWINDOW_H

#include <string>
#include <vector>
#include <queue>

using std::string;
using std::vector;
using std::queue;

#include <QWidget>
#include <QStandardItemModel>
#include <QGridLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QTableView>
#include <QPushButton>
#include <QTreeView>
#include <QTextEdit>

#include "compiler.h"

const int START = 0;
const int ROW = 30;
const int COL = 5;
const int CWIGROW = ROW - 4;
const int WIGROW = 1;
const int WIGCOL = 1;
const int WIDTH = 1024;
const int HEIGHT = 768;
const int COLSPAN = WIDTH / COL;
const int ROWSPAN = HEIGHT / ROW;

const int STARTSIZE = 100;

enum Page {
    P_TEXT = 0, P_TOKEN, P_AST, P_TDT, P_VFDT, P_CODE
};

inline Page operator++(Page &p) {
    switch(p) {
    case P_TEXT:
        p = P_TOKEN;
        break;
    case P_TOKEN:
        p = P_AST;
        break;
    case P_AST:
        p = P_TDT;
        break;
    case P_TDT:
        p = P_VFDT;
        break;
    case P_VFDT:
        p = P_CODE;
        break;
    default:
        break;
    }
    return p;
}

inline Page operator--(Page &p) {
    switch(p) {
    case P_TOKEN:
        p = P_TEXT;
        break;
    case P_AST:
        p = P_TOKEN;
        break;
    case P_TDT:
        p = P_AST;
        break;
    case P_VFDT:
        p = P_TDT;
        break;
    case P_CODE:
        p = P_VFDT;
        break;
    default:
        break;
    }
    return p;
}

class CompilerMainWindow : public QWidget
{
    Q_OBJECT
private:
    Page page;
    bool hasDealt;
    string plainText;
    string message;
    vector<QStandardItem *> tokenItems;
    vector<QStandardItem *> astItems;
    vector<QStandardItem *> tdtItems;
    vector<QStandardItem *> vfdtItems;
    int tokenSize;
    int astSize;
    int tdtSize;
    int vfdtSize;
    queue<int> nextBfs;
    string code;

    bool running;

    QStandardItemModel *tokenModel;
    QStandardItemModel *astModel;
    QStandardItemModel *tdtModel;
    QStandardItemModel *vfdtModel;

    Compiler *compiler;

    QGridLayout *layout;
    QLabel *messageLabel;
    QLabel *takePlace[2];
    QPlainTextEdit *textEdit;
    QTableView *tokenTable;
    QPushButton *dealButton;
    QPushButton *nextButton;
    QPushButton *backButton;
    QTreeView *astTree;
    QTableView *tdtTable;
    QTableView *vfdtTable;
    QTextEdit *codeEdit;

    void generateTokens();
    void generateAst();
    void generateTdt();
    void generateVfdt();
    void generateCode();

    bool getCodeFromFile(string fileName);

    void hidePage();
    void showPage();

    void makeAstModel(int parent, string value, unsigned int count);

public:
    CompilerMainWindow(QWidget *parent = 0);
    ~CompilerMainWindow();

private slots:
    void deal();
    void next();
    void back();
};

#endif // COMPILERMAINWINDOW_H
