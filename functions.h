#pragma once
#pragma warning(disable: 4996)
#include <iostream>
#include <vector>
#include <cstdlib>  
#include <ctime>  
#include <fstream>
#include <cstring>
#include "cmath"
#include <iomanip>
#include <sstream>
#include <random>
#include <regex>
#include <limits>
#include <set>
#include <iterator>

struct Return {
    std::string tasks;
    std::string answers;
    std::string interval;
    int random_point;
};

struct kostyl {
    std::string tasks;
    std::string interval;
    int random_point;
    int naib;
    int naim;
};

struct kostyl2 {
    std::string answers;
    std::string yfunc;
    std::string gfunc;
};

using namespace std;

std::string output_directory_tasks = "C:\\Users\\artio\\source\\repos\\donus1perdonus\\practice\\tasks";
std::string output_directory_answers = "C:\\Users\\artio\\source\\repos\\donus1perdonus\\practice\\answers";


// Функция для генерации случайного полинома
vector<int> generatePolynomial(int degree) {
    vector<int> coefficients(degree + 1);
    for (int i = 0; i <= degree; i++) {
        coefficients[i] = rand() % 21 - 10;  // Генерируем случайные коэффициенты от -10 до 10
    }
    return coefficients;
}

// Функция для нахождения производной полинома
vector<int> findDerivative(vector<int> coefficients) {
    vector<int> derivative(coefficients.size() - 1);
    for (int i = 1; i < coefficients.size(); i++) {
        derivative[i - 1] = coefficients[i] * i;  // Умножаем коэффициент на его степень
    }
    return derivative;
}

// Функция для вычисления значения производной полинома в заданной точке
int computeDerivative(const vector<int>& coefficients, double x) {
    int result = 0;
    int power = 1;
    for (int i = 1; i < coefficients.size(); ++i) {
        result += i * coefficients[i] * pow(x, power - 1);
        power++;
    }
    return result;
}

string Return_to_string(const vector<int>& polynomial) {
    string result;
    int degree = polynomial.size() - 1;
    for (int i = degree; i >= 0; i--) {
        if (polynomial[i] == 0) {//если коэффициент равен 0, то пропускаем его, не записываем член
            continue;
        }
        else if (polynomial[i] == 1 && i != 0) {//если коэффициент равен 1 и степень не равна 0
            if (i == 1) {//если степень 1, то печатаем просто х
                result += "x";
            }
            else {//иначе печатаем как положено
                result += "x^" + to_string(i);
            }
        }
        else if (polynomial[i] > 1) {
            if (i != degree) {
                result += " + ";
            }
            result += to_string(polynomial[i]);
            if (i > 0 && i != 1) {
                result += "x^" + to_string(i);
            }
            else if (i == 1) {
                result += "x";
            }
        }
        else if (polynomial[i] < -1) {
            //if (i != degree){
            result += " - ";
            //}
            result += to_string(polynomial[i] * (-1));
            if (i > 0 && i != 1) {
                result += "x^" + to_string(i);
            }
            else if (i == 1) {
                result += "x";
            }
        }
        else if (polynomial[i] == -1) {
            if (i > 0 && i != 1) {
                result += " - x^" + to_string(i);
            }
            else if (i == 1) {
                result += " - x";
            }
        }
    }
    return result;
}

Return Pr_Polinom() {
    int degree = 5;  // Задаем степень полинома
    vector<int> polynomial = generatePolynomial(degree);
    Return p;

    // Записываем задание в поле класса
    p.tasks = Return_to_string(polynomial);

    //записываем ответ в поле класса
    vector<int> derivative = findDerivative(polynomial);

    for (int i = derivative.size() - 1; i >= 0; i--) {
        if (derivative[i] == 0) {//если коэффициент равен 0, то пропускаем его, не записываем член
            continue;
        }
        else if (derivative[i] == 1 && i != 0) {//если коэффициент равен 1 и степень не равна 0
            if (i == 1) {//если степень 1, то печатаем просто х
                p.answers += "x";
            }
            else {//иначе печатаем как положено
                p.answers += "x^" + to_string(i);
            }
        }
        else if (derivative[i] > 1) {
            if (i != derivative.size() - 1) {
                p.answers += " + ";
            }
            p.answers += to_string(derivative[i]);
            if (i > 0 && i != 1) {
                p.answers += "x^" + to_string(i);
            }
            else if (i == 1) {
                p.answers += "x";
            }
        }
        else if (derivative[i] < 0) {
            //if (i != derivative.size()){
            p.answers += " - ";
            //}
            p.answers += to_string(derivative[i] * -1);
            if (i > 0 && i != 1) {
                p.answers += "x^" + to_string(i);
            }
            else if (i == 1) {
                p.answers += "x";
            }
        }
        else if (derivative[i] == -1) {
            if (i > 0 && i != 1) {
                p.answers += " - x^" + to_string(i);
            }
            else if (i == 1) {
                p.answers += " - x";
            }
        }
    }

    return p;
}

Return Pr_Polinom_in_dot() {
    Return p;
    // Генерируем случайную точку, в которой будем вычислять значение полинома и его производной
    p.random_point = static_cast<int>(rand() % 21 - 10); // Случайная точка от -10 до 10
    int degree = 5;  // Задаем степень полинома

    //генерируем полином
    vector<int> polynomial = generatePolynomial(degree);
    //записываем полином
    p.tasks = Return_to_string(polynomial);

    int derivativeVal = computeDerivative(polynomial, p.random_point);
    // Записываем данные в файл
    p.answers = to_string(derivativeVal);

    return p;
}

// Функция для генерации случайного предела
std::string generate_limit() {
    Return p;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> coef_dis(1, 10); // Генерируем только положительные коэффициенты
    std::uniform_int_distribution<> x_dis(0, 2); // Генерируем значения для x: 0, inf, -inf
    std::uniform_int_distribution<> rand_x(-10, 10); // Генерируем случайное число в диапазоне [-10, 10]

    int a = coef_dis(gen);
    int b = coef_dis(gen);
    int c = coef_dis(gen);

    int x_val;
    int x_gen = x_dis(gen);
    if (x_gen == 0) {
        x_val = 0; // Нуль
    }
    else if (x_gen == 1) {
        x_val = (gen() % 2 == 0) ? 10 : -10; // Бесконечность
    }
    else {
        x_val = rand_x(gen); // Случайное число в [-10, 10]
    }

    std::stringstream ss;
    ss << "\\lim_{x \\to ";
    if (x_val == 10) {
        ss << "\\infty";
    }
    else if (x_val == -10) {
        ss << "-\\infty";
    }
    else {
        ss << x_val;
    }
    ss << "} (";
    if (a != 1) ss << a << "x^2";
    else ss << "x^2";
    if (b != 0) {
        if (b > 0) {
            if (b == 1) ss << " + x";
            else ss << " + " << b << "x";
        }
        else {
            if (b == -1) ss << " - x";
            else ss << " - " << (-b) << "x";
        }
    }
    if (c != 0) {
        if (c > 0) ss << " + " << c;
        else ss << " - " << (-c);
    }
    ss << ")";
    p.tasks = ss.str();
    std::cout << p.tasks << std::endl;
    return p.tasks;
}

// Функция для решения предела
double solve_limit(double a, double b, double c, double x_val) {
    if (std::isinf(x_val)) {
        if (x_val > 0) {
            return (a > 0) ? std::numeric_limits<double>::infinity() : -std::numeric_limits<double>::infinity();
        }
        else {
            return (a > 0) ? -std::numeric_limits<double>::infinity() : std::numeric_limits<double>::infinity();
        }
    }
    else if (x_val == 0) {
        return c; // При x -> 0, значение полинома будет равно c
    }
    return a * x_val * x_val + b * x_val + c;
}

Return Limits() {
    Return p;
    p.tasks = generate_limit();
    std::regex re(R"(\\lim_\{x \\to (\d+|\\infty|-\\infty)\} \((\d*)x\^2\s*([+-]?\s*\d*)x\s*([+-]?\s*\d+)\))");
    std::smatch match;
    int a = 1, b = 0, c = 0;
    double x_val = 0;

    if (std::regex_search(p.tasks, match, re)) {
        std::string x_str = match[1].str();
        std::string a_str = match[2].str();
        std::string b_str = match[3].str();
        std::string c_str = match[4].str();

        if (x_str == "\\infty") {
            x_val = std::numeric_limits<double>::infinity();
        }
        else if (x_str == "-\\infty") {
            x_val = -std::numeric_limits<double>::infinity();
        }
        else {
            x_val = std::stod(x_str);
        }

        if (!a_str.empty()) {
            a = std::stoi(a_str);
        }

        b_str.erase(std::remove(b_str.begin(), b_str.end(), ' '), b_str.end());
        c_str.erase(std::remove(c_str.begin(), c_str.end(), ' '), c_str.end());

        b = std::stoi(b_str);
        c = std::stoi(c_str);
    }

    double result = solve_limit(a, b, c, x_val);

    if (std::isinf(result)) {
        if (result > 0) {
            p.answers = "\\infty";
        }
        else {
            p.answers = "-\\infty";
        }
    }
    else {
        p.answers = std::to_string(result);
    }

    std::cout << p.answers << std::endl;
    return p;
}

// Функция для генерации задания на поиск наибольшего и наименьшего значений функции на промежутке
string generateTask(int a, int n, int b, int c, int intervalStart, int intervalEnd) {
    Return p;
    if (a != 1)
        p.tasks += to_string(a);
    if (n == 1) {
        p.tasks += "x";
    }
    else {
        p.tasks += "x^" + to_string(n);
    }
    if (b != 1)
        p.tasks += " + " + to_string(b);
    else if (b == 1 && (a != 1 || n == 0))
        p.tasks += " + ";
    if (n - 1 == 1) {
        p.tasks += "x";
    }
    else {
        p.tasks += "x^" + to_string(n - 1);
    }
    if (c != 1)
        p.tasks += " - " + to_string(c);
    else if (c == 1)
        p.tasks += " - 1";
    return p.tasks;
}

// Функция для нахождения наибольшего и наименьшего значений функции на промежутке
void findMinMax(int a, int n, int b, int c, int intervalStart, int intervalEnd, int& minVal, int& maxVal) {
    // Производная: dy/dx = n * a * x^(n-1) + (n-1) * b * x^(n-2)
    int deriv_a = n * a;
    int deriv_b = (n - 1) * b;

    // Найдем критические точки, приравняв производную к нулю: deriv_a * x^(n-1) + deriv_b * x^(n-2) = 0
    // 12x^2 + 16x = 0
    // x(12x + 16) = 0
    // x = 0 or 12x + 16 = 0 => x = -16/12 => x = -4/3
    set<double> criticalPoints;

    if (deriv_a != 0) {
        criticalPoints.insert(0);
        if (deriv_b != 0) {
            criticalPoints.insert(-deriv_b / deriv_a);
        }
    }

    // Проверим значения функции в критических точках и на концах промежутка
    auto checkPoint = [&](double x) {
        return a * pow(x, n) + b * pow(x, n - 1) - c;
        };

    minVal = checkPoint(intervalStart);
    maxVal = minVal;

    int points[] = { intervalStart, intervalEnd };
    for (int point : points) {
        int value = checkPoint(point);
        if (value < minVal) minVal = value;
        if (value > maxVal) maxVal = value;
    }
    for (int point : criticalPoints) {
        if (point >= intervalStart && point <= intervalEnd) {
            int value = checkPoint(point);
            if (value < minVal) minVal = value;
            if (value > maxVal) maxVal = value;
        }
    }
}

// Функция для генерации случайного числа в промежутке, исключая повторения
int randomInRangeUnique(set<int>& generated, int min, int max) {
    static bool initialized = false;
    if (!initialized) {
        srand(time(NULL)); // Инициализируем генератор случайных чисел только один раз
        initialized = true;
    }
    int num;
    do {
        num = min + rand() % (max - min + 1);
    } while (generated.count(num) > 0); // Повторяем, пока число уже не сгенерировано
    generated.insert(num); // Добавляем сгенерированное число в множество
    return num;
}

kostyl Pr_Naib_Naim() {
    //srand(time(NULL));

    // Генерируем случайные значения коэффициентов
    set<int> generated; // Множество для отслеживания сгенерированных чисел
    int a = randomInRangeUnique(generated, 1, 10); // случайное число от 1 до 10
    int n = randomInRangeUnique(generated, 2, 4); // случайное число от 2 до 4 (степень функции)
    int b = randomInRangeUnique(generated, 1, 10); // случайное число от 1 до 10
    int c = randomInRangeUnique(generated, 1, 10); // случайное число от 1 до 10

    // Генерируем случайный промежуток
    int intervalStart = randomInRangeUnique(generated, -10, 10);
    int intervalEnd = randomInRangeUnique(generated, -10, 10);
    if (intervalStart > intervalEnd) {
        swap(intervalStart, intervalEnd);
    }
    kostyl p;
    p.interval = "[" + to_string(intervalStart) + ", " + to_string(intervalEnd) + "]";
    p.tasks = generateTask(a, n, b, c, intervalStart, intervalEnd);

    // Находим наибольшее и наименьшее значения функции на промежутке
    int minVal, maxVal;
    findMinMax(a, n, b, c, intervalStart, intervalEnd, minVal, maxVal);
    
    p.naim = minVal;
    p.naib = maxVal;

    return p;
}

string generateYFunc(int k, int d) {
    string yfunc = "y = ";
    if (k != 1) {
        yfunc += to_string(k);
    }
    yfunc += "x - " + to_string(d);
    return yfunc;
}

string generateGFunc(int a, int n, int b, int m, int c, bool& isPositive) {
    string gfunc;
    if (a != 1) {
        gfunc += to_string(a);
    }
    if (n == 1) {
        gfunc += "x";
    }
    else {
        gfunc += "x^" + to_string(n);
    }
    if (rand() % 2 == 0) {
        gfunc += " + ";
        isPositive = true;
    }
    else {
        gfunc += " - ";
        isPositive = false;
    }
    if (b != 1) {
        gfunc += to_string(b);
    }
    if (m == 1) {
        gfunc += "x";
    }
    else {
        gfunc += "x^" + to_string(m);
    }
    gfunc += " - " + to_string(c);
    return gfunc;
}

pair<double, double> findPointOfTangency(int k, int a, int n, int b, int m, bool isPositive) {
    double x1, x2;

    if (n == 3 && m == 2) {
        double A = a * n;
        double B = b * m;
        double C = -k;

        double discriminant = B * B - 4 * A * C;
        if (discriminant >= 0) {
            if (isPositive) {
                x1 = (-B + sqrt(discriminant)) / (2 * A);
                x2 = (-B - sqrt(discriminant)) / (2 * A);
            }
            else {
                x1 = (B + sqrt(discriminant)) / (2 * A);
                x2 = (B - sqrt(discriminant)) / (2 * A);
            }
        }
        else {
            return make_pair(NAN, NAN);
        }
    }
    else if (n == 2 && m == 1) {
        x1 = (k - b * m) / (a * n);
        x2 = NAN;
    }
    else {
        return make_pair(NAN, NAN);
    }

    return make_pair(x1, x2);
}

bool verifySolution(double x, int k, int d, int a, int n, int b, int m, int c) {
    double lhs = k * x - d;
    double rhs = a * pow(x, n) + b * pow(x, m) - c;
    return fabs(lhs - rhs) < 1e-6;
}

kostyl2 Pr_Geom() {
    srand(time(NULL));

    int k, d, a, n, b, m, c;
    pair<double, double> pointsOfTangency;
    bool validSolution = false;
    bool isPositive = true;
    string yfunc, gfunc, answer;

    while (!validSolution) {
        k = (rand() % 10) + 1;
        d = (rand() % 10) + 1;
        a = (rand() % 10) + 1;
        n = (rand() % 2) + 2;
        b = (rand() % 10) + 1;
        m = (rand() % ((int)n - 1)) + 1;
        c = (rand() % 10) + 1;

        yfunc = generateYFunc(k, d);
        gfunc = generateGFunc(a, n, b, m, c, isPositive);

        pointsOfTangency = findPointOfTangency(k, a, n, b, m, isPositive);

        if ((!isnan(pointsOfTangency.first) && verifySolution(pointsOfTangency.first, k, d, a, n, b, m, c)) ||
            (!isnan(pointsOfTangency.second) && verifySolution(pointsOfTangency.second, k, d, a, n, b, m, c))) {
            validSolution = true;
        }
    }

    if (!isnan(pointsOfTangency.first) && verifySolution(pointsOfTangency.first, k, d, a, n, b, m, c)) {
        answer += to_string(pointsOfTangency.first);
    }
    answer += "\n";

    kostyl2 result;
    result.answers = answer;
    result.yfunc = yfunc;
    result.gfunc = gfunc;

    return result;
}

// Функция для подключения и выполнения операций с таблицами variants и answers
void AddVariantAndAnswer(int id_topic, const std::wstring& username, int number_of_variant, const std::string& path_of_tasks, const std::string& path_of_answers) {

    std::ofstream error_log("error_log.txt");
    std::cerr.rdbuf(error_log.rdbuf());

    // Конфигурация подключения к базе данных
    const char* conninfo = "host=localhost dbname=practice user=postgres password=123456";

    // Устанавливаем соединение с базой данных
    PGconn* conn = PQconnectdb(conninfo);

    // Проверяем успешность соединения
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return;
    }

    // Начало транзакции
    PGresult* res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "BEGIN command failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQfinish(conn);
        return;
    }
    PQclear(res);

    // Получение последнего id_variant из таблицы variants
    res = PQexec(conn, "SELECT id_variant FROM variants ORDER BY id_variant DESC LIMIT 1");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "SELECT command did not return tuples properly: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQexec(conn, "ROLLBACK");
        PQfinish(conn);
        return;
    }

    int last_id_variant = 0;
    if (PQntuples(res) == 1) {
        last_id_variant = std::stoi(PQgetvalue(res, 0, 0));
    }
    PQclear(res);

    // Если таблица пуста, last_id_variant останется 0
    // Генерация нового id_variant
    int new_id_variant = last_id_variant + 1;

    // Получение последнего id_answer из таблицы answers
    res = PQexec(conn, "SELECT id_answer FROM answers ORDER BY id_answer DESC LIMIT 1");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "SELECT command did not return tuples properly: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQexec(conn, "ROLLBACK");
        PQfinish(conn);
        return;
    }

    int last_id_answer = 0;
    if (PQntuples(res) == 1) {
        last_id_answer = std::stoi(PQgetvalue(res, 0, 0));
    }
    PQclear(res);

    // Если таблица пуста, last_id_answer останется 0
    // Генерация нового id_answer
    int new_id_answer = last_id_answer + 1;

    // Формирование строки текущей даты
    std::ostringstream date_stream;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    date_stream << std::put_time(&tm, "%Y-%m-%d");
    std::string current_date = date_stream.str();

    // Преобразование wstring в string для username
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::string username_str = !username.empty() ? converter.to_bytes(username) : "empty";

    // Вставка нового варианта в таблицу variants
    std::ostringstream query_variants;
    query_variants << "INSERT INTO variants (id_variant, fk_topic_id, variant_file_path, added_date, generate_user_variant, number_of_variant) "
        << "VALUES (" << new_id_variant << ", " << id_topic << ", '" << path_of_tasks << "', '" << current_date << "', '" << username_str << "', " << number_of_variant << ")";

    res = PQexec(conn, query_variants.str().c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "INSERT into variants failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQexec(conn, "ROLLBACK");
        PQfinish(conn);
        return;
    }
    PQclear(res);

    // Вставка нового ответа в таблицу answers
    std::ostringstream query_answers;
    query_answers << "INSERT INTO answers (id_answer, answer_file_path, added_date, number_of_answer, fk_id_variant, generate_user_answer) "
        << "VALUES (" << new_id_answer << ", '" << path_of_answers << "', '" << current_date << "', " << number_of_variant << ", " << new_id_variant << ", '" << username_str << "')";

    res = PQexec(conn, query_answers.str().c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "INSERT into answers failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQexec(conn, "ROLLBACK");
        PQfinish(conn);
        return;
    }
    PQclear(res);

    // Фиксация транзакции
    res = PQexec(conn, "COMMIT");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "COMMIT command failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQexec(conn, "ROLLBACK");
        PQfinish(conn);
        return;
    }
    PQclear(res);

    // Завершаем соединение с базой данных
    PQfinish(conn);

    std::cout << "Данные успешно добавлены в таблицы variants и answers." << std::endl;
}

int Functions(int id_topic, int number_of_tasks, int variants, const string& group, int control_work, wstring username) {

    setlocale(LC_ALL, "RUS");

    std::ofstream error_log("error_log.txt");
    std::cerr.rdbuf(error_log.rdbuf());

    srand(time(0));  // Инициализируем генератор случайных чисел

    for (int variant = 1; variant <= variants; variant++) {
        // Создаем имена файлов для заданий и ответов с номером варианта
        string filename_task = "output_task" + to_string(variant) + ".tex";
        string filename_answer = "output_answer" + to_string(variant) + ".tex";

        ofstream latexFile_task(filename_task, ios::out | ios::binary); // файл для заданий, который будет генерироваться в PDF
        ofstream latexFile_answer(filename_answer, ios::out | ios::binary); // файл для ответов, который будет генерироваться в PDF

        if (!latexFile_task.is_open() || !latexFile_answer.is_open()) {
            cerr << "Не удалось открыть файлы для записи." << endl;
            return 1;
        }

        // Начало LaTeX-документа
        ofstream* files[2] = { &latexFile_task, &latexFile_answer };
        for (auto& file : files) {
            *file << u8"\\documentclass{article}\n";
            *file << u8"\\usepackage[T2A]{fontenc} % Указываем кодировку T2A для поддержки кириллицы\n"
                u8"\\usepackage[utf8]{inputenc} % Указываем UTF-8 кодировку для ввода символов\n"
                u8"\\usepackage[russian]{babel} % Подключаем пакет babel для русификации\n";
            *file << u8"\\begin{document}\n";
            *file << u8"Контрольная работа: " << to_string(control_work) << u8"\n\n";
            *file << u8"Группа: " << group << u8"\n\n";
        }

        latexFile_task << u8"Вариант: " << to_string(variant) << u8"\n\n";
        latexFile_answer << u8"Вариант: " << to_string(variant) << u8"\n\n";

        latexFile_task << "\\begin{enumerate}\n";
        latexFile_answer << "\\begin{enumerate}\n";

        switch (id_topic) {
        case 0: // для производных
            // генерация заданий

            for (int i = 1; i <= number_of_tasks; i++) {
                int random_task = rand() % 4; // выбираем случайную функцию
                if (random_task == 0) {
                    Return ob = Pr_Polinom();
                    latexFile_task << u8"\\item Задание. Найти производную от полинома.\n";
                    latexFile_task << u8"\\[ f(x) = " << ob.tasks << u8" \\]\n\n";
                    latexFile_answer << u8"\\item Ответ на задание №" << i << u8": \\[" << ob.answers << u8" \\]\n\n";
                }
                else if (random_task == 1) {
                    Return ob1 = Pr_Polinom_in_dot();
                    latexFile_task << u8"\\item Задание. Найти значение производной в точке " << to_string(ob1.random_point) << u8".\n";
                    latexFile_task << u8"\\[ f(x) = " << ob1.tasks << u8" \\]\n\n";
                    latexFile_answer << u8"\\item Ответ на задание №" << i << u8": " << ob1.answers << u8"\n\n";
                }
                else if (random_task == 2) {
                    kostyl ob2 = Pr_Naib_Naim();
                    latexFile_task << u8"\\item Задание. Найти наибольшее и наименьшее значения функции на промежутке " << ob2.interval << u8". \n\n";
                    latexFile_task << u8"\\[ f(x) = " << ob2.tasks << u8" \\]\n\n";
                    latexFile_answer << u8"\\item Ответ на задание №" << i << u8": \n\n";
                    latexFile_answer << u8"Наименьшее значение: " << ob2.naim << u8"\n\n";
                    latexFile_answer << u8"Наибольшее значение: " << ob2.naim << u8"\n\n";
                }
                else if (random_task == 3) {
                    kostyl2 ob3 = Pr_Geom();
                    latexFile_task << u8"\\item Прямая: \\[" << ob3.yfunc << u8"\\] Параллельна касательной к графику функции: \\[" << "g = " << ob3.gfunc << u8"\\]\n Найдите абсциссу точки касания.\n\n";
                    latexFile_answer << u8"\\item Ответ на задание №" << i << u8": \\[" << ob3.answers << u8"\\]\n\n";
                }
                // Аналогично для других случаев, если они будут
            }
            break;
        case 1:
            for (int i = 1; i <= number_of_tasks; i++) {
                Return ob = Limits();
                latexFile_task << u8"\\item Задание. Найти предел.\n";
                latexFile_task << u8"\\[ " << ob.tasks << u8" \\]\n\n";
                latexFile_answer << u8"\\item Ответ на задание №" << i << u8": \\[" << ob.answers << u8" \\]\n\n";
            }
            break;
        default:
            break;
        }
        latexFile_task << "\\end{enumerate}\n";
        latexFile_answer << "\\end{enumerate}\n";
        latexFile_task << u8"\\end{document}\n";
        latexFile_answer << u8"\\end{document}\n";

        // Закрытие файлов
        latexFile_task.close();
        latexFile_answer.close();

        std::string pdflatex_command_task = "pdflatex -output-directory=" + std::string(output_directory_tasks) + " " + filename_task;
        std::string pdflatex_command_answer = "pdflatex -output-directory=" + std::string(output_directory_answers) + " " + filename_answer;

        // Компиляция LaTeX в
        system((pdflatex_command_task).c_str());
        system((pdflatex_command_answer).c_str());
        cout << "PDF файлы для варианта " << variant << " сгенерированы." << endl;

        AddVariantAndAnswer(id_topic, username, variant, output_directory_tasks, output_directory_answers);
    }
    return 0;
}