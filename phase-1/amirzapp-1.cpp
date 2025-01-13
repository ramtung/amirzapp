#include <iostream>
#include <string>
using namespace std;

int main() {
    const string target = "سازمان";
    cout << "حروف کلمه: ا س م ن ا ز" << endl;
    cout << "لطفاً کلمه‌ مورد نظر را وارد کنید. برای خروج از برنامه کنترل-دی را فشار دهید." << endl;

    string input;
    while (cin >> input) {
        if (input == target) {
            cout << "تبریک! شما کلمه را پیدا کردید." << endl;
            break;
        } else {
            cout << "کلمه‌ی وارد شده اشتباه است. لطفاً دوباره تلاش کنید." << endl;
        }
    }
}