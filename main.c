#include <curses.h>
#include <string.h>
#define TAM_TELA 29
#define PER 31

char t[TAM_TELA][TAM_TELA]; //variavel global da matriz do tabuleiro


//Função para verificar para onde o boneco pode se mover
int moverBoneco(char t[TAM_TELA][TAM_TELA], int cy, int cx)
{
    int i,j;

    for(i=cy; i< cy + 3; i++) //percorre todas as linhas do boneco
    {
        for(j=cx - 1; j<= cx + 1; j++) //percorre todas as colunas do boneco
        {
            if(i< 0 || i>= TAM_TELA || j < 0 || j >= TAM_TELA) //Verifica se a posição é valida
                return 0;
            if(t[i][j] != ' ' && t[i][j] != 'S')
                return 0;
        }
    }
    return 1;
}

//Função para exibir o painel de senha e vereficar se esta correta
void painelSenha(char pergunta[50], char* senha, int li, int lf, int ci, int cf)
{
    char entrada[10];
    int i, j;

    //limpa a área lateral
    for(i = 10; i < 30; i++)
    {
        move(i, PER);
        clrtoeol();
    }

    echo();
    curs_set(1);

    mvprintw(10, PER, "Responda:");
    mvprintw(11, PER, "%s", pergunta);

    mvprintw(13, PER, "Senha: ");
    move(13, PER + 8);
    getnstr(entrada, 9);

    noecho();
    curs_set(0);

    if(strcmp(entrada, senha) == 0) //compara o valor digitado com a senha
    {
        mvprintw(15, PER, "Correta! Porta aberta.");
        if(li == lf) //Porta horizontal
        {
            for(j = ci; j <= cf; j++)
                t[li][j] = ' '; //Abre a porta, apaga da coluna `ci` até `cf` na linha `li`
        }
        else //Porta vertical
        {
            for(i = li; i <= lf; i++)
                t[i][ci] = ' '; //Abre a porta, apaga da linha `li` até `lf` na coluna `ci`
        }
    }
    else
    {
        mvprintw(15, PER, "Senha incorreta.");
    }

    mvprintw(17, PER, "Pressione qualquer tecla para voltar...");
    getch(); //Espera o jogador pressionar alguma tecla

    //limpa lateral de novo
    for(int i = 10; i < 30; i++)
    {
        move(i, PER);
        clrtoeol();
    }
}

//Função que verifica qual painel foi ativado pelo boneco
int identificarPainel(int cy, int cx)
{
    if (cy >= 6 && cy <= 8 && cx >= 6 && cx <= 8) return 0; //Painel 0 - controla porta horizontal esquerda
    if (cy >= 20 && cy <= 22 && cx >= 6 && cx <= 8) return 1; //Painel 1 - controla porta vertical inferior
    if (cy >= 20 && cy <= 22 && cx >= 20 && cx <= 22) return 2; //Painel 2 - controla porta horizontal direita
    if (cy >= 6 && cy <= 8 && cx >= 20 && cx <= 22) return 3; //Painel 3 - controla porta vertical superior

    return -1; // nenhum painel
}

//Verifica se todas as partes do boneco podem ativar o painel
int bonecoSobrePainel(int cy, int cx)
{
    int i, j;

    for(i = cy; i < cy + 3; i++) //percorre todas as linhas do boneco
    {
        for(j = cx - 1; j <= cx + 1; j++) //percorre todas as colunas do boneco
        {
            if (i >= 0 && i < TAM_TELA && j >= 0 && j < TAM_TELA && t[i][j] == 'S') //verifica se está em cima de um painel
            {
                return identificarPainel(cy + 1, cx); //Chama a função e verifica qual o painel esta sendo ativado
            }
        }
    }
    return -1; //Nenhum painel encontrado
}

//Função para mostrar a mensagem de vitória
void mostrarMensagemVitoria()
{
    clear(); // Limpa a tela
    attron(COLOR_PAIR(4));
    mvprintw(TAM_TELA/2 - 1, PER, " * * * * * * * * * * * * * * * * * * * ");
    mvprintw(TAM_TELA/2,     PER, " *  PARABENS, VOCE ESCAPOU DA CASA!  * ");
    mvprintw(TAM_TELA/2 + 1, PER, " * * * * * * * * * * * * * * * * * * * ");
    mvprintw(TAM_TELA/2 + 3, PER, " Pressione Q para sair...");
    attroff(COLOR_PAIR(4));
    refresh();

    while(getch() != 'q');  //sai do loop quando 'q' for pressionado
}

int main()
{
    int entrada = ' ';
    int cx =  2, cy =  2; //Posição inicial do boneco
    int i, j, tx, ty, painel;

    char perguntas0[1][50] = {"1- Raiz quadrada de 16?"};
    char senha0[10] = "4";

    char perguntas1[1][50] = {"2- Quantos continentes existem?"};
    char senha1[10] = "7";

    char perguntas2[1][50] = {"3- Qual e a metade de meia duzia?"};
    char senha2[10] = "3";

    char perguntas3[1][50] = {"4- Qual o unico numero primo que e par?"};
    char senha3[10] = "2";

    //Inicializa tabuleiro
    for(i = 0; i < TAM_TELA; i++)
    {
        for(j = 0; j < TAM_TELA; j++)
        {
            if (i == 0 || i == TAM_TELA - 1 || j == 0 || j == TAM_TELA - 1)
                t[i][j] = '#';
            else
                t[i][j] = ' ';
        }
    }

    //Define as paredes internas
    for(i = 0; i < TAM_TELA; i++)
        t[i][14] = '#'; //Parede vertical central

    for(j = 0; j < TAM_TELA; j++)
        t[14][j] = '#'; //Parede horizontal central


    //Define as portas

    for(j = 6; j <= 8; j++) //Porta horizontal esquerda
        t[14][j] = 'D';

    for(i = 20; i <= 22; i++) //Porta vertical inferior
        t[i][14] = 'D';

    for(j = 20; j <= 22; j++) //Porta horizontal direita
        t[14][j] = 'D';

    for(i = 6; i <= 8; i++) //Porta vertical superior
        t[i][TAM_TELA-1] = 'D';

    //Define os painéis de senha
    t[7][7] = 'S'; //painel 0
    t[21][7] = 'S'; //painel 1
    t[21][21] = 'S'; //painel 2
    t[7][21] = 'S'; //painel 3

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);   //Habilita teclas especiais (ex: ENTER)
    curs_set(0); //Esconde o cursor

    //Inicializa cores
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    init_pair(3, COLOR_RED, COLOR_RED);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);

    while(entrada != 'q') //Roda até o jogador pressionar 'q'
    {
        clear();

        //Impressão do tabuleiro já com as suas respectivas cores
        for(i = 0; i < TAM_TELA; i++)
        {
            for(j = 0; j < TAM_TELA; j++)
            {
                switch(t[i][j])
                {
                    case '#': //Paredes
                        attron(COLOR_PAIR(1));
                        mvaddch(i, j, '#');
                        attroff(COLOR_PAIR(1));
                        break;
                    case 'D': //Portas
                        attron(COLOR_PAIR(2));
                        mvaddch(i, j, 'D');
                        attroff(COLOR_PAIR(2));
                        break;
                    case 'S': //Paineis
                        attron(COLOR_PAIR(3));
                        mvaddch(i, j, 'S');
                        attroff(COLOR_PAIR(3));
                        break;
                    default:
                        mvaddch(i, j, t[i][j]);
                }
            }
        }

       //Impressão do boneco
        mvprintw(cy, cx,   "*");
        mvprintw(cy+1, cx-1, "/|\\");
        mvprintw(cy+2, cx-1, "/ \\");

        // Mostra as instruções
        attron(A_STANDOUT);
        mvprintw(1, PER, "Voce esta preso em uma casa, para escapar");
        mvprintw(2, PER, "responda todas as perguntas corretamente.");
        mvprintw(3, PER, "Instrucoes: pressione ENTER com o boneco");
        mvprintw(4, PER, "sobre o quadrado vermelho para digitar a senha.");
        mvprintw(5, PER, "Se estiver correta, a porta sera liberada.");
        attroff(A_STANDOUT);

        refresh();

        entrada = getch();

        //Movimentação do boneco
        ty = cy; //Transfere as posições para variáveis temporárias, evitando erros na movimentação
        tx = cx;
        switch(entrada)
        {
            case 'w': //cima
                ty = cy - 1;
                break;
            case 's': //baixo
                ty = cy + 1;
                break;
            case 'a': //esquerda
                tx = cx - 1;
                break;
            case 'd': //direita
                tx = cx + 1;
                break;
        }

        //Verifica se o movimento é valido
        if(moverBoneco(t, ty, tx))
        {
            cx = tx;
            cy = ty;
        }

        //Verifica se o jogador ativou um painel
        if(entrada == '\n' || entrada == KEY_ENTER)
        {
            painel = bonecoSobrePainel(cy, cx);

            switch(painel)
            {
                case 0: //Painel 0 - libera porta horizontal direita
                    painelSenha(perguntas0[0], senha0, 14, 14, 6, 8);
                    break;
                case 1: //Painel 1 - libera porta vertical inferior
                    painelSenha(perguntas1[0], senha1, 20, 22, 14, 14);
                    break;
                case 2: //Painel 2 - libera porta horizontal esquerda
                    painelSenha(perguntas2[0], senha2, 14, 14, 20, 22);
                    break;
                case 3: //Painel 3 - libera porta vertical superior
                    painelSenha(perguntas3[0], senha3, 6, 8, 28, 28);
                    break;
            }
        }

        //Verifica se o jogador escapou
        if(cx >= 27 && cy >= 6 && cy <= 8 && t[cy][cx] == ' ')
        {
            mostrarMensagemVitoria();
            break;
        }
    }

    endwin();
    return 0;
}
