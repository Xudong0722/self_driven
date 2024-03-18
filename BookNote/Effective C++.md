# Effective C++

# 零.Effective Modern C++([https://github.com/CnTransGroup/EffectiveModernCppChinese](https://github.com/CnTransGroup/EffectiveModernCppChinese))

## 区别使用() 和 {} 创建对象

* 括号初始化是最广泛使用的初始化语法，它防止变窄转换(Narrowing Conversion)，并且对于C++最令人头疼的解析有天生的免疫性

```cpp
  int varInt{ 1.0 };
  std::vector<int> arrInt{ 1.0, 2 };

Error:
Element '1': conversion from 'double' to '_Ty' requires a narrowing conversion
conversion from 'double' to 'int' requires a narrowing conversion
```

* 在构造函数重载决议中，括号初始化尽最大可能与std::initializer_list参数匹配，即便其他构造函数看起来是更好的选择
* 对于数值类型的std::vector来说使用花括号初始化和小括号初始化会造成巨大的不同
* 在模板类选择使用小括号初始化或使用花括号初始化创建对象是一个挑战。

## 使用override声明重写函数

重写(overriding) -/- 重载(overloading)

重写一个函数需满足：

* 基类函数必须是virtual
* 基类和派生类函数名必须完全一样（除非是析构函数)
* 基类和派生类函数形参类型必须完全一样
* 基类和派生类函数常量性constness必须完全一样
* 基类和派生类函数的返回值和异常说明（exception specifications）必须兼容
* (C++11) 函数的引用限定符(reference qualifiers)必须完全一样，它限定成员函数只能用于左值或者右值
* 为重写函数加上override
* 成员函数引用限定让我们可以区别对待左值对象和右值对象（即*this)

# 一.让自己习惯C++

## 1.条款一

将C++视为一个由四个次语言组成的联合语言，不要认为传引用一定比传参要好，C++的高效编程守则视状况而变化，取决于使用C++的哪一个部分

C  object-Oriented-C++   Template-C++STL

## 2.条款二 尽量以const ，enum，inline 替换 #define

1.尽量不使用宏来定义常量，其一，如果使用define来定义一个常量，在预编译阶段会把代码中的符号替换成常量，该符号可能未进入符号表，如果程序出错可能只看到常量而看不到符号表，浪费时间

其二，define不在意作用域，无法定义class内部常量

2.一个用法，enum hack，无法使用in-class初值时，可以使用enum来定义常量，直接使用，例如，编译器在编译期间就需要知道数组的大小，所以我们如果要定义数组的话，需要一个常量，可以用const，也可以用enum，书中意思enum用的很多，应该是使用空间更小--符号

3.对于单纯常量，最好以const对象或enums替换#define，对于形似函数的宏，最好改用inline函数替换#​define(这里暂持保留意见

## 3.条款三 尽量使用const

1.如果const出现在*左边，如const int* p 或者 int const* p  表示被指物是一个常量， 如果const出现在*右边，如int* const p;表示指针自身是常量

2.bitwise constness认为const成员函数不能修改类里的任何一个bit，logical constness则认为一个const成员函数可以修改它所处理的对象内的某些bits满足bitwise constness的成员函数可能不适当，可能会改变对象的某些值

## 4.条款四 确定对象被使用前已先被初始化

1.对于类的构造函数来说，总是应该使用初始化列表的方法来初始化成员，这样做有时候绝对必要(如果成员变量是const或reference的话，必须要初值而不能赋值)，且往往比赋值更高效(成员是类的话会先调用默认构造函数，再赋值的话，默认构造函数的调用就浪费了)

2.在初始化列表中条列各个成员时，最好总是以其声明次序为次序，虽然打乱次序也是合法的，但那时不好的习惯，可能对后面检阅代码时造成疑惑

**Singleton**： C++保证，函数内的local static对象会在"该函数被调用期间""首次遇上该对象的定义式"时被初始化，所以可以用一个函数调用(返回一个reference-local static对象)替换直接访问non-local static对象

static对象的生命周期从构造到程序结束为止：

non-local static对象：global对象、定义于namespace作用域内的对象、在class内、在函数内、以及在file作用域内被声明为static的对象

local static对象：函数内的static对象

```cpp
clause04.cpp
#include <iostream>

class FileSystem{
public:
    std::size_t numDisks() const{ return 1; }
};

// extern FileSystem tfs;
FileSystem& tfs()
{
    static FileSystem fs;
    return fs;
}


clause18.cpp
#include <bits/stdc++.h>
#include "clause04.cpp"

using namespace std;

class Directory{
public:
    Directory();
};

Directory::Directory()
{
    std::size_t disks = tfs().numDisks();
    std::cout<<"disks: "<<disks<<'\n';
}

Directory& tempDir()
{
    static Directory td;
    return td;
}

int main()
{
    tempDir();

    return 0;
}
```

## 5.条款五 了解C++默默编写并调用哪些函数

1.class Empty();    编译器会为之生成default构造函数，copy构造函数，copy assignment操作符和析构函数(non-virtual)

2.编译器可以拒绝提供生成copy构造函数(内含reference成员，内含const成员，base class 将copy assignment 操作符声明为private时编译器不会为其derivedclass 生成copy assignment操作符)

## 6.条款六 若不想使用编译器自动生成的函数，就该明确拒绝

1.为了驳回编译器默认提供的功能(拷贝构造函数和拷贝构造运算符)，我们可以将相应的函数声明为private且不实现。 或者使用一个Uncopyable这样的base class

## 7.条款七 为多态基类声明virtual析构函数

1.polymorphic base classes 应该声明一个virtual析构函数，保证在delete一个指向派生类对象的基类指针时不会出现内存泄漏的问题。一个好用的方法是：如果一个class至少有一个virtual函数，那么它应该拥有一个virtual析构函数

2.如果一个class的设计目的就不是为了作为baseclass使用，或者不具备多态性，就不该声明virtual析构函数。

3.将class的成员函数设置成xirtual函数是有代价的，一个拥有虚函数的class会保存一个vptr和vptl，造成一个对象的实际内存往往比我们想象得要大的多

## 8.条款八 别让异常逃离析构函数

1.析构函数绝对不要抛出异常，如果在析构函数中抛出异常，那么异常点之后的代码很有可能不会执行(如果后面是释放资源的代码，则会造成内存泄漏)

2.抛出异常时，C++编译器会在异常传播过程中执行 Stack Unwinding，执行栈中已构造好的对象的析构函数，如果在析构函数中抛出异常，那么可能发生程序在处理一个异常时收到另一个异常，导致程序崩溃或发生未定义行为

3.如果一个被析构函数调用的函数可能会抛出异常，析构函数要全部捕捉它们，然后吞下这些异常(不传播)或停止程序

## 9.条款九 绝不在构造或析构过程中调用virtual函数

1.构造过程：

1）在一个继承体系中，存在一个和类型相关的虚函数，如果每个类的构造函数都要调用这个虚函数，在我们定义一个drived class时，drivedclass的构造函数会先调用base class的构造函数来初始化base的部分，这个时候被调用的虚函数是base的版本，绝对不会下降到derived的版本，因为C++不会允许你使用未经过初始化的部分，而这个虚函数几乎肯定会操作derived的部分，所以这就无法达成我们的期望(我们希望调用derived版本的virtualfunction)

2）即根本来说，在derived class构造函数执行之前，derived class不是一个derivedclass，而被视作一个base class

2.析构过程

同理，derived class的部分析构完成之后，进入base class析构之后对象就成为了一个base class对象，这里面调用虚函数也无法达到我们的期望

## 10.条款十 令operator=返回一个reference to *this

此条款适用于所有赋值相关运算，此条款被所有内置类型和string,vector，complex，shared_ptr共同遵守

## 11.条款十一 在operator= 中处理 “自我赋值”

这种情况一般是“别名”带来的后果，比如a[i] = a[j];  *px = *py; … 他们可能指向同一个对象，造成自我赋值，如果在重载operator=中涉及delete再new的情形，加上自我赋值就会出现指针指向一个已被删除的对象，要避免这种情况，我们需要“证同测试”来检验自我赋值，或者精心安排语句顺序来保证正确性，以及copy-and-swap。

## 12.条款十二 复制对象时勿忘其每一个成分

1.copy构造函数应该确保复制(对象内的所有成员变量)，及所有"Base class"的部分，编译器不会给你这方面的警告，如果你自己实现了copy函数的话

2.不要尝试以某一个copying函数去调用另一个copying函数(例如，拷贝构造函数调用拷贝赋值运算符函数)，将两个函数的共同机制放在第三方函数里(private的init)，然后共同调用

# 三.资源管理

## 13.条款十三 让对象管理资源

1.以对象管理资源的两个关键想法：

1). RAII(Resource Acquisition  Is Initialization)，即资源获取时机便是初始化时机，因为我们总是在获得一笔资源后于同一语句内以它初始化某个管理对象

2). 管理对象运行析构函数确保资源被释放(智能指针)

2.两个常被使用的RAII classes分别是shared_ptr和auto_ptr

## 14.条款十四 在资源管理类中小心copying行为

‍

# 四.设计与声明

## 18.条款十八 让接口容易被正确使用，不易被误用

1.好的接口很容易被正确使用，不容易被误用。你应该在你的所有接口中努力达成这 些性质。

2. "促进正确使用”的办法包括接口的一致性，以及与内置类型的行为兼容。

3. "阻止误用”的办法包括建立新类型、限制类型上的操作，束缚对象值，以及消除 客户的资源管理责任。

```cpp
#include <bits/stdc++.h>

struct Year{
explicit Year(int y): val(y){};
int val;
};

// struct Month{
// explicit Month(int m): val(m){};
// int val;
// };

struct Day{
explicit Day(int d): val(d){};
int val;
};

class Month{
public:
    static Month Jan() { return Month(1); }
    static Month Feb() { return Month(2); }
    //...
    static Month Dec() { return Month(12); }
private:
    explicit Month(int m): month(m){}
    int month;
};

class Date{
public:
    Date(const Month& m, const Day& d, const Year& y): month(m),day(d),year(y){}
private:
    Month month;
    Day day;
    Year year;
};
int main()
{
    Date d(Month::Feb(), Day(28),Year(2022));

    return 0;
}
```

4.tr1::shared_p 支持定制型删除器 (custom deleter) 。这可防范 DLL 问题，可被 用来自动解除互斥锁 (mutexes; 见条款 14) 等等。

‍

## 19.条款十九 设计class犹如设计type

* **新type 的对象应该如何被创建和销毁？** 这会影响到你的 class 的构造函数和析构函 数以及内存分配函数和释放函数 (operatornew, operatornew[], opera or delete  operator delete[]）的设计，当然前提是如果你打算撰写它们。
* **对象的初始化和对象的赋值该有什么样的差别？** 这个答案决定你的构造函数和赋值 (assignment) 操作符的行为，以及其间的差异。很重要的是别混淆了“初始化” 和“赋值”，因为它们对应千不同的函数调用。
* **新 type 的对象如果被 pass by value （以值传递），意味着什么？** 记住， copy 构造 函数用来定义一个 type 的如pass-by-value 该如何实现。
* **什么是新type的合法值？** 对 class 的成员变量而言，通常只有某些数值集是有 效的。那些数值集决定了你的 class 必须维护的约束条件 (invariants) ，也就决定 了你的成员函数（特别是构造函数、赋值操作符和所谓 “setter" 函数）必须进行的 错误检查工作。它也影响函数抛出的异常、以及（极少被使用的）函数异常明细列 (exception specifications)。
* **你的新 type 需要配合某个继承图系 (inheritance graph) 吗？** 如果你继承自某些既 有的 classes，你就受到那些 classes 的设计的束缚，特别是受到“它们的函数是 virtual non-virtual" 的影响（见条款 34 和条款 36) 。如果你允许其他 classes 继承你的 class ，那会影响你所声明的函数一—－尤其是析构函数一是否为 virtual （见条款 7)
* **你的新 type 需要什么样的转换？** 你的 type 生存于其他一海票 types 之间，因而彼此 该有转换行为吗？如果你希望允许类型 之物被隐式转换为类型 T2 之物，就必须 class Tl 内写一个类型转换函数 (operator T2) 或在 class T2 内写一个 non-explicit-one-argument （可被单一实参调用）的构造函数。如果你只允许 explicit 构造函数存在，就得写出专门负责执行转换的函数，且不得为类型转换操作符 (type conversion operators) non-explicit-one-argument 构造函数。（条款15有隐式和显式转换函数的范例。
* **什么样的操作符和函数对此新 type 而言是合理的？** 这个问题的答案决定你将为你 class 声明哪些函数。其中某些该是 member 函数，某些则否（见条款 23,24,46)
* **什么样的标准函数应该驳回？** 那些正是你必须声明为 private 者（见条款 6)
* **谁该取用新 type 的成员？** 这个提问可以帮助你决定哪个成员为 public, 哪个为 protected, 哪个为 private 。它也帮助你决定哪一个 classes 和／或 functions 应该是 friends ，以及将它们嵌套千另一个之内是否合理。
* **什么是新 type 的“未声明接口” (undeclared interface) ？** 它对效率、异常安全性 （见条款 29) 以及资源运用（例如多任务锁定和动态内存）提供何种保证？你在这 些方面提供的保证将为你的 class 实现代码加上相应的约束条件。
* **你的新 type 有多么一般化？** 或许你其实并非定义一个新 type ，而是定义一整个 types 家族。果真如此你就不该定义一个新 class, 而是应该定义一个新的 class template
* **你真的需要一个新 type 吗？** 如果只是定义新的 derived class 以便为既有的 class 加机能，那么说不定单纯定义一或多个 non-member 函数或 templates, 更能够达到目标

## 20.条款二十 宁以pass-by-reference-to-const 替换pass by value

1.尽量以pass-by-reference-to-const 替换 pass-by-value。前者通常比较高效，并可避免切割问题(slicing problem)**静态类型，如果是指针或者引用就是动态绑定**

Tips: 如果类中包含一个其他类类型的成员且这个成员的类型没有默认构造函数，那么编译器将无法初始化该成员。

```cpp
#include <bits/stdc++.h>

// class String{
// public:
//     String(std::string _str = ""):str(_str){ std::cout<<"String::constructor\n"; }
//     String(const String& _str){
//         std::cout<<"String::Copy constructor\n";
//     }
//     ~String(){
//         std::cout<<"String::destructor\n";
//     }
// private:
//     std::string str;
// };

class Person{
public:
    Person(){ std::cout<<"Person::constructor\n"; }
    Person(const Person& per){
        name = per.name;
        address = per.address;
        std::cout<<"Person::Copy constructor\n";
    }
    ~Person(){
        std::cout<<"Person::destructor\n";
    }
private:
    std::string name;
    std::string address;
};

class Student: public Person{
public:
    Student(){ std::cout<<"Student::constructor\n"; }
    Student(const Student& stu){
        schoolName = stu.schoolName;
        schoolAddress = stu.schoolAddress;
        std::cout<<"Student::Copy constructor\n";
    }
    ~Student(){
        std::cout<<"Student::destructor\n";
    }
private:
    std::string schoolName;
    std::string schoolAddress;
};

bool validateStudent(Student s)
{
    return 0x0&0x1;
}

int main()
{
    Student plato;
  
    std::cout<<"------\n";
    bool platoIsOK = validateStudent(plato);
    return 0;
}

```

2.**以上规则并不适用于内置类型，以及STL的迭代器和函数对象。对它们而言，pass-by-value 往往比较适当**​

```cpp
void dealWithInt(int a) //如果我们单纯只使用int的值而不修改的话，使用int会比int&效率更高，因为引用是用指针实现的，使用int&的代价在这里比直接用int要高
{
    //......
}
```

## 21.条款二十一 必须返回对象时，别妄想返回其reference

**绝不要返回 pointer reference 指向一个 local stack 对象，或返回 reference 指向一 heap-allocated 对象，或返回 pointer reference 指向一个 local static 对象而有可 能同时需要多个这样的对象。** 条款4已经为“在单线程环境中合理返回 reference 指向一个 local static 对象”提供了一份设计实例。

```cpp
#include <bits/stdc++.h>

class Rational{
public:
    Rational(int numerator = 0, int denominator = 1): n(numerator), d(denominator) {}

private:
    int n,d;
    friend const Rational& operator*(const Rational& lhs, const Rational& rhs);
    friend bool operator ==(const Rational& lhs, const Rational& rhs);
};

const Rational& operator*(const Rational& lhs, const Rational& rhs)
{
    static Rational result;

    result.n = lhs.n * rhs.n;
    result.d = lhs.d * rhs.d;
    std::cout<<result.d<<" "<<result.n<<'\n';
    return result;
}

bool operator==(const Rational& lhs, const Rational& rhs)
{
    std::cout<<lhs.d<<" "<<rhs.d<<'\n';
    std::cout<<lhs.n<<" "<<rhs.n<<'\n';
    return lhs.d == rhs.d && lhs.n == rhs.n;
}
int main()
{
    Rational a,b;
    Rational c(2,3), d(4,5);

    if((a * b) == (c * d)){   //永远相等，因为返回的是引用，永远是现值
        std::cout<<"equal\n";  
    }else{
        std::cout<<"not equal\n";
    }

    return 0;
}
```

## 22.条款二十二 将成员变量声明为private

1.切记将成员变量声明为 private 。这可赋予客户访问数据的一致性、可细微划分访问 控制、允诺约束条件获得保证，并提供 class 作者以充分的实现弹性。 

2.protected 并不比 public 更具封装性。(假设有一个protected成员变量，而我们取消了它，所有使用它的derived classes都会被破坏)

‍

## 23.条款二十三 宁以non-member、non-friend替换member函数

1. 宁可拿non-member、non-friend函数替换member函数。这样做可以增加封装性、包裹弹性和技能扩充性

	条款 22 曾说过，成员变量应该是 private, 因为如果它们不是，就有无限量的函数 可以访问它们，它们也就毫无封装性。能够访问 private 成员变量的函数只有 class member 函数加上 friend 函数而已。如果要你在一个 member 函数（它不只可以访问 class 内的 private 数据，也可以取用 private 函数、 enums typedefs 等等）和一个 non-member, non-friend 函数（它无法访问上述任何东西）之间做抉择，而且两者提供相同机能，那 么，导致较大封装性的是 non-member non-friend 函数，因为它并不增加" 能够访问class 内之 private 成分＂的函数数量。这就解释了为什么 clearBrowser （一个 non-member non-friend 函数）比 clearEverything （一个 member 函数）更受欢迎的原因：它导致 WebBrowser class 有较大的封装性。

‍

## 24.条款二十四 若所有参数皆需类型转换，请为此采用non-member函数

1. 如果需要为了某个函数的所有参数(包括被this指针所指的那个隐喻参数)进行类型转换，那么这个函数必须是non-member的

2. member函数的对立面是non-member函数，而不是friend函数，对于friend函数，可以避免就尽量避免，

‍

## 24.考虑写出一个不抛异常的swap函数

1. 通常我们不被允许改变std命名空间内的任何东西，但是被允许为标准templates(如swap)制造特化版本，使它专属于我们自己的class

```cpp
#include <iostream>

namespace mystd{
    template<class T>
    void swap(T&a, T&b)
    {
        T temp(a);
        a = b;
        b = temp;
    }
}


class Widget{
    double* data;       // 我们真正要swap的对象
public:
    //Widget(const Widget& rhs);
    Widget(double* param): data(param){}

    void output(){
        std::cout<<*data<<"\n";
    }
    Widget& operator=(const Widget& rhs)
    {
        *data = *(rhs.data);
        return *this;
    }

    void swap(Widget& other)
    {
        using std::swap;
        swap(data, other.data);
    }
};
namespace std{
    template<>   // std中针对widget类的全特化版本swap，调用Widget对象的swap public对象
    void swap<Widget>(Widget& a, Widget& b){
        a.swap(b);
    }
}

int main()
{
    double a = 1.0, b = 2.0;
    Widget c(&a);
    Widget d(&b);

    c.output();
    d.output();
    std::swap(c,d);
    c.output();
    d.output();
    return 0;
}
```

2. 成员版swap绝不可抛出异常。原因？ swap的一个最好的应用是帮助class提供强烈的异常安全性？  *需要查一下资料*

高效率的swap几乎总是基于对内置类型的操作，而内置类型上的操作绝不会抛出异常

3. 调用swap时应针对std::swap使用using声明式

# 五.实现(Implementations)

## 26. 条款二十六 尽可能延后变量定义式的出现时间

1. 不单纯延后变量的定义，知道非得使用该变量的前一刻位置，甚至应该尝试延后这份定义知道能够给它初值实参为止。

即以具明显意义之初值将变量初始化，还可以附带说明变量的目的

```cpp
#include <iostream>
#include <string>

class Sentence{
    int length{0};
    std::string str;
public:
    Sentence(){
        std::cout<<"Default Constructor\n";
    }
    Sentence(std::string str_): str(str_){
        std::cout<<"Sentence Constructor\n";
        length = str_.length();
    }
    int GetLength()const{
        return this->length;
    }
    void DoSomething(const std::string& s){
        //....
    }
    ~Sentence(){
        std::cout<<"Sentence Destructor\n";
    }
};

void Function(const std::string& str){
    Sentence a;     //定义时机太早， 浪费了一次构造资源，且没有正确析构掉
    if(str.length() < 10){
        throw "Error";
    }
    //...
    a.DoSomething(str);
    return ;
}
int main()
{
    std::string article{"12345"};

    Function(article);

    return 0;
}
```

2. 如果变量只在循环内使用，那么是将这个变量定义在循环外并在每次迭代时赋值给它，还是该把它定义在循环内（这里考虑的对象执行构造，析构，或赋值动作都需要一定的成本）

衡量：放在循环内部，那么n次循环需要n次构造和n次析构，放在外部，需要一次构造一次析构外加n次赋值操作，这是性能方面的考量，但是放在外部，变量的作用域更大，可能会影响程序的维护性和可理解性。所以需要综合考虑

‍

# 六.继承与面向对象设计

## 34.条款三十四 区分接口继承和实现继承

1. 声明non-virtual函数的目的是为了让derived-class继承函数的接口及一份强制实现
2. 声明pure-virtual函数的目的是具体指定接口继承
3. impure virtual函数具体指定接口继承及缺省实现继承

## 35.条款三十五 考虑virtual函数以外的其他选择

1. 当你为解决问题而寻找某个设计方法时，不妨考虑 virtual 函数的替代方案- **strategy模式**

    1. 使用NVI(non-virtual interface)
    2. 将virtual函数替换为"函数指针成员变量"
    3. 以tr1::function 成员变量替换virtual函数

# 七.模板与泛型编程

## 41.条款四十一 了解隐式接口和编译器多态

1. classes templates 都支持接口 (interfaces) 和多态 (polymorphism)

2. 对classes而言接口是显式的 (explicit) ，以函数签名为中心。多态则是通过 virtual函数发生于运行期。

3. 对 template 参数而言，接口是隐式的 (implicit) ，奠基于有效表达式。多态则是通过 template 具现化和函数重载解析 (function overloading resolution) 发生于编译期。

## 42.条款四十二 了解typename的双重意义

1. 声明 template 参数时，前缀关键字 class typename 可互换。

2. 请使用关键字 typename 标识嵌套从属类型名称；但不得在 base class lists （基类列表）或 member initialization list （成员初始化列表）内以它作为 base class 修饰符。

‍

## 43.条款四十三 学习处理模板化基类内的名称

```cpp
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <fstream>
#include <thread>

class CompanyA {
public:
  void sendClearText(const std::string& msg);
  void sendEncrypted(const std::string& msg);
};

class CompanyB {
public:
  void sendClearText(const std::string& msg);
  void sendEncrypted(const std::string& msg);
};

class CompanyZ {
public:
  void sendEncrypted(const std::string& msg);
};

class MsgInfo {};

template<typename Company>
class MsgSender {
public:
  void SendClear(const MsgInfo& info)
  {
    std::string msg;
    Company c;
    c.sendClearText(msg);
  }
  void SendSecret(const MsgInfo& info)
  {
    std::string msg;
    Company c;
    c.sendClearText(msg);
  }
};

template<>  //total template specialization
class MsgSender<CompanyZ> {
public:
  void sendSecret(const MsgInfo& info);
};

template<typename Company>
class LoggingMsgSender : public MsgSender<Company> {
  using MsgSender<Company>::sendClear;  // Solution 2:告诉编译器，去base class作用域内查找
  void sendClearMsg(const MsgInfo& info)
  {
    //Solution 1: this->sendClear(info);    假设sendClear将被继承
    sendClear(info);   // sendClear identifier not found
			//编译器在编译时不知道MsgSender对象是否有sendClear函数，只有具现化之后才能得知
			//编译器知道模块可能被全特化(如MsgSender<CompanyZ>), 所以编译器拒绝在模板化基类内寻找继承而来的函数
  }
};
int main()
{
  return 0;
} 
```

在 derived class templates 内通过 “this->" 指涉 base class templates 内的成员 名称，或藉由一个明白写出的 “base class 资格修饰符“完成。

‍

## 44.条款四十四 将与参数无关的代码抽离templates

‍

‍

//搞懂编译期和运行期 & 栈分配

‍
