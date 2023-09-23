grammar ChemEq;
WS          : [ \t\r\n]+ -> skip ;
Number      : [0-9]+ ;
Element     : [A-Z] | [A-Z] [a-z] ;
subspecies  : Element | Element '_' Number ;
polyatomic  : '(' (subspecies)+ ')' '_' Number ;
species     : (subspecies)+ | (subspecies)+ (polyatomic)+ ;
term        : species | Number species ;
products    : term | products '+' term ;
reactants   : products ;
equation    : reactants '=' products ;
