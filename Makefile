LD = clang++
CXX = clang++
FLEX = flex++
FLEXFLAGS= -Cfa --8bit --noyywrap --warn

lexer_tester: lexer.o lexer_test.o
	$(LD) $(LDFLAGS) -o $@ $^

lexer_test.o: lexer_test.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $^

lexer.o: lexer.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $^

lexer.cc: lexer.l
	$(FLEX) $(FLEXFLAGS) -o $@ $^

clean:
	$(RM) lexer.cc *.o lexer_tester
