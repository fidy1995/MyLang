# MyLang
The compiler for MyLang designed for SE302-The Principle of Compiler. Written in Qt together with
```
Jingcheng Qian
Zhouhao Zhang
```
from Software Institute, Shanghai Jiaotong University. The names are in no particular order.

## Variable defining
The variables should be defined like this:

    var
        [your variable name] : [MyLang type name];
        ...
It is translated to C++ code like this:
```C++
[standard type name] [your variable name];
```
If you want to use locals in functions, just refer to next part.

## Function defining
The functions should be defined like this:

    // note you can use no attributes
    function [your function name]([attribute name]:[MyLang type name], ...)
    return [MyLang type name]
    var         // if you are using locals
        [your variable name] : [MyLang type name];
        ...
    begin
        [your function]
    end
It is translated to C++ code like this:
```C++
[standard type name] [your function name]([standard type name] [attribute name], ...) {
    [your function]
    return [return attribute name];
}
```
### Main
Unlike the function, the compiler just scan for the begin-end. pair:

    begin
        [main function]
    end.

## Type Description
The types should be defined like this:

    type
        [your type name] = [MyLang type name];
        ...
It is translated to C++ code like this:
```C++
typedef [standart type name] [your type name];
```

## Class Description
The class should be defined like this:

    type
        [your class name] = class | [your parent class name]
        begin
            [your attribute name] : [MyLang type name]
            ...
        end
Since all attributes and functions in the class of MyLang is public, it is translated into C++ code like this:
```C++
class [your class name] (: [your parent class name]) {
    [standard type name] [your attribute name];
    ...
};
```
