#include "algorithms/read_and_write.h"

/* int main() {

    int numStates = 4;
    int numSymbols = 2;
    int initialState = 0;
    Array finalStates;
    finalStates.a[0] = 3;
    finalStates.length = 1;
    
    NDFA automaton = createNDFAAutomaton(numStates, numSymbols, initialState, finalStates);
    printf("NDFA created\n");

    Array toStatesArray;
    int fromState = 0;
    int symbol = LAMBDA;
    toStatesArray.a[0] = 2;
    toStatesArray.length = 1;
    symbol = 0;
    addTransitionToNDFA(&automaton, fromState, toStatesArray, symbol);

    toStatesArray.a[0] = 3;
    fromState = 2;
    symbol = 1;
    addTransitionToNDFA(&automaton, fromState, toStatesArray, symbol);

    toStatesArray.a[0] = 3;
    fromState = 3;
    symbol = 0;
    addTransitionToNDFA(&automaton, fromState, toStatesArray, symbol);

    toStatesArray.a[0] = 1;
    toStatesArray.a[1] = 3;
    toStatesArray.length = 2;
    fromState = 0;
    symbol = LAMBDA;
    addTransitionToNDFA(&automaton, fromState, toStatesArray, symbol);



    printf("States: %d\n", automaton.states);
    printf("Symbols: %d\n", automaton.symbols);
    printf("Initial state: %d\n", automaton.initialState);
    printf("Final states: ");
    for (int i = 0; i < automaton.finalStates.length; i++) {
        printf("%d ", automaton.finalStates.a[i]);
    }
    printf("\n");

    printf("Delta:\n");
    for (int i = 0; i < automaton.states; i++) {
        for (int j = 0; j < automaton.symbols; j++) {
            printf("%d -> %d ->: [ ", i, j);
            for (int k = 0; k < automaton.states; k++) {
                if (automaton.delta[i][j][k] != EMPTY) {
                    printf("%d ", automaton.delta[i][j][k]);
                }
            }
            printf("]\n");
        }
    }
}
 */




int main( int argc, char *argv[]) {

    printf("Amount of states for the automaton: %s\n", argv[1]);
    printf("Amount of alphabet symbols: %s\n", argv[2]);
    printf("Name of the input file: %s\n", argv[3]);
    printf("String to evaluate: %s\n", argv[4]);
    printf("Name of the output file: %s\n", argv[5]);

    int numStates = atoi(argv[1]);
    int numSymbols = atoi(argv[2]);
    char* fileName = argv[3];
    char* string = argv[4];

    // createFromFile(fileName; cantEstados; numSymbols)    
    NDFA ndfa;

/********************************* ACA EMPIEZA LA LECTURA *****************************************/

    // This is the data to capture from the file.
    Array finalStates;
    finalStates.length = 0;
    int initialState;
    int fromState;
    int symbol;
    Array toStatesArray;
    toStatesArray.length = 0;
    ArrayOfTransitionsNDFA arrayOfTransitions;
    arrayOfTransitions.length = 0;

    // leer archivo
    FILE *file;
    file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error\n");
        exit(1);
    }

    char line[100];
    while (fgets(line, 100, file) != NULL) {
        // We capture the lines with the pattern "inic->", while we are reading the file.
        char *inic = strstr(line, "inic->");
        if (inic) {
            // The number is captured as a string, so we convert it to an integer.
            char numberString[3];
            numberString[0] = inic[7];
            numberString[1] = inic[8];
            numberString[2] = '\0';
            initialState = atoi(numberString);  
        }

        // We capture the lines with the pattern "[shape=doublecircle];", while we are reading the file.
        char *final = strstr(line, "[shape=doublecircle];");
        if (final) {
            char stringNumber[3];
            stringNumber[0] = line[0];
            stringNumber[1] = line[1];
            stringNumber[2] = '\0';
            int estadoFinal = atoi(stringNumber);
            finalStates.a[finalStates.length] = estadoFinal;
            finalStates.length++;
        }

        // We capture the lines with the pattern "label", while we are reading the file.
        char *transition = strstr(line, "label");
        if (transition) {
            char stringNumber[3];
            stringNumber[0] = line[0];
            stringNumber[1] = line[1];
            stringNumber[2] = '\0';
            int from = atoi(stringNumber);
            fromState = from;
            
            stringNumber[3];
            stringNumber[0] = line[4];
            stringNumber[1] = line[5];
            stringNumber[2] = '\0';
            int to = atoi(stringNumber);

            char label[(15 - strlen(line) - 4)]; 
            for (int i = 15; i < strlen(line) - 4; i++) {
                label[i - 15] = line[i];
            }
            label[strlen(line) - 4 - 15] = '\0';

            Array labels; 
            labels.length = 0;
            char s[2];
            s[0] = ',';
            s[1] = '\0';
            char* token = strtok(label, s);
            while( token != NULL ) {
                int label;
                if (token[0] == '_')
                {
                    label = LAMBDA;
                } else {
                    stringNumber[0] = token[0];
                    stringNumber[1] = token[1];
                    stringNumber[2] = '\0';
                    label = atoi(stringNumber);
                }
                labels.a[labels.length] = label;
                labels.length++;

                token = strtok(NULL, s);
            }
            
            // Then, we add every transition captured to the array of transitions of the automaton.
            int flag = FALSE;
            for (int i = 0; i < arrayOfTransitions.length; i++){
                for (int j = 0; j < labels.length; j++){

                    // We check if a transition with the same fromState and symbol already exists.
                    if (arrayOfTransitions.array[i].from == from && arrayOfTransitions.array[i].symbol == labels.a[j]){
                        arrayOfTransitions.array[i].to.a[arrayOfTransitions.array[i].to.length] = to;
                        arrayOfTransitions.array[i].to.length++;
                        flag = TRUE;
                    }
                }
            }

            // If the transition with the same fromState and symbol doesn't exists...
            if (flag == FALSE){
                for (int i = 0; i < labels.length; i++) {
                    // We create a new transition and add it to the array of transitions of the automaton.
                    TransitionNDFA newTransition;
                    newTransition.from = from;
                    newTransition.symbol = labels.a[i];
                    newTransition.to.a[0] = to;
                    newTransition.to.length = 1;
                    arrayOfTransitions.array[arrayOfTransitions.length] = newTransition;
                    arrayOfTransitions.length++;
                }
            }
        }
    }
    
    ndfa = createNDFAAutomaton(numStates, numSymbols, initialState, finalStates);
    for (int i = 0; i < arrayOfTransitions.length; i++){
        addTransitionToNDFA(&ndfa, arrayOfTransitions.array[i].from, arrayOfTransitions.array[i].to, arrayOfTransitions.array[i].symbol);
    }   
    


/********************************* ACA TERMINA LA LECTURA *****************************************/




    int result = belongsToLanguage(ndfa, string);
    if (result == TRUE) {
        printf("La cadena es aceptada\n");
    } else {
        printf("La cadena no es aceptada\n");
    }
    return 0;



}