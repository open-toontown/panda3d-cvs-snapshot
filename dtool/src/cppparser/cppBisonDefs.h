// Filename: cppBisonDefs.h
// Created by:  drose (17Jan99)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://www.panda3d.org/license.txt .
//
// To contact the maintainers of this program write to
// panda3d@yahoogroups.com .
//
////////////////////////////////////////////////////////////////////

#ifndef CPPBISON_H
#define CPPBISON_H

// This header file defines the interface to the yacc (actually,
// bison) parser and grammar.  None of these interfaces are intended
// to be used directly; they're defined here strictly to be used by
// the CPPParser and CPPExpressionParser classes.

#include <dtoolbase.h>

#include <string>

#include "cppExtensionType.h"
#include "cppFile.h"

using namespace std;

class CPPParser;
class CPPExpression;
class CPPPreprocessor;
class CPPDeclaration;
class CPPInstance;
class CPPType;
class CPPStructType;
class CPPEnumType;
class CPPSimpleType;
class CPPInstanceIdentifier;
class CPPParameterList;
class CPPTemplateParameterList;
class CPPScope;
class CPPIdentifier;

void parse_cpp(CPPParser *cp);
CPPExpression *parse_const_expr(CPPPreprocessor *pp,
                                CPPScope *new_current_scope,
                                CPPScope *new_global_scope);
CPPType *parse_type(CPPPreprocessor *pp,
                    CPPScope *new_current_scope,
                    CPPScope *new_global_scope);

extern CPPScope *current_scope;
extern CPPScope *global_scope;
extern CPPPreprocessor *current_lexer;


// This structure holds the return value for each token.
// Traditionally, this is a union, and is declared with the %union
// declaration in the parser.y file, but unions are pretty worthless
// in C++ (you can't include an object that has member functions in a
// union), so we'll use a class instead.  That means we need to
// declare it externally, here.

class YYSTYPE {
public:
  string str;
  union {
    int integer;
    double real;
    CPPScope *scope;
    CPPDeclaration *decl;
    CPPInstance *instance;
    CPPType *type;
    CPPStructType *struct_type;
    CPPEnumType *enum_type;
    CPPSimpleType *simple_type;
    CPPInstanceIdentifier *inst_ident;
    CPPParameterList *param_list;
    CPPTemplateParameterList *template_param_list;
    CPPExtensionType::Type extension_enum;
    CPPExpression *expr;
    CPPIdentifier *identifier;
  } u;
};

// This structure takes advantage of a bison feature to track the
// exact location in the file of each token, for more useful error
// reporting.  We define it up here so we can reference it in the
// lexer.

struct cppyyltype {
  int timestamp;
  int first_line;
  int first_column;
  int last_line;
  int last_column;
  char *text;
  CPPFile file;
};
#define YYLTYPE cppyyltype

#endif
