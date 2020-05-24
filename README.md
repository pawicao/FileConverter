# File Converter
**_Compilers and compiler design theory_ AGH course project**<br>

The goal is to turn a source XML file into an Abstrax Syntax Tree which can be later parsed to any other format or just stored.
The process is divided into two parts - lexing and parsing. Lexing is basically converting source file into a sequence of tokens that will later be used by parser. Parsing is a process of turning this sequence of tokens into an AST.

**1. XML -> Abstract Syntax Tree**

For XML the tokens that can be output from lexer are listed below

 - XML_EQUALS (=),  
 - XML_ATTRIBUTE, 
 -  XML_LBRACKET (<),   
 - XML_LSLASH (</),    
 - XML_RBRACKET(>),  
 - XML_RSLASH(/>),  
 -  XML_VALUE,
 
Lexer is based on a finite state machine - it reads characters from the source until it's sure that it has read a token and passes this token to parser. In some cases certain amount of lookahead is required - for instance if lexer reads '<', it cannot know whether it is XML_LBRACKET or beginning of XML_LSLASH - therefore it must read another characted and decide basing on whether this character equals '/' or not and then if it's not '/' lexer must put it back as it is the beginning of a new token.
More TBD

**2. JSON**

Firstly we have to define tokens for JSON. A token is a structure that describes and categorizes an element from your text input. In case of JSON possible tokens are:

 - LEFT_BRACE = '{'
 - RIGHT_BRACE = '}'
 - LEFT_BRACKET = '['
 - RIGHT_BRACKET = ']'
 - COLON = ':'
 - COMMA = ','
 - KEY (always String)
 - (VALUE)
 
Possible value types, defined as tokens:

- STRING 
- NUMBER
- TRUE = true
- FALSE = false
- JNULL = null





 
 
 
_Oskar Pawica, Joanna Kusek, Filip Jachura_
