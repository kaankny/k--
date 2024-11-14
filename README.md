## assign
(int, string, bool, float)
```
assign int age = 19;
assign string name = "Kaan";
assign string surname = "Kanyilmaz";
assign string fullName = name + " " + surname;	
assign bool isStudent = true;
```

## write, writeln
```
writeln("Age: " + age);
writeln("Name: " + name);
writeln("Surname: " + surname);
writeln("Full Name: " + fullName);
writeln("Is Student: " + isStudent);
```

## read
```
// SOON
read("Enter your age: ", age);
read("Enter your name: ", name);
read("Enter your surname: ", surname);
read("Are you a student? (true/false): ", isStudent);
```

## if-elseif-else
```
if (age > 18) {
	writeln("You are an adult.");
} elseif (age == 18) {
	writeln("You are 18 years old.");
} else {
	writeln("You are a child.");
} endif;
```

## while
```
assign int i = 0;
while (i < 5) {
	writeln("i: " + i);
	i = i + 1;
} endwhile;
```

## for
```
for (assign int i = 0; i < 5; i = i + 1) {
	writeln("i: " + i);
} endfor;

assign int i = 0;
for (; i < 5; i = i + 1) {
	writeln("i: " + i);
} endfor;

for (;;)
{
	writeln(num);
	num = num + 1;
	if (num == 10)
	{
		break;
	} endif;
} endfor;
```

## funcation definition
```
function isPrime(num: int) -> bool {
	assign bool isPrime = false;
	assign int divisor = 2;
	assign bool isLooping = true;
	assign int remainder = 0;

	if (num <= 1)
	{
		isPrime = false;
		writeln("Gecersiz sayi girdiniz.");
	}
	else
	{
		for (assign int i = 2; (i < num) && (isLooping); i = i + 1)
		{
			remainder = num - (num / i) * i;

			if (remainder == 0)
			{
				isLooping = false;
				isPrime = false;
			}
			else
			{
				isPrime = true;
			} endif;
		} endfor;

		if (isPrime)
		{
			writeln("Girdiginiz sayi asaldir.");
		}
		else
		{
			writeln("Girdiginiz sayi asal degildir.");
		} endif;
	} endif;
	return isPrime;
} endfunction;

```

## function call
```
assign int num = 7;
assign bool isPrime = call isPrime(num);
writeln("isPrime: " + call isPrime(num));	
```

## type casting
```
assign float f = 3.14;
assign int i = (int)f;
writeln("f: " + f);
writeln("i: " + i);
```
