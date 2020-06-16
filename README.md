<assignment> = <identifier> "=" <expression> 
<expression> 	= <term> [ <("+" | "-")> <term>]*
<term>		= <factor> [<("*" | "/")> <factor>]*
<factor>	= <sinal> "(" <expression> ")"
<sinal>		= {"-"} (<number> | <identifier>)
<identifier> = <variable> | <function>