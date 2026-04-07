/* Livia Negrucci Cantowitz RA:10389419 */

#include <stdio.h>
#include <string.h>

#define INTEIRO               0
#define INTEIRO_COM_SINAL     1
#define PONTO_FLUTUANTE       2
#define PONTO_FLUTUANTE_SINAL 3
#define INTEIRO_HEX           4
#define ERRO                 -1


int scanner(const char *str, int *pos)
{
    int c;

    /* Pula espacos */
    while (str[*pos] == ' ')
        (*pos)++;

    if (str[*pos] == '\0')
        return ERRO;

    /* ===========================================================
     * AFD com rotulos e goto
     *
     * q0  : estado inicial
     * q1  : leu sinal (+/-)
     * q2  : inteiro sem sinal iniciado em 1-9 [ACEITACAO, aceita virgula]
     * q2b : inteiro sem sinal com zero extra a esquerda [ACEITACAO, sem virgula]
     * q3  : inteiro com sinal [ACEITACAO, aceita virgula]
     * q4  : leu ',' apos int. sem sinal (espera digito obrigatorio)
     * q5  : ponto flutuante sem sinal [ACEITACAO]
     * q6  : leu ',' apos int. com sinal (espera digito obrigatorio)
     * q7  : ponto flutuante com sinal [ACEITACAO]
     * q8  : leu '0' inicial (decide: int, hex ou flutuante)
     * q9  : leu "0x" (espera digito hex obrigatorio)
     * q10 : inteiro hexadecimal [ACEITACAO]
     * q11 : leu sinal+'0' (se terminar = ERRO; se ',' = flutuante com sinal)
     * ===========================================================
     */

q0:
    c = str[*pos];
    if (c == '+' || c == '-')          { (*pos)++; goto q1;  }
    if (c >= '1' && c <= '9')          { (*pos)++; goto q2;  }
    if (c == '0')                      { (*pos)++; goto q8;  }
    goto q_erro;

q1:
    c = str[*pos];
    if (c >= '1' && c <= '9')          { (*pos)++; goto q3;  }
    if (c == '0')                      { (*pos)++; goto q11; }
    goto q_erro;

q2:
    c = str[*pos];
    if (c == '\0' || c == ' ')         goto q_aceita_int;
    if (c >= '0' && c <= '9')          { (*pos)++; goto q2;  }
    if (c == ',')                      { (*pos)++; goto q4;  }
    goto q_erro;

q2b:
    c = str[*pos];
    if (c == '\0' || c == ' ')         goto q_aceita_int;
    if (c >= '0' && c <= '9')          { (*pos)++; goto q2b; }
    goto q_erro;

q3:
    c = str[*pos];
    if (c == '\0' || c == ' ')         goto q_aceita_int_sinal;
    if (c >= '0' && c <= '9')          { (*pos)++; goto q3;  }
    if (c == ',')                      { (*pos)++; goto q6;  }
    goto q_erro;

q4:
    c = str[*pos];
    if (c >= '0' && c <= '9')          { (*pos)++; goto q5;  }
    goto q_erro;

q5:
    c = str[*pos];
    if (c == '\0' || c == ' ')         goto q_aceita_flut;
    if (c >= '0' && c <= '9')          { (*pos)++; goto q5;  }
    goto q_erro;

q6:
    c = str[*pos];
    if (c >= '0' && c <= '9')          { (*pos)++; goto q7;  }
    goto q_erro;

q7:
    c = str[*pos];
    if (c == '\0' || c == ' ')         goto q_aceita_flut_sinal;
    if (c >= '0' && c <= '9')          { (*pos)++; goto q7;  }
    goto q_erro;

q8:
    c = str[*pos];
    if (c == '\0' || c == ' ')         goto q_aceita_int;
    if (c == 'x' || c == 'X')         { (*pos)++; goto q9;  }
    if (c == ',')                      { (*pos)++; goto q4;  }
    if (c >= '0' && c <= '9')          { (*pos)++; goto q2b; }
    goto q_erro;

q9:
    c = str[*pos];
    if ((c >= '0' && c <= '9') ||
        (c >= 'a' && c <= 'f') ||
        (c >= 'A' && c <= 'F'))        { (*pos)++; goto q10; }
    goto q_erro;

q10:
    c = str[*pos];
    if (c == '\0' || c == ' ')         goto q_aceita_hex;
    if ((c >= '0' && c <= '9') ||
        (c >= 'a' && c <= 'f') ||
        (c >= 'A' && c <= 'F'))        { (*pos)++; goto q10; }
    goto q_erro;

q11:
    c = str[*pos];
    if (c == '\0' || c == ' ')         goto q_erro;
    if (c == ',')                      { (*pos)++; goto q6;  }
    goto q_erro;

q_aceita_int:
    return INTEIRO;

q_aceita_int_sinal:
    return INTEIRO_COM_SINAL;

q_aceita_flut:
    return PONTO_FLUTUANTE;

q_aceita_flut_sinal:
    return PONTO_FLUTUANTE_SINAL;

q_aceita_hex:
    return INTEIRO_HEX;

q_erro:
    while (str[*pos] != '\0' && str[*pos] != ' ')
        (*pos)++;
    return ERRO;
}

/* main: chama scanner sucessivamente e imprime o resultado*/
int main(void)
{
    const char *str = "21 +4,5 6 00,123 +0,456 -42. xx -123";

    int pos  = 0;
    int tipo = 0;

    while (str[pos] != '\0')
    {
        while (str[pos] == ' ')
            pos++;

        if (str[pos] == '\0')
            break;

        tipo = scanner(str, &pos);

        switch (tipo)
        {
            case INTEIRO:
                printf("<INTEIRO>\n");
                break;
            case INTEIRO_COM_SINAL:
                printf("<INTEIRO COM SINAL>\n");
                break;
            case PONTO_FLUTUANTE:
                printf("<P.FLUTUANTE>\n");
                break;
            case PONTO_FLUTUANTE_SINAL:
                printf("<P.FLUTUANTE COM SINAL>\n");
                break;
            case INTEIRO_HEX:
                printf("<INTEIRO HEXADECIMAL>\n");
                break;
            default:
                printf("<ERRO>\n");
                break;
        }
    }

    return 0;
}
