#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <iostream>
#include <string>
#include <complex>
#include <functional>
#include <ctime>
#include <memory>

typedef std::complex<double> Complex;

typedef std::function<double(double)> MathFunction;

struct PersonID {
    int series;
    int number;

    PersonID() : series(0), number(0) {
    }

    PersonID(int s, int n) : series(s), number(n) {
    }

    bool operator==(const PersonID &other) const {
        return series == other.series && number == other.number;
    }

    bool operator!=(const PersonID &other) const {
        return !(*this == other);
    }

    bool operator<(const PersonID &other) const {
        if (series != other.series) {
            return series < other.series;
        }
        return number < other.number;
    }

    bool operator>(const PersonID &other) const {
        return other < *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const PersonID &id) {
        os << id.series << "-" << id.number;
        return os;
    }
};

class Person {
protected:
    PersonID id;
    std::string firstName;
    std::string middleName;
    std::string lastName;
    std::time_t birthDate;

public:
    Person() : birthDate(0) {
    }

    Person(const PersonID &personId, const std::string &first, const std::string &middle,
           const std::string &last, std::time_t birth)
        : id(personId), firstName(first), middleName(middle), lastName(last), birthDate(birth) {
    }

    const PersonID &GetID() const { return id; }
    const std::string &GetFirstName() const { return firstName; }
    const std::string &GetMiddleName() const { return middleName; }
    const std::string &GetLastName() const { return lastName; }
    std::time_t GetBirthDate() const { return birthDate; }

    std::string GetFullName() const {
        return firstName + " " + middleName + " " + lastName;
    }

    bool operator==(const Person &other) const {
        return id == other.id;
    }

    bool operator!=(const Person &other) const {
        return !(*this == other);
    }

    bool operator<(const Person &other) const {
        return id < other.id;
    }

    bool operator>(const Person &other) const {
        return other < *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const Person &person) {
        os << person.GetFullName() << " (ID: " << person.id << ")";
        return os;
    }
};

class Student : public Person {
private:
    std::string group;
    int studentID;
    double averageGrade;

public:
    Student() : studentID(0), averageGrade(0.0) {
    }

    Student(const PersonID &personId, const std::string &first, const std::string &middle,
            const std::string &last, std::time_t birth, const std::string &studentGroup,
            int id, double avgGrade)
        : Person(personId, first, middle, last, birth),
          group(studentGroup), studentID(id), averageGrade(avgGrade) {
    }

    const std::string &GetGroup() const { return group; }
    int GetStudentID() const { return studentID; }
    double GetAverageGrade() const { return averageGrade; }

    void SetAverageGrade(double grade) { averageGrade = grade; }

    friend std::ostream &operator<<(std::ostream &os, const Student &student) {
        os << "Student: " << student.GetFullName()
                << " (ID: " << student.id << "), Group: " << student.group
                << ", Student ID: " << student.studentID
                << ", Avg. Grade: " << student.averageGrade;
        return os;
    }
};

class Teacher : public Person {
private:
    std::string department;
    std::string position;
    int experience; // Стаж работы в годах

public:
    Teacher() : experience(0) {
    }

    Teacher(const PersonID &personId, const std::string &first, const std::string &middle,
            const std::string &last, std::time_t birth, const std::string &dept,
            const std::string &pos, int exp)
        : Person(personId, first, middle, last, birth),
          department(dept), position(pos), experience(exp) {
    }

    const std::string &GetDepartment() const { return department; }
    const std::string &GetPosition() const { return position; }
    int GetExperience() const { return experience; }

    friend std::ostream &operator<<(std::ostream &os, const Teacher &teacher) {
        os << "Teacher: " << teacher.GetFullName()
                << " (ID: " << teacher.id << "), Department: " << teacher.department
                << ", Position: " << teacher.position
                << ", Experience: " << teacher.experience << " years";
        return os;
    }
};

#endif
