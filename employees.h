#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <random>
#include <algorithm>

static const double INVALID_SALARY = -1;

// объект описывающий работника
class TEmployee {
    size_t Id;
public:
    std::string Name;
    std::string Surname;
    std::string Position;
    std::string Department;
    double Salary;
    TEmployee(); // ID если не указывать, то инициализируется нулевым значением
    TEmployee(const std::string& nm, const std::string& surnm, const std::string& pst, const std::string& dp, const double& slr);
    size_t GetId() const;
    static size_t idCounter;
};

// генератор работников
class TEmployeeGenerator {
    std::map<size_t, TEmployee> randomDudes;
public:
    TEmployeeGenerator(const std::map<size_t, TEmployee>&);
    void GenerateRandomEmployees(size_t);
    std::map<size_t, TEmployee> GetMap() const;
};

// структура описывающая департамент
struct DepData {
    size_t employeesNumber;
    double salaryTotal;
};

void Split(const std::string& line, char delimiter, std::vector<std::string>* columns);
std::map<size_t, TEmployee> ReadEmployees(const std::string& fileName);

// удалить/добавить
void DeleteEmployeeById(std::map<size_t, TEmployee>* EmpMap, size_t idToDel);
void AddEmployee(std::map<size_t, TEmployee>* EmpMap, TEmployee& empToAdd);

// поиск
std::map<size_t, TEmployee> byDepartmentAndPositionMap(const std::map<size_t, TEmployee>& EmpMap,
            const std::string& depToFind, const std::string& positionToFind = "");
std::map<size_t, TEmployee> byNameAndSurnameMap(const std::map<size_t, TEmployee>& EmpMap,
            const std::string& nameToFind, const std::string& SurnameToFind = "");
std::map<size_t, TEmployee> bySalaryMap(const std::map<size_t, TEmployee>& EmpMap,
            const double max, const double min = 0);

// сортировать по з/п
bool CompareSalaryTEmpoyees(const TEmployee& lhv, const TEmployee& rhv);

bool comparePairLess(const std::pair<size_t, double>& lhv,
            const std::pair<size_t, double>& rhv);
bool comparePairMore(const std::pair<size_t, double>& lhv,
            const std::pair<size_t, double>& rhv);

std::vector<std::pair<size_t, double>> sortBySalary(const std::map<size_t, TEmployee>& EmpMap);

// получить контейнер из структур описывающих департаменты
std::map<std::string, DepData> getDepDateMap(const std::map<size_t, TEmployee>& EmpMap);
std::map<std::string, double> avgByDep(const std::map<std::string, DepData>& departments);

// использование контернеров для каждой характеристики отдельно
std::map<std::string, double> departmentAndSalary(const std::map<size_t, TEmployee>& empMap);
std::map<std::string, double> departmentAndStaffNum(const std::map<size_t, TEmployee>& empMap);
std::map<std::string, double> departmentAndAvgSalary(const std::map<size_t, TEmployee>& empMap);

std::ostream& operator<<(std::ostream& os, const TEmployee& empl);

// з/п
double GetMaxSalary(const std::map<size_t, TEmployee>& empMap);
double GetMinSalary(const std::map<size_t, TEmployee>& empMap);

//template <typename T, U>



