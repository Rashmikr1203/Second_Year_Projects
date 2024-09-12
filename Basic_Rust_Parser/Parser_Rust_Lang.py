import ply.lex as lex
import ply.yacc as yacc

tokens = (
    'LITERAL',
    'VARIABLE',
    'ARITHMETIC_OPERATOR',
    'COMPARISON_OPERATOR',
    'FUNCTION_CALL',
    'CONDITIONAL_EXPRESSION',
    'LOGICAL_OPERATOR',
    'ASSIGNMENT_OPERATOR',
    'PARENTHESIZED_EXPRESSION',
    'EQ',
    'FUNCTION_DEFINITION',
    'DECLARATION',
    'STATEMENT',
    'LOOP',
    'CONDITIONAL',
    'PATTERN_MATCHING',
    'WHILE',
    'OPEN',
    'CLOSE',
    'TYPE',
    'LPAREN',
    'RPAREN',
    'ID',
    'COMMA',
    'LET',
    'ELSE',
    'PRINTLN',
    'SEMI',
)

t_PRINTLN = r'println!'
t_LITERAL = r'[a-zA-Z0-9_]+'
t_VARIABLE = r'[a-zA-Z0-9_]+'
t_EQ = r'='
t_OPEN = r'{'
t_CLOSE = r'}'
t_ARITHMETIC_OPERATOR = r'\+|\-|\*|\/'
t_COMPARISON_OPERATOR = r'\==|\!=|\<|\>'
t_FUNCTION_CALL = r'[a-zA-Z0-9_]+\(([^()]|\([^()]*\))*\)'
t_CONDITIONAL_EXPRESSION = r'if\(([^{}]|\{[^{}]*\})*\)\{[^{}]*\}else\{[^{}]*\}'
t_LOGICAL_OPERATOR = r'\&\&|\|\|'
t_ASSIGNMENT_OPERATOR = r'\=|\+\=|\-\='
t_PARENTHESIZED_EXPRESSION = r'\(([^()]|\([^()]*\))*\)'
t_FUNCTION_DEFINITION = r'function\s[a-zA-Z0-9_]+\(([^()]|\([^()]*\))*\)\{[^{}]*\}'
t_DECLARATION = r'var|const'
t_STATEMENT = r'.*?[a-zA-Z0-9_]+;'
t_LOOP = r'for|while'
t_CONDITIONAL = r'if'
t_PATTERN_MATCHING = r'match'
t_WHILE = r'while'
t_LPAREN = r'\('
t_RPAREN = r'\)'
t_COMMA = r','
t_LET = r'let'
t_ELSE = r'else'
t_TYPE = r'[a-zA-Z_][a-zA-Z0-9_]*'
t_ID = r'[a-zA-Z_][a-zA-Z0-9_]*'
t_SEMI=r';'
t_ignore = ' \t\n'
def t_error(t):
    print(f"Illegal character '{t.value[0]}'")
    t.lexer.skip(1)

lexer = lex.lex()

def p_WhileLoop(p):
    'WhileLoop : WHILE Expression OPEN Block CLOSE'
    pass

def p_statement_println(p):
    'Printstatement : PRINTLN Statement SEMI'
    print('Found println statement')

def p_ArithmeticExpression(p):
    '''ArithmeticExpression : Expression ARITHMETIC_OPERATOR Expression'''
    pass

def p_ComparisonExpression(p):
    '''ComparisonExpression : Expression COMPARISON_OPERATOR Expression'''
    pass

def p_FunctionCall(p):
    '''FunctionCall : FUNCTION_CALL'''
    pass

def p_ConditionalExpression(p):
    '''ConditionalExpression : CONDITIONAL_EXPRESSION'''
    pass

def p_LogicalExpression(p):
    '''LogicalExpression : Expression LOGICAL_OPERATOR Expression'''
    pass

def p_AssignmentExpression(p):
    '''AssignmentExpression : VARIABLE ASSIGNMENT_OPERATOR Expression'''
    pass

def p_ParenthesizedExpression(p):
    '''ParenthesizedExpression : PARENTHESIZED_EXPRESSION'''
    pass

def p_FunctionDefinition(p):
    '''FunctionDefinition : FUNCTION_DEFINITION'''
    pass

def p_Declaration(p):
    '''Declaration : DECLARATION LITERAL EQ Expression'''
    pass

def p_Statement(p):
    '''Statement : STATEMENT SEMI'''
    pass

def p_Loop(p):
    '''Loop : LOOP OPEN Block CLOSE'''
    pass

def p_Conditional(p):
    '''Conditional : CONDITIONAL OPEN Block CLOSE'''
    pass

def p_FunctionDefinitionB(p):
    '''FunctionDefinition : FUNCTION_DEFINITION LITERAL Expression OPEN Block CLOSE'''
    pass

def p_PatternMatching(p):
    '''PatternMatching : PATTERN_MATCHING Expression OPEN Block CLOSE'''
    pass

def p_TypeConversion(p):
    '''
    TypeConversion : TYPE LPAREN Expression RPAREN
    '''
    print("Syntax is valid")

def p_tuple_elements(p):
    '''
    tuple_elements : COMMA Type tuple_elements
                   | empty
    '''
    pass

def p_Type(p):
    '''
    Type : TYPE
    '''
    pass

def p_empty(p):
    'empty :'
    pass

def p_TupleDeclaration(p):
    '''
    TupleDeclaration : LPAREN Type tuple_elements RPAREN
    '''
    print("Syntax is valid")

def p_Pattern(p):
    '''
    Pattern : ID
    '''
    pass

def p_LetElseConditional(p):
    '''
    LetElseConditional : LET Pattern EQ Expression ELSE Block
    '''
    print("Syntax is valid")

def p_WhileLetLoop(p):
    '''
    WhileLetLoop : WHILE LET Pattern EQ Expression Block
    '''
    print("Syntax is valid")

def p_Expression(p):
    '''Expression : LITERAL
                   | VARIABLE
                   | ArithmeticExpression
                   | ComparisonExpression
                   | FunctionCall
                   | ConditionalExpression
                   | LogicalExpression
                   | AssignmentExpression
                   | ParenthesizedExpression
                   | FunctionDefinition
                   | Statement

    '''
    pass

def p_Block(p):
    '''Block : OPEN DECLARATION CLOSE
              | OPEN STATEMENT CLOSE
              | OPEN LOOP CLOSE
              | OPEN CONDITIONAL CLOSE
              | OPEN FUNCTION_DEFINITION CLOSE
              | OPEN PATTERN_MATCHING CLOSE
              | OPEN LET CLOSE
              | OPEN WhileLoop CLOSE
              | OPEN Declaration CLOSE
              | OPEN Loop CLOSE
              | OPEN Conditional CLOSE
              | OPEN PatternMatching CLOSE
              | OPEN TypeConversion CLOSE
              | OPEN TupleDeclaration CLOSE
              | OPEN LetElseConditional CLOSE
              | OPEN WhileLetLoop CLOSE 
              | OPEN CLOSE'''
    pass

class MyParserError(Exception):
    pass

def p_error(p):
    raise MyParserError(f"Syntax error at '{p.value}'")

parser = yacc.yacc(errorlog=yacc.NullLogger())

if __name__ == '__main__':
    code = ''
    result = ''
    while True:
        try:
            user_input = input('>>> ')
            code += user_input + '\n'
            lexer.input(code)
            for token in lexer:
                print(token)
            if user_input.strip() == '':
                result = parser.parse(code)
                if not result and code.strip():
                    raise MyParserError("Invalid syntax")
            if isinstance(result, (list, tuple)):
                for statement in result:
                    print(statement)
            else:
                print(result)
            code = ''
        except MyParserError as e:
            print(e)
            print('Error: invalid syntax')
            code = ''  # Reset code to continue parsing
        except Exception as e:
            print(e)
            print('Error: something went wrong')