#include <windows.h>
#include <iostream>
#include <libpq-fe.h>
#include <string>
#include <codecvt>
#include <vector>
#include <utility>
#include "functions.h"

// Глобальные переменные
const char* conninfo = "dbname=practice user=postgres password=123456";
const char* tableconn = "SELECT * FROM disciplines";
std::wstring selectedDiscipline;
int selectedTopicID = -1;
std::vector<int> topicIDs;
HWND hwndButton;
HWND hwndComboBox;
HWND hwndListBox;
HWND hwndButtonTopic;
HWND hwndButtonDiscipline;

// Новые глобальные переменные для хранения данных из текстовых полей
HWND hwndGroupEdit;
HWND hwndVariantsEdit;
HWND hwndTasksEdit;
HWND hwndControlWork;
std::wstring group;
int variants = 0;
int tasks = 0;
int control_work = 0;

std::wstring usernameStr;

// Функция для конвертации строки из UTF-8 в UTF-16
std::wstring UTF8ToUTF16(const std::string& utf8Str) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(utf8Str);
}

// Функция для преобразования строки UTF-16 в UTF-8
std::string UTF16ToUTF8(const std::wstring& utf16Str) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(utf16Str);
}

// Функция для получения списка дисциплин из базы данных
std::vector<std::wstring> GetDisciplines() {
    std::vector<std::wstring> disciplines;

    // Установка соединения с базой данных
    PGconn* conn = PQconnectdb(conninfo);

    // Проверка соединения
    if (PQstatus(conn) != CONNECTION_OK) {
        MessageBoxA(NULL, "Connection to database failed", "Error", MB_ICONERROR);
        PQfinish(conn);
        return disciplines;
    }

    // Выполнение SQL запроса
    PGresult* res = PQexec(conn, "SELECT name_discipline FROM disciplines");

    // Проверка результата запроса
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        MessageBoxA(NULL, PQerrorMessage(conn), "Error", MB_ICONERROR);
        PQclear(res);
        PQfinish(conn);
        return disciplines;
    }

    // Извлечение результатов запроса
    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        const char* discipline_name = PQgetvalue(res, i, 0);
        std::wstring wide_discipline_name = UTF8ToUTF16(discipline_name);
        disciplines.push_back(wide_discipline_name);
    }

    // Освобождение результата и завершение соединения
    PQclear(res);
    PQfinish(conn);

    return disciplines;
}

// Функция для получения списка тем по выбранной дисциплине из базы данных
std::pair<std::vector<std::wstring>, std::vector<int>> GetTopicsByDiscipline(const std::wstring& discipline) {
    std::vector<std::wstring> topicNames;
    std::vector<int> topicIds;

    // Устанавливаем соединение с базой данных
    PGconn* conn = PQconnectdb(conninfo);

    // Проверяем успешность соединения
    if (PQstatus(conn) != CONNECTION_OK) {
        return std::make_pair(topicNames, topicIds);
    }

    // Подготавливаем SQL запрос
    std::string query = "SELECT name_topic, id_topic FROM topics WHERE fk_discipline_id = (SELECT id_discipline FROM disciplines WHERE name_discipline = '" + UTF16ToUTF8(discipline) + "')";
    // Выполняем SQL запрос
    PGresult* res = PQexec(conn, query.c_str());

    // Проверяем успешность выполнения запроса
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        PQfinish(conn);
        return std::make_pair(topicNames, topicIds);
    }

    // Извлекаем результаты запроса
    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        const char* topic_name = PQgetvalue(res, i, 0);
        int topic_id = atoi(PQgetvalue(res, i, 1)); // Преобразуем строку в целое число (ID)
        std::wstring wide_topic_name = UTF8ToUTF16(topic_name);
        topicNames.push_back(wide_topic_name);
        topicIds.push_back(topic_id);
    }

    // Освобождаем результат и закрываем соединение
    PQclear(res);
    PQfinish(conn);

    return std::make_pair(topicNames, topicIds);
}

// Функция для регистрации нового пользователя
void RegisterUser(const std::wstring& username, const std::wstring& password) {
    // Устанавливаем соединение с базой данных
    PGconn* conn = PQconnectdb(conninfo);

    // Проверяем успешность соединения
    if (PQstatus(conn) != CONNECTION_OK) {
        MessageBoxW(NULL, UTF8ToUTF16(PQerrorMessage(conn)).c_str(), L"Error", MB_ICONERROR);
        PQfinish(conn);
        return;
    }

    // Генерируем SQL запрос для добавления нового пользователя
    std::string query = "INSERT INTO users (name_user, password_user) VALUES ('" + UTF16ToUTF8(username) + "', '" + UTF16ToUTF8(password) + "')";

    // Выполните SQL-запрос
    PGresult* res = PQexec(conn, query.c_str());

    // Проверьте успешность выполнения запроса
    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
        MessageBoxW(NULL, L"Пользователь зарегистрировался успешно", L"Success", MB_ICONINFORMATION);
    }
    else {
        MessageBoxW(NULL, L"Не удалось зарегестрировать пользователь", L"Error", MB_ICONERROR);
    }

    // Освободите результат и закройте соединение
    PQclear(res);
    PQfinish(conn);
}

// Функция для авторизации пользователя
bool LoginUser(const std::wstring& username, const std::wstring& password) {
    // Устанавливаем соединение с базой данных
    PGconn* conn = PQconnectdb(conninfo);

    // Проверяем успешность соединения
    if (PQstatus(conn) != CONNECTION_OK) {
        MessageBoxW(NULL, UTF8ToUTF16(PQerrorMessage(conn)).c_str(), L"Error", MB_ICONERROR);
        PQfinish(conn);
        return false;
    }

    // Генерируем SQL запрос для проверки авторизации пользователя
    std::string query = "SELECT COUNT(*) FROM users WHERE name_user = '" + UTF16ToUTF8(username) + "' AND password_user = '" + UTF16ToUTF8(password) + "'";

    // Выполняем SQL запрос
    PGresult* res = PQexec(conn, query.c_str());

    // Проверяем успешность выполнения запроса
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        MessageBoxW(NULL, UTF8ToUTF16(PQerrorMessage(conn)).c_str(), L"Error", MB_ICONERROR);
        PQclear(res);
        PQfinish(conn);
        return false;
    }

    // Извлекаем результат запроса (должен быть один ряд с одним столбцом)
    int count = atoi(PQgetvalue(res, 0, 0));

    // Освобождаем результат и закрываем соединение
    PQclear(res);
    PQfinish(conn);

    // Если количество совпадений равно 1, то авторизация успешна
    return count == 1;
}

// Функция для обновления отображения имени пользователя в главном окне
void UpdateUsernameDisplay(HWND hwnd, const std::wstring& username) {
    // Создаем статическое текстовое поле для отображения имени пользователя
    CreateWindowW(L"STATIC", username.c_str(), WS_VISIBLE | WS_CHILD, 700, 110, 200, 25, hwnd, NULL, NULL, NULL);
}

// Проверка наличия пользователя с таким именем перед регистрацией
bool IsUsernameExists(const std::wstring& username) {
    // Установка соединения с базой данных
    PGconn* conn = PQconnectdb(conninfo);

    // Проверка соединения
    if (PQstatus(conn) != CONNECTION_OK) {
        MessageBoxA(NULL, "Connection to database failed", "Error", MB_ICONERROR);
        PQfinish(conn);
        return false;
    }

    // Подготовка SQL запроса для проверки существования пользователя с заданным именем
    std::string query = "SELECT COUNT(*) FROM users WHERE name_user = '" + UTF16ToUTF8(username) + "'";
    PGresult* res = PQexec(conn, query.c_str());

    // Проверка результата запроса
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        MessageBoxA(NULL, PQerrorMessage(conn), "Error", MB_ICONERROR);
        PQclear(res);
        PQfinish(conn);
        return false;
    }

    // Извлечение результата запроса
    int rows = PQntuples(res);
    if (rows > 0) {
        // Если количество записей больше 0, значит, пользователь с таким именем уже существует
        int count = atoi(PQgetvalue(res, 0, 0));
        if (count > 0) {
            // Освобождение результата и закрытие соединения
            PQclear(res);
            PQfinish(conn);
            return true; // Пользователь с таким именем уже существует
        }
    }

    // Освобождение результата и закрытие соединения
    PQclear(res);
    PQfinish(conn);

    return false; // Пользователь с таким именем не существует
}

// Функция для обработки ввода текста
std::wstring GetWindowTextWString(HWND hwnd) {
    int length = GetWindowTextLength(hwnd);
    wchar_t* buffer = new wchar_t[length + 1];
    GetWindowText(hwnd, buffer, length + 1);
    std::wstring text(buffer);
    delete[] buffer;
    return text;
}

// Функция окна
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {

        // Создание кнопки "Choose Discipline"
        hwndButtonDiscipline = CreateWindow(
            L"BUTTON",  // класс окна
            L"Выбрать дисциплину",  // текст на кнопке
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стили окна
            20,  // x-координата
            10,  // y-координата
            150,  // ширина
            30,  // высота
            hwnd,  // хендл родительского окна
            (HMENU)2,  // идентификатор кнопки
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),  // экземпляр программы
            NULL);  // дополнительные параметры

        // Получение списка дисциплин
        std::vector<std::wstring> disciplines = GetDisciplines();

        // Создание выпадающего списка
        hwndComboBox = CreateWindow(
            L"COMBOBOX",  // класс окна
            L"",  // текст на выпадающем списке (пока пусто)
            CBS_DROPDOWNLIST | WS_VISIBLE | WS_CHILD | WS_VSCROLL,  // стили окна
            20,  // x-координата
            50,  // y-координата
            150,  // ширина
            200,  // высота
            hwnd,  // хендл родительского окна
            (HMENU)8,  // идентификатор выпадающего списка
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),  // экземпляр программы
            NULL);  // дополнительные параметры

        // Добавление дисциплин в выпадающий список
        for (const auto& discipline : disciplines) {
            SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)discipline.c_str());
        }

        // Создание списка для тем
        hwndListBox = CreateWindow(
            L"COMBOBOX",  // класс окна
            L"",  // текст списка (пока пусто)
            CBS_DROPDOWNLIST | WS_VISIBLE | WS_CHILD | WS_VSCROLL,  // стили окна  // стили окна
            180,  // x-координата
            50,  // y-координата
            350,  // ширина
            200,  // высота
            hwnd,  // хендл родительского окна
            (HMENU)3,  // идентификатор списка
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),  // экземпляр программы
            NULL);  // дополнительные параметры

        // Создание кнопки "Choose Topic"
        hwndButtonTopic = CreateWindow(
            L"BUTTON",  // класс окна
            L"Выбрать тему",  // текст на кнопке
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стили окна
            180,  // x-координата
            10,  // y-координата
            150,  // ширина
            30,  // высота
            hwnd,  // хендл родительского окна
            (HMENU)9,  // идентификатор кнопки
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),  // экземпляр программы
            NULL);  // дополнительные параметры

        // Создание меток и текстовых полей для ввода группы, количества вариантов и количества заданий
        CreateWindow(L"STATIC", L"Группа:", WS_VISIBLE | WS_CHILD, 180, 260, 100, 20, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        hwndGroupEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 380, 260, 100, 20, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

        CreateWindow(L"STATIC", L"Варианты:", WS_VISIBLE | WS_CHILD, 180, 290, 100, 20, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        hwndVariantsEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 380, 290, 100, 20, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

        CreateWindow(L"STATIC", L"Задания:", WS_VISIBLE | WS_CHILD, 180, 320, 100, 20, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        hwndTasksEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 380, 320, 100, 20, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

        CreateWindow(L"STATIC", L"Номер работы:", WS_VISIBLE | WS_CHILD, 180, 350, 100, 20, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        hwndControlWork = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 380, 350, 100, 20, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

        // Метка "Имя пользователя"
        CreateWindowW(L"STATIC", L"Имя:", WS_VISIBLE | WS_CHILD, 650, 13, 65, 20, hwnd, NULL, NULL, NULL);

        // Поле для ввода имени пользователя
        CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 720, 10, 150, 25, hwnd, (HMENU)4, NULL, NULL);

        // Метка "Пароль"
        CreateWindowW(L"STATIC", L"Пароль:", WS_VISIBLE | WS_CHILD, 650, 43, 65, 20, hwnd, NULL, NULL, NULL);

        // Поле для ввода пароля пользователя
        CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 720, 40, 150, 25, hwnd, (HMENU)5, NULL, NULL);

        // Кнопка "Register"
        CreateWindowW(L"BUTTON", L"Регистрация", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 700, 80, 100, 25, hwnd, (HMENU)6, NULL, NULL);

        // Кнопка "Login"
        CreateWindowW(L"BUTTON", L"Авторизация", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 805, 80, 100, 25, hwnd, (HMENU)7, NULL, NULL);

        break;
    }

    case WM_COMMAND: {

        switch (LOWORD(wParam)) {
        case 6: { // Если нажата кнопка "Register"
            // Получение введенного имени пользователя
            wchar_t username[256];
            GetDlgItemText(hwnd, 4, username, 256);
            usernameStr = username;

            // Получение введенного пароля пользователя
            wchar_t password[256];
            GetDlgItemText(hwnd, 5, password, 256);

            // Проверка, не существует ли уже пользователь с таким именем
            if (IsUsernameExists(username)) {
                MessageBoxW(NULL, L"Пользователь с таким именем уже существует!", L"Ошибка регистрации", MB_ICONERROR);
            }
            else {
                // Добавление нового пользователя в базу данных
                RegisterUser(username, password);
            }
            break;
        }
        case 7: { // Кнопка "Login"
            // Получение введенного имени пользователя
            wchar_t username[256];
            GetDlgItemText(hwnd, 4, username, 256);
            usernameStr = username;

            // Получение введенного пароля пользователя
            wchar_t password[256];
            GetDlgItemText(hwnd, 5, password, 256);

            // Попытка авторизации пользователя
            if (LoginUser(username, password)) {
                // Успешная авторизация, обновление отображения имени в главном окне
                UpdateUsernameDisplay(hwnd, username);
            }
            else {
                // Неудачная авторизация, вывод сообщения об ошибке
                MessageBoxW(hwnd, L"Неверное имя пользователя или пароль", L"Ошибка авторизации", MB_OK | MB_ICONERROR);
            }
            break;
        }
        }

        std::pair<std::vector<std::wstring>, std::vector<int>> topics;

        // Обработка нажатия кнопки "Choose Discipline"
        if (LOWORD(wParam) == 2) {
            int selectedItemIndex = SendMessage(hwndComboBox, CB_GETCURSEL, 0, 0);
            if (selectedItemIndex != CB_ERR) {
                wchar_t buffer[256];
                SendMessage(hwndComboBox, CB_GETLBTEXT, selectedItemIndex, (LPARAM)buffer);
                selectedDiscipline = buffer; // Сохраняем выбранную дисциплину
                // Выводим выбранную дисциплину
                MessageBoxW(NULL, selectedDiscipline.c_str(), L"Selected Discipline", MB_OK);

                // Получаем список тем по выбранной дисциплине
                topics = GetTopicsByDiscipline(selectedDiscipline);

                // Очищаем список тем
                SendMessage(hwndListBox, CB_RESETCONTENT, 0, 0);

                // Добавляем полученные темы в список
                for (size_t i = 0; i < topics.first.size(); ++i) {
                    std::wstring topic = topics.first[i];
                    int topicId = topics.second[i];
                    // Создаем пару из названия темы и ее идентификатора и добавляем в список
                    std::wstring topicInfo = topic + L" (" + std::to_wstring(topicId) + L")";
                    SendMessage(hwndListBox, CB_ADDSTRING, 0, (LPARAM)topicInfo.c_str());
                }
            }
        }

        // Обработка нажатия кнопки "Choose Topic"
        if (LOWORD(wParam) == 9) {
            // Получаем индекс выбранной темы в списке
            int selectedIndex = SendMessage(hwndListBox, CB_GETCURSEL, 0, 0);
            if (selectedIndex != CB_ERR) {
                wchar_t buffer[256];
                // Получаем текст выбранной темы
                SendMessage(hwndListBox, CB_GETLBTEXT, selectedIndex, (LPARAM)buffer);

                // Отображаем выбранную тему в окне сообщения
                std::wstring selectedTopic(buffer);
                MessageBoxW(NULL, selectedTopic.c_str(), L"Selected Topic", MB_OK);

                int topicId = selectedIndex;

                // Получаем значения из текстовых полей
                std::wstring group = GetWindowTextWString(hwndGroupEdit);
                int variants = std::stoi(GetWindowTextWString(hwndVariantsEdit));
                int tasks = std::stoi(GetWindowTextWString(hwndTasksEdit));
                int control_work = std::stoi(GetWindowTextWString(hwndControlWork));

                // Вызываем определенную функцию и передаем ID выбранной темы и значения новых полей
                Functions(topicId, tasks, variants, UTF16ToUTF8(group), control_work, usernameStr);
            }
        }

        break;
    }
    case WM_DESTROY: {
        // Завершаем работу приложения при закрытии окна
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Точка входа в программу
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

    setlocale(LC_ALL, "Rus");

    // Регистрируем класс окна
    const wchar_t* CLASS_NAME = L"Sample Window Class";

    WNDCLASSW wc = {};

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    // Создаем окно
    HWND hwnd = CreateWindowExW(
        0,  // дополнительные стили окна
        CLASS_NAME,  // имя класса окна
        L"Sample Window",  // заголовок окна
        WS_OVERLAPPEDWINDOW,  // стили окна
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,  // размер и позиция окна
        NULL,  // хендл родительского окна
        NULL,  // хендл меню
        hInstance,  // экземпляр программы
        NULL);  // дополнительные параметры

    if (hwnd == NULL) {
        return 0;
    }

    // Отображаем окно
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Запускаем цикл обработки сообщений
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
