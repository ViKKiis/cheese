#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include "PatternTemplates1.h"

using namespace std;

enum class CheeseType : int {
    Mozzarella = 1,
    Cheddar = 2,
    Gorgonzola = 3,
    Undefined = 0
};

class ServingStrategy {
public:
    virtual ~ServingStrategy() {}
    virtual void Serve() = 0;
};

class WhiteWineServing : public ServingStrategy {
public:
    void Serve() override {
        wcout << L"Сочетается с белым вином." << endl;
    }
};

class RedWineServing : public ServingStrategy {
public:
    void Serve() override {
        wcout << L"Сочетается с красным вином." << endl;
    }
};

class DessertWineServing : public ServingStrategy {
public:
    void Serve() override {
        wcout << L"Сочетается с десертным вином." << endl;
    }
};

class EatingStrategy {
public:
    virtual ~EatingStrategy() {}
    virtual void Eat() = 0;
};

class EnjoySlowlyEatingStrategy : public EatingStrategy {
public:
    void Eat() override {
        wcout << L"Данный сыр лучше есть неспеша, наслаждаясь его нежным вкусом " << endl;
    }
};

class ChewThoroughlyEatingStrategy : public EatingStrategy {
public:
    void Eat() override {
        wcout << L"Сыр нужно тщательно разжевывать, чтобы ощутить его текстуру" << endl;
    }
};

class SliceThinEatingStrategy : public EatingStrategy {
public:
    void Eat() override {
        wcout << L"Тонко нарежьте сыр на кусочки и подайте на стол" << endl;
    }
};

ServingStrategy* CreateServingStrategy(CheeseType cheeseType) {
    switch (cheeseType) {
        case CheeseType::Mozzarella: return new WhiteWineServing;
        case CheeseType::Cheddar: return new RedWineServing;
        case CheeseType::Gorgonzola: return new DessertWineServing;
        default: return nullptr;
    }
}

class Cheese {
private:
    double Weight;
    ServingStrategy* servingStrategy;  // подача,с каким вином сочетается
    EatingStrategy* eatingStrategy;  // советы, как есть сыр

protected:
    wstring Name;
    wstring Origin;
    int Age;
    bool IsTasty;

public:
    Cheese(const wstring& name, const wstring& origin, int age, double weight)
        : Name(name), Origin(origin), Age(age), Weight(weight), IsTasty(false), servingStrategy(nullptr), eatingStrategy(nullptr) {}

    virtual ~Cheese() {
        delete servingStrategy;
        delete eatingStrategy;
    }

    void SetServingStrategy(ServingStrategy* strategy) {
        servingStrategy = strategy;
    }

    void SetEatingStrategy(EatingStrategy* strategy) {
        eatingStrategy = strategy;
    }

    void Info() const {
        wcout << L"Сыр: " << Name << L", Происхождение: " << Origin
               << L", Возраст: " << Age << L" месяцев" << L", Вес: " << Weight << L" кг" << endl;
    }

    virtual void Taste() = 0;

    void Wine() {
        if (servingStrategy) {
            servingStrategy->Serve();
        }
    }

    void Eat() {
        if (eatingStrategy) {
            eatingStrategy->Eat();
        }
    }

    void ServeCheese() {
        Info();
        Taste();
        Wine();
        Eat();
    }

    bool IsGood() const { return IsTasty; }
    void SetTasty(bool tasty) { IsTasty = tasty; }
    int GetAge() const { return Age; }
    double GetWeight() const { return Weight; }
    wstring GetOrigin() const { return Origin; }
};

class Mozzarella : public Cheese {
public:
    Mozzarella() : Cheese(L"Моцарелла", L"Италия", 7, 0.20) {
        SetServingStrategy(new WhiteWineServing());
        SetEatingStrategy(new EnjoySlowlyEatingStrategy());
    }


    void Taste() override {
        wcout << L"Моцарелла мягкая и сливочная." << endl;
    }
};

class Cheddar : public Cheese {
public:
    Cheddar() : Cheese(L"Чеддер", L"Англия", 12, 1.63) {
        SetServingStrategy(new RedWineServing());
        SetEatingStrategy(new ChewThoroughlyEatingStrategy());
    }

    void Taste() override {
        wcout << L"Чеддер имеет острый и насыщенный вкус." << endl;
    }
};

class Gorgonzola : public Cheese {
public:
    Gorgonzola() : Cheese(L"Горгонзола", L"Италия", 2, 1.15) {
        SetServingStrategy(new DessertWineServing());
        SetEatingStrategy(new SliceThinEatingStrategy());
    }

    void Taste() override {
        wcout << L"Горгонзола обладает сильным и острым вкусом." << endl;
    }
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
        currentCheese->Eat();
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


    wcout << L"Какой сыр создать (1 - Mozzarella, 2 - Cheddar, 3 - Gorgonzola)? ";
    CheeseType type = CheeseType::Undefined;
    int ii;
    cin >> ii;

    type = static_cast<CheeseType>(ii);
    Cheese* newCheese = CreateCheese(type);

    newCheese->ServeCheese();

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

    return 0;
}




