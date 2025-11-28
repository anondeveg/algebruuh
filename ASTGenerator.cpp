#include <strings.h>
#include <algorithm>
#include <iterator>
#include <vector>
#include "internals.h"

enum NodeType{TERM,BINARY_OP};
struct ASTNode {
	NodeType type;
	term;
	char op;
	ASTNode* right; // have to make it a pointer so that i don't have infinite recursion
	ASTNode* left;

};

void internalPostOrderASTGenerate(int index, std::vector<Term> term){
	if (!(index >= term.size())){
		NodeType AST;

		if NodeType == Term{
		}
		};

		}
}
void ASTGenerate(std::vector<Term> lex){
		std::reverse(lex.begin(),lex.end());
		internalPostOrderASTGenerate(0,lex);
	
	} 




