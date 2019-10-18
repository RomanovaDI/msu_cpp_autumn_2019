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

class calculator{
  private:
    Token_value curr_tok = PRINT;        // Хранит последний возврат функции get_token().
    double number_value;                 // Хранит целый литерал или литерал с плавающей запятой.
    int error_flag = 0;                  // Хранит флаг ошибки.
  public:
    Token_value curr_tok_val();                 // Возвращает значение curr_tok.
    //double expr(std::istream*, bool);           // Обязательное объявление.
    double error(int err_num);                  // Функция error() устанавливает флаг ошибки.
    int ret_err_flag();                         // Возвращает состояние флага ошибки.
    Token_value get_token(std::istream* input); // Берёт символ из потока и анализирует его.
    double prim(std::istream* input, bool get); // prim() - обрабатывает первичные выражения.
    double term(std::istream* input, bool get); // term() - умножение и деление.
    double expr(std::istream* input, bool get); // expr() - сложение и вычитание.
};

/****************************************************************************/

// Возвращает значение curr_tok.
Token_value calculator::curr_tok_val() {
  return curr_tok;
}

// Возвращает состояние флага ошибки.
int calculator::ret_err_flag() {
  return error_flag;
}

// Функция error() устанавливает флаг ошибки.
double calculator::error(int err_num) {
  error_flag = 1;
  std::string err_message;
  if (err_num == 0)
    err_message = "Divide by 0";
  if (err_num == 1)
    err_message = "Bad Token";
  if (err_num == 2)
    err_message = "primary expected";
  if (err_num == 3)
    err_message = "Error input";
  if (err_num == 4)
    err_message = "Empty input";
  std::cerr << "error: " << err_message << std::endl;
  return 1;
}

// Берёт символ из потока и анализирует его.
Token_value calculator::get_token(std::istream* input) {
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
      error(1);
      return curr_tok = ERR;
  }
}

/* Каждая функция синтаксического анализа принимает аргумент типа bool
 * указывающий, должна ли функция вызывать get_token() для получения
 * очередной лексемы. */

// prim() - обрабатывает первичные выражения.
double calculator::prim(std::istream* input, bool get) {
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
      return error(2);
  }
}

// term() - умножение и деление.
double calculator::term(std::istream* input, bool get) {
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
        return error(0);
      default:
        return left;
    }
  }
}

// expr() - сложение и вычитание.
double calculator::expr(std::istream* input, bool get) {
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

  calculator lets_calc;

  switch (argc) {
    case 2:
      input = new std::istringstream(argv[1]);
      break;
    default:
      lets_calc.error(3);
      return 1;
  }

  lets_calc.get_token(input);
  if (lets_calc.curr_tok_val() == END) {
    lets_calc.error(4);
  }
  // Снимает ответственность expr() за обработку пустых выражений.
  if (lets_calc.curr_tok_val() == PRINT) {
    lets_calc.error(4);
  }
  if (lets_calc.curr_tok_val() == ERR) {
    lets_calc.error(3);
  }

  // expr() -> term() -> prim() -> expr() ...
  double result = 0;
  if (!lets_calc.ret_err_flag())
    result = lets_calc.expr(input, false);
  if (!lets_calc.ret_err_flag())
    std::cout << result;

  delete input;
  return lets_calc.ret_err_flag();
}