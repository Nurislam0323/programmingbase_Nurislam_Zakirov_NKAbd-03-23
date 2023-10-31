#include <iostream>
#include <vector>
#include <string>
using namespace std;
class Animal {
public:
    Animal(const string& name, int age, const string& activity)
        : name(name), age(age), activity(activity), status("Healthy") {}

    void SetStatus(const std::string& newStatus) {
        status = newStatus;
    }
    void Run() {
        cout << name << " is running." << endl;
    }

    void Jump() {
        cout << name << " is jumping." << endl;
    }

    virtual void MakeSound() {
        cout << "Animal sound" << endl;
    }

    void Feed() {
        cout << name << " is being fed." << endl;
        MakeSound();
    }
    virtual string GetFeauter() {
        return "";
    }
    void GetStatus() {
        cout << "Name: " << name << "\n"
            << "Age: " << age << "\n"
            << "Activity: " << activity << "\n"
            << "Status: " << status << "\n"
            << GetFeauter()<<"\n";
    }

protected:
    string name;
    int age;
    string activity;
    string status;
};

class Cat : public Animal {
public:
    Cat(const string& name, int age, const string& activity, const string& furColor)
        : Animal(name, age, activity), furColor(furColor) {}

    void ClimbTree() {
        cout << name << " is climbing a tree." << endl;
    }

    void MakeSound() override {
        cout << name << " meows." << endl;
    }
    string GetFeauter() override {
        return name + "'s fur color is " + furColor;
    }

private:
    string furColor;
};

class Dog : public Animal {
public:
    Dog(const string& name, int age, const string& activity, const string& breed)
        : Animal(name, age, activity), breed(breed) {}

    void Play() {
        cout << name << " играет с мячиком" << endl;
    }

    void MakeSound() override {
        cout << name << " rrrafs." << endl;
    }
    string GetFeauter() override {
        return name + "'s breed is " + breed;
    }

private:
    string breed;
};

class Elephant : public Animal {
public:
    Elephant(const string& name, int age, const string& activity, int trunkLength)
        : Animal(name, age, activity), trunkLength(trunkLength) {}

    void SprayWater() {
        cout << name << " is spraying water with its trunk." << endl;
    }

    void MakeSound() override {
        cout << name << " trumpets." << endl;
    }
    string GetFeauter() override {
        return name + "'s trunk lenght is " + to_string(trunkLength);
    }

private:
    int trunkLength;
};

int main() {
    setlocale(LC_ALL, "Russian");
    vector<Animal*> zoo;

    Cat cat("Whiskers", 5, "Playing with a ball of yarn", "Gray");
    Dog dog("Rex", 3, "Chasing its tail", "Golden Retriever");
    Elephant elephant("Dumbo", 10, "Taking a bath", 2);

    zoo.push_back(&cat);
    zoo.push_back(&dog);
    zoo.push_back(&elephant);

    for (const auto& animal : zoo) {
        animal->Feed();
        animal->GetStatus();
    }

    return 0;
}