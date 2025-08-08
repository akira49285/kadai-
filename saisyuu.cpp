#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <algorithm>

using namespace std;

class kessai {
private:
    string date;      
    string category;
    string description;
    int amount;

public:
    kessai(string date, string category, string description, int amount)
        : date(date), category(category), description(description), amount(amount) {
    }

    string getDate() const { return date; }
    string getCategory() const { return category; }
    string getDescription() const { return description; }
    int getAmount() const { return amount; }

    string getMonth() const {
        return date.substr(0, 7); 
    }

    string toCSV() const {
        stringstream ss;
        ss << date << "," << category << "," << description << "," << amount;
        return ss.str();
    }

    static kessai fromCSV(const string& line) {
        stringstream ss(line);
        string date, category, description, amountStr;
        getline(ss, date, ',');
        getline(ss, category, ',');
        getline(ss, description, ',');
        getline(ss, amountStr, ',');
        return kessai(date, category, description, stoi(amountStr));
    }

    void display() const {
        cout << left << setw(12) << date
            << setw(15) << category
            << setw(25) << description
            << right << setw(8) << amount << " 円" << endl;
    }
};

class HouseholdAccountBook {
private:
    vector<kessai> kessais;

public:
    void addkessai(const kessai& kessai) {
        kessais.push_back(kessai);
        cout << "決済を追加しました。\n";
    }

    void showAllkessais() const {
        cout << "\n【家計簿一覧】\n";
        if (kessais.empty()) {
            cout << "決済が登録されていません。\n";
            return;
        }

        for (auto it = kessais.begin(); it != kessais.end(); ++it) {
            it->display();
        }
    }

    void showMonthlySummary() const {
        map<string, int> monthSum;
        for (auto it = kessais.begin(); it != kessais.end(); ++it) {
            monthSum[it->getMonth()] += it->getAmount();
        }

        cout << "\n【月別決済合計】\n";
        for (const auto& pair : monthSum) {
            cout << pair.first << " の合計: " << pair.second << " 円\n";
        }
    }

    void saveToFile(const string& filename) const {
        ofstream ofs(filename);
        for (auto it = kessais.begin(); it != kessais.end(); ++it) {
            ofs << it->toCSV() << "\n";
        }
        cout << "ファイルに保存しました。\n";
    }

    void loadFromFile(const string& filename) {
        ifstream ifs(filename);
        if (!ifs) {
            cout << "ファイルが存在しません。新規作成します。\n";
            return;
        }

        string line;
        while (getline(ifs, line)) {
            kessais.push_back(kessai::fromCSV(line));
        }

        cout << "ファイルから読み込みました。\n";
    }
};


string inputDate() {
    string date;
    while (true) {
        cout << "日付 (YYYY-MM-DD): ";
        cin >> date;
        if (date.size() == 10 && date[4] == '-' && date[7] == '-') break;
        cout << "日付形式が正しくありません。\n";
    }
    return date;
}

int main() {
    HouseholdAccountBook book;
    book.loadFromFile("kessais.csv");

    int choice;
    while (true) {
        cout << "\n--- 家計簿アプリ ---\n";
        cout << "1. 収入を追加\n";
        cout << "2. 収入を表示\n";
        cout << "3. 月別集計\n";
        cout << "4. ファイルに保存\n";
        cout << "5. 終了\n";
        cout << "選択肢を入力: ";
        cin >> choice;

        if (choice == 1) {
            string date = inputDate();
            string category, description;
            int amount;

            cout << "カテゴリ: ";
            cin >> category;
            cout << "内容: ";
            cin.ignore();
            getline(cin, description);
            cout << "金額（円）: ";
            cin >> amount;

            kessai exp(date, category, description, amount);
            book.addkessai(exp);
        }
        else if (choice == 2) {
            book.showAllkessais();
        }
        else if (choice == 3) {
            book.showMonthlySummary();
        }
        else if (choice == 4) {
            book.saveToFile("kessais.csv");
        }
        else if (choice == 5) {
            cout << "終了します。\n";
            break;
        }
        else {
            cout << "無効な選択です。\n";
        }
    }

    return 0;
}
