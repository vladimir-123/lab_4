#include "employees.h"

size_t TEmployee::idCounter = 0; // уникальный айдишник

TEmployee::TEmployee():
            Name("Error_TEmployee_Name"), Surname("Error_TEmployee_Surname"),
            Position("Error_TEmployee_Position"), Department("Error_TEmployee_Department"),
            Salary(INVALID_SALARY) {}

// ID теперь гененрируется автоматически
TEmployee::TEmployee(const std::string& nm, const std::string& surnm, const std::string& pst,
            const std::string& dp, const double& slr):
            Name(nm), Surname(surnm), Position(pst), Department(dp), Salary(slr)
{
    ++TEmployee::idCounter; // изменяю айдишник
    Id = TEmployee::idCounter;
}

size_t TEmployee::GetId() const {
    return Id;
}

void Split(const std::string& line,
           char delimiter,
           std::vector<std::string>* columns)
{
    size_t begin = 0;
    for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] == delimiter) {
            std::string word = line.substr(begin, i - begin);
            columns->push_back(word);
            ++i;
            begin = i;
        }
    }
    std::string word = line.substr(begin);
    columns->push_back(word);
}

std::map<size_t, TEmployee> ReadEmployees(const std::string& fileName) {
    std::ifstream inputFile(fileName);
    std::string line;
    std::map<size_t, TEmployee> employees;

    std::getline(inputFile, line);

    while (std::getline(inputFile, line)) {
        //разделюя строку запятыми
        std::vector<std::string> columns;
        Split(line, ',', &columns);
        //убираю знак доллара
        columns[3] = columns[3].substr(1, columns[3].size() - 1);
        //отделяю имя от фамилии
        std::vector<std::string> subColumns;
        Split(columns[0], ';', &subColumns);
        //изменяю имя и фамилию
        subColumns[1].pop_back();
        subColumns[0] = subColumns[0].substr(1, subColumns[0].size() - 1);
        //создаю объект
        TEmployee emp(subColumns[1], subColumns[0], columns[1], columns[2], std::stod(columns[3]));
        employees[emp.GetId()] = emp;
    }
    return employees;
}

void DeleteEmployeeById(std::map<size_t, TEmployee>* EmpMap, size_t idToDel) {
    if (EmpMap->find(idToDel) != EmpMap->end()) {
        EmpMap->erase(idToDel);
    }
}

void AddEmployee(std::map<size_t, TEmployee>* EmpMap, TEmployee& empToAdd) {
    if (EmpMap->find(empToAdd.GetId()) != EmpMap->end()) {
        return;
    } else {
        EmpMap->insert(std::pair<size_t, TEmployee> (empToAdd.GetId(), empToAdd));
    }
}

std::map<size_t, TEmployee> byDepartmentAndPositionMap(const std::map<size_t, TEmployee>& EmpMap,
            const std::string& depToFind, const std::string& positionToFind) {
    std::map<size_t, TEmployee> temp;
    if ("" == positionToFind) {
        for (const auto& x : EmpMap) {
            if (x.second.Department == depToFind) {
                temp.insert(x);
            }
        }
    } else {
        for (const auto& x : EmpMap) {
            if (x.second.Department == depToFind && x.second.Position == positionToFind) {
                temp.insert(x);
            }
        }
    }
    return temp;
}

// департамент описывается штатом и бюджетом зп
std::map<std::string, DepData> getDepDateMap(const std::map<size_t, TEmployee>& EmpMap) {
    std::map<std::string, DepData> temp;
    for (const auto& x : EmpMap) {
        ++temp[x.second.Department].employeesNumber; // штат
        temp[x.second.Department].salaryTotal += x.second.Salary; // бюджет
    }
    return temp;
}

std::map<std::string, double> avgByDep(const std::map<std::string, DepData>& departments) {
    std::map<std::string, double> temp;
    for (const auto& x : departments) {
        temp[x.first] = x.second.salaryTotal/x.second.employeesNumber;
    }
    return temp;
}

std::map<std::string, double> departmentAndSalary(const std::map<size_t, TEmployee>& empMap) {
    std::map<std::string, double> temp;
    for(const auto& x : empMap) {
        temp[x.second.Department] += x.second.Salary;
    }
    return temp;
}

std::map<std::string, double> departmentAndStaffNum(const std::map<size_t, TEmployee>& empMap) {
    std::map<std::string, double> temp;
    for(const auto& x : empMap) {
        ++temp[x.second.Department];
    }
    return temp;
}

std::map<std::string, double> departmentAndAvgSalary(const std::map<size_t, TEmployee>& empMap) {
    std::map<std::string, double> temp;
    std::map<std::string, double> depAndSal = departmentAndSalary(empMap);
    std::map<std::string, double> depAndStaff = departmentAndStaffNum(empMap);
    for(const auto& x : empMap) {
        temp[x.second.Department] = depAndSal[x.second.Department] / depAndStaff[x.second.Department];
    }
    return temp;
}

std::ostream& operator<<(std::ostream& os, const TEmployee& empl) {
    os << empl.Name << " " << empl.Surname << ", "
       << empl.Department << ": " << empl.Position << " $" << empl.Salary << "; ID: " << empl.GetId();
       return os;
}

double GetMaxSalary(const std::map<size_t, TEmployee>& empMap) {
    double max = empMap.begin()->second.Salary;
    for(const std::pair<size_t, TEmployee>& x : empMap) {
        if (x.second.Salary > max) {
            max = x.second.Salary;
        }
    }
    return max;
}

double GetMinSalary(const std::map<size_t, TEmployee>& empMap) {
    double min = empMap.begin()->second.Salary;
    for(const std::pair<size_t, TEmployee>& x : empMap) {
        if (x.second.Salary < min) {
            min = x.second.Salary;
        }
    }
    return min;
}


TEmployeeGenerator::TEmployeeGenerator(const std::map<size_t, TEmployee>& Dudes): randomDudes(Dudes) {}

void TEmployeeGenerator::GenerateRandomEmployees(size_t N) {
    std::map<size_t, TEmployee> GivenMap = randomDudes;
    randomDudes.clear();
    
    std::random_device rd;
    size_t size = GivenMap.size();

    std::map<size_t, TEmployee>::iterator randomName;
    std::map<size_t, TEmployee>::iterator randomSurame;
    std::map<size_t, TEmployee>::iterator randomPosition;
    std::map<size_t, TEmployee>::iterator randomDepartment;
    size_t salaryRange = GetMaxSalary(GivenMap) - GetMinSalary(GivenMap);
    double randomSalary;

    for (int i = 0; i < N; ++i) {
        randomName = std::next(GivenMap.begin(), rd() % size);
        randomSurame = std::next(GivenMap.begin(), rd() % size);
        randomPosition = std::next(GivenMap.begin(), rd() % size);
        randomDepartment = std::next(GivenMap.begin(), rd() % size);
        randomSalary = rd() % salaryRange;
        TEmployee temp (randomName->second.Name, randomSurame->second.Surname,
            randomPosition->second.Position, randomDepartment->second.Department, randomSalary);
        AddEmployee(&randomDudes, temp);
    }
}

std::map<size_t, TEmployee> TEmployeeGenerator::GetMap() const {
    return randomDudes;
}

std::map<size_t, TEmployee> byNameAndSurnameMap(const std::map<size_t, TEmployee>& EmpMap,
            const std::string& nameToFind, const std::string& SurnameToFind) {
    std::map<size_t, TEmployee> temp;
    if ("" == SurnameToFind) {
        std::string tempName;
        std::string tempSurname;

        for (const auto& x : EmpMap) {
            tempName = x.second.Name.substr(0, nameToFind.size());
            tempSurname = x.second.Surname.substr(0, nameToFind.size());

            if (tempName == nameToFind || tempSurname == nameToFind) {
                temp.insert(x);
            }
        }
    } else {
        for (const auto& x : EmpMap) {
            if (x.second.Name == nameToFind && x.second.Surname == SurnameToFind) {
                temp.insert(x);
            }
        }
    }
    return temp;
}

std::map<size_t, TEmployee> bySalaryMap(const std::map<size_t, TEmployee>& EmpMap,
            const double max, const double min) {
    std::map<size_t, TEmployee> temp;
    if (0 == min) {
        for (const auto& x : EmpMap) {
            if (x.second.Salary >= max) {
                temp.insert(x);
            }
        }
    } else {
        for (const auto& x : EmpMap) {
            if (x.second.Salary <= max && x.second.Salary >= min) {
                temp.insert(x);
            }
        }
    }
    return temp;
}

bool CompareSalaryTEmpoyees(const TEmployee& lhv, const TEmployee& rhv) {
    return (lhv.Salary < rhv.Salary);
}

bool comparePairLess(const std::pair<size_t, double>& lhv,
            const std::pair<size_t, double>& rhv) {
    return (lhv.second < rhv.second);
}

bool comparePairMore(const std::pair<size_t, double>& lhv,
            const std::pair<size_t, double>& rhv) {
    return (lhv.second > rhv.second);
}

std::vector<std::pair<size_t, double>> sortBySalary(const std::map<size_t, TEmployee>& EmpMap) {
    std::vector<std::pair<size_t, double>> VSalary;
    VSalary.reserve(EmpMap.size());
    std::pair<size_t, double> tempPair;

    for (const auto& x : EmpMap) {
        tempPair = std::make_pair(x.first, x.second.Salary);
        VSalary.push_back(tempPair);
    }
    std::sort(VSalary.begin(), VSalary.end(), comparePairMore);
    return VSalary;
}







