#include "finite_automaton.h"

/**
 * This function creates a non deterministic finite automaton.
 * 
 * @param numStates The number of states.
 * @param numSymbols The number of symbols.
 * @param initialState The initial state of the automaton.
 * @param finalStates The set of final states of the automaton.
 * @return The non deterministic finite automaton.
 * 
 */
NDFA createNDFAAutomaton(int numStates, int numSymbols, int initialState, Array finalStates) {
    NDFA automaton;
    automaton.states = numStates;
    automaton.symbols = numSymbols;
    automaton.initialState = initialState;
    automaton.finalStates.length = 0;

    for (int i = 0; i < numStates; i++) {
        for (int j = 0; j < numSymbols; j++) {
            for(int k = 0; k < numStates; k++) {
                automaton.delta[i][j][k] = EMPTY;
                automaton.delta[i][LAMBDA][k] = EMPTY;
            }
        }
    }

    arrayCopy(&automaton.finalStates, finalStates);
    return automaton;
}

/**
 * This function adds a transition to a non deterministic finite automaton.
 * 
 * @param automaton The non deterministic finite automaton.
 * @param fromState The origin state of the transition.
 * @param toStates The set of states to transitionate.
 * @param symbol The symbol of the transition.
 */
void addTransitionToNDFA(NDFA *automaton, int fromState, Array toStates, int symbol) {
    for (int i = 0; i < toStates.length; i++) {
        automaton->delta[fromState][symbol][i] = toStates.a[i];
    }
}


/**
 * This functions checks if a given NDFA automaton contains a given array as a final state.
 * 
 * @param automaton The automaton to check.
 * @param states The array of states to check.
 * @return TRUE if the automaton contains the array as a final state, FALSE otherwise.
*/
int containsFinalState(NDFA automaton, Array states) {
    int result = FALSE;
    for (int i = 0; i < states.length && result == FALSE; i++) {
        for (int j = 0; j < automaton.finalStates.length && result == FALSE; j++) {
            if (states.a[i] == automaton.finalStates.a[j]) {
                result = TRUE;
            }
        }
    }
    return result;
}