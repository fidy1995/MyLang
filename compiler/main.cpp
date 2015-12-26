#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <string>
#include <vector>

#include "compiler.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
/*    string str = "type myInt = integer;myChar= char;myChar2 = myChar;myClass = class begin i : integer;end;myClass2 = myClass begin j : myInt; function voi() var i:integer; begin return; end;end;var\nn:integer;\nk:integer;\na[100]:integer; st:myClass2; { 定义数组，数组标号从0开始到99 }\n{ 变量定义部分结束，以下是函数定义部分 }\nfunction qsort(l:myInt,h:integer) { 注意这里没有返回值，因而函数无return语句 }\nvar { 函数内变量定义部分。}\ni:integer;\n    j:myInt;\n    m:integer;\n    t:integer;\nbegin { 函数主程序部分 }\nst.voi();i = l; j = h; \nm = a[(i+j) div 2]; \nwhile i <= j do\nbegin\nwhile a[i] < m do i = i + 1;\nwhile m < a[j] do j = j - 1;\nif i <= j then\nbegin\nt = a[i]; a[i] = a[j]; a[j] = t;\ni = i + 1; j = j - 1;\nend;\nend;\nif i<h then qsort(i,h);\nif j>l then qsort(l,j);\nend;\n{ 函数定义部分结束，以下是主程序部分 }\nbegin\n    readInteger(n); { 读取个数 }\n    for k = 0 to n-1 do { 注意数组标号从零开始，最多到n-1 }\n        readInteger(a[k]); { 读取数组数值 }\n    qsort(0, n-1); { 调用快排 }\n  for k = 0 to n-1 do\n        writelnInteger(a[k]); { 换行输出数组数值 }\nend.";

    Compiler com(str);
    cout << com.getTokenError() << ' ' << com.getAstError() << ' ' << com.getAnalysisError() << '\n';

    std::vector<TypeDescriptor> TDs = com.getTDs();
    std::vector<VarFuncDescriptor> VFDs = com.getVFDs();
    std::string code = com.getCode();

    cout << "TDT: \n";
    for (unsigned int i = 0; i < TDs.size(); i++) {
        TypeDescriptor desc = TDs[i];
        cout << desc.name << " | " << desc.parent << " | " << desc.origin << '\n';
    }
    cout << "VFDT: \n";
    for (unsigned int i = 0; i < VFDs.size(); i++) {
        VarFuncDescriptor desc = VFDs[i];
        cout << desc.name << " | " << desc.parent << " | " << desc.origin << " | " << desc.attribute << " | " << desc.env << '\n';
    }
    cout << '\n' << code << '\n';*/
}
