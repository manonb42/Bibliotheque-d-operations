#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utility.h"
#include "unbounded_int.h"


typedef struct {
    char *pcLabel;
    unbounded_int *pxUnboundedInt;
} Variable;

typedef struct VariableNode {
    Variable *pxVariable;
    struct VariableNode *pxNext;
} VariableNode;

enum OperationEntry {
    left, right
};


void free_variable(Variable **ppxVariable) // pas sur lol
{
    if (ppxVariable && *ppxVariable) {
        Variable *pxVariable = *ppxVariable;

        free(pxVariable->pcLabel);
        free_unbounded_int(&(pxVariable->pxUnboundedInt));
        free(*ppxVariable);
        *ppxVariable = NULL;
    }
}


unbounded_int *clone_unbounded_int(unbounded_int xUnboundedInt) {
    unbounded_int *pxClone = (unbounded_int *) calloc(1, sizeof(unbounded_int));
    pxClone->signe = xUnboundedInt.signe;
    pxClone->len = xUnboundedInt.len;


    pxClone->premier = alloc_chiffre(xUnboundedInt.premier->c);

    pxClone->dernier = alloc_chiffre(xUnboundedInt.dernier->c);

    pxClone->premier->precedent = pxClone->dernier;
    pxClone->dernier->suivant = pxClone->premier;

    chiffre *pxChiffre = xUnboundedInt.premier;
    chiffre *pxCurrent = pxClone->premier;

    while (pxChiffre != xUnboundedInt.dernier) {
        pxCurrent->suivant = alloc_chiffre(pxChiffre->c);
        pxCurrent->suivant->precedent = pxCurrent;
        pxCurrent = pxCurrent->suivant;

        pxChiffre = pxChiffre->suivant;
    }

    pxCurrent->suivant = pxClone->dernier;
    pxClone->dernier->precedent = pxCurrent;

    return pxClone;
}

static int isTherePrintKeyword(char *pcLine) {
    return str_start_with(pcLine, "print ");
}

static char *whereIsArithmeticOperation(char *pcLine) {
    pcLine++;
    for (pcLine = pcLine; pcLine[1] != '\0'; pcLine++) {
        if (*pcLine == '+' || *pcLine == '-' || *pcLine == '*')
            if (*(pcLine - 1) == *(pcLine + 1) && *(pcLine - 1) == ' ')
                return pcLine;
    }
    return NULL;
}

static int isVariableAssignment(char *pcLine) {
    return !isTherePrintKeyword(pcLine) && whereIsArithmeticOperation(pcLine) != NULL && strstr(pcLine, "=");
}

static int isAlphabetLetter(char cChar) {
    return (97 <= (int) cChar && (int) cChar <= 122) || (65 <= (int) cChar && (int) cChar <= 90);
}

static char *extractLabelFromPrintKeyword(char *pcLine) {
    char *pcLabel = NULL;

    if (pcLine) {
        char *pcCurrentChar = strstr(pcLine, "print ");

        if (pcCurrentChar)
            pcCurrentChar += strlen("print ");

        while (*pcCurrentChar == ' ' && *pcCurrentChar != '\0')
            ++pcCurrentChar;

        if (*pcCurrentChar != '\0') {
            size_t szLabelLength = 0;
            char *pcFirstChar = pcCurrentChar;

            while (isAlphabetLetter(*pcCurrentChar)) {
                ++pcCurrentChar;
                ++szLabelLength;
            }

            pcLabel = (char *) calloc(szLabelLength + 1, sizeof(char));

            for (size_t szChar = 0; szChar < szLabelLength; ++szChar)
                pcLabel[szChar] = *(pcFirstChar + szChar);
        }
    }

    return pcLabel;
}


static VariableNode *findVariableNode(char *pcLabel, VariableNode *pxHead) {
    VariableNode *pxNode = NULL;

    VariableNode *pxCurrent = pxHead;

    while (pxCurrent && strcmp(pxCurrent->pxVariable->pcLabel, pcLabel) != 0)
        pxCurrent = pxCurrent->pxNext;

    if (pxCurrent)
        pxNode = pxCurrent;

    return pxNode;
}


static char *extractLabelFromAssignmentLine(char *pcLine) {
    char *pcLabel = NULL;

    if (pcLine) {
        char *pcCurrentChar = pcLine;

        while (*pcCurrentChar == ' ' && *pcCurrentChar != '\0')
            ++pcCurrentChar;

        if (*pcCurrentChar != '\0') {
            size_t szLabelLength = 0;
            char *pcFirstChar = pcCurrentChar;

            while (isAlphabetLetter(*pcCurrentChar)) {
                ++pcCurrentChar;
                ++szLabelLength;
            }

            pcLabel = (char *) calloc(szLabelLength + 1, sizeof(char));

            for (size_t szChar = 0; szChar < szLabelLength; ++szChar)
                pcLabel[szChar] = *(pcFirstChar + szChar);
        }
    }

    return pcLabel;
}

static int isNumberCharacter(char cChar) {
    return 48 <= (int) cChar && (int) cChar <= 57;
}




static unbounded_int *extractValueToAssignFromAssignmentLine(char *pcLine, VariableNode *pxVariableList) {
    unbounded_int *pxUnboundedInt = NULL;

    char *pcCurrentChar = strchr(pcLine, '=');
    ++pcCurrentChar;

    int i;
    for (i = 0; pcCurrentChar[i] && pcCurrentChar[i] != ' '; i++);
    pcCurrentChar[i - 1] = '\0';

    /*while (*pcCurrentChar == ' ' && *pcCurrentChar != '\0')
       ++pcCurrentChar; */

    pcCurrentChar = strstrip(pcCurrentChar);


    if (*pcCurrentChar == '\0') {
        printf("Mauvaise entrée");
        exit(1);
    }


    if (isdigit(*pcCurrentChar)) {
        unbounded_int *out = calloc(1, sizeof(unbounded_int));
        unbounded_int tmp = string2unbounded_int(pcCurrentChar);

        memcpy(out, &tmp, sizeof(unbounded_int));
        return out;

    } else {
        VariableNode *node = findVariableNode(pcCurrentChar, pxVariableList);
        //TODO: vérifier si node unbounded int valide : à priori si
        // le label n'existe elle devrait retourner zéro
        return clone_unbounded_int(*node->pxVariable->pxUnboundedInt);

    }
    char cSign = *pcCurrentChar == '-' ? '-' : '+';

    if (*pcCurrentChar == '-')
        ++pcCurrentChar;

    size_t szLength = 0;
    chiffre *pxFirstDigit = (chiffre *) calloc(1, sizeof(chiffre));
    chiffre *pxCurrentDigit = pxFirstDigit;
    pxCurrentDigit->c = *pcCurrentChar;
    ++szLength;
    ++pcCurrentChar;

    while (isNumberCharacter(*pcCurrentChar)) {
        chiffre *pxNewDigit = (chiffre *) calloc(1, sizeof(chiffre));
        pxNewDigit->c = *pcCurrentChar;
        ++szLength;
        pxNewDigit->precedent = pxCurrentDigit;
        pxCurrentDigit->suivant = pxNewDigit;
        pxCurrentDigit = pxNewDigit;

        ++pcCurrentChar;
    }

    chiffre *pxLastDigit = pxCurrentDigit;

    pxUnboundedInt = (unbounded_int *) calloc(1, sizeof(unbounded_int));
    pxUnboundedInt->signe = cSign;
    pxUnboundedInt->len = szLength;
    pxUnboundedInt->premier = pxFirstDigit;
    pxUnboundedInt->dernier = pxLastDigit;

    return pxUnboundedInt;
}

static void pushVariable(VariableNode **ppxVariableList, Variable *pxVariableToPush) {
    if (ppxVariableList) {
        if (!(*ppxVariableList)) {
            *ppxVariableList = (VariableNode *) calloc(1, sizeof(VariableNode));
            (*ppxVariableList)->pxVariable = pxVariableToPush;
        } else {
            VariableNode *pxCurrentNode = *ppxVariableList;

            while (pxCurrentNode && strcmp(pxCurrentNode->pxVariable->pcLabel, pxVariableToPush->pcLabel) != 0)
                pxCurrentNode = pxCurrentNode->pxNext;

            if (!pxCurrentNode) {
                VariableNode *pxCurrentNode = *ppxVariableList;

                while (pxCurrentNode->pxNext)
                    pxCurrentNode = pxCurrentNode->pxNext;

                pxCurrentNode->pxNext = (VariableNode *) calloc(1, sizeof(VariableNode));
                pxCurrentNode->pxNext->pxVariable = pxVariableToPush;
            } else {
                free_variable(&(pxCurrentNode->pxVariable));
                pxCurrentNode->pxVariable = pxVariableToPush;
            }
        }
    }
}

static char *extractLabelLeftHandSideAssignment(char *pcLine) {
    char *pcLabel = NULL;

    if (pcLine) {
        char *pcCurrentChar = pcLine;

        while (!isAlphabetLetter(*pcCurrentChar))
            ++pcCurrentChar;

        if (*pcCurrentChar != '\0') {
            size_t szLabelLength = 0;
            char *pcFirstChar = pcCurrentChar;

            while (isAlphabetLetter(*pcCurrentChar)) {
                ++pcCurrentChar;
                ++szLabelLength;
            }

            pcLabel = (char *) calloc(szLabelLength + 1, sizeof(char));

            for (size_t szChar = 0; szChar < szLabelLength; ++szChar)
                pcLabel[szChar] = *(pcFirstChar + szChar);
        }
    }

    return pcLabel;
}

static int isSign(char cChar) {
    return cChar == '-' || cChar == '+';
}

static unbounded_int *
extractOperationEntry(char *pcLine, enum OperationEntry eOperationEntry, VariableNode *pxVariableList,
                      int *piReleaseOutputAfterUsage) {
    unbounded_int *pxUnboundedInt = NULL;

    if (pcLine) {
        char *pcCurrentChar = NULL;

        if (eOperationEntry == left) {
            pcCurrentChar = strstr(pcLine, "=");
            ++pcCurrentChar;
        } else //if (eOperationEntry == right)
        {
            pcCurrentChar = strstr(pcLine, " + ");

            if (!pcCurrentChar)
                pcCurrentChar = strstr(pcLine, " - ");

            if (!pcCurrentChar)
                pcCurrentChar = strstr(pcLine, " * ");

            pcCurrentChar += 2;
        }

        while (!isAlphabetLetter(*pcCurrentChar) && !isNumberCharacter(*pcCurrentChar) && !isSign(*pcCurrentChar))
            ++pcCurrentChar;

        if (isAlphabetLetter(*pcCurrentChar)) {
            char *pcEntry = NULL;
            size_t szEntryLength = 0;
            char *pcFirstChar = pcCurrentChar;

            while (isAlphabetLetter(*pcCurrentChar)) {
                ++pcCurrentChar;
                ++szEntryLength;
            }

            pcEntry = (char *) calloc(szEntryLength + 1, sizeof(char));

            for (size_t szChar = 0; szChar < szEntryLength; ++szChar)
                pcEntry[szChar] = *(pcFirstChar + szChar);

            VariableNode *pxVariable = findVariableNode(pcEntry, pxVariableList);
            pxUnboundedInt = pxVariable->pxVariable->pxUnboundedInt;

            *piReleaseOutputAfterUsage = FALSE;

            free(pcEntry);
        } else //if (isNumberCharacter(*pcCurrentChar))
        {
            char cSign = '+';

            if (*pcCurrentChar == '-' && isNumberCharacter(pcCurrentChar[1]))
                cSign = '-';

            if (isSign(*pcCurrentChar))
                ++pcCurrentChar;

            size_t szLength = 0;
            chiffre *pxFirstDigit = (chiffre *) calloc(1, sizeof(chiffre));
            chiffre *pxCurrentDigit = pxFirstDigit;
            pxCurrentDigit->c = *pcCurrentChar;
            ++szLength;
            ++pcCurrentChar;

            while (isNumberCharacter(*pcCurrentChar)) {
                chiffre *pxNewDigit = (chiffre *) calloc(1, sizeof(chiffre));
                pxNewDigit->c = *pcCurrentChar;
                ++szLength;
                pxNewDigit->precedent = pxCurrentDigit;
                pxCurrentDigit->suivant = pxNewDigit;
                pxCurrentDigit = pxNewDigit;

                ++pcCurrentChar;
            }

            chiffre *pxLastDigit = pxCurrentDigit;

            pxUnboundedInt = (unbounded_int *) calloc(1, sizeof(unbounded_int));
            pxUnboundedInt->signe = cSign;
            pxUnboundedInt->len = szLength;
            pxUnboundedInt->premier = pxFirstDigit;
            pxUnboundedInt->dernier = pxLastDigit;
            pxUnboundedInt->premier->precedent = pxLastDigit;
            pxUnboundedInt->dernier->suivant = pxFirstDigit;

            *piReleaseOutputAfterUsage = TRUE;
        }
    }

    return pxUnboundedInt;
}


Variable *alloc_variable(char *pcLabel, unbounded_int *pxUnboundedInt) {
    Variable *pxVariable = calloc(1, sizeof(Variable));
    pxVariable->pcLabel = strdup(pcLabel);
    pxVariable->pxUnboundedInt = clone_unbounded_int(*pxUnboundedInt);
    return pxVariable;
}


Variable *
executeArithmeticOperation(char *pcLine, unbounded_int *pxLeftEntry, unbounded_int *pxRightEntry, char *pcLabelResult) {
    Variable *pxResult = NULL;
    unbounded_int xResult;

    if (strstr(pcLine, " + "))
        xResult = unbounded_int_somme(*pxLeftEntry, *pxRightEntry);
    else if (strstr(pcLine, " - "))
        xResult = unbounded_int_difference(*pxLeftEntry, *pxRightEntry);
    else //if (strstr(pcLine, " * "))
        xResult = unbounded_int_produit(*pxLeftEntry, *pxRightEntry);

    pxResult = alloc_variable(pcLabelResult, &xResult);

    return pxResult;
}


void interpret_file(FILE *pxInFile, FILE *pxOutFile) {

    int iMaxLineLength = 0;
    int iCurrentLineLength = 0;
    int iChar = 0;

    /* ICI BEUG MÊME POUR LE PRINT MSKN
    while ((iChar = fgetc(pxInFile)) != EOF)
    {
       if (iChar == '\n')
       {
          if (iCurrentLineLength > iMaxLineLength)
             iMaxLineLength = iCurrentLineLength;

          iCurrentLineLength = 0;
       }
       else
          ++iCurrentLineLength;
    }

    char *pcLine = (char *)calloc(iMaxLineLength + 1, sizeof(char));

    fclose(pxInFile);
    */

    iMaxLineLength = 1024;

    char *pcLine = (char *) calloc(iMaxLineLength + 1, sizeof(char));
    VariableNode *pxVariableList = NULL;
    char* arithmeticalOperation;

    while (fgets(pcLine, iMaxLineLength + 1, pxInFile) != NULL) {
        pcLine = strstrip(pcLine);
        if (isTherePrintKeyword(pcLine)) {
            char *pcLabel = extractLabelFromPrintKeyword(pcLine);
            VariableNode *pxVariableNode = findVariableNode(pcLabel, pxVariableList);

            fprintf(pxOutFile, "%s = ", pcLabel);

            char *pcUnboundedInt = NULL;

            if (pxVariableNode)
                pcUnboundedInt = unbounded_int2string(*pxVariableNode->pxVariable->pxUnboundedInt);
            else {
                pcUnboundedInt = (char *) calloc(2, sizeof(char));
                *pcUnboundedInt = '0';
            }

            fprintf(pxOutFile, "%s\n", pcUnboundedInt);

            if (pcLabel)
                free(pcLabel);

            if (pcUnboundedInt)
                free(pcUnboundedInt);
        } else if ((arithmeticalOperation = whereIsArithmeticOperation(pcLine)) != NULL) {
            // ERIK BON COURAGE
            char *pcLabelLeftHandSide = extractLabelLeftHandSideAssignment(pcLine);
            char temp = *arithmeticalOperation;
            *arithmeticalOperation = '\0';

            unbounded_int leftEntry = string2unbounded_int(strstrip(pcLine + 1));

            *arithmeticalOperation = temp;

            unbounded_int rightEntry = string2unbounded_int(strstrip(arithmeticalOperation + 1));

            Variable *pxResult = executeArithmeticOperation(pcLine, &leftEntry, &rightEntry, pcLabelLeftHandSide);

            pushVariable(&pxVariableList, pxResult);

            free(pcLabelLeftHandSide);
        } else if (isVariableAssignment(pcLine)) {
            char *pcLabel = extractLabelFromAssignmentLine(pcLine);
            unbounded_int *pxUnboundedInt = extractValueToAssignFromAssignmentLine(pcLine, pxVariableList);
            Variable *pxVariable = alloc_variable(pcLabel, pxUnboundedInt);

            pushVariable(&pxVariableList, pxVariable);

            free(pcLabel);
            free_unbounded_int(&pxUnboundedInt);
        }

        memset(pcLine, 0, sizeof(char) * (iMaxLineLength + 1));
    }

    free(pcLine);

    fclose(pxOutFile);
}


int main(int argc, char *argv[]) {

    // lire les arg
    FILE *in = stdin;
    FILE *out = stdout;

    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-i") == 0) in = fopen(argv[i + 1], "r");
        if (strcmp(argv[i], "-o") == 0) out = fopen(argv[i + 1], "w");
    }
    interpret_file(in, out);


/* 
   if (!(pcFileName && pcResultFileName)) return;
   FILE *pxFile = fopen(pcFileName, "r");

   if (!pxFile)
   {
      printf("Fichier d'entree non trouve !\n");
      exit(1);
   }

   pxFile = fopen(pcFileName, "r");

   FILE *pxResultFile = fopen(pcFileName, "w");

   
   if (!pxResultFile)
   {
      printf("Fichier de sortie non valide !\n");
      exit(1);
   }  */
    return 0;
}




/// string utils

char *strstrip(char *s) {
    size_t size;
    char *end;

    size = strlen(s);

    if (s[size - 1] == "\n") {
        s[size - 1] = '\0';
        size--;
    }

    if (!size)
        return s;

    end = s + size - 1;
    while (end >= s && isspace(*end))
        end--;
    *(end + 1) = '\0';

    while (*s && isspace(*s))
        s++;

    return s;
}

int str_start_with(const char *str, const char *prefix) {
    return strncmp(prefix, str, strlen(prefix)) == 0;
}
