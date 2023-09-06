#include <iostream>
#include <fstream>

int main()
{
    int n, h, m;
    std::ifstream fin("field.txt");
    if (!fin.is_open()) {
        std::cout << "File field.txt didn't faind";
        return 1;
    }
    std::ofstream fout("command.txt", std::ios_base::trunc);
    if (!fout.is_open()) {
        std::cout << "File command.txt didn't faind";
        return 1;
    }

    fin >> n >> h >> m;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < h; j++) {
            int x;
            fin >> x;
            if (x == -2) {
                fout << "o "<< i + 1 << ' ' << j + 1;
                fin.close();
                fout.close();
            }

        }
    }

}