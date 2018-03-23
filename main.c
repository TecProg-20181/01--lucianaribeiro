#include <stdio.h>

typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> red
    // 1 -> green
    // 2 -> blue
    Pixel pixel[512][512];
    unsigned int width;
    unsigned int height;
} Image;

int maximo(int, int);
int minimo(int, int);

Image escalaCinza(Image);
Image sepia(Image);
Image blur(Image, int);
Image rotacionar90direita(Image);
Image espelhamento(Image, int);
Image inverterCores(Image);
Image cortarImagem(Image, int, int, int, int);


int main() {
    Image img;

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    // read all pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j].red,
                                 &img.pixel[i][j].green,
                                 &img.pixel[i][j].blue);

        }
    }

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = escalaCinza(img);
                break;
            }
             case 2: { // Filtro Sepia
                 img = sepia(img);
                 break;
             }
             case 3: { // Blur
                 int tamanho = 0;
                 scanf("%d", &tamanho);
                 img = blur(img, tamanho);
                 break;
             }
             case 4: { // Rotacao
                 int quantidade = 0;
                 scanf("%d", &quantidade);

                 quantidade %= 4;

                 for (int j = 0; j < quantidade; ++j) {
                     img = rotacionar90direita(img);
                 }
                 break;
             }
             case 5: { // Espelhamento
                 int horizontal = 0;
                 scanf("%d", &horizontal);

                 img = espelhamento(img, horizontal);

                 break;
             }
             case 6: { // Inversao de Cores
                 img = inverterCores(img);
                 break;
             }
             case 7: { // Cortar Imagem
                 int x, y;
                 scanf("%d %d", &x, &y);
                 int width, height;
                 scanf("%d %d", &width, &height);

                 img = cortarImagem(img, x, y, width, height);
                 break;
             }
        }

    }

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j].red,
                                   img.pixel[i][j].green,
                                   img.pixel[i][j].blue);

        }
        printf("\n");
    }
    return 0;
}



int maximo(int sizeOne, int sizeTwo) {
    if (sizeOne > sizeTwo)
        return sizeOne;
    return sizeTwo;
}

int minimo(int sizeOne, int sizeTwo) {
    if (sizeOne < sizeTwo)
        return sizeOne;
    return sizeTwo;
}

Image escalaCinza(Image img) {

    int width = img.width;
    int height = img.height;

    for (unsigned int count1 = 0; count1 < img.height; ++count1) {
        for (unsigned int count2 = 0; count2 < img.width; ++count2) {
            int media = img.pixel[count1][count2].red +
                        img.pixel[count1][count2].green +
                        img.pixel[count1][count2].blue;
            media /= 3;
            img.pixel[count1][count2].red = media;
            img.pixel[count1][count2].green = media;
            img.pixel[count1][count2].blue = media;
        }
    }

    return img;
}

  Image sepia(Image img) {

    int width = img.width;
    int height = img.height;

    for (unsigned int count1 = 0; count1 < height; ++count1) {
        for (unsigned int count2 = 0; count2 < width; ++count2) {

            unsigned short int pixel[3];

            pixel[0] = img.pixel[count1][count2].red;
            pixel[1] = img.pixel[count1][count2].green;
            pixel[2] = img.pixel[count1][count2].blue;

            int sample =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
            int menor_r = minimo(255, sample);
            img.pixel[count1][count2].red = menor_r;

            sample =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            menor_r = minimo(255, sample);
            img.pixel[count1][count2].green = menor_r;

            sample =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            menor_r = minimo(255, sample);
            img.pixel[count1][count2].blue = menor_r;
        }
    }

    return img;
  }

 Image blur(Image img, int tamanho) {

   Image blur;

   int width = img.width;
   int height = img.height;

     for (unsigned int count1 = 0; count1 < height; ++count1) {
         for (unsigned int count2 = 0; count2 < width; ++count2) {

             Pixel media = {0, 0, 0};

             int menor_height = minimo(height - 1, count1 + tamanho/2);
             int min_width = minimo(width - 1, count2 + tamanho/2);

             for(int countX = maximo(0, count1 - tamanho/2); countX <= menor_height; ++countX) {
                 for(int countY = maximo(0, count2 - tamanho/2); countY <= min_width; ++countY) {
                     media.red += img.pixel[countX][countY].red;
                     media.green += img.pixel[countX][countY].green;
                     media.blue += img.pixel[countX][countY].blue;
                 }
             }

             media.red /= tamanho * tamanho;
             media.green /= tamanho * tamanho;
             media.blue /= tamanho * tamanho;

             img.pixel[count1][count2].red = media.red;
             img.pixel[count1][count2].green = media.green;
             img.pixel[count1][count2].blue = media.blue;
         }
     }
     return img;
 }

 Image rotacionar90direita(Image img) {

     Image rotacionada;

     rotacionada.width = img.height;
     rotacionada.height = img.width;

     int width = rotacionada.width;
     int height = rotacionada.height;

     for (unsigned int count1 = 0, countY = 0; count1 < height; ++count1, ++countY) {
         for (int count2 = width - 1, countX = 0; count2 >= 0; --count2, ++countX) {
             rotacionada.pixel[count1][count2].red = img.pixel[countX][countY].red;
             rotacionada.pixel[count1][count2].green = img.pixel[countX][countY].green;
             rotacionada.pixel[count1][count2].blue = img.pixel[countX][countY].blue;
         }
     }

     return rotacionada;
 }

 Image espelhamento(Image img, int horizontal) {

   int width = img.width;
   int height = img.height;

   if (horizontal == 1)
      width /= 2;
   else
      height /= 2;

   for (int count1 = 0; count1 < height; ++count1) {
       for (int count2 = 0; count2 < width; ++count2) {

           int countX = count1;
           int countY = count2;

           if (horizontal == 1)
            countY = img.width - 1 - count2;
           else
            countX = img.height - 1 - count1;

           Pixel sample;
           sample.red = img.pixel[count1][count2].red;
           sample.green = img.pixel[count1][count2].green;
           sample.blue = img.pixel[count1][count2].blue;

           img.pixel[count1][count2].red = img.pixel[countX][countY].red;
           img.pixel[count1][count2].green = img.pixel[countX][countY].green;
           img.pixel[count1][count2].blue = img.pixel[countX][countY].blue;

           img.pixel[countX][countY].red = sample.red;
           img.pixel[countX][countY].green = sample.green;
           img.pixel[countX][countY].blue = sample.blue;
       }
   }

   return img;
 }

 Image inverterCores(Image img) {

     int width = img.width;
     int height = img.height;

     for (unsigned int count1 = 0; count1 < height; ++count1) {
         for (unsigned int count2 = 0; count2 < width; ++count2) {
             img.pixel[count1][count2].red = 255 - img.pixel[count1][count2].red;
             img.pixel[count1][count2].green = 255 - img.pixel[count1][count2].green;
             img.pixel[count1][count2].blue = 255 - img.pixel[count1][count2].blue;
         }
     }
     return img;
 }

 Image cortarImagem(Image img, int countX, int countY, int width, int height) {

     Image cortada;

     cortada.width = width;
     cortada.height = height;

     for(int count1 = 0; count1 < height; ++count1) {
         for(int count2 = 0; count2 < width; ++count2) {
             cortada.pixel[count1][count2].red = img.pixel[count1 + countY][count2 + countX].red;
             cortada.pixel[count1][count2].green = img.pixel[count1 + countY][count2 + countX].green;
             cortada.pixel[count1][count2].blue = img.pixel[count1 + countY][count2 + countX].blue;
         }
     }

     return cortada;
 }
