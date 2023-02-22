# CS236_FinalProj

This is a project for BYU CS236. Find the specifications below.

## Project 1 Guide
CS236 - Discrete Structures
Instructor: Brett Decker
Spring 2021
CS236 Projects Overview
There are five projects for CS236 that are each a piece of creating an interpreter for Datalog programs (see https://www.geeksforgeeks.org/compiler-vs-interpreter-2/ for
details about interpreters). An interpreter is comprised of three components: the Lexer, the
Parser, and the Execution engine. In Project 1 you will build the Lexer. In Project 2 you
will create the Parser. In Projects 3, 4, and 5 you will develop the Execution engine.
Here is a graphical representation:
A lexer is a program that takes as input a program file and scans the file for tokens. The
output to the lexer is a collection of tokens. These tokens can then be read by the parser.
The parser determines the meaning of the program based on the order and type of tokens.
This meaning is then given to the execution engine, which executes the program.
Why Tokens?
You might be questioning the reason we are converting programming files into tokens. Programming languages are created to allow each programmer to use variable names, method
names, comments, strings, etc. that are specific to him or her or the application domain.
These items are very important for readability and organization to the programmer, but they
are not essential for the computer to execute the code. A lexer turns programmer-specific
“items” into programmer-independent structures called “tokens.” Each token contains the
information the computer needs, namely three items: 1) a token name, which is selected
from a set of predefined names, e.g. “STRING”; 2) the specific “item” for that token, e.g.
"Hello World"; and 3) a line number. This information allows the computer to get one step
closer to execution.
1
Project 1: Lexer
Building a lexer will help us apply our study of finite-state machines and automata. The
lexer is either an implementation – a finite-state machine or several finite-state automata –
that takes the Datalog program file as its input and outputs a collection of tokens. Good
software design is about breaking out the functionality into smaller pieces, which we call decomposition. All projects are required to be completed in C++ and we strongly recommend
using sound Object Oriented (OO) design practices. These guides are meant to help you
learn and use such practices.
This guide contains the approach to implementing a lexer for Project 1. It is an algorithm
called ‘Parallel and Max’ and its OO design closely matches finite-state automata theory
and uses inheritance. Once you have the algorithm implemented, it is easy to build and test
the needed automata classes incrementally. You are required to implement this algorithm.
Remember, this project is the start of your interpreter; it will be used in every other project
in this course. Develop your code in an iterative manner: create a small piece of functionality
and test it as you go. This will save you time in the long run – nothing takes more time
than coding by the “Hack and Pray” philosophy.
Approach: Parallel and Max
First, make sure you watch the Project 1 video in its entirety before reading
this.
This approach will take upfront effort to learn the algorithm, but it will make the overall
coding much easier. It also encourages incremental development and will reduce your time
spent debugging.
You are provided starter code found in the zip file Project1 Starter.zip. This guide
describes some of the code and what you need to do with it.
In this approach we take advantage of FSA theory and string recognition. Note that each
valid Token is a specific string that could be generated by a regular expression (reminder, you
cannot use a regular expression library for this project). That means that each type of token
could define a language accepted by a finite-state automaton. We can use this to decompose
the problem of lexing into multiple finite-state automata, instead of one, monolithic finitestate machine.
Your entry point is main.cpp, which should do the following:
• Your main file should contain only the main function. No global variables. Your
main function should be small and serve the following purposes: verify command-line
arguments, instantiate an instance of the Lexer class, and pass the input to Lexer
and let it run. For this project, your main will then need to get the Tokens from the
Lexer and print them out in a specified format (so the pass-off driver can check it
for correctness). At the end, your main function should perform any clean up (e.g.
deallocation of memory).
2
The Lexer class (.h file) should store a collection of finite-state automata. It also needs to
store all the generated tokens. See the code below:
class Lexer {
private:
std::vector<Token*> tokens;
std::vector<Automaton*> automata;
public:
Lexer() {
tokens = new std::vector<Token*>();
automata = new std::vector<Automaton*>();
// Add all of the Automaton instances
//automata.push_back( your Colon automaton );
//automata.push_back( your Colon Dash automaton );
...
}
~Lexer() {
// TODO clean up all allocated memory
}
void Run(std::string input);
// Other needed methods here
...
}
The Parallel and Max algorithm is implemented in the public method Run. This algorithm
does the following:
• Initialize two variables: maximum read value to 0 and corresponding maximum finitestate automaton (denoted as the “max automaton”) to the first finite-state automaton
in the collection
• For each finite-state automaton
– Give it the input string and have it return whether or not it accepts the string
(first n-symbols of the string)
– Get the number of symbols (characters) read by it
– Update the maximum read value if more than the current value and then update
max automaton to reference the current finite-state automaton
• Now that we’ve found the finite-state automaton that will read the maximum amount
of characters, tell it to generate a Token given the input it could accept
3
• Store new Token and update the input string (remove the characters read – remember
the number of characters in stored in our variable
• While the input has more characters repeat
Here is pseudo code (not syntactically correct C++ code):
Lexer::Run(string input) {
set lineNumber to 1
// While there are more characters to tokenize
loop on input.size() > 0 {
set maxRead to 0
set maxAutomaton to the first automaton in automata
// TODO: you need to handle whitespace inbetween tokens
// Here is the "Parallel" part of the algorithm
// Each automaton runs with the same input
foreach automaton in automata {
inputRead = automaton.Start(input)
if (inputRead > maxRead) {
set maxRead to inputRead
set maxAutomaton to automaton
}
}
// Here is the "Max" part of the algorithm
if maxRead > 0 {
set newToken to maxAutomaton.CreateToken(...)
increment lineNumber by maxAutomaton.NewLinesRead()
add newToken to collection of all tokens
}
// No automaton accepted input
// Create single character undefined token
else {
set maxRead to 1
set newToken to a new undefined Token
(with first character of input)
add newToken to collection of all tokens
}
// Update ‘input‘ by removing characters read to create Token
remove maxRead characters from input
}
}
Note that you must keep track of line numbers for each token. The above pseudo code and algorithm rely on inheritance. There must be some base class for all automata, e.g. Automaton,
4
that contains the following virtual methods: Start, NewLinesRead, and CreateToken. Your
Automaton class definition should resemble the following:
class Automaton
{
protected:
int inputRead = 0;
int newLines = 0;
TokenType type;
public:
Automaton(TokenType type) { this->type = type; }
virtual ~Automaton() {}
// Start the automaton and return the number of characters read
// read == 0 indicates the input was rejected
// read > 0 indicates the input was accepted
int Start(const std::string& input) {
inputRead = 0;
S0(input);
return inputRead;
}
// Every subclass must define this method
virtual void S0(const std::string& input) = 0;
Token* CreateToken(std::string input, int lineNumber) {
return new Token(type, input, lineNumber); }
int NewLinesRead() const { return newLines; }
};
Note that both CreateToken and NewLinesRead are defined, but that Start is a pure-virtual
method and thus must be defined in every class that inherits from Automaton.
You now need to create a class that inherits from Automaton for each type of Token. Somewhat counter-intuitive, you also need to create a subclass for undefined types of Token:
multi-block comments that never end and strings that are never closed. IMPORTANT: The
order of the automata in your collection does matter. Your automata that match to keywords should come first. This will make sure your automaton for a keywords is chosen as
the “max automaton” instead of your identifier automaton (see below for why).
You may be wondering how this algorithm works (or if it even does). Let’s consider the
syntax of a C++ program (you should be able to relate this to the syntax of the Datalog
program you are parsing). How does a compiler tokenize the following line: int intx =
5;? These are the corresponding tokens (while the whitespace is important for separating
tokens, it is just consumed): <integer><identifier="intx"><assignmentoperator>
5
<numberliteral="5"><semicolon>. The compiler has to be able to tokenize “int” differently from “intx.” Using our algorithm, we’d have an automaton to check for the keyword
“int.” We’d also have an automaton for identifiers. When the input is “intx = 5;” (after
tokenizing the first two tokens) the keyword automaton will say that it can accept up to 3
characters: ‘i’, ‘n’, ‘t’, but the identifier will say that is can accept up to 4 characters, so the
identifier automaton will be chosen to create the next token, instead of the keyword. Now
let’s consider the first “int.” As long as your keyword automaton for int is checked first,
when the identifier automaton says it also can read 3 characters, it will not be selected, since
there is already an automaton (your int one) that said it can read 3 characters.
The first place to start is by create FSA diagrams for each of the automaton you will need.
Doing this upfront will reduce coding time and help you work out your understanding of the
token types. Learning how to create diagrams and then turn them into code is an important
software engineering skill. Let’s show the FSA diagrams for the automata that will recognize
the ‘Schemes’ token and the ‘:’ token.
s0 s1 s2 s3 s4 s5 s6 s7
S c h e m e s
s0 s1
:
Note that this approach is very easy to unit test. Each Automaton subclass can be tested
separately. The overall algorithm in the Lexer class can also be tested separately. You can
incrementally build new automata subclasses and add them to the Lexer for testing, thus
increasing the functionality a step at a time.
Conclusion
Start this project as early as possible. You will code better when not rushed, and you will
be more inclined to test as you go (which will reduce overall coding time). See Project 1 on
the course website for requirements and specifications and the starter code.

## Project 2 Guide
CS236 - Discrete Structures
Instructor: Brett Decker
Fall 2021
CS236 Projects Overview
There are five projects for CS236 that are each a piece of creating an interpreter for Datalog programs (see https://www.geeksforgeeks.org/compiler-vs-interpreter-2/ for
details about interpreters). An interpreter is comprised of three components: the Lexer, the
Parser, and the Execution engine. In Project 1 you will build the Lexer. In Project 2 you
will create the Parser. In Projects 3, 4, and 5 you will develop the Execution engine.
Here is a graphical representation:
A parser is a program that takes as input a collection of tokens (from a lexer). The parser
then tries to parse the tokens according to the programming language grammar to determine
correctness. The parser also determines the meaning of the program based on the order and
type of tokens. This meaning is then given to the execution engine, which executes the
program.
Project 2: Parser
Building a parser will help us apply our study of grammars. First, go read the entire
specification for Project 2. Good software design is about breaking out the functionality
into smaller pieces, which we call decomposition. The project specification requires a specific
decomposition of classes used to store the meaning of program. But, before we get to the
meaning we need to parse the tokens to ensure they follow the rules of the Datalog grammar.
You will create a recursive-descent parser for this. See the lecture notes and the practice
(on recursive-descent parsing) for pseudo-code. Consider creating the pseudo-code (maybe
as comments in a C++ file) for the entire recursive-descent parser before writing any code.
This guide will go into the three step process for the project and how to get your code
correct.
1
Datalog Grammar
You should have already read the project specification. The Datalog grammar has to be
understood before coding. The convention with the Datalog grammar is that terminals are
in all caps like LEFT PAREN and that nonterminals are in in lower-camel-case as in scheme
– don’t confuse the nonterminal scheme with the keyword Schemes which is represented as
the terminal SCHEMES. It is important to understand that the Datalog grammar deals with
tokens, not text. Each token created by your lexer is a single terminal. The collection of
tokens creates the terminal string for the parser to check. Let’s explore the grammar a little
more by looking at the scheme nonterminal:
scheme → ID LEFT PAREN ID idList RIGHT PAREN
idList → COMMA ID idList | λ
Look at the production from the idList to λ. This is what makes the list work. It allows
idList to produce zero or more terminal IDs (separated by COMMAs). Let’s look at a concrete
example of a scheme. This is the line in the text file:
snap(S,N,A,P)
Your lexer converts this into tokens:
ID LEFT_PAREN ID COMMA ID COMMA ID COMMA ID RIGHT_PAREN
The logic for parsing the nonterminal scheme will be in a function ParseScheme that takes
as input the above tokens. Given the above input, this function will check and advance past
the first three tokens (ID LEFT PAREN ID) and then call the parse function for idList –
let’s call it ParseIdList. This function will check if the first token is a COMMA or not. If it
is not, the function simply returns. This is how we encode taking the lambda production in
code – do nothing. If the function finds the COMMA it will advance the input and check for
an ID. It throws an exception if there is not an ID. If there is an ID, then the function calls
itself, recursively. Given our above example, see if you can determine how many times the
ParseIdList function will be called. Once this function finally returns back to ParseScheme,
ParseScheme will check for the final RIGHT PAREN. The answer is that ParseIdList gets
called four times, because there are three COMMAs.
Three Step Process
The project specification gives a three step approach to implementing the project. It is
worth mentioning a second time.
• First: write (and test) the recursive-descent parser to check the grammar (don’t build
any data structures)
• Second: write (and test) classes for data structures (Rule, Predicate, Parameter, etc)
• Third: add code to the parser to create data structures (if you have unit tests, you can
re-run them easily to make sure nothing breaks. You can also test this code to make
sure you create the right data structures with the right information)
2
Difficulties
Granted you understand the algorithm for recursive-descent parsing, this project is conceptually easier than Project 1 (if you don’t feel confident with recursive-descent parsing yet,
study the lecture notes again, then get help – once you feel comfortable with the algorithm,
the project will go smoothly). The devil is in the details – if your parsing methods are not
perfect, your parser will not work. The urge to copy and paste is irresistible (at least for
me) because of the similarity of the parsing methods. Unit testing is the key to catching
copy/paste errors (because who wants to do all that typing?).
How to Test
As previously mentioned, good software design is about decomposition and encapsulation.
But these principles can sometimes be at odds with unit testing. Your first design might be
to create Parser class that has a single public entry method, say Parse(...) that takes a
collection of tokens. Then you might add all the nonterminal and terminal specific parsing
methods as private methods. This is not a bad approach; in fact, it is a good one. But it is
hard to test. The only method you can test is the entry one, Parse(...). This means you’ll
have to run the entire parsing code for each test. How do we make testing easier without
violating encapsulation and switching our private methods to be public? Consider putting
extensive debugging information in the exceptions thrown until you have a fully working
parser. It is also acceptable, for this course, to make all function in the Parser class public,
but understand that this is not best-practice.
Conclusion
Start this project as early as possible. You will code better when not rushed, and you will
be more inclined to test as you go (which will reduce overall coding time). See Project 2 on
the course website for requirements and specifications.
3

## Project 2 Guide
CS236 - Discrete Structures
Instructor: Brett Decker
Fall 2021
CS236 Projects Overview
There are five projects for CS236 that are each a piece of creating an interpreter for Datalog programs (see https://www.geeksforgeeks.org/compiler-vs-interpreter-2/ for
details about interpreters). An interpreter is comprised of three components: the Lexer, the
Parser, and the Execution engine. In Project 1 you will build the Lexer. In Project 2 you
will create the Parser. In Projects 3, 4, and 5 you will develop the Execution engine.
Here is a graphical representation:
A parser is a program that takes as input a collection of tokens (from a lexer). The parser
then tries to parse the tokens according to the programming language grammar to determine
correctness. The parser also determines the meaning of the program based on the order and
type of tokens. This meaning is then given to the execution engine, which executes the
program.
Project 2: Parser
Building a parser will help us apply our study of grammars. First, go read the entire
specification for Project 2. Good software design is about breaking out the functionality
into smaller pieces, which we call decomposition. The project specification requires a specific
decomposition of classes used to store the meaning of program. But, before we get to the
meaning we need to parse the tokens to ensure they follow the rules of the Datalog grammar.
You will create a recursive-descent parser for this. See the lecture notes and the practice
(on recursive-descent parsing) for pseudo-code. Consider creating the pseudo-code (maybe
as comments in a C++ file) for the entire recursive-descent parser before writing any code.
This guide will go into the three step process for the project and how to get your code
correct.
1
Datalog Grammar
You should have already read the project specification. The Datalog grammar has to be
understood before coding. The convention with the Datalog grammar is that terminals are
in all caps like LEFT PAREN and that nonterminals are in in lower-camel-case as in scheme
– don’t confuse the nonterminal scheme with the keyword Schemes which is represented as
the terminal SCHEMES. It is important to understand that the Datalog grammar deals with
tokens, not text. Each token created by your lexer is a single terminal. The collection of
tokens creates the terminal string for the parser to check. Let’s explore the grammar a little
more by looking at the scheme nonterminal:
scheme → ID LEFT PAREN ID idList RIGHT PAREN
idList → COMMA ID idList | λ
Look at the production from the idList to λ. This is what makes the list work. It allows
idList to produce zero or more terminal IDs (separated by COMMAs). Let’s look at a concrete
example of a scheme. This is the line in the text file:
snap(S,N,A,P)
Your lexer converts this into tokens:
ID LEFT_PAREN ID COMMA ID COMMA ID COMMA ID RIGHT_PAREN
The logic for parsing the nonterminal scheme will be in a function ParseScheme that takes
as input the above tokens. Given the above input, this function will check and advance past
the first three tokens (ID LEFT PAREN ID) and then call the parse function for idList –
let’s call it ParseIdList. This function will check if the first token is a COMMA or not. If it
is not, the function simply returns. This is how we encode taking the lambda production in
code – do nothing. If the function finds the COMMA it will advance the input and check for
an ID. It throws an exception if there is not an ID. If there is an ID, then the function calls
itself, recursively. Given our above example, see if you can determine how many times the
ParseIdList function will be called. Once this function finally returns back to ParseScheme,
ParseScheme will check for the final RIGHT PAREN. The answer is that ParseIdList gets
called four times, because there are three COMMAs.
Three Step Process
The project specification gives a three step approach to implementing the project. It is
worth mentioning a second time.
• First: write (and test) the recursive-descent parser to check the grammar (don’t build
any data structures)
• Second: write (and test) classes for data structures (Rule, Predicate, Parameter, etc)
• Third: add code to the parser to create data structures (if you have unit tests, you can
re-run them easily to make sure nothing breaks. You can also test this code to make
sure you create the right data structures with the right information)
2
Difficulties
Granted you understand the algorithm for recursive-descent parsing, this project is conceptually easier than Project 1 (if you don’t feel confident with recursive-descent parsing yet,
study the lecture notes again, then get help – once you feel comfortable with the algorithm,
the project will go smoothly). The devil is in the details – if your parsing methods are not
perfect, your parser will not work. The urge to copy and paste is irresistible (at least for
me) because of the similarity of the parsing methods. Unit testing is the key to catching
copy/paste errors (because who wants to do all that typing?).
How to Test
As previously mentioned, good software design is about decomposition and encapsulation.
But these principles can sometimes be at odds with unit testing. Your first design might be
to create Parser class that has a single public entry method, say Parse(...) that takes a
collection of tokens. Then you might add all the nonterminal and terminal specific parsing
methods as private methods. This is not a bad approach; in fact, it is a good one. But it is
hard to test. The only method you can test is the entry one, Parse(...). This means you’ll
have to run the entire parsing code for each test. How do we make testing easier without
violating encapsulation and switching our private methods to be public? Consider putting
extensive debugging information in the exceptions thrown until you have a fully working
parser. It is also acceptable, for this course, to make all function in the Parser class public,
but understand that this is not best-practice.
Conclusion
Start this project as early as possible. You will code better when not rushed, and you will
be more inclined to test as you go (which will reduce overall coding time). See Project 2 on
the course website for requirements and specifications.
3

##Project 4 Guide
CS236 - Discrete Structures
Instructor: Brett Decker
Fall 2020
CS236 Projects Overview
There are five projects for CS236 that are each a piece of creating an interpreter for Datalog programs (see https://www.geeksforgeeks.org/compiler-vs-interpreter-2/ for
details about interpreters). An interpreter is comprised of three components: the Lexer, the
Parser, and the Execution engine. In Project 1 you will build the Lexer. In Project 2 you
will create the Parser. In Projects 3, 4, and 5 you will develop the Execution engine.
Here is a graphical representation:
An execution engine is a program that takes as input the “meaning” of source code (from
a parser). It executes the meaning of the program defined by the source code. For this
project, you will implement the generation of new Datalog facts using the rules to complete
the evaluation of queries for the Datalog execution engine.
Project 4: Execution Engine, Part II
Building this next part of the execution engine for Datalog will help you apply your study of
relational data models. First, go read the entire specification for Project 4. Good software
design is about breaking out the functionality into smaller pieces, which we call decomposition. If you have a good design from your previous projects you should be able to add in the
new functionality with relative easy. If your design is a mess, adding the new functionality
will be a nightmare.
Four Step Process
It is strongly encouraged to approach this project in four steps:
(1) create the natural join function,
(2) create the code to evaluate all rules,
(3) create the fixed-point algorithm code to converge for rule evaluation,
(4) add in the new code to finalize the database interpreter.
1
During and after each step, test your code for correctness and perform clean-up before
moving to the next step (do this after step 4 before trying to pass off). You could write all
the code and then just try to pass off. But then you will have now idea where the error in
your code is. Testing at each step helps you localize your error detection and correction. See
the project specification, lecture note (on Relational Data Model), and the notes from Dr.
Goodrich for details on how to implement the first three steps (the fourth step is dependent
on your code).
Conclusion
Start this project as early as possible. You will code better when not rushed, and you will
be more inclined to test as you go (which will reduce overall coding time). See Project 4 on
the course website for requirements and specifications.
2

## Project 5 Guide
CS236 - Discrete Structures
Instructor: Brett Decker
Fall 2020
CS236 Projects Overview
There are five projects for CS236 that are each a piece of creating an interpreter for Datalog programs (see https://www.geeksforgeeks.org/compiler-vs-interpreter-2/ for
details about interpreters). An interpreter is comprised of three components: the Lexer, the
Parser, and the Execution engine. In Project 1 you will build the Lexer. In Project 2 you
will create the Parser. In Projects 3, 4, and 5 you will develop the Execution engine.
Here is a graphical representation:
An execution engine is a program that takes as input the “meaning” of source code (from a
parser). It executes the meaning of the program defined by the source code. For this project,
you will improve the algorithm that implements the generation of new Datalog facts using
the rules to complete the Datalog engine, which completes the Datalog interpreter.
Project 5: Execution Engine, Part III
Building this next part of the execution engine for Datalog will help you apply your study of
graphs and algorithms, especially depth-first search. First, go read the entire specification
for Project 5. Next, make sure you understand the depth-first search forest algorithm and
the algorithm on finding strongly connected components in directed graphs. Good software
design is about breaking out the functionality into smaller pieces, which we call decomposition. If you have a good design from your previous projects you should be able to add in the
new functionality with relative easy. If your design is a mess, adding the new functionality
will be a nightmare.
Six Step Process
It is strongly encouraged to approach this project in six steps:
(1) create the Graph class (just the data structure),
(2) create the function to build the original and reverse Graphs from your Datalog rules,
1
(3) create two depth-first search function to operate on an object of Graph: 1) returns
the post-ordering; 2) returns the search tree,
(4) create two depth-first search forest functions: 1) returns the post-ordering; 2) returns
the forest,
(5) integrate all of the above code to implement the function for the algorithm for finding
strongly connected components,
(6) integrate all of the above code to modify your rule evaluation based on the order of
the strongly connected components.
During and after each step, test your code for correctness and perform clean-up before moving
to the next step (do this after step 6 before trying to pass off). You could write all the code
and then just try to pass off. But then you will have no idea where errors in your code are.
Testing at each step helps you localize your error detection and correction. Consider having
your Graph class have a map from integer to set, map<int, set<int>>, that contains your
adjacency list (the integers in your map should correlate to the indices of your set of vertices.
See the project specification and lecture notes (on Depth-First Search Forest) for details on
how to implement the steps 2-5 (the last step is dependent on your code).
Pseudo-code
There is pseudo-code in the reading Depth-First Search Forest: Finding Strongly Connected
Components for both depth-first search and depth-first search forest. You will need to add
code to keep track of the postorder. Consider using this pseudo-code as a starting point. Get
your depth-first search code implemented and testing, then implement and test depth-first
search forest. Then add in the postorder code. Decomposition and incremental development
will reduce the number of errors in your code and the time you spend debugging.
Conclusion
Start this project as early as possible. You will code better when not rushed, and you will
be more inclined to test as you go (which will reduce overall coding time). See Project 5 on
the course website for requirements and specifications.
2
