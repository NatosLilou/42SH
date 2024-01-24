Final Grammar of our shell:
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
    (continues on next page)
    18
    (continued from previous page)
    rule_for =
    'for' WORD ( [';'] | [ {'\n'} 'in' { WORD } ( ';' | '\n' ) ] ) {'\n'} 'do' compound_list
    ,→'done' ;
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

6.1 Step 1
6.1.1 Getting started
This section does not give any assignment, but rather advice about how to properly start the project.
Lexer / Parser
For this step, you will already need to execute commands. To do so you will need an AST. But this AST
is given by a parser which itself depends on a lexer.
You cannot afford to wait until you have a fully functional lexer to start writing your parser. Likewise
you cannot wait for the parser to handle every rule to start writing execution functions.
You should first write a first temporary rudimentary lexer and parser to be able to execute something
early in the step. Even if it means rewriting a better lexer and parser later.
This first lexer should only lex what is needed this step, that is:
• if
• then
• elif
• else
• fi
• ;
• \n
• '
• words
6.1.2 Usage
There are three ways 42sh must read its input Shell program:
• It must read its input from a string, given using -c
42sh$ ./42sh -c "echo Input as string"
Input as string
• It must read from a file, directly given as a positional argument
42sh$ cat -e script.sh
echo Input as file$
42sh$ ./42sh script.sh
Input as file
23
• It must also read commands from standard input when no other source is provided.
42sh$ cat -e script.sh
echo Input through stdin$
42sh$ ./42sh < script.sh
Input through stdin
42sh$ cat script.sh | ./42sh
Input through stdin
Your project needs to interpret its command line arguments. If an invalid option is detected, you must
print an error message and a usage message, both on the error output, and exit with a an error1
.
The command line syntax is: 42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]
Your 42sh must accept at least the following option:
• -c [SCRIPT] instead of reading the script from a file, directly interpret the argument as a shell
script
There is no other mandatory option, but you are free to implement some if it can ease your development or debugging process.
For instance you could implement a --verbose option for logging, and the --pretty-print option,
introduced elsewhere in this subject.
Tips
Consider using fmemopen(3) to handle string inputs as FILEs.
Pitfalls:
• Implement all three ways of reading input as we will test them.
Pretty printer
During development, the ASTs your parser outputs might contain mistakes. In order to uncover those
mistakes, understand what part of your code is at fault and move on, you have to compare what you
expected to parse with what you actually parsed. This process is a whole lot easier when you can
display the output of your parser.
Pretty-printing is a very efficient way to diagnose these bugs. Pretty-printing is the process of writing
your AST (or any data structure) as easy to read text. As an example, the following command:
if echo ok; then echo foobar > example.txt; fi
Could be pretty-printed as:
if { command "echo" "ok" }; then { redir ">example.txt" command "echo" "foobar"; }
The format is up to you, of course.
Directly seeing the shape and contents of your AST will save you a lot of time during debugging sessions
on your own or with the assistants. To visualize the AST you can use visual tools like Graphviz.
1 An exit status is an error if it’s not zero
24
What you can do is printing an output you can reparse. By doing that, you can test the output by giving
it as argument to your 42sh.
You should be able to enable this feature through a command-line option or an environment variable
when calling your program.
For instance:
./42sh --pretty-print example.sh
or
PRETTY_PRINT=1 ./42sh example.sh
This feature does not take a lot of time to implement, and brings immense quality of life improvements
during development. You should probably implement it as soon as you have an AST. Do not restrain
yourself and feel free to add any information you want.
6.1.3 Exit Status and Errors
You have to handle errors as described in the corresponding section of the SCL.
Going further...
In 42sh, we use the exit code 2 for command line argument and grammar errors.
6.1.4 Simple commands
Tips
Most features have a few lines of grammar at the top, which describes what the minimum grammar
requirements for this feature are.
Please read the section of the guide about shell syntax to learn more.
To avoid copying the entire shell grammar for every feature, every grammar block only shows the
changes compared to the previous feature.
In case this evolution is unclear, the guide contains a full shell grammar.
input =
list '\n'
| list EOF
| '\n'
| EOF
;
(* Of course, many of these rules makes poor sense for now... *)
list = and_or ;
and_or = pipeline ;
pipeline = command ;
command = simple_command ;
(continues on next page)
25
(continued from previous page)
simple_command = WORD { element } ;
element = WORD ;
Implement execution of simple commands such as ls /bin. You will need:
• a lexer which produces word tokens
• a parser that accept word tokens and produces at least a simple_command AST node
• an execution module which travels your AST and executes the simple_command node, waits for
its status code, and returns it
Tips
Use the execvp(3) version of exec(3), it searches the location of the executable in the PATH for you.
6.1.5 Command lists
(* list makes more sense now, isn't it ? *)
list = and_or { ';' and_or } [ ';' ] ;
and_or = pipeline ;
pipeline = command ;
command = simple_command ;
At this stage of the project, command, pipeline, and and_or are the same as simple_command. As you
implement more features, this will change.
Your shell has to be able to group commands together in a list. At this stage, you only have to handle
command lists as follows:
# this line must be represented in a single `command_list` AST node
echo foo; echo bar
# just like this one with a semicolon at the end
echo foo; echo bar;
In order to handle command lists:
• your lexer must recognize ; tokens
• you need to parse and design a special AST node for command lists
6.1.6 “If” commands
(* command can now also be a shell_command *)
command =
simple_command
| shell_command
;
(* for the time being, it is limited to a single rule_if *)
(continues on next page)
26
(continued from previous page)
shell_command = rule_if ;
rule_if = 'if' compound_list 'then' compound_list [else_clause] 'fi' ;
else_clause =
'else' compound_list
| 'elif' compound_list 'then' compound_list [else_clause]
;
compound_list = and_or [';'] {'\n'} ;
You have to handle if commands, as in the SCL):
• your lexer must recognize if, then, elif and else as special token types
• your AST has to have a node for conditions
• your parser must handle the if token returned by the lexer, parse the condition, the then token,
the true branch, a series of elif branches, the else branch, and finally, fi
• your execution module must evaluate the condition, and run the then or the else compound_list
depending on its exit code
Pitfalls:
    • The condition and body of ifs are compound lists. We’ll implement them in the next part but note how we do not simply reduce the rule to and_or. We need separators to delimit commands and recognize if’s separator tokens such as then and fi.

6.1.7 Compound lists
    compound_list =
    {'\n'} and_or { ( ';' | '\n' ) {'\n'} and_or } [';'] {'\n'} ;
    Compound lists are just like command lists, with a few tweaks:
    • this variant only appears inside code blocks such as conditions or functions
    • compound list can separate commands using newlines instead of ;
    Compound lists are what enables conditions like this:
    if false; true; then
    echo a
    echo b; echo c;
    fi
    Or even:
    if false
    true
    then
    echo a
    echo b; echo c
    fi
    In order to handle compound lists:
    • your lexer must recognize newline tokens
    • you do not need another AST type, as compound lists are executed just like lists

6.1.8 Single quotes
    Implementing this feature takes two main changes:
    • implement lexing of single quotes
    • implement expansion of single quotes during execution
    The behavior of single quotes is specified by the SCL.

6.1.9 True and False builtins
    As builtins, true and false are parsed as simple commands, but are not executed in the same manner:
    they are directly executed inside your shell, without requiring fork or exec.
    true (resp. false) must do nothing and return 0 (resp. 1).
6.1.10 The echo built-in
    The echo builtin command prints its arguments separated by spaces, and prints a final newline. Your implementation of this command does not have to comply with POSIX.
    You have to handle the following options:
    • -n inhibits printing a newline.
    • -e enable the interpretation of \n, \t and \\ escapes.
    • -E disable the interpretation of \n, \t and \\ escapes.
    Going further...
    You need to call fflush(stdout) after running builtins.

6.1.11 Comments
    Implement the recognition of comments. As specified by the SCL, a comment always start with a # and
    can only be on one line.
    # This is a comment echo "Hello world"
    # This is also a comment
    Be careful!
    The # character is not considered as the beginning of a comment if it is quoted, escaped or is not
    the first character of a word.
    42sh$ echo \#escaped "#"quoted not#first #commented
    #escaped #quoted not#first
6.2 Step 2

6.2.1 Redirections
    command =
    simple_command
    | shell_command { redirection }
    ;
    simple_command =
    prefix { prefix }
    | { prefix } WORD { element }
    ;
    prefix = redirection ;
    redirection = [IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' ) WORD ;
    element =
    WORD
    | redirection
    ;

6.2.3 Negation
    pipeline = ['!'] command { '|' {'\n'} command } ;
    Adding a ! reverses the exit status of the pipeline (even if there is no pipe):
    • true exits 0
    • ! true exits 1
    • false exits 1
    • ! false exits 0
    Pitfalls:
        • Even if negation appears inside pipelines in the grammar, it makes little sense to perform negation inside a pipeline AST node. You can just have a separate AST node and only create it when needed.

6.2.4 “While” and “until” commands
    shell_command =
    rule_if
    | rule_while
    | rule_until
    ;
    rule_while = 'while' compound_list 'do' compound_list 'done' ;
    rule_until = 'until' compound_list 'do' compound_list 'done' ;
    Implement the execution of while and until loops, as specified by the SCL.

6.2.5 Operators
and_or = pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;

6.2.6 Double Quotes and Escape Character
    Implement the lexing and expansion of double quotes and escape characters, as described in the SCL.

6.2.7 Variables
    prefix =
    ASSIGNMENT_WORD
    | redirection
    ;
    Implement variable assignment and simple variable subsitutions, as described by the SCL.
    ASSIGNMENT_WORD is a special WORD as specified by the SCL. You do not have to implement expansion modifiers, only the “$name” and “${name}” formats will be tested
    The following special variables must be properly expanded:
    • $@
    • $*
    • $?
    • $$
    • $1 … $n
    • $#
    • $RANDOM
    • $UID
    The following environment variables1 must also be properly expanded:
    • $OLDPWD
    • $PWD
    • $IFS

6.2.8 “For” commands
    shell_command =
    rule_if
    | rule_while
    | rule_until
    | rule_for
    ;
    rule_for =
    'for' WORD ( [';'] | [ {'\n'} 'in' { WORD } ( ';' | '\n' ) ] ) {'\n'} 'do' compound_list
    ,→'done' ;

6.3 Step 3

6.3.1 Built-in commands
“exit”
    Implement the exit builtin.
    For more information about exit, please refer to the SCL
    Pitfalls:
        • All resources should be released (allocated memory and file descriptors) before calling exit. The easiest way to handle this is to have a special kind of “error” which stops execution and exits normally.
    
“cd”
    Implement the cd builtin. You do not have to implement the -L and -P options, nor to follow the required behavior for the CDPATH variable. However, you have to implement cd -. For more information about cd, please refer to the SCL. You must also update the PWD and OLDPWD environment variables. Beware, the shell keeps track of the path which was taken through symlinks:
    mkdir -p /tmp/test_dir
    ln -s /tmp/test_dir /tmp/link
    cd /tmp/link
# the shell knows the current directory is also known as /tmp/link
echo "$PWD"
# pwd does not, as it is an external command. env -i ensures the PWD
# environment variable is not passed down, and avoids executing a potential
# builtin implementation of pwd
env -i pwd
“export”
    Implement the export builtin. You do not have to handle printing all exported variables only the export NAME=VALUE and export NAME uses will be tested. For more information about export, please refer to the SCL.

“continue” and “break”
    Implement the continue and break builtins. For more information about these, please refer to the SCL:
    • SCL specification for continue
    • SCL specification for break
Pitfalls:
    • Mind the corner cases of breaking / continuing out of more loops than are currently active.
    
“dot”
    Implement the . builtin as specified by the SCL.

“unset”
    Implement the unset builtin with all its options, as specified by the SCL.
6.3.2 Command blocks :
    shell_command =
    '{' compound_list '}'
    | rule_if
    | rule_while
    | rule_until
    | rule_for
    ;
    Command blocks are a way to explicitly create command lists. These are useful for making function bodies, as well as grouping commands together in redirections, but can be used anywhere.
    { echo a; echo b; } | tr b h
    foo() { echo this is inside a command block; }

6.3.3 Functions :
    command =
    simple_command
    | shell_command { redirection }
    | funcdec { redirection }
    ;
    funcdec = WORD '(' ')' {'\n'} shell_command ;
    Implement function definition and execution. This includes, of course, redirections to functions and
    argument transmission. Don’t forget to take a look at the SCL part on function definition.
    Pitfalls:
        • Functions have to get a reference to some part of your AST, which should otherwise be freed at the end of each command. It means that you either have to make a copy of part of your AST (the body of the function), or prevent it from being freed at the end of this “line”. This can be accomplished fairly easily using reference counting.
        • Functions can be defined in any command. This is valid:
            foo() {
            bar() {
            echo foobar
            }
            }
            # defines bar
            foo
            # prints foobar
            bar

6.3.4 Command Substitution:
    Implement command substitution as described by the SCL.
    Pitfalls:
        • At this point, your lexer needs to be recursive and remember what kind of context is currently active. The context is saved when entering a new context and restored when leaving a context.
        • Your will have a hard time keeping your lexer and expansion in sync unless you create some kind of library.
6.3.5 Subshells:
    shell_command =
    '{' compound_list '}'
    | '(' compound_list ')'
    | rule_for
    | rule_while
    | rule_until
    | rule_if
    ;
    Subshells run commands in a new process.
    42sh$ a=sh; (a=42; echo -n $a);echo $a
    42SH
