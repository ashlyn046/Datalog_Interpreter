#ifndef PROJECT1_STARTER_CODE_IDENTIFIERAUTOMATON_H
#define PROJECT1_STARTER_CODE_IDENTIFIERAUTOMATON_H

#include "Automata/Automaton.h"

class IdentifierAutomaton : public Automaton {
private:
    void S1(const std::string& input);
public:
    IdentifierAutomaton() : Automaton(TokenType::ID) {} // Call the base constructor

    void S0(const std::string& input) override;
};

#endif //PROJECT1_STARTER_CODE_IDENTIFIERAUTOMATON_H
