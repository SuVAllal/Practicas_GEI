let rec fib n =
	if n <= 1 then n
	else fib (n-1) + fib (n-2);;

let rec fibto n i =
	if fib i <= n then begin
		print_int (fib i);
		print_newline();
		fibto n (i + 1)
	end
	else print_newline();;


let () =
	if Array.length Sys.argv <> 2 then
		print_string("fibto: Invalid number of arguments\n")
	else
		let n = int_of_string Sys.argv.(1) in
		fibto n 0;;