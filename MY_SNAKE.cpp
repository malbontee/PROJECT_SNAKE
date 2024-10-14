#include "snake_game.h"

// Функция инициализации игры
void init_game()
{
    game_over = false;
    dir = stop;
    head_x = game_width / 2; // Стартовая позиция головы змеи по центру
    head_y = game_height / 2;
    fruit_x = rand() % game_width; // Случайная позиция яблока
    fruit_y = rand() % game_height;
    player_points = 0;
    tail_length = 0; // Хвост змеи отсутствует в начале
    tail_x = new int[100]; // Выделение памяти для хвоста
    tail_y = new int[100];
}

// Функция для скрытия курсора
void hide_cursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false; // Скрыть курсор
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// Функция отрисовки игрового поля
void render_game(string player_name)
{
    set_console_cursor(0, 0); // Перемещаем курсор в начало вместо очистки экрана

    set_window_title("Snake Game - " + player_name);

    // Верхняя граница
    set_text_color(13); // Фиолетовый цвет границы
    cout << string(game_width + 2, '=') << "\n";

    // Поле игры
    for (int i = 0; i < game_height; i++) {
        set_text_color(13); // Фиолетовый цвет для вертикальных границ
        cout << "="; // Левый край
        set_text_color(7); // Вернуть стандартный цвет для поля
        for (int j = 0; j < game_width; j++) {
            if (i == head_y && j == head_x) {
                set_text_color(10); // Зелёная змея
                cout << "O"; // Голова змеи
                set_text_color(7); // Вернуть стандартный цвет
            }
            else if (i == fruit_y && j == fruit_x) {
                set_text_color(12); // Красное яблоко
                cout << "@"; // Символ яблока
                set_text_color(7); // Вернуть стандартный цвет
            }
            else {
                bool print_tail = false;
                for (int k = 0; k < tail_length; k++) {
                    if (tail_x[k] == j && tail_y[k] == i) {
                        set_text_color(10); // Зелёный цвет для хвоста
                        cout << "o"; // Хвост змеи
                        print_tail = true;
                    }
                }
                if (!print_tail) cout << " "; // Пустое пространство
            }
        }
        set_text_color(13); // Фиолетовый цвет для правой вертикальной границы
        cout << "="; // Правый край
        cout << "\n";
    }

    // Нижняя граница
    set_text_color(13); // Фиолетовый цвет для нижней границы
    cout << string(game_width + 2, '=') << "\n";

    // Очки и имя игрока
    set_text_color(11); // Цвет для текста
    cout << player_name << "'s Points: " << player_points << "\n"; // Окрашиваем текст
    set_text_color(7); // Вернуть стандартный цвет
}

// Функция обновления игры
void update_game()
{
    // Сохраняем предыдущие позиции хвоста
    int prev_x = tail_x[0];
    int prev_y = tail_y[0];
    int prev2_x, prev2_y;
    tail_x[0] = head_x;
    tail_y[0] = head_y;

    // Обновляем позиции частей хвоста
    for (int i = 1; i < tail_length; i++) {
        prev2_x = tail_x[i];
        prev2_y = tail_y[i];
        tail_x[i] = prev_x;
        tail_y[i] = prev_y;
        prev_x = prev2_x;
        prev_y = prev2_y;
    }

    // Движение головы змеи
    switch (dir) {
    case dir_left:
        head_x--;
        break;
    case dir_right:
        head_x++;
        break;
    case dir_up:
        head_y--;
        break;
    case dir_down:
        head_y++;
        break;
    }
    if (head_x >= game_width || head_x < 0 || head_y >= game_height || head_y < 0) {
        game_over = true; // Змейка умирает при выходе за границы
    }

    // Проверка на столкновение с хвостом
    for (int i = 0; i < tail_length; i++) { // Проверяем весь хвост
        if (tail_x[i] == head_x && tail_y[i] == head_y) {
            game_over = true; // Если столкнулись с хвостом — конец игры
            return; // Прекращаем выполнение функции
        }
    }
    // Проверка на поедание яблока
    if (head_x == fruit_x && head_y == fruit_y) {
        player_points += 10;
        fruit_x = rand() % game_width;
        fruit_y = rand() % game_height;
        tail_length++;
    }
}

// Обработка ввода с клавиатуры
void process_input()
{
    if (_kbhit()) {
        switch (_getch()) {
        case 27: // Клавиша Escape
            game_over = true;
            break;
        case 75: // Влево
            if (dir != dir_right) dir = dir_left; // Запрет поворота на 180°
            break;
        case 77: // Вправо
            if (dir != dir_left) dir = dir_right; // Запрет поворота на 180°
            break;
        case 72: // Вверх
            if (dir != dir_down) dir = dir_up; // Запрет поворота на 180°
            break;
        case 80: // Вниз
            if (dir != dir_up) dir = dir_down; // Запрет поворота на 180°
            break;
        }
    }
}

// Установка сложности игры с обработкой исключений
int set_game_difficulty()
{
    int difficulty = 100;
    char choice;

    while (true) { // Цикл для повторного ввода в случае ошибки
        cout << "\nSET DIFFICULTY\n1: Easy\n2: Medium\n3: Hard\nChoose difficulty level: ";
        cin >> choice;

        try {
            switch (choice) {
            case '1':
                difficulty = 150;
                return difficulty; // Возвращаем значение сложности
            case '2':
                difficulty = 100;
                return difficulty; // Возвращаем значение сложности
            case '3':
                difficulty = 50;
                return difficulty; // Возвращаем значение сложности
            default:
                throw invalid_argument("Invalid choice! Please enter 1, 2, or 3.");
            }
        }
        catch (invalid_argument& e) {
            cerr << e.what() << "\n"; // Выводим сообщение об ошибке
        }
    }
}

// Установка позиции курсора в консоли
void set_console_cursor(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Установка цвета текста
void set_text_color(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Установка заголовка окна
void set_window_title(const string& title)
{
    SetConsoleTitleA(title.c_str()); // Устанавливаем заголовок
}

// Очистка динамической памяти
void free_memory()
{
    delete[] tail_x;
    delete[] tail_y;
}

// Основная функция игры
int main()
{
    string player_name;
    cout << "Enter your name: ";
    cin >> player_name;

    hide_cursor(); // Скрываем курсор
    int difficulty = set_game_difficulty(); // Устанавливаем сложность
    init_game(); // Инициализируем игру
    clock_t start_time = clock(); // Запоминаем время начала игры

    while (!game_over) {
        render_game(player_name); // Отрисовываем игровое поле
        process_input(); // Обрабатываем ввод
        update_game(); // Обновляем состояние игры
        Sleep(difficulty); // Задержка в зависимости от сложности
    }

    clock_t end_time = clock(); // Запоминаем время окончания игры
    double duration = double(end_time - start_time) / CLOCKS_PER_SEC; // Вычисляем продолжительность игры

    // Сообщение о завершении игры
    set_console_cursor(0, game_height + 2);
    cout << "Game Over! Your score: " << player_points << "\n";
    cout << "Time played: " << duration << " seconds\n";

    free_memory(); // Очищаем динамическую память

    
}
