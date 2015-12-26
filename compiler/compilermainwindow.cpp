#include "compilermainwindow.h"

#include <fstream>

#define MY_DEBUG

#ifdef MY_DEBUG
#include <iostream>
using std::cout;
using std::endl;
#endif

using std::ifstream;

CompilerMainWindow::CompilerMainWindow(QWidget *parent)
    : QWidget(parent)
{
    // initial window attributes
    //this->setWindowTitle("Compiler for MyLang");
    //this->setMaximumSize(WIDTH, HEIGHT);
    //this->setMinimumSize(WIDTH, HEIGHT);
    this->setFixedSize(WIDTH, HEIGHT);

    // initial other attributes
    tokenSize = STARTSIZE;
    astSize = STARTSIZE;
    tdtSize = STARTSIZE;
    vfdtSize = STARTSIZE;
    running = false;
    hasDealt = false;
    page = P_TEXT;
    plainText = "";
    code = "";
    message = "Members: 5130379041   5130379038   5130379033"; // add your student numbers here
    tokenModel = NULL;
    astModel = NULL;
    tdtModel = NULL;
    vfdtModel = NULL;

    compiler = NULL;

    // a font that suits everyone
    QFont *font = new QFont("Consolas", 16);

    // the layout settings
    layout = new QGridLayout(this);
    this->setLayout(layout);

    // the message label settings
    messageLabel = new QLabel(this);
    messageLabel->setFont(*font);
    messageLabel->setText(QString::fromStdString(message));
    layout->addWidget(messageLabel, START, START, WIGROW, COL);

    // the gap between message label and text zone
    takePlace[0] = new QLabel(this);
    layout->addWidget(takePlace[0], START + 1, START, WIGROW, COL);

    // the text edit settings
    textEdit = new QPlainTextEdit(this);
    textEdit->setFont(*font);
    //textEdit->setText(plainText);
    layout->addWidget(textEdit, START + 2, START, CWIGROW, COL);

    // the token label settings
    tokenTable = new QTableView(this);
    tokenTable->setFont(*font);
    layout->addWidget(tokenTable, START + 2, START, CWIGROW, COL);
    tokenTable->hide();

    // the ast tree view setting
    astTree = new QTreeView(this);
    astTree->setFont(*font);
    layout->addWidget(astTree, START + 2, START, CWIGROW, COL);
    astTree->hide();

    // the tdt table settings
    tdtTable = new QTableView(this);
    tdtTable->setFont(*font);
    layout->addWidget(tdtTable, START + 2, START, CWIGROW, COL);
    tdtTable->hide();

    // the vfdt table settings
    vfdtTable = new QTableView(this);
    vfdtTable->setFont(*font);
    layout->addWidget(vfdtTable, START + 2, START, CWIGROW, COL);
    vfdtTable->hide();

    // the code edit settings
    codeEdit = new QTextEdit(this);
    codeEdit->setFont(*font);
    //textEdit->setText(plainText);
    layout->addWidget(codeEdit, START + 2, START, CWIGROW, COL);
    codeEdit->hide();

    // the deal settings
    dealButton = new QPushButton(this);
    dealButton->setText("Let's Rock!!");
    dealButton->setFont(*font);
    layout->addWidget(dealButton, ROW - 1, COL / 2);

    // the back settings
    backButton = new QPushButton(this);
    backButton->setText("Back");
    backButton->setFont(*font);
    layout->addWidget(backButton, ROW - 1, START);
    backButton->hide();

    // the next settings
    nextButton = new QPushButton(this);
    nextButton->setText("Next");
    nextButton->setFont(*font);
    layout->addWidget(nextButton, ROW - 1, COL - 1);
    nextButton->hide();

    // delete any newed items
    delete font;
    // connectors
    connect(backButton, SIGNAL(released()), this, SLOT(back()));
    connect(nextButton, SIGNAL(released()), this, SLOT(next()));
    connect(dealButton, SIGNAL(released()), this, SLOT(deal()));
}

CompilerMainWindow::~CompilerMainWindow()
{
    // delete all things
    delete astModel;
    delete tokenModel;
    delete layout;
    delete messageLabel;
    delete takePlace[0];
    delete takePlace[1];
    delete textEdit;
    delete tokenTable;
    delete dealButton;
    delete nextButton;
    delete backButton;
    delete astTree;
    if (compiler) {
        delete compiler;
    }

    // delete vectors
    // NOT IMPLEMENTED
}

void CompilerMainWindow::generateTokens()
{
    if (tokenModel) {
        delete tokenModel;
        tokenModel = NULL;
    }
    tokenModel = new QStandardItemModel();
    vector<Token> tokens = compiler->getTokens();
    tokenModel->setHeaderData(0, Qt::Horizontal, QString("Token Name"));
    tokenModel->setHeaderData(1, Qt::Horizontal, QString("Token Type"));
    unsigned int size = tokens.size();
    tokenItems.resize(2 * size, NULL);
    for (int i = 0; i < size; i++) {
        tokenItems[i       ] = new QStandardItem(QString::fromStdString(tokens[i].getValue()));
        tokenItems[i + size] = new QStandardItem(QString::fromStdString(tokens[i].getTypeToString()));
        tokenModel->setItem(i, 0, tokenItems[i       ]);
        tokenModel->setItem(i, 1, tokenItems[i + size]);
    }

    tokenTable->setModel(tokenModel);
    tokenTable->resizeColumnsToContents();
}

void CompilerMainWindow::generateAst()
{
    /*
     * You must change the class and the method here.
     * 这个算法有一些复杂，具体来说是这样的：
     * pNode是一个存所有string节点的vector，
     * items是一个存所有要构建树的model的item的vector
     * nextBfs是存着搜索队列，用数字代表了目标在pNode和items中的位置
     * 首先通过编译器提供的方法得到根节点，存入pNode中，
     * 然后把0放入nextBfs中（根节点的位置），
     * 接下来，queue的最前端就是下一个要bfs的点，得到它之后取出它的孩子，
     * 依次进行构建item，存入队列，加入pNode，
     * 直到队列中不存在任何将被bfs的节点为止。
     */

    int parent;
    vector<AST *> pNode;
    AST *sn = compiler->getAST();

    // new model
    if (astModel) {
        delete astModel;
        astModel = NULL;
    }
    astModel = new QStandardItemModel();

    // add the root
    makeAstModel(-1, sn->getValue(), 0);
    nextBfs.push(0);
    pNode.push_back(sn);

    while (!nextBfs.empty()) {
        parent = nextBfs.front();
        vector<AST *> sv = pNode[parent]->getChildren();
        // bfs the node, if sv.size() == 0 then not loop
        for (int i = 0; i < sv.size(); i++) {
            makeAstModel(parent, sv[i]->getValue(), pNode.size());
            nextBfs.push(pNode.size());
            pNode.push_back(sv[i]);
        }
        nextBfs.pop();
    }

    astTree->setModel(astModel);
}

void CompilerMainWindow::makeAstModel(int parent, string value, unsigned int count)
{
    if (count >= astItems.size()) {
        astSize *= 2;
        astItems.resize(astSize);
    }
    astItems[count] = new QStandardItem(QString::fromStdString(value));
    if (parent < 0) {
        astModel->appendRow(astItems[count]);
    }
    else {
        astItems[parent]->appendRow(astItems[count]);
    }
}

void CompilerMainWindow::generateTdt() {
    if (tdtModel) {
        delete tdtModel;
        tdtModel = NULL;
    }
    tdtModel = new QStandardItemModel();
    vector<TypeDescriptor> tds = compiler->getTDs();
    tdtModel->setHeaderData(0, Qt::Horizontal, QString("Name"));
    tdtModel->setHeaderData(1, Qt::Horizontal, QString("Parent"));
    tdtModel->setHeaderData(2, Qt::Horizontal, QString("Origin"));

    unsigned int size = tds.size();
    tdtItems.resize(3 * size, NULL);
    for (int i = 0; i < size; i++) {
        tdtItems[i + 0 * size] = new QStandardItem(QString::fromStdString(tds[i].name));
        tdtItems[i + 1 * size] = new QStandardItem(QString::fromStdString(tds[i].parent));
        tdtItems[i + 2 * size] = new QStandardItem(QString::fromStdString(tds[i].origin));
        tdtModel->setItem(i, 0, tdtItems[i + 0 * size]);
        tdtModel->setItem(i, 1, tdtItems[i + 1 * size]);
        tdtModel->setItem(i, 2, tdtItems[i + 2 * size]);
    }

    tdtTable->setModel(tdtModel);
    tdtTable->resizeColumnsToContents();
}

void CompilerMainWindow::generateVfdt() {
    if (vfdtModel) {
        delete vfdtModel;
        vfdtModel = NULL;
    }
    vfdtModel = new QStandardItemModel();
    vector<VarFuncDescriptor> vfds = compiler->getVFDs();
    vfdtModel->setHeaderData(0, Qt::Horizontal, QString("Name"));
    vfdtModel->setHeaderData(1, Qt::Horizontal, QString("Parent"));
    vfdtModel->setHeaderData(2, Qt::Horizontal, QString("Origin"));
    vfdtModel->setHeaderData(3, Qt::Horizontal, QString("Attribute"));
    vfdtModel->setHeaderData(4, Qt::Horizontal, QString("Env"));

    unsigned int size = vfds.size();
    vfdtItems.resize(5 * size, NULL);
    for (int i = 0; i < size; i++) {
        vfdtItems[i + 0 * size] = new QStandardItem(QString::fromStdString(vfds[i].name));
        vfdtItems[i + 1 * size] = new QStandardItem(QString::fromStdString(vfds[i].parent));
        vfdtItems[i + 2 * size] = new QStandardItem(QString::fromStdString(vfds[i].origin));
        vfdtItems[i + 3 * size] = new QStandardItem(QString::fromStdString(vfds[i].attribute));
        vfdtItems[i + 4 * size] = new QStandardItem(QString::fromStdString(vfds[i].env));
        vfdtModel->setItem(i, 0, vfdtItems[i + 0 * size]);
        vfdtModel->setItem(i, 1, vfdtItems[i + 1 * size]);
        vfdtModel->setItem(i, 2, vfdtItems[i + 2 * size]);
        vfdtModel->setItem(i, 3, vfdtItems[i + 3 * size]);
        vfdtModel->setItem(i, 4, vfdtItems[i + 4 * size]);
    }

    vfdtTable->setModel(vfdtModel);
    vfdtTable->resizeColumnsToContents();
}

void CompilerMainWindow::generateCode() {
    code = compiler->getCode();
    codeEdit->setPlainText(QString::fromStdString(code));
}

void CompilerMainWindow::hidePage() {
    switch (page) {
    case P_TEXT:
        textEdit->hide();
        nextButton->hide();
        return;
    case P_TOKEN:
        tokenTable->hide();
        backButton->hide();
        nextButton->hide();
        return;
    case P_AST:
        astTree->hide();
        backButton->hide();
        nextButton->hide();
        return;
    case P_TDT:
        tdtTable->hide();
        backButton->hide();
        nextButton->hide();
        return;
    case P_VFDT:
        vfdtTable->hide();
        backButton->hide();
        nextButton->hide();
        return;
    case P_CODE:
        codeEdit->hide();
        backButton->hide();
        return;
    default:
        // never here
        return;
    }
}

void CompilerMainWindow::showPage() {
    switch (page) {
    case P_TEXT:
        textEdit->show();
        nextButton->show();
        return;
    case P_TOKEN:
        tokenTable->show();
        backButton->show();
        nextButton->show();
        return;
    case P_AST:
        astTree->show();
        backButton->show();
        nextButton->show();
        return;
    case P_TDT:
        tdtTable->show();
        backButton->show();
        nextButton->show();
        return;
    case P_VFDT:
        vfdtTable->show();
        backButton->show();
        nextButton->show();
        return;
    case P_CODE:
        codeEdit->show();
        backButton->show();
        return;
    default:
        // never here
        return;
    }
}

bool CompilerMainWindow::getCodeFromFile(string fileName) {
    ifstream ifs(fileName.c_str());
    if (ifs.bad() || ifs.fail()) {
        return false;
    }

    plainText.clear();

    while (!ifs.eof()) {
        string str;
        getline(ifs, str);
        plainText.append(str);
        plainText.append("\n");
    }
    return true;
}

void CompilerMainWindow::deal()
{
    if (!running) {
        running = true;
        /*if (compiler) {
            delete compiler;
            compiler = NULL;
        }*/
        tokenItems.clear();
        tokenItems.resize(tokenSize, NULL);
        astItems.clear();
        astItems.resize(astSize, NULL);
        messageLabel->setText("Analizing your code: code->data...");
        plainText = textEdit->toPlainText().toStdString();

        // a simple gui command, read code from a file
        if (plainText.substr(0, 13) == "#GUIREADFROM:") {
            string read = plainText.substr(13);
            if (getCodeFromFile(read) == false) {
                messageLabel->setText(QString("GUI command error: No such file"));
                running = false;
                return;
            }
            textEdit->hide();
            textEdit->setPlainText(QString::fromStdString(plainText));
            textEdit->show();
        }

        compiler = new Compiler(plainText);
        if (compiler->getTokenErrorNum() != 0) {
            running = false;
            messageLabel->setText(QString::fromStdString(compiler->getTokenError()));
            return;
        }
        if (compiler->getAstErrorNum() != 0) {
            running = false;
            messageLabel->setText(QString::fromStdString(compiler->getAstError()));
            return;
        }
        if (compiler->getAnalysisErrorNum() != 0) {
            running = false;
            messageLabel->setText(QString::fromStdString(compiler->getAnalysisError()));
            return;
        }
        messageLabel->setText("Analizing your code: data->tokens...");
        generateTokens();
        messageLabel->setText("Analizing your code: tokens->AST...");
        generateAst();
        messageLabel->setText("Analizing your code: generating tdt...");
        generateTdt();
        messageLabel->setText("Analizing your code: generating vfdt...");
        generateVfdt();
        messageLabel->setText("Analizing your code: generating C++ code...");
        generateCode();
        messageLabel->setText("All has been done! Now you can view the results!");
        if (!hasDealt) {
            hasDealt = true;
            nextButton->show();
        }
        plainText = "";
        running = false;
    }
    else {
        messageLabel->setText("Please wait! We are dealing something...");
    }
}

void CompilerMainWindow::next()
{
    if (!running) {
        running = true;
        hidePage();
        page++;
        showPage();
        running = false;
        return;
    }
    else {
        messageLabel->setText("Please wait! We are dealing something...");
    }
}

void CompilerMainWindow::back()
{
    if (!running) {
        running = true;
        hidePage();
        page--;
        showPage();
        running = false;
    }
    else {
        messageLabel->setText("Please wait! We are dealing something...");
    }
}

