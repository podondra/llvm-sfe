LD = clang++
CXX = clang++
DEBUG = -g -O0
CXXFLAGS = -std=c++11 -pedantic-errors -Wall $(DEBUG) -Wno-deprecated-register
FLEX = flex++
FLEXFLAGS= -Cfa --8bit --noyywrap --warn

parser_test: ast.o parser.o parser_test.o lexer.o
	$(LD) $(LDFLAGS) -o $@ $^

parser_test.o: parser_test.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $^

parser.o: parser.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $^

ast.o: ast.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $^

lexer_test: lexer.o lexer_test.o
	$(LD) $(LDFLAGS) -o $@ $^

lexer_test.o: lexer_test.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $^

lexer.o: lexer.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $^

lexer.cc: lexer.l
	$(FLEX) $(FLEXFLAGS) -o $@ $^

clean:
	$(RM) lexer.cc *.o lexer_test parser_test
