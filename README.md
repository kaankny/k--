"kanyilmaz" - "kan" = "yilmaz"



assign int age = 1;
assign int num = 1 + 5 * (age + 2);
assign string name = "kaan";
assign string surname = "kanyilmaz";
assign string message = "Welcome " + name + " " + surname + "!";

t_ast_node_assign
├── varType: "int"
├── varName: "age"
└── expr: t_ast_node_literal
    ├── valueType: "int"
    └── intValue: 1

t_ast_node_assign
├── varType: "int"
├── varName: "num"
└── expr: t_ast_node_expr (op: '+')
    ├── left: t_ast_node_literal
    │   ├── valueType: "int"
    │   └── intValue: 1
    └── right: t_ast_node_expr (op: '*')
        ├── left: t_ast_node_literal
        │   ├── valueType: "int"
        │   └── intValue: 5
        └── right: t_ast_node_expr (op: '+')
            ├── left: t_ast_node_variable
            │   └── varName: "age"
            └── right: t_ast_node_literal
                ├── valueType: "int"
                └── intValue: 2

t_ast_node_assign
├── varType: "string"
├── varName: "name"
└── expr: t_ast_node_literal
    ├── valueType: "string"
    └── stringValue: "kaan"

t_ast_node_assign
├── varType: "string"
├── varName: "surname"
└── expr: t_ast_node_literal
    ├── valueType: "string"
    └── stringValue: "kanyilmaz"

t_ast_node_assign
├── varType: "string"
├── varName: "message"
└── expr: t_ast_node_expr (op: '+')
    ├── left: t_ast_node_expr (op: '+')
    │   ├── left: t_ast_node_expr (op: '+')
    │   │   ├── left: t_ast_node_expr (op: '+')
    │   │   │   ├── left: t_ast_node_literal
    │   │   │   │   ├── valueType: "string"
    │   │   │   │   └── stringValue: "Welcome "
    │   │   │   └── right: t_ast_node_variable
    │   │   │       └── varName: "name"
    │   │   └── right: t_ast_node_literal
    │   │       ├── valueType: "string"
    │   │       └── stringValue: " "
    │   └── right: t_ast_node_variable
    │       └── varName: "surname"
    └── right: t_ast_node_literal
        ├── valueType: "string"
        └── stringValue: "!"

-------------------------------------------------------------------------------

assign int c = 2 + a;
t_ast_node_assign:
	varType = 'int'
	varName = 'c'
	expr:
		left = t_ast_node_literal:
			intValue = 2
		op = '+'
		right = t_ast_node_variable:
			varName = 'a'


t_ast_node_assign
├── varType: "string"
├── varName: "message"
└── expr: t_ast_node_expr ('+')
    ├── left: t_ast_node_expr ('+')
    │   ├── left: t_ast_node_expr ('+')
    │   │   ├── left: t_ast_node_literal ("Welcome ")
    │   │   └── right: t_ast_node_variable (name)
    │   └── right: t_ast_node_literal (" ")
    └── right: t_ast_node_expr ('+')
        ├── left: t_ast_node_variable (surname)
        └── right: t_ast_node_literal ("!")



function add(a: int, b: int) -> int {
    return a + b;
}
