## assign
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
```
