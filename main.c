#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


bool undoable = false;        //se è true allora alla prima c o d mi si cancella la pila REDO, quindi sopra a top
unsigned int limitmax = 0, limitprec = 0, i = 0, h = 9000;
int limitcurr = 0;
char **testo,*tempo;


//struttura per PUSH e POP pila UNDO/REDO
struct Aider {
    bool cmd;               //true è change, false è delete
    unsigned int a;
    unsigned int b;
    char **righe;   //array che tiene puntatori a linee cancellate/salvate
    struct Aider *next;
};
struct Aider *testau = NULL, *testar = NULL;


//__inline int __attribute__((always_inline))Memcmp(const char *s){ return (s[0]=='.' && s[1]=='\n'); }
__inline int __attribute__((always_inline)) Strlen(char *s) {
    char *p = s;
    while (*p != '\0')
        ++p;
    return (int) (p - s);
}


__inline void __attribute__((always_inline)) DEL(register unsigned int a, register unsigned int b) {
    for (i -= b - a + 1;testo[a - 1] != NULL; ++a, ++b)
        testo[a - 1] = testo[b];


}


__inline void __attribute__((always_inline)) INCR(register unsigned int b) {
    while (b >= h)
    {
        h = h + 9000;
        testo = realloc(testo, h * sizeof(char *));
        memset(testo + h - 9000, 0, sizeof(char *) * 9000);
    }
}


__inline void __attribute__((always_inline)) PUSHtU(struct Aider *push) {
    push->next = testau;
    testau = push;
}


__inline void __attribute__((always_inline)) undo(register unsigned int a) {
    getc_unlocked(stdin);
    //leggo newline
    if (limitmax > 0)
    {
        undoable = true;
        if ((limitcurr + (int) a) <= (int) limitprec)           //limitcurr è verso il basso, gli altri 2 verso l'alto
            limitcurr = limitcurr + (int) a;
        else
            limitcurr = (int) limitprec;
    }
}


__inline void __attribute__((always_inline)) redo(register unsigned int a) {
    getc_unlocked(stdin);
    //leggo newline
    if (undoable)
    {
        if ((int) ((int) limitmax - (int) limitprec + limitcurr - (int) a) >= 0)
            limitcurr = limitcurr - (int) a;
        else
            limitcurr = (int) (limitprec - limitmax);
    }
}


__inline void __attribute__((always_inline)) print(register unsigned int a, register unsigned int b) {
    if (!a) //caso 0,xp
    {
        fwrite_unlocked(".\n", 1, 2, stdout);
        ++a;
    }
    if (b != 0)
    {   //caso 0,x se la a è 0 incrementala per necessità del for
        for (; a <= b && testo[a - 1] != NULL; ++a)
            fwrite_unlocked(testo[a - 1], 1, Strlen(testo[a - 1]), stdout);
        //caso che sto stampando righe non esistenti , fino a b stampo .
        for (; a <= b; ++a)
            fwrite_unlocked(".\n", 1, 2, stdout);
    }  //caso 0,0p solo punto
}


__inline void __attribute__((always_inline))insert(char *line, register unsigned int a, register unsigned int j, struct Aider *push) {


    if (!testo[a - 1])  ++i;


    push->righe[j] = testo[a - 1];
    testo[a - 1] = strdup(line);
    //salvo dopo
}


__inline void __attribute__((always_inline)) POPtR() {


    // top assign into temp
    struct Aider *temp = testau;


    // assign second node to top
    testau = testau->next;


    temp->next = testar;


    testar = temp;
}


__inline void __attribute__((always_inline)) POPtU() {


    // top assign into temp
    struct Aider *temp = testar;


    // assign second node to top
    testar = testar->next;


    temp->next = testau;


    testau = temp;


}


void Delete(register unsigned int a, register unsigned int b, struct Aider *push) {
    ++limitmax;
    limitprec = limitmax;//limitmax lo abbasso cancellando elementi


    push->cmd = false;
    switch (b) {
        case 0:
            push->b = push->a = 0;
            push->righe = NULL;
            break;
        default:


            push->righe = calloc((b - a + 1), sizeof(char *));
            register unsigned int t = 0, m = a;
            for (; a <= b && a > 0 && testo[a - 1] != NULL ; ++a, ++t, --i)
                push->righe[t] = testo[a - 1];


            if (t != 0)
                push->a = m;
            else
                push->a = 0;


            push->b = a - 1;
            if (t != (b - m + 1))
                push->righe = realloc(push->righe, sizeof(char *) * (t + 1));




            for (;  m > 0 && testo[m - 1] != NULL  && b < h; ++m, ++a)
                testo[m - 1] = testo[a - 1];


            break;
    }


    PUSHtU(push);
}
//#define FREE(x) (free(x), x = NULL)
//__inline struct Aider * NextNode( struct Aider * pNode, struct Aider * pPrevNode){ return ((struct Aider *) ((unsigned long) pNode->next ^ ( unsigned long )pPrevNode) ); }
//-fsanitize=address -g -O3
//-DCMAKE_C_FLAGS="-std=gnu11 -pipe -O2 -static -s -lm"
//cc main.c -DEVAL -std=gnu11 -O2 -pipe -static -s -o main -lm
//(./main<test10.txt)>out.txt
//(struct Piece*[20])*(struct Piece*)ptr
//int n=0; while(putchar_unlocked(ptr[y]->str[n++])!=10);


void undu() {
    limitprec = limitprec - limitcurr;
    register unsigned int j,a;
    //undo
    while (limitcurr > 0) {
        //ripristino top    //if 0,0,null allora tutto vuoto devo mettere e non fare la pop
        if (testau->cmd) {   //change
            for (j = 0, a = testau->a; a <= testau->b; ++a, ++j) {
                tempo = testo[a - 1];
                testo[a - 1] = testau->righe[j];
                testau->righe[j] = tempo;
                if (!testo[a - 1]) --i;
            }
        } else {   //delete
            switch (testau->a) {
                case 0:
                    break;
                default:


                    for (j = i;j >= testau->a; --j)     //shift       // spostamento e ripristino controllo
                        testo[j + testau->b - testau->a] = testo[j - 1];


                    for (j = 0, a = testau->a; a <= testau->b; ++a, ++j, ++i)
                        testo[a - 1] = testau->righe[j];


                    break;
            }//DEVO SCORRERE L'ARRAY IN AVANTI E devo mettere in ptr[a] l'indirizzo che c'è in top->addr[0], poi in a+1 top->addr[1], fino a b e
        }


        POPtR();


        --limitcurr;
    }


    //redo
    while (limitcurr < 0) {


        if (testar->cmd)        //change
        {
            for (j = 0, a = testar->a; a <= testar->b; ++a, ++j)
            {
                if (!testo[a - 1])
                {
                    tempo = testo[a - 1];
                    testo[a - 1] = testar->righe[j];
                    testar->righe[j] = tempo;
                    if (testo[a - 1] != NULL) ++i;
                } else
                    {
                    tempo = testo[a - 1];
                    testo[a - 1] = testar->righe[j];
                    testar->righe[j] = tempo;
                    }
            }
        } else                //delete
            if (testar->a != 0)
                DEL(testar->a, testar->b);


        POPtU();


        ++limitcurr;
    }




    if (!undoable)
    {
        testar = NULL;
        limitmax = limitprec;
    }


}


int main() {
    register unsigned int b, u, cmd, j, a = getc_unlocked(stdin);
    //a=primo numero, b=secondo, cmd =p/d/c, u=,/u/r, i=#elementi totali, h=#celle callocate, j per salvare righe in addr,
    char line[1024];
    testo = calloc(9000, sizeof(char *));
    //memorizza linea inserita e la copia nella struct


    setvbuf(stdin, NULL, _IOLBF, 1024);
    //stdin legge una sola riga il buffer
    setvbuf(stdout, NULL, _IOFBF, 1024);
    //svuota il buffer appena riempito con qualcosa


    //START finchè non inserisco q per terminare il programma
    while (a != 'q') {
        a = a - '0';
        //trasformo il char nel suo corrispondente intero
        u = getc_unlocked(stdin);
        //prendo la virgola oppure u oppure r oppure la seconda cifra del primo numero
        while (u >= '0' && u <= '9')
        {
            a = (a << 3u) + (a << 1u) + u - '0';
            u = getc_unlocked(stdin);
        }
        //unisco le cifre per formare il numero (multiply by 10)
        // u è u oppure r oppure ,
        switch (u) {
            case ',':
                b = getc_unlocked(stdin) - '0';
                //secondo numero in intero, siamo dopo la virgola (o cifre o comando)
                cmd = getc_unlocked(stdin);
                //finchè non è qualcosa di diverso da una cifra
                while (cmd <= '9')
                {
                    b = (b << 3u) + (b << 1u) + cmd - '0';
                    cmd = getc_unlocked(stdin);
                }//unisco le cifre secondo numero
                getc_unlocked(stdin);
                struct Aider *push;
                //leggo newline
                switch (cmd) {


                    case 'c':   //Il testo che segue il comando deve essere fatto di un numero di righe pari a ind2-ind1+1. ind1 deve essere o un indirizzo effettivamente presente nel testo, oppure il primo indirizzo dopo l’ultima riga presente nel testo (o 1 se il testo `e ancora vuoto). leggo la prima linea di change, in caso fosse solo . esce al for
                                //se è true allora alla prima c o d mi si cancella la pila REDO, quindi sopra a top
                        if (undoable)
                        {
                            undoable = false;
                            undu();
                        }
                        ++limitmax;
                        limitprec = limitmax;


                        INCR(b);


                        push = malloc(sizeof(struct Aider));
                        push->cmd = true;
                        push->a = a; //me salvo i 2 punti e poi quando faccio la undo parto dal primo e faccio la pop di tutti fino a b
                        push->b = b;
                        push->righe = malloc(sizeof(char *) * (b - a + 1));


                        //leggo prossima linea, .? allora al for esce, altrimenti riparte con j,t salvati   //continuo finchè non c'è . oppure a supera b
                        for (fgets(line, 1024, stdin), j = 0; !(line[0] == '.' && line[1] == '\n'); ++a, ++j, fgets(line, 1024, stdin))
                            insert(line, a, j, push);


                        PUSHtU(push);
                        break;


                    case 'p': //Stampa le righe tra ind1 ed ind2, inclusi. Laddove non sia presente una riga nel testo nella posizione da stampare, viene stampata una riga contenente il solo carattere ’.’ seguito da un “\n”
                        if (undoable) undu();


                        print(a, b);
                        break;


                    case 'd':
                        //Cancella le righe comprese tra ind1 ed ind2 (estremi inclusi), spostando verso l’alto le righe successive a quella di indirizzo ind2 (se ce ne sono). La cancellazione di una riga che non esiste nel testo non ha alcun effetto
                        //FIXME:0,xd può succedere? che fa?
                        if (undoable)
                        {
                            undoable = false;
                            undu();
                        }
                        push = malloc(sizeof(struct Aider));
                        Delete(a, b, push);
                        break;


                    default:
                        break;


                }
                break;


            case 'u':   //Effettua l’annullamento (undo) di un numero di comandi (c o d) pari a quello specificato
                        // Una sequenza di comandi di undo consecutivi annulla un numero di passi pari alla somma dei passi specificati in ognuno di essi.
                         // Se il numero di comandi di cui effettuare l’annullamento è superiore a quello dei comandi eseguiti, vengono annullati tutti i passi.
                        // L’esecuzione di un comando di modifica del testo (c, d) dopo un undo cancella gli effetti dei comandi annullati definitivamente.
                        // Si noti che nel numero dei comandi da annullare si contano anche i comandi che non hanno effetto (come la cancellazione di un blocco di righe che non esistono)
                undo(a);
                break;


            case 'r':   //Annulla l’effetto di undo per un numero di comandi a partire dalla versione corrente 10u + 5r è a tutti gli effetti equivalente al solo comando 5u.
                        // Nel caso in cui il numero di comandi di cui effettuare il redo `e superiore a quelli annullati correntemente, viene effettuato il numero massimo di redo possibili.
                redo(a);
                break;


            default:
                break;
        }
        a = getc_unlocked(stdin);
    }
}
