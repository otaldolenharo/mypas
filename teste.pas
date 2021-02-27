program Teste;

	{ lexical level = 1 }

	{ initiate declarative scope }
	var i, j, n: integer;
	    x, y: real;
	    t, w: double;

	function fact(n: integer): integer;
		{ declarative scope }
		var m: integer;
		{ imperative scope }
		begin
			if n > 2 then
				m := n * fact(n - 1)
			else
				m := n
			return m;
		end;

	{ end declarative scope }

	{ imperative scope }

	begin

		t := w * fact (j)
	end.

	O resto é comentário

