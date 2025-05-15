#include <string>
#include <iostream>

using namespace std;
int main()
{
    string s{"test"};
    char *s1 = {"1234"};

    bool flag = (s1[4] == '\0');
    bool flag1 = (s[4] == '\0');
    cout << s.size() << endl
         << strlen(s1) << endl
         << s1[4] << endl
         << flag << endl
         << flag1 << endl;
    return 0;
}