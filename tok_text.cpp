#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <shlobj_core.h>

using namespace std;

void create_filename(PWSTR path, WCHAR* cp_path, string s) {
    int i = 0;
    while (path[i] != '\0') cp_path[i] = path[i++];
    for_each(s.begin(), s.end(), [&](char& c) { cp_path[i++] = c; });
    cp_path[i] = '\0';
}

int main(void) {
    setlocale(LC_ALL, "korean");
    std::locale::global(std::locale("ko_KR.UTF-8"));
    PWSTR path = NULL; WCHAR cp_path[100]; WCHAR write_path[100];
    SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &path); //바탕화면 경로 가져오기.
    create_filename(path, cp_path, "\\a.txt");
    wifstream readFile(cp_path);
    int num = 1;

    string write_string = "\\a\\" + to_string(num) + ".txt";
    create_filename(path, write_path, write_string);
    wofstream writeFile;
    writeFile.open(write_path);
    
    wstring tok = L"Next";      //token word

    if (readFile.is_open()) {       //open으로 대체 가능
        while (readFile.good()) {
            if (!writeFile.is_open()) writeFile.open(write_path);
            WCHAR c = readFile.get();
            writeFile << c;
            if (c == L'N') {     //tok의 기준으로 N을 만나면 Next인지 확인하는 작업.
                int i = 0;
                while (true) {
                    if (tok[i++] != c) break;
                    c = readFile.get();
                    writeFile << c;
                }
                if (tok.size() == i - 1) {
                    writeFile.close();
                    num++;
                    string write_string = "\\a\\" + to_string(num) + ".txt";
                    create_filename(path, write_path, write_string);
                    cout << num << endl;
                }
            }
        }
        readFile.close();
    }
    writeFile.close();
    CoTaskMemFree(path);
    return 0;
}