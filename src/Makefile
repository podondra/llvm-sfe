LD = clang++
LDFLAGS = $(LLVMFLAGS) -L/home/podondra/pjp/llvm-obj/lib -lLLVMCore -lLLVMSupport -lrt -ldl -ltinfo -lpthread -lm
CXX = clang++
CXXFLAGS = -std=c++11 -pedantic-errors -Wall -Wno-deprecated-register -g
FLEX = flex++
FLEXFLAGS= -Cfa --8bit --noyywrap --warn
LLVMFLAGS = -I/home/podondra/pjp/llvm-3.8.0.src/include -I/home/podondra/pjp/llvm-obj/include  -fPIC -fvisibility-inlines-hidden -Wall -W -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wno-missing-field-initializers -pedantic -Wno-long-long -Wno-uninitialized -Wdelete-non-virtual-dtor -Wno-comment -std=c++11 -ffunction-sections -fdata-sections   -fno-exceptions -fno-rtti -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS

llvm_sfe: parser.o lexer.o ast.o 
	$(LD) $^ -o llvm-sfe $(LDFLAGS)

parser_test: ast.o parser.o parser_test.o lexer.o
	$(LD) $(LDFLAGS) -o $@ $^

parser_test.o: parser_test.cc parser.h lexer.h ast.h
	$(CXX) $(CXXFLAGS) $(LLVMFLAGS) -o $@ -c $<

parser.o: parser.cc parser.h lexer.h ast.h
	$(CXX) $(CXXFLAGS) $(LLVMFLAGS) -o $@ -c $<

ast.o: ast.cc ast.h
	$(CXX) $(CXXFLAGS) $(LLVMFLAGS) -o $@ -c $<

lexer_test: lexer.o lexer_test.o
	$(LD) $(LDFLAGS) -o $@ $^

lexer_test.o: lexer_test.cc lexer.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

lexer.o: lexer.cc lexer.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

lexer.cc: lexer.l
	$(FLEX) $(FLEXFLAGS) -o $@ $^

clean:
	$(RM) lexer.cc *.o lexer_test parser_test llvm-sfe
