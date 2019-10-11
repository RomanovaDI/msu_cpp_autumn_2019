#include <cctype>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

enum Token_value : char {
  NUMBER,           END,               ERR,
  PLUS='+',         MINUS='-',         MUL='*',        DIV='/',
  PRINT=';',
};

enum Number_value : char {
  NUM0='0', NUM1='1', NUM2='2',
  NUM3='3', NUM4='4', NUM5='5',
  NUM6='6', NUM7='7', NUM8='8',
  NUM9='9',
};

Token_value curr_tok = PRINT;        // Хранит последний возврат функции get_token().
double number_value;                 // Хранит целый литерал или литерал с плавающей запятой.
int error_flag;                    // Хранит флаг ошибки.

double expr(std::istream*, bool);    // Обязательное объявление.

/****************************************************************************/

// Функция error() имеет тривиальный характер: инкрементирует счётчик ошибок.
double error(const std::string& error_message) {
  error_flag = 1;
  std::cerr << "error: " << error_message << std::endl;
  return 1;
}

Token_value get_token(std::istream* input) {
  char ch;

  do {    // Пропустить все пробельные символы кроме '\n'.
    if (!input->get(ch)) {
      return curr_tok = END; // Завершить работу калькулятора.
    }
  } while (ch != '\n' && isspace(ch));

  switch (ch) {
    case 0: // При вводе символа конца файла, завершить работу калькулятора.
      return curr_tok = END;
    case PRINT:
    case '\n':
      return curr_tok = PRINT;
    case MUL:
    case DIV:
    case PLUS:
    case MINUS:
      return curr_tok = Token_value(ch); // Приведение к целому и возврат.
    case NUM0: case NUM1: case NUM2: case NUM3: case NUM4:
    case NUM5: case NUM6: case NUM7: case NUM8: case NUM9:
      input->putback(ch); // Положить назад в поток...
      *input >> number_value; // И считать всю лексему.
      return curr_tok = NUMBER;
    default:
      error("Bad Token");
      return curr_tok = ERR;
  }
}

/* Каждая функция синтаксического анализа принимает аргумент типа bool
 * указывающий, должна ли функция вызывать get_token() для получения
 * очередной лексемы. */

// prim() - обрабатывает первичные выражения.
double prim(std::istream* input, bool get) {
  if (get) {
    get_token(input);
  }

  switch (curr_tok) {
    case NUMBER: {
      double v = number_value;
      get_token(input);
      return v;
    }
    case MINUS:
      return -prim(input, true);
    default:
      return error("primary expected");
  }
}

// term() - умножение и деление.
double term(std::istream* input, bool get) {
  double left = prim(input, get);
  for ( ; ; ) {
    switch (curr_tok) {
      case MUL:
        left *= prim(input, true);
        break;
      case DIV:
        if (double d = prim(input, true)) {
          left /= d;
          break;
        }
        return error("Divide by 0");
      default:
        return left;
    }
  }
}

// expr() - сложение и вычитание.
double expr(std::istream* input, bool get) {
  double left = term(input, get);
  for ( ; ; ) {
    switch (curr_tok) {
      case PLUS:
        left += term(input, true);
        break;
      case MINUS:
        left -= term(input, true);
        break;
      default:
        return left;
    }
  }
}

int main(int argc, char* argv[]) {
  std::istream* input = nullptr; // Указатель на поток.

  switch (argc) {
    case 2:
      input = new std::istringstream(argv[1]);
      break;
    default:
      error("Error input");
      return 1;
  }

  while (*input) {
    get_token(input);
    if (curr_tok == END) {
      break;
    }

    // Снимает ответственность expr() за обработку пустых выражений.
    if (curr_tok == PRINT) {
      continue;
    }

    if (curr_tok == ERR) {
      break;
    }

    // expr() -> term() -> prim() -> expr() ...
    std::cout << expr(input, false);
  }

  return error_flag;
}