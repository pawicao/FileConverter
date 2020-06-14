# File Converter
**_Compilers and compiler design theory_ AGH course project**

_Oskar Pawica, Joanna Kusek, Filip Jachura_

## Description
The goal of the project is to turn a source XML/YAML/JSON file into an Abstrax Syntax Tree which can be later converted to any other format or just stored.
The process is divided into two parts - lexing and parsing. Lexing is basically converting source file into a sequence of tokens that will later be used by parser. Parsing is a process of turning this sequence of tokens into an AST.

## Project structure
### XML
#### XML -> Abstract Syntax Tree

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

### JSON
#### JSON -> Abstract Syntax Tree

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

### YAML
#### YAML -> Abstract Syntax Tree

Parsing YAML requires handling many language-specific elements, such as proper indentation, different kind of scalars (folded block scalars, linear block scalars, plain scalars, quoted scalars) and comments as well as arrays of elements etc. Compared to XML and JSON in most of the cases there are no bracket-wrappers for keys, values and children, therefore tokens must be split into different elements based on the combination of newlines and the size of indent in each line of document.

The lexer tokens are defined as follows:

 - MAPPER_TOKEN (:),
 - QUOTED_SCALAR_TOKEN ("text" or 'text' - both are respected),
 - PLAIN_SCALAR_TOKEN (text),
 - LITERAL_BLOCK_TOKEN (starts with '|' or '|-' and handles blocks of plain text),
 - FOLDED_BLOCK_TOKEN (starts with '>' and handles blocks of plain text, folding the newlines into spaces),
 - HYPHEN_TOKEN (-),
 - INDENT_TOKEN ( ),
 - TRUE_TOKEN (true),
 - FALSE_TOKEN (false),
 - YNULL_TOKEN (null),
 - COMMENT_TOKEN (#),
 - EOL_TOKEN,
 - INVALID_TYPE_TOKEN
 
YAML parser works the same way as described above XML and JSON parsers. Firstly, YAMLLexer reads the characters of files and divides them into tokens. Then the tokens are handled by the YAMLParser which - basing on the indentations and the arrangement of different tokens - recursively creates AST elements.

_Warning: YAML forbid tabs as the method of indentation, therefore only space indents are allowed._

#### Abstract Syntax Tree -> YAML

Converting AST into YAML document is pretty straightforward. Starting from the root for each AST element the algorithm seeks any children which have the same name. If such are found, they are grouped into arrays using boolean flags. Afterwards the elements are written into the YAML file - with proper indentation for all children, arrays and block scalars.

## Usage

After compilation of the project, one must specify the input file (XML, YAML or JSON) using **-i \<PATH>** option.

Other option that has to be specified must be the result language of the conversion (XML, YAML or JSON): **-l \<xml/yaml/json>**.

One can specify the output filename using **-o \<PATH>** option. If no output filename specified, the output is printed to **stdout**.

The whole manual is described below:
```
Usage: FileConverter [options...]
 -i <PATH>              Path of the file to be parsed.
 -o <PATH>              Path of the file to be written to. If not specified, stdout is used.
 -l <xml/yaml/json>     Result language (can be xml, yaml or json).
```
