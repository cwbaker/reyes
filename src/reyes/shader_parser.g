
shader {   
    %whitespace "[ \t\r\n]*";
    %left ',';
    %left '(' ')' '[' ']';
    %left 'varying' 'uniform';
    %left 'output' 'extern';
    %left 'float' 'string' 'color' 'point' 'vector' 'normal' 'matrix' 'void' 'light' 'surface' 'volume' 'displacement' 'imager';
    %left 'return' 'break' 'continue' 'if' 'while' 'for' 'solar' 'illuminance' 'illuminate' identifier '{';
    %left string_literal;
    %left unary_minus;
    %right '?';
    %right '=' '+=' '-=' '*=' '/=';
    %left '||';
    %left '&&';
    %left '>' '>=' '<' '<=' '==' '!=';
    %left '+' '-';
    %left '.' '^' '/' '*';
    %left 'else';

    unit: definitions [pass]
        ;
    
    definitions: definitions definition [add_to_list]
               | definition [create_list]
               | %precedence 'float' [empty_list]
               ;
    
    definition: shader_type identifier '(' formals ')' '{' statements '}' [shader_definition]
              | type identifier '(' formals ')' '{' statements '}' [function_definition]
              ;
    
    formals: formals formal [add_to_list]
           | formal [create_list]
           | %precedence 'varying' [empty_list]
           ;           
    
    formal: output detail type definition_expressions ';' [formal]
          ;
      
    variable_definition: extern detail type definition_expressions [variable_definition]
                       ;
            
    definition_expressions: definition_expressions ',' definition_expression [add_to_list]
                          | definition_expression [create_list]
                          ;
            
    definition_expression: identifier [definition_expression]
                         | identifier '=' expression [definition_expression_with_assignment]
                         ;
    
    shader_type: 'light' [light_shader]
               | 'surface' [surface_shader]
               | 'volume' [volume_shader]
               | 'displacement' [displacement_shader]
               | 'imager' [imager_shader]
               ;
               
    type: 'float' [float_type]
        | 'string' [string_type]
        | 'color' [color_type]
        | 'point' [point_type]
        | 'vector' [vector_type]
        | 'normal' [normal_type]
        | 'matrix' [matrix_type]
        | 'void' [void_type]
        ;
        
    detail: 'varying' [varying]
          | 'uniform' [uniform]
          | %precedence 'float' [null]
          ;
            
    output: 'output' [output]
          | %precedence 'output' [null]
          ;
    
    extern: 'extern' [extern]
          | %precedence 'extern' [null]
          ;

    statements: statements statement [add_to_list]
              | statement [create_list]
              | %precedence 'return' [empty_list]
              ;
              
    statement: '{' statements '}' [block_statement]
             | variable_definition ';' [pass]
             | assign_expression ';' [pass]
             | call ';' [pass]
             | 'return' expression ';' [return_statement]
             | 'break' optional_integer ';' [break_statement]
             | 'continue' optional_integer ';' [continue_statement]
             | 'if' '(' expression ')' statement %precedence 'else' [if_statement]
             | 'if' '(' expression ')' statement 'else' statement [if_else_statement]
             | 'while' '(' expression ')' statement [while_statement]
             | 'for' '(' expression ';' expression ';' expression ')' statement [for_statement]
             | 'solar' '(' parameters ')' statement [solar_statement]
             | 'illuminate' '(' parameters ')' statement [illuminate_statement]
             | 'illuminance' '(' parameters ')' statement [illuminance_statement]
             ;

    expression: '(' expression ')' [compound_expression]
              | integer [integer_expression]
              | real [real_expression]
              | string_literal [string_expression]
              | texture [pass]
              | identifier %precedence '(' [identifier_expression]
              | identifier '[' expression ']' [index_expression]
              | call [pass]
              | assign_expression [pass]
              | triple [pass]
              | sixteentuple [pass]
              | expression '.' expression [dot_expression]
              | expression '^' expression [cross_expression]
              | expression '*' expression [multiply_expression]
              | expression '/' expression [divide_expression]
              | expression '+' expression [add_expression]
              | expression '-' expression [subtract_expression]
              | expression '>' expression [greater_expression]
              | expression '>=' expression [greater_equal_expression]
              | expression '<' expression [less_expression]
              | expression '<=' expression [less_equal_expression]
              | expression '==' expression [equal_expression]
              | expression '!=' expression [not_equal_expression]
              | expression '&&' expression [and_expression]
              | expression '||' expression [or_expression]
              | '-' expression %precedence unary_minus [negate_expression]
              | expression '?' expression ':' expression %precedence '?' [ternary_expression]
              | typecast expression %precedence 'else' [typecast_expression]
              ;

    optional_integer: integer [integer_expression]
                    |
                    ;

    integer: "[0-9]+";
    
    real: "[0-9]*\.?[0-9]+([eE](\+|\-)?[0-9]+)?";

    optional_string: string_literal [string_expression]
                   | %precedence string_literal
                   ;
    
    string_literal: "[\"']:string:";
    
    triple: '(' expression ',' expression ',' expression ')' [triple_expression];

    sixteentuple: '(' 
                      expression ',' expression ',' expression ',' expression ','
                      expression ',' expression ',' expression ',' expression ','
                      expression ',' expression ',' expression ',' expression ','
                      expression ',' expression ',' expression ',' expression 
                  ')' [sixteentuple_expression]
                ;
        
    typecast: 'float' [float_type]
            | 'string' [string_type]
            | 'color' optional_string [color_typecast]
            | 'point' optional_string [point_typecast]
            | 'vector' optional_string [vector_typecast]
            | 'normal' optional_string [normal_typecast]
            | 'matrix' optional_string [matrix_typecast]
            ;
            
    assign_expression: identifier '=' expression [assign_expression]
                     | identifier '+=' expression [add_assign_expression]
                     | identifier '-=' expression [subtract_assign_expression]
                     | identifier '*=' expression [multiply_assign_expression]
                     | identifier '/=' expression [divide_assign_expression]
                     | identifier '[' expression ']' '=' expression [index_assign_expression]
                     | identifier '[' expression ']' '+=' expression [index_add_assign_expression]
                     | identifier '[' expression ']' '-=' expression [index_subtract_assign_expression]
                     | identifier '[' expression ']' '*=' expression [index_multiply_assign_expression]
                     | identifier '[' expression ']' '/=' expression [index_divide_assign_expression]
                     ;

    parameters: parameters ',' expression [add_to_list]
              | expression [create_list]
              | [empty_list]
              ;

    call: identifier '(' parameters ')' [call_expression]
        ;

    texture: 'texture' '(' parameters ')' [texture_expression]
           | 'environment' '(' parameters ')' [environment_expression]
           | 'shadow' '(' parameters ')' [shadow_expression]
           ;
           
    identifier: "[A-Za-z_][A-Za-z_0-9]*"
              ; 
}
