#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include "PatternTemplates1.h"

using namespace std;

class Cheese {
private:
    double Weight;

protected:
    wstring Name;
    wstring Origin;
    int Age;
    bool IsTasty;

public:
    Cheese(const wstring& name, const wstring& origin, int age, double weight)
        : Name(name), Origin(origin), Age(age), Weight(weight), IsTasty(false) {}

    virtual ~Cheese() {}

    void Info() const {
        wcout << L"Сыр: " << Name << L", Происхождение: " << Origin
            << L", Возраст: " << Age << L" месяцев" << L", Вес: " << Weight << L" кг" << endl;
    }

    virtual void Taste() = 0;
    virtual void Wine() = 0;

    bool IsGood() const { return IsTasty; }
    void SetTasty(bool tasty) { IsTasty = tasty; }
    int GetAge() const { return Age; }
    double GetWeight() const { return Weight; }
    wstring GetOrigin() const { return Origin; }
};

class Mozzarella : public Cheese {
public:
    Mozzarella() : Cheese(L"Моцарелла", L"Италия", 7, 0.20) {}
    ~Mozzarella() {}

    void Taste() override {
        wcout << L"Моцарелла мягкая и сливочная." << endl;
    }

    void Wine() override {
        wcout << L"Сочетается с белым вином." << endl;
    }
};

class Cheddar : public Cheese {
public:
    Cheddar() : Cheese(L"Чеддер", L"Англия", 12, 1.63) {}
    ~Cheddar() {}

    void Taste() override {
        wcout << L"Чеддер имеет острый и насыщенный вкус." << endl;
    }

    void Wine() override {
        wcout << L"Сочетается с красным вином." << endl;
    }
};

class Gorgonzola : public Cheese {
public:
    Gorgonzola() : Cheese(L"Горгонзола", L"Италия", 2, 1.15) {}
    ~Gorgonzola() {}

    void Taste() override {
        wcout << L"Горгонзола обладает сильным и острым вкусом." << endl;
    }

    void Wine() override {
        wcout << L"Хорошо сочетается с десертным вином." << endl;
    }
};

enum class CheeseType : int {
    Mozzarella = 1,
    Cheddar = 2,
    Gorgonzola = 3,
    Undefined = 0
};

Cheese* CreateCheese(CheeseType type) {
    if (type == CheeseType::Mozzarella) return new Mozzarella;
    else if (type == CheeseType::Cheddar) return new Cheddar;
    else if (type == CheeseType::Gorgonzola) return new Gorgonzola;
    return nullptr;
}

void CheeseEmAll(Iterator<Cheese*>* it) {
    for (it->First(); !it->IsDone(); it->Next()) {
        Cheese* currentCheese = it->GetCurrent();
        currentCheese->Info();
        currentCheese->Taste();
        currentCheese->Wine();
    }
}

class CheeseWeightDecorator : public IteratorDecorator<Cheese*> {
private:
    double MinWeight;
public:
    CheeseWeightDecorator(Iterator<Cheese*>* it, double minWeight)
        : IteratorDecorator<Cheese*>(it), MinWeight(minWeight) {}

    void First() override {
        It->First();
        while (!It->IsDone() && It->GetCurrent()->GetWeight() <= MinWeight) {
            It->Next();
        }
    }

    void Next() override {
        do {
            It->Next();
        } while (!It->IsDone() && It->GetCurrent()->GetWeight() <= MinWeight);
    }
};

class CheeseOriginDecorator : public IteratorDecorator<Cheese*> {
private:
    wstring Origin;
public:
    CheeseOriginDecorator(Iterator<Cheese*>* it, const wstring& origin)
        : IteratorDecorator<Cheese*>(it), Origin(origin) {}

    void First() override {
        It->First();
        while (!It->IsDone() && It->GetCurrent()->GetOrigin() != Origin) {
            It->Next();
        }
    }

    void Next() override {
        do {
            It->Next();
        } while (!It->IsDone() && It->GetCurrent()->GetOrigin() != Origin);
    }
};

class CheeseAgeDecorator : public IteratorDecorator<Cheese*> {
private:
    int MinAge;
public:
    CheeseAgeDecorator(Iterator<Cheese*>* it, int minAge)
        : IteratorDecorator<Cheese*>(it), MinAge(minAge) {}

    void First() override {
        It->First();
        while (!It->IsDone() && It->GetCurrent()->GetAge() <= MinAge) {
            It->Next();
        }
    }

    void Next() override {
        do {
            It->Next();
        } while (!It->IsDone() && It->GetCurrent()->GetAge() <= MinAge);
    }
};

int main() {
    setlocale(LC_ALL, "");

    wcout << L"Какой сыр создать (1 - Mozzarella, 2 - Cheddar, 3 - Gorgonzola)?" << endl;
    CheeseType type = CheeseType::Undefined;
    int ii;
    cin >> ii;
    type = static_cast<CheeseType>(ii);

    Cheese* newCheese = CreateCheese(type);
    newCheese->Info();
    newCheese->Taste();
    newCheese->Wine();
    delete newCheese;

    size_t N = 0;
    wcout << L"Введите количество сыров: ";
    cin >> N;
    cout << endl;

    StackClass<Cheese*> cheeseStack;
    for (size_t i = 0; i < N; i++) {
        int cheese_num = rand() % 3 + 1;
        CheeseType cheese_type = static_cast<CheeseType>(cheese_num);
        Cheese* newCheese = CreateCheese(cheese_type);
        cheeseStack.Push(newCheese);
    }

    wcout << L"Размер стека сыров: " << cheeseStack.Size() << endl;

    Iterator<Cheese*>* it2 = cheeseStack.GetIterator();
    CheeseEmAll(it2);
    delete it2;

    cout << endl;

    ArrayClass<Cheese*> cheeseArray;
    for (size_t i = 0; i < N; i++) {
        int cheese_num = rand() % 3 + 1;
        CheeseType cheese_type = static_cast<CheeseType>(cheese_num);
        Cheese* newCheese = CreateCheese(cheese_type);
        cheeseArray.Add(newCheese);
    }

    wcout << L"Размер массива сыров: " << cheeseArray.Size() << endl;

    Iterator<Cheese*>* it3 = cheeseArray.GetIterator();
    cout << "_____" << endl;

    Iterator<Cheese*>* weightIt = new CheeseWeightDecorator(it3, 1.0);
    wcout << L"Сыры с весом больше 1 кг:" << endl;
    CheeseEmAll(weightIt);
    delete weightIt;
    cout << "_____" << endl;

    Iterator<Cheese*>* it4 = cheeseArray.GetIterator();
    Iterator<Cheese*>* originIt = new CheeseOriginDecorator(it4, L"Италия");
    wcout << L"Сыры с происхождением Италия:" << endl;
    CheeseEmAll(originIt);
    delete originIt;
    cout << "_____" << endl;

    Iterator<Cheese*>* it5 = cheeseArray.GetIterator();
    Iterator<Cheese*>* ageIt = new CheeseAgeDecorator(it5, 5);
    wcout << L"Сыры старше 5 месяцев:" << endl;
    CheeseEmAll(ageIt);
    delete ageIt;
    delete it5;

    std::list<Cheese*> cheeseList;
    for (size_t i = 0; i < N; i++) {
        int cheese_num = rand() % 3 + 1;
        CheeseType cheese_type = static_cast<CheeseType>(cheese_num);
        Cheese* newCheese = CreateCheese(cheese_type);
        cheeseList.push_back(newCheese);
    }

    wcout << L"Размер списка сыров (list): " << cheeseList.size() << endl;

    Iterator<Cheese*>* adaptedIt = new ConstIteratorAdapter<std::list<Cheese*>, Cheese*>(&cheeseList);
    Iterator<Cheese*>* filteredIt = new CheeseOriginDecorator(adaptedIt, L"Италия");

    CheeseEmAll(filteredIt);

    delete filteredIt;

    return 0;
}




