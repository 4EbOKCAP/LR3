#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <limits>

using namespace std;

// ==================== БАЗОВЫЙ КЛАСС HUMAN ====================
class Human {
protected:
    string lastName;
    string firstName;
    string patronymic;
    string address;

public:
    Human() : lastName(""), firstName(""), patronymic(""), address("") {}

    Human(string ln, string fn, string pn, string addr)
        : lastName(ln), firstName(fn), patronymic(pn), address(addr) {}

    Human(const Human& other)
        : lastName(other.lastName), firstName(other.firstName),
          patronymic(other.patronymic), address(other.address) {}

    virtual ~Human() {}

    string getLastName()   const { return lastName; }
    string getFirstName()  const { return firstName; }
    string getPatronymic() const { return patronymic; }
    string getAddress()    const { return address; }

    // Виртуальный метод show (без параметров) – будет переопределён
    virtual void show() const {
        cout << lastName << " " << firstName << " " << patronymic
             << ", адрес: " << address;
    }
};

// ==================== ПРОИЗВОДНЫЙ КЛАСС АБИТУРИЕНТ ====================
class Abiturient : public Human {
private:
    int mathScore;
    int rusScore;
    int infScore;

public:
    Abiturient() : Human(), mathScore(0), rusScore(0), infScore(0) {}

    Abiturient(string ln, string fn, string pn, string addr,
               int math, int rus, int inf)
        : Human(ln, fn, pn, addr), mathScore(math), rusScore(rus), infScore(inf) {}

    Abiturient(const Abiturient& other)
        : Human(other), mathScore(other.mathScore),
          rusScore(other.rusScore), infScore(other.infScore) {}

    int sum() const { return mathScore + rusScore + infScore; }

    // Переопределение виртуального show() – полный вывод
    void show() const override {
        Human::show();
        cout << ", баллы: мат=" << mathScore << ", рус=" << rusScore
             << ", инф=" << infScore << ", сумма=" << sum() << endl;
    }

    // ПЕРЕГРУЗКА 1: вывод только баллов и суммы
    void show(bool onlyScores) const {
        if (onlyScores) {
            cout << getLastName() << " " << getFirstName()
                 << ": мат=" << mathScore << ", рус=" << rusScore
                 << ", инф=" << infScore << ", сумма=" << sum() << endl;
        } else {
            show(); // полный вывод
        }
    }

    // ПЕРЕГРУЗКА 2: вывод абитуриента, если сумма баллов >= minSum
    void show(int minSum) const {
        if (sum() >= minSum) {
            show(); // полный вывод
        }
    }

    int getMath() const { return mathScore; }
    int getRus()  const { return rusScore; }
    int getInf()  const { return infScore; }
};

// ==================== ПРОИЗВОДНЫЙ КЛАСС ШКОЛЬНИК ====================
class Schoolchildren : public Human {
private:
    string schoolName;
    int grade;
    double averageScore;

public:
    Schoolchildren() : Human(), schoolName(""), grade(1), averageScore(0.0) {}

    Schoolchildren(string ln, string fn, string pn, string addr,
                   string school, int gr, double avg)
        : Human(ln, fn, pn, addr), schoolName(school), grade(gr), averageScore(avg) {}

    Schoolchildren(const Schoolchildren& other)
        : Human(other), schoolName(other.schoolName),
          grade(other.grade), averageScore(other.averageScore) {}

    void nextGrade() {
        if (grade < 11) {
            grade++;
            cout << getLastName() << " " << getFirstName()
                 << " перешёл(ла) в " << grade << " класс!" << endl;
        } else {
            cout << getLastName() << " " << getFirstName()
                 << " уже в выпускном классе (11)" << endl;
        }
    }

    // Переопределение виртуального show() – полный вывод
    void show() const override {
        Human::show();
        cout << ", школа: " << schoolName << ", класс: " << grade
             << ", средний балл: " << averageScore << endl;
    }

    // ПЕРЕГРУЗКА 1: вывод только школы и класса (краткий вариант)
    void show(bool onlySchool) const {
        if (onlySchool) {
            cout << getLastName() << " " << getFirstName()
                 << ": школа " << schoolName << ", класс " << grade << endl;
        } else {
            show();
        }
    }

    // ПЕРЕГРУЗКА 2: вывод школьника, если средний балл >= minAvg
    void show(double minAvg) const {
        if (averageScore >= minAvg) {
            show(); // полный вывод
        }
    }

    string getSchool() const { return schoolName; }
    int getGrade()    const { return grade; }
    double getAvg()   const { return averageScore; }
};

// ==================== ГЛАВНАЯ ФУНКЦИЯ ====================
int main() {
    setlocale(LC_ALL, "Russian");
    cout << "========== ЛАБОРАТОРНАЯ РАБОТА №3 (ВАРИАНТ 2) ==========" << endl;
    cout << "===== ДЕМОНСТРАЦИЯ ПЕРЕГРУЗКИ МЕТОДА show() =====\n" << endl;

    
    vector<Human*> people;

    // -------------------- ЗАГРУЗКА АБИТУРИЕНТОВ ИЗ ФАЙЛА --------------------
    cout << "--- Загрузка абитуриентов из файла abiturients.txt ---" << endl;
    ifstream f("abiturients.txt");
    if (!f.is_open()) {
        ofstream f2("abiturients.txt");
        f2 << "Иванов,Иван,Иванович,Москва,85,78,92\n"
           << "Петров,Петр,Петрович,СПб,45,60,30\n"
           << "Сидорова,Анна,Сергеевна,Казань,70,65,88\n"
           << "Козлов,Дмитрий,Алексеевич,Новосибирск,39,55,70\n"
           << "Смирнова,Елена,Владимировна,Екатеринбург,95,90,100\n"
           << "Васильев,Алексей,Андреевич,НижнийНовгород,50,42,35\n"
           << "Кузнецова,Мария,Петровна,Ростов,88,79,91\n"
           << "Новиков,Артем,Игоревич,Самара,60,55,48\n";
        f2.close();
        f.open("abiturients.txt");
    }

    string line;
    while (getline(f, line)) {
        size_t p1 = line.find(',');
        size_t p2 = line.find(',', p1 + 1);
        size_t p3 = line.find(',', p2 + 1);
        size_t p4 = line.find(',', p3 + 1);
        size_t p5 = line.find(',', p4 + 1);
        size_t p6 = line.find(',', p5 + 1);

        string ln = line.substr(0, p1);
        string fn = line.substr(p1 + 1, p2 - p1 - 1);
        string pn = line.substr(p2 + 1, p3 - p2 - 1);
        string addr = line.substr(p3 + 1, p4 - p3 - 1);
        int math = atoi(line.substr(p4 + 1, p5 - p4 - 1).c_str());
        int rus  = atoi(line.substr(p5 + 1, p6 - p5 - 1).c_str());
        int inf  = atoi(line.substr(p6 + 1).c_str());

        people.push_back(new Abiturient(ln, fn, pn, addr, math, rus, inf));
    }
    f.close();
    cout << "Загружено абитуриентов: " << people.size() << endl;

    // -------------------- СОЗДАНИЕ ШКОЛЬНИКОВ --------------------
    cout << "\n--- Создание школьников ---" << endl;
    Schoolchildren* s1 = new Schoolchildren("Смирнов", "Алексей", "Викторович",
                                            "Москва", "Гимназия №1", 9, 4.7);
    Schoolchildren* s2 = new Schoolchildren("Кузнецова", "Ольга", "Дмитриевна",
                                            "СПб", "Школа №123", 10, 4.9);
    Schoolchildren* s3 = new Schoolchildren("Попов", "Николай", "Андреевич",
                                            "Казань", "Лицей №5", 8, 4.2);
    Schoolchildren* s4 = new Schoolchildren("Васильева", "Анна", "Игоревна",
                                            "Новосибирск", "Школа №7", 11, 4.8);
    people.push_back(s1);
    people.push_back(s2);
    people.push_back(s3);
    people.push_back(s4);
    cout << "Добавлено школьников: 4" << endl;

    // -------------------- ВЫВОД ВСЕХ ОБЪЕКТОВ ЧЕРЕЗ ПОЛИМОРФНЫЙ show() --------------------
    cout << "\n========== ВСЕ ОБЪЕКТЫ (полиморфный show) ==========" << endl;
    for (auto* p : people) {
        p->show();
    }

    // -------------------- ДЕМОНСТРАЦИЯ ПЕРЕГРУЗКИ show() ДЛЯ ABITURIENT --------------------
    cout << "\n========== ПЕРЕГРУЗКА show() В КЛАССЕ Abiturient ==========" << endl;
    // Получаем указатель на первого абитуриента в массиве (не школьника)
    Abiturient* firstAb = nullptr;
    for (auto* p : people) {
        firstAb = dynamic_cast<Abiturient*>(p);
        if (firstAb) break;
    }
    if (firstAb) {
        cout << "1) Полный вывод (show()):" << endl;
        firstAb->show();
        cout << "\n2) Краткий вывод (show(true)):" << endl;
        firstAb->show(true);
        cout << "\n3) Фильтр по сумме баллов (show(200)) - только если сумма >=200:" << endl;
        firstAb->show(200);
        cout << "   (у данного абитуриента сумма = " << firstAb->sum() << ")" << endl;
    }

    // -------------------- ДЕМОНСТРАЦИЯ ПЕРЕГРУЗКИ show() ДЛЯ SCHOOLCHILDREN --------------------
    cout << "\n========== ПЕРЕГРУЗКА show() В КЛАССЕ Schoolchildren ==========" << endl;
    Schoolchildren* firstSc = s1; // берём первого созданного
    if (firstSc) {
        cout << "1) Полный вывод (show()):" << endl;
        firstSc->show();
        cout << "\n2) Краткий вывод (show(true)):" << endl;
        firstSc->show(true);
        cout << "\n3) Фильтр по среднему баллу (show(4.8)) - только если средний балл >=4.8:" << endl;
        firstSc->show(4.8);
        cout << "   (у данного школьника средний балл = " << firstSc->getAvg() << ")" << endl;
    }

    // -------------------- ПЕРЕГРУЗКА show() ДЛЯ ВСЕХ АБИТУРИЕНТОВ (пример с фильтром) --------------------
    cout << "\n========== ПЕРЕГРУЗКА show(int) ДЛЯ ВСЕХ АБИТУРИЕНТОВ ==========" << endl;
    int sumThreshold;
    cout << "Введите порог суммы баллов для абитуриентов: ";
    cin >> sumThreshold;
    cout << "Абитуриенты с суммой >= " << sumThreshold << ":" << endl;
    for (auto* p : people) {
        Abiturient* a = dynamic_cast<Abiturient*>(p);
        if (a) {
            a->show(sumThreshold);   
        }
    }

    // -------------------- ПЕРЕГРУЗКА show(double) ДЛЯ ВСЕХ ШКОЛЬНИКОВ --------------------
    cout << "\n========== ПЕРЕГРУЗКА show(double) ДЛЯ ВСЕХ ШКОЛЬНИКОВ ==========" << endl;
    double avgThreshold;
    cout << "Введите порог среднего балла для школьников: ";
    cin >> avgThreshold;
    cout << "Школьники со средним баллом >= " << avgThreshold << ":" << endl;
    for (auto* p : people) {
        Schoolchildren* sc = dynamic_cast<Schoolchildren*>(p);
        if (sc) {
            sc->show(avgThreshold);   
        }
    }

    // -------------------- ОЧИСТКА ПАМЯТИ --------------------
    cout << "\n========== ОСВОБОЖДЕНИЕ ПАМЯТИ ==========" << endl;
    for (auto* p : people) {
        delete p;
    }
    people.clear();
    cout << "Все объекты удалены." << endl;

    return 0;
}