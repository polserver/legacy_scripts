const CONST_E := 2.718282;
const CONST_PI := 3.141592;


function Log(base, answer)

	local x := 1.000;
	local logcount := 0.000;
	local diff := 1.000;
	local success := 0;

	While (success = 0)

		x := x * base;

		If (Abs(answer - x) < 0.001)
			logcount := logcount + 1;
			success := 1;
		else
			If (x > answer)
				diff := diff / 2;
				logcount := logcount - diff;
				x := Exp(base, logcount);
			else
				logcount := logcount + diff;
			endif
		endif
	
		Count := Count + 1;

		If (Count > 200) 
			success := 1;
		endif

	endwhile

	return logcount;

endfunction


function Abs(x)

	if (x < 0)
		x := 0 - x;
	endif
	return x;

endfunction


function Mod(Numerator, Denominator)

	While (Numerator > Denominator)
		Numerator := Numerator - Denominator;
	endwhile

	return Numerator;

endfunction


function Trunc(number, places)

	local base := Exp(10, places);

	number := number * base;
	number := Cint(number);
	number := CDbl(number);
	number := number / base;

	return number;

endfunction


function Fac(factorial);

	local x;
	While (factorial > 0)
		x := x * factorial;
		factorial := factorial - 1;
	endwhile
	return x;

endfunction


function Exp(base, power)

	local i;
	local x := 1;
	local check := Cint(power);
	if (power = check)
		for(i:=1;i<=power;i:=i+1)
			x := x * base;
		endfor
		return x;
	else
		local m, n;
		m := power - check;
		m := m * 1000;
		n := YRoot(base, 1000);
		x := Exp(n, m);
		return x;
	endif

endfunction


function YRoot(x, y)

	
	local i := 0.000;
	local n := 0.000;
	local root := 2.000;
	local a := CInt(Sqr(x));

	if (a < 4)
		a := 4;
	endif

	local success := 0;
	
	
	While (success = 0)		

		n := Exp(root, y);
		i := x - n;
		i := i / (a * root);
		i := Trunc(i, 3);
		root := root + i;
		if (i = 0)
			success := 1;
			if ( Exp(Root, y) > x )
				root := root - 0.001;
			endif
			if ( Exp((root + 0.001), y) = x)
				root := root + 0.001;
			endif
		endif
	endwhile

	return root;

endfunction


function Sqr(x)

	local i := 0.000;
	local n := 0.000;
	local root := 2.000;
	local success := 0;
	
	While (success = 0)		

		n := Exp(root, 2);
		i := x - n;
		i := i / (2 * root);
		i := Trunc(i, 3);
		root := root + i;
		if (i = 0)
			success := 1;
			if ( Exp(Root, 2) > x )
				root := root - 0.001;
			endif
		endif
	endwhile

	return root;

endfunction


function Mean(ArrayX)

	local n, i:=0.000;

	foreach n in ArrayX
		i := i + ArrayX[n];
	endforeach

	n := CDbl(i / Len(ArrayX) );

	return n;

endfunction


function StdDev(ArrayX)

	local m, n, i := 0.000;
	local ArrayMean := Mean(ArrayX);
	local ArrayLen := Len(ArrayX);

	foreach m in ArrayX
		n := ArrayX[m] - ArrayMean;
		n := Exp(n, 2);
		i := i + n;
	endforeach

	i := i / (ArrayLen - 1);
	n := Sqr (i);

	return n;

endfunction


function ZTest(ArrayX, mu, sigma)

	local i, n;
	local z:= 0.000;
	local ArrayMean := Mean(ArrayX);
	local ArrayLen := Len(ArrayX);

	i := ArrayMean - mu;
	n := sigma / Sqr(ArrayLen);

	z := i/n;

	return := z;

endfunction


function TTest(ArrayX, mu)

	local i, n, s;
	local t:= 0.000;
	local ArrayMean := Mean(ArrayX);
	local ArrayLen := Len(ArrayX);
	local s := StdDev(ArrayX);

	i := ArrayMean - mu;
	n := s / Sqr(ArrayLen);

	t := i/n;

	return := t;

endfunction

function BitIn(ArrayX)

	local i := 8;
	local n;
	local loop := 1;
	local bitvalue := 0;

	While (i >= 0)

		n := 8 - i;
		if (ArrayX[i] = 1)
			bitvalue := bitvalue + Exp(2, n);
		endif
		i := i - 1;

	endwhile

	return bitvalue;

endfunction


function BitOut(x)

	x := Cint(x);
	local loop := 1;
	local bitvalue := 0;
	local ArrayX := {0,0,0,0,0,0,0,0};
	local n;

	while (loop <= 8)

		n := 8 - loop;
		bitvalue := Exp(2, n);
		if ( (x - bitvalue) >= 0)
			ArrayX[loop] := 1;
			x := x - bitvalue;
		endif
		loop := loop + 1;

	endwhile

	return ArrayX;

endfunction


function BitAnd(x, y)

	local ArrayX := BitOut(x);
	local ArrayY := BitOut(y);
	local z := 0;
	local loop := 1;
	local n;

	while (loop <= 8)
	
		n := loop - 1;
		if ( (ArrayX[loop] = 1) AND (ArrayY[loop] = 1) )
			z := z + Exp(2, n);
		endif
		loop := loop + 1;

	endwhile

	return z;

endfunction


function BitOr(x, y)

	local ArrayX := BitOut(x);
	local ArrayY := BitOut(y);
	local z := 0;
	local loop := 1;
	locan n;

	while (loop <= 8)

		n := loop - 1;
		if ( (ArrayX[loop] = 1) OR (ArrayY[loop] = 1) )
			z := z + Exp(2, n);
		endif
		loop := loop + 1;

	endwhile

	return z;

endfunction


function BitXor(x, y)

	local ArrayX := BitOut(x);
	local ArrayY := BitOut(y);
	local z := 0;
	local loop := 1;
	local n;

	while (loop <= 8)

		n := loop - 1;
		if (ArrayX[loop] - ArrayY[loop] <> 0)
			z := z + Exp(2, n);
		endif
		loop := loop + 1;

	endwhile

	return z;

endfunction


function BitEqv(x, y)

	local ArrayX := BitOut(x);
	local ArrayY := BitOut(y);
	local z := 0;
	local loop := 1;
	local n;

	while (loop <= 8)

		n := loop - 1;
		if (ArrayX[loop] - ArrayY[loop] = 0)
			z := z + Exp(2, n);
		endif
		loop := loop + 1;

	endwhile

	return z;

endfunction


function BitImp(x, y)

	local ArrayX := BitOut(x);
	local ArrayY := BitOut(y);
	local z := 0;
	local loop := 1;
	local n;

	while (loop <= 8)

		n := loop - 1;
		if (ArrayX[loop] - ArrayY[loop] < 1)
			z := z + Exp(2, n);
		endif
		loop := loop + 1;

	endwhile

	return z;

endfunction


function BitNot(x)

	local ArrayX := BitOut(x);
	local z := 0;
	local loop := 1;
	local n;

	while (loop <= 8)

		n := loop - 1;
		if (ArrayX[loop] = 0)
			ArrayX[loop] := 1;
			z := z + Exp(2, n);
		else
			ArrayX[loop] := 0;
		endif

		loop := loop + 1;

	endwhile

	return z;

endfunction


function FlagSet(flag, bit);

	bit := 9 - bit;
	
	local FlagArray := BitOut(flag);
	FlagArray[bit] := 1;

	bit := BitIn(FlagArray);

	return bit;

endfunction


function FlagReset(flag, bit);

	local x := 1;
	local y := 0;
	local n := 0;
	
	bit := 9 - bit;
	
	local FlagArray := BitOut(flag);
	FlagArray[bit] := 0;

	While (x <= 8)

		if (FlagArray[x] = 1)
			n := x - 1;
			y := y + Exp(2, n);
		endif

		x := x + 1;

	endwhile

	return y;

endfunction


function FlagCheck(flag, bit);

	local FlagArray := BitOut(flag);
	bit := 9 - bit;

	if (FlagArray[bit] = 1)
		return 1;
	else
		return 0;
	endif

endfunction