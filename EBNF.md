# EBNF grammar of sml

```ebnf
program = { statement } ;
statement = expression, ";" ;
expression = term, { ("+" | "-"), term } ;
term = unary, { ("*" | "/"), unary } ;
unary = ["+" | "-"], factor ;
factor = number | ("(", expression, ")") ;
```

`number` represents T_NUMBER.
