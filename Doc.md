# Final Grammar of our shell:
    input =
    list '\n'
    | list EOF
    | '\n'
    | EOF
    ;
    
    list = and_or { ( ';' | '&' ) and_or } [ ';' | '&' ] ;
    
    and_or = pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;
    
    pipeline = ['!'] command { '|' {'\n'} command } ;
    
    command =
    simple_command
    | shell_command { redirection }
    | funcdec { redirection }
    ;
    
    simple_command =
    prefix { prefix }
    | { prefix } WORD { element }
    ;
    
    shell_command =
    '{' compound_list '}'
    | '(' compound_list ')'
    | rule_for
    | rule_while
    | rule_until
    | rule_case
    | rule_if
    ;
    
    funcdec = WORD '(' ')' {'\n'} shell_command ;
    
    redirection =
    [IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' ) WORD
    | [IONUMBER] ( '<<' | '<<-' ) HEREDOC
    ;
    
    prefix =
    ASSIGNMENT_WORD
    | redirection
    ;
    
    element =
    WORD
    | redirection
    ;
    
    compound_list =
    {'\n'} and_or { ( ';' | '&' | '\n' ) {'\n'} and_or } [ ';' | '&' ] {'\n'} ;
    
    rule_for =
    'for' WORD ( [';'] | [ {'\n'} 'in' { WORD } ( ';' | '\n' ) ] ) {'\n'} 'do' compound_list 'done' ;
    
    rule_while = 'while' compound_list 'do' compound_list 'done' ;
    
    rule_until = 'until' compound_list 'do' compound_list 'done' ;
    
    rule_case = 'case' WORD {'\n'} 'in' {'\n'} [case_clause] 'esac' ;
    
    rule_if = 'if' compound_list 'then' compound_list [ else_clause ] 'fi' ;
    
    else_clause =
    'else' compound_list
    | 'elif' compound_list 'then' compound_list [ else_clause ]
    ;
    
    case_clause = case_item { ';;' {'\n'} case_item } [';;'] {'\n'} ;
    
    case_item = ['('] WORD { '|' WORD } ')' {'\n'} [compound_list] ;

# Usage
There are three ways 42sh reads its input Shell program:
## Input as string
• It reads its input from a string, given using -c \
`42sh$ ./42sh -c "echo Input as string"`

## Input as file
• It reads from a file, directly given as a positional argument. \
`42sh$ cat -e script.sh` \
`echo Input as file$` \
`42sh$ ./42sh script.sh`

## Input through stdin
• It also reads commands from standard input when no other source is provided. \
`42sh$ cat -e script.sh` \
`echo Input through stdin$` \
`42sh$ ./42sh < script.sh`

## Input through stdin
`42sh$ cat script.sh | ./42sh`


The project interprets its command line arguments. If an invalid option is detected,
an error message and a usage message are printed, both on the error output, and 42sh exit with an error 1. \
The command line syntax is: `42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]`


Your 42sh accepts the following option: \
• -c [SCRIPT] instead of reading the script from a file, directly interpret the argument as a shell script.
