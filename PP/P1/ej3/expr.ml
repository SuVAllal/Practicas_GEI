(*EXPRESIÓN 1*)
();;
(*- : unit = ()*)


(*EXPRESIÓN 2*)
2 + 5 * 3;;
(*- : int = 17*)


(*EXPRESIÓN 3*)
1.25 *. 2.0;;
(*- : float = 2.5*)


(*EXPRESIÓN 4*)
(*2 - 2.0;;*)
(*Error de tipos (compilación), se está operando con un int y un float a la vez*)


(*EXPRESIÓN 5*)
(*3.0 + 2.0*)
(*Error de sintaxis (compilación), debería ser "3.0 +. 2.0"*)


(*EXPRESIÓN 6*)
5 / 3;;
(*- : int = 1*)


(*EXPRESIÓN 7*)
5 mod 3;;
(*- : int = 2*)


(*EXPRESIÓN 8*)
2.0 *. 3.0 ** 2.0;;
(*Devuelve como resultado 18*)
(*- : float = 18.*)


(*EXPRESIÓN 9*)
2.0 ** 3.0 ** 2.0;;
(*- : float = 512.*)


(*EXPRESIÓN 10*)
sqrt;;
(*- : float -> float = <fun>*)


(*EXPRESIÓN 11*)
(*sqrt 4;;*)
(*Error de tipos (compilación), la función debería recibir un flotante*)


(*EXPRESIÓN 12*)
int_of_float;;
(*- : float -> int = <fun>*)


(*EXPRESIÓN 13*)
float_of_int;;
(*- : int -> float = <fun>*)


(*EXPRESIÓN 14*)
3.0 = float_of_int 3;;
(*- : bool = true*)


(*EXPRESIÓN 15*)
(*int_of_float -2.9;;*)
(*Error de tipos (compilación), toma el guión como una resta, no como el valor negativo del número*)


(*EXPRESIÓN 16*)
int_of_float 2.1 + int_of_float (-2.9);;
(*- : int = 0*)


(*EXPRESIÓN 17*)
truncate;;
(*- : float -> int = <fun>*)


(*EXPRESIÓN 18*)
truncate 2.1 + truncate (-2.9);;
(*- : int = 0*)


(*EXPRESIÓN 19*)
floor;;
(*- : float -> float = <fun>*)


(*EXPRESIÓN 20*)
floor 2.1 +. floor (-2.9);;
(*- : float = -1*)


(*EXPRESIÓN 21*)
ceil;;
(*- : float -> float = <fun>*)


(*EXPRESIÓN 22*)
ceil 2.1 +. ceil (-2.9);;
(*- : float = 1.*)


(*EXPRESIÓN 23*)
int_of_char;;
(*- : char -> int = <fun>*)


(*EXPRESIÓN 24*)
int_of_char 'A';;
(*- : int = 65*)


(*EXPRESIÓN 25*)
char_of_int;;
(*- : int -> char = <fun>*)


(*EXPRESIÓN 26*)
char_of_int 66;;
(*- : char = 'B'*)


(*EXPRESIÓN 27*)
Char.code;;
(*- : char -> int = <fun>*)


(*EXPRESIÓN 28*)
Char.code 'B';;
(*- : int = 66;*)


(*EXPRESIÓN 29*)
Char.chr;;
(*- : int -> char = <fun>*)


(*EXPRESIÓN 30*)
Char.chr 67;;
(*- : char = 'C'*)


(*EXPRESIÓN 31*)
'\067';;
(*- : char = 'C'*)


(*EXPRESIÓN 32*)
Char.chr (Char.code 'a' - Char.code 'A' + Char.code 'M');;
(*- : char = 'm'*)


(*EXPRESIÓN 33*)
Char.lowercase_ascii;;
(*- : char -> char = <fun>*)


(*EXPRESIÓN 34*)
Char.lowercase_ascii 'M';;
(*- : char = 'm'*)


(*EXPRESIÓN 35*)
Char.uppercase_ascii;;
(*- : char -> char = <fun>*)


(*EXPRESIÓN 36*)
Char.uppercase_ascii 'm';;
(*- : char = 'M'*)


(*EXPRESIÓN 37*)
"this is a string";;
(*- : string = "this is a string"*)


(*EXPRESIÓN 38*)
String.length;;
(*- : string -> int = <fun>*)


(*EXPRESIÓN 39*)
String.length "longitud";;
(*- : int = 8*)


(*EXPRESIÓN 40*)
(*"1999" + "1";;*)
(*Error de tipos (compilación), el operador + se utiliza en operaciones de enteros, no con strings*)


(*EXPRESIÓN 41*)
"1999" ^ "1";;
(*- : string = "19991"*)


(*EXPRESIÓN 42*)
int_of_string;;
(*- : string -> int = <fun>*)


(*EXPRESIÓN 43*)
int_of_string "1999" + 1;;
(*- : int = 2000*)


(*EXPRESIÓN 44*)
"\065\066";;
(*- : string = "AB"*)


(*EXPRESIÓN 45*)
string_of_int;;
(*- : int -> string = <fun>*)


(*EXPRESIÓN 46*)
string_of_int 010;;
(*- : string = "10"*)


(*EXPRESIÓN 47*)
not true;;
(*- : bool = false*)


(*EXPRESIÓN 48*)
true && false;;
(*- : bool = false*)


(*EXPRESIÓN 49*)
true || false;;
(*- : bool = true*)


(*EXPRESIÓN 50*)
(1 < 2) = false;;
(*- : bool = false*)


(*EXPRESIÓN 51*)
"1" < "2";;
(*- : bool = true*)


(*EXPRESIÓN 52*)
2 < 12;;
(*- : bool = true*)


(*EXPRESIÓN 53*)
"2" < "12";;
(*- : bool = false*)


(*EXPRESIÓN 54*)
"uno" < "dos";;
(*- : bool = false*)


(*EXPRESIÓN 55*)
if 3 = 4 then 0 else 4;;
(*- : int = 4*)


(*EXPRESIÓN 56*)
if 3 = 4 then "0" else "4";;
(*- : string = "4"*)


(*EXPRESIÓN 57*)
(*if 3 = 4 then 0 else "4";;*)
(*Error de tipos (compilación), la sentencia condicional utiliza dos tipos distintos en las salidas, un entero y un string*)


(*EXPRESIÓN 58*)
(if 3 < 5 then 8 else 10) + 4;;
(*- : int = 12*)