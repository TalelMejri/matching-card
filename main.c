#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#define NB_LIGNES_CARTE 10
#define NB_COLONNES_CARTE 10

typedef struct {
 int decouvert;
 int nbr;
}cell;

int numberCorrect(cell mat[NB_LIGNES_CARTE][NB_COLONNES_CARTE]){
    int compt=0;
        for(int i=0;i<NB_LIGNES_CARTE;i++){
            for(int j=0;j<NB_COLONNES_CARTE;j++){
                if(mat[i][j].decouvert==2){
                    compt++;
                }
            }
        }
    return compt/2;
}

int gagner(cell mat[NB_LIGNES_CARTE][NB_COLONNES_CARTE]) {
    int nbr = numberCorrect(mat);

    int totalCells = NB_LIGNES_CARTE * NB_COLONNES_CARTE;

    if (nbr == totalCells / 2) {
        return 1;
    } else {
        return 0;
    }
}

void  afficherCarte(SDL_Surface* ecran, cell carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],int compt){
    SDL_Rect position;
    SDL_WM_SetCaption("Carte", NULL);
    SDL_Surface* tab[50],*texte=NULL;
    tab[0]=IMG_Load("images/cachee.png");
    tab[1] = IMG_Load("images/cstam.jpg");
    tab[2] = IMG_Load("images/embs.png");
    tab[3] = IMG_Load("images/iastam.jpg");
    tab[4] = IMG_Load("images/tsyp11.jpg");
    tab[5] = IMG_Load("images/tsyp12.jpg");
    tab[7] = IMG_Load("images/tysp11.jpg");
    tab[8] = IMG_Load("images/tsypx.jpg");

    SDL_Rect positionMenu;
    positionMenu.x = 610;
    positionMenu.y = 0;

    TTF_Font* font = NULL;
    TTF_Init();
    font = TTF_OpenFont("Aloevera-OVoWO.ttf", 20);

    SDL_Color blanc = { 255, 255, 255 };
    SDL_Rect positionText;
    positionText.x = 630;
    positionText.y = 150;

    SDL_Surface *menu = NULL;
    menu = IMG_Load("images/banner.png");

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

    SDL_BlitSurface(menu, NULL, ecran, &positionMenu);

    char welcome[50]="Welcome To";
    texte = TTF_RenderText_Blended(font,welcome,blanc);
    positionText.x =660;
    positionText.y =150;
    SDL_BlitSurface(texte, NULL, ecran, &positionText);

    positionText.x =620;
    positionText.y =180;
    char ch5[50] = "IEEE Enicarthage";
    texte = TTF_RenderText_Blended(font, ch5, blanc);
    SDL_BlitSurface(texte, NULL, ecran, &positionText);

    char nbr2[50];
    int nbrCorrect=numberCorrect(carte);
    sprintf(nbr2,"Correct  %d",nbrCorrect);
    texte = TTF_RenderText_Blended(font,nbr2,blanc);
    positionText.x =670;
    positionText.y =300;
    SDL_BlitSurface(texte, NULL, ecran, &positionText);
    SDL_Color blue = {0, 109, 255};

    char compteur[50];
    sprintf(compteur,"Time %d",compt);
    texte = TTF_RenderText_Blended(font,compteur,blue);
    positionText.x =680;
    positionText.y =500;
    SDL_BlitSurface(texte, NULL, ecran, &positionText);

    for (int i = 0; i < NB_LIGNES_CARTE; i++) {
        for (int j = 0; j < NB_COLONNES_CARTE; j++) {
            position.y = i * 59;
            position.x = j * 60;
            if (carte[i][j].decouvert == 0) {
                SDL_BlitSurface(tab[0], NULL, ecran, &position);
            } else {
                SDL_BlitSurface(tab[carte[i][j].nbr], NULL, ecran, &position);
            }
        }
    }

    SDL_Flip(ecran);

    SDL_FreeSurface(menu);
    SDL_FreeSurface(texte);

    SDL_EnableKeyRepeat(0, 0);
}

int check(int n,cell mat[NB_LIGNES_CARTE][NB_COLONNES_CARTE]){
    int cnt=0;
    for(int i=0;i<NB_LIGNES_CARTE;i++){
        for(int j=0;j<NB_COLONNES_CARTE;j++){
            if(mat[i][j].decouvert==1 && mat[i][j].nbr==n){
                    cnt++;
            }
        }
    }
    return cnt;
}

void deroulerJeu(SDL_Surface* ecran, cell carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE]) {
    int continuer = 1;
    SDL_Event event;
    int TotalClique=0;
    Uint32 startTime = SDL_GetTicks();
    while (continuer == 1 && gagner(carte)==0) {
        Uint32 currentTime = SDL_GetTicks();
        int elapsedTime = (currentTime - startTime ) / 1000;
        if (elapsedTime >= 120) {
            continuer = 0;
            break;
        }
        afficherCarte(ecran, carte,elapsedTime);
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    continuer = 0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        continuer = 0;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                             int x_clicked=(event.button.x)/60;
                             int y_clicked=(event.button.y)/60;
                             if(x_clicked<=NB_LIGNES_CARTE-1 && y_clicked<=NB_COLONNES_CARTE-1){
                                       TotalClique++;
                                        if(TotalClique==3){
                                                for(int i=0;i<NB_LIGNES_CARTE;i++){
                                                    for(int j=0;j<NB_COLONNES_CARTE;j++){
                                                         if(carte[i][j].decouvert==1){
                                                                carte[i][j].decouvert=0;
                                                         }
                                                    }
                                                 }
                                                 TotalClique=1;
                                        }
                                        int n=carte[y_clicked][x_clicked].nbr;
                                        if(TotalClique==1){
                                            carte[y_clicked][x_clicked].decouvert = 1;
                                        }else if(TotalClique==2){
                                            carte[y_clicked][x_clicked].decouvert = 1;
                                            int value=check(n,carte);
                                            if(value==2){
                                                 for(int i=0;i<NB_LIGNES_CARTE;i++){
                                                    for(int j=0;j<NB_COLONNES_CARTE;j++){
                                                         if(carte[i][j].decouvert==1){
                                                                carte[i][j].decouvert=2;
                                                         }
                                                    }
                                                 }
                                            }
                                        }
                            }
                    }
                    break;
            }
        }
        SDL_Delay(100);
    }
}

void depart(SDL_Surface  *ec){
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_WM_SetIcon(IMG_Load("images/logo.jpg"), NULL);
    positionMenu.x = 0;
    positionMenu.y = 0;
    SDL_WM_SetCaption("IEEE ENICAR",NULL);
    menu = IMG_Load("images/backIntro.jpg");
    SDL_BlitSurface(menu, NULL, ec, &positionMenu);
    SDL_Flip(ec);
    SDL_Delay(5000);
}

void chargerFichierVersTableau (int nb_lignes,int nb_colonnes,cell mat[nb_lignes][nb_colonnes],char* nomFichier){
    int i,j;
    FILE*fic=fopen(nomFichier,"r");
    if (fic == NULL){
    exit(1);}
    else{
      for(i=0;i<nb_lignes;i++){
      for(j=0;j<nb_colonnes;j++)
     {
        int c;
        fscanf(fic,"%d",&c);
        mat[i][j].decouvert=0;
        mat[i][j].nbr=c;
     }
       }}
     fclose(fic);
    }

int main(int argc, char *argv[])
{
    SDL_Surface *ecran = NULL;
    cell Mat[NB_LIGNES_CARTE][NB_COLONNES_CARTE];
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(IMG_Load("images/logo.jpg"), NULL);
    ecran = SDL_SetVideoMode(800,600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    depart(ecran);
    SDL_WM_SetCaption("IEEE ENICAR",NULL);
    TTF_Init();
    chargerFichierVersTableau( NB_LIGNES_CARTE, NB_COLONNES_CARTE,Mat,"puzzel.txt");
    deroulerJeu(ecran,Mat);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}
