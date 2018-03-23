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

int pixel_igual(Pixel p1, Pixel p2) {
    if (p1.red == p2.red &&
        p1.green == p2.green &&
        p1.blue == p2.blue)
        return 1;
    return 0;
}


Image escala_de_cinza(Image img) {

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int media = img.pixel[i][j].red +
                        img.pixel[i][j].green +
                        img.pixel[i][j].blue;
            media /= 3;
            img.pixel[i][j].red = media;
            img.pixel[i][j].green = media;
            img.pixel[i][j].blue = media;
        }
    }

    return img;
}

  Image sepia(Image img) {

    for (unsigned int x = 0; x < img.height; ++x) {
        for (unsigned int j = 0; j < img.width; ++j) {
            unsigned short int pixel[3];
            pixel[0] = img.pixel[x][j].red;
            pixel[1] = img.pixel[x][j].green;
            pixel[2] = img.pixel[x][j].blue;

            int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
            int menor_r = (255 >  p) ? p : 255;
            img.pixel[x][j].red = menor_r;

            p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            menor_r = (255 >  p) ? p : 255;
            img.pixel[x][j].green = menor_r;

            p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            menor_r = (255 >  p) ? p : 255;
            img.pixel[x][j].blue = menor_r;
        }
    }

    return img;
  }

 Image blur(Image img, int tamanho) {

   Image blur;

     for (unsigned int i = 0; i < img.height; ++i) {
         for (unsigned int j = 0; j < img.width; ++j) {
             Pixel media = {0, 0, 0};

             int menor_height = minimo(img.height - 1, i + tamanho/2);
             int min_width = minimo(img.width - 1, j + tamanho/2);

             for(int x = maximo(0, i - tamanho/2); x <= menor_height; ++x) {
                 for(int y = maximo(0, j - tamanho/2); y <= min_width; ++y) {
                     media.red += img.pixel[x][y].red;
                     media.green += img.pixel[x][y].green;
                     media.blue += img.pixel[x][y].blue;
                 }
             }

             media.red /= tamanho * tamanho;
             media.green /= tamanho * tamanho;
             media.blue /= tamanho * tamanho;

             img.pixel[i][j].red = media.red;
             img.pixel[i][j].green = media.green;
             img.pixel[i][j].blue = media.blue;
         }
     }
     return img;
 }

 Image rotacionar90direita(Image img) {

     Image rotacionada;

     rotacionada.width = img.height;
     rotacionada.height = img.width;

     for (unsigned int i = 0, y = 0; i < rotacionada.height; ++i, ++y) {
         for (int j = rotacionada.width - 1, x = 0; j >= 0; --j, ++x) {
             rotacionada.pixel[i][j].red = img.pixel[x][y].red;
             rotacionada.pixel[i][j].green = img.pixel[x][y].green;
             rotacionada.pixel[i][j].blue = img.pixel[x][y].blue;
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

   for (int i2 = 0; i2 < height; ++i2) {
       for (int j = 0; j < width; ++j) {
           int x = i2, y = j;

           if (horizontal == 1) y = img.width - 1 - j;
           else x = img.height - 1 - i2;

           Pixel aux1;
           aux1.red = img.pixel[i2][j].red;
           aux1.green = img.pixel[i2][j].green;
           aux1.blue = img.pixel[i2][j].blue;

           img.pixel[i2][j].red = img.pixel[x][y].red;
           img.pixel[i2][j].green = img.pixel[x][y].green;
           img.pixel[i2][j].blue = img.pixel[x][y].blue;

           img.pixel[x][y].red = aux1.red;
           img.pixel[x][y].green = aux1.green;
           img.pixel[x][y].blue = aux1.blue;
       }
   }

   return img;
 }

 Image inverter_cores(Image img) {
     for (unsigned int i = 0; i < img.height; ++i) {
         for (unsigned int j = 0; j < img.width; ++j) {
             img.pixel[i][j].red = 255 - img.pixel[i][j].red;
             img.pixel[i][j].green = 255 - img.pixel[i][j].green;
             img.pixel[i][j].blue = 255 - img.pixel[i][j].blue;
         }
     }
     return img;
 }

 Image cortar_imagem(Image img, int x, int y, int width, int height) {
     Image cortada;

     cortada.width = width;
     cortada.height = height;

     for(int i = 0; i < height; ++i) {
         for(int j = 0; j < width; ++j) {
             cortada.pixel[i][j].red = img.pixel[i + y][j + x].red;
             cortada.pixel[i][j].green = img.pixel[i + y][j + x].green;
             cortada.pixel[i][j].blue = img.pixel[i + y][j + x].blue;
         }
     }

     return cortada;
 }


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
                img = escala_de_cinza(img);
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
                 img = inverter_cores(img);
                 break;
             }
             case 7: { // Cortar Imagem
                 int x, y;
                 scanf("%d %d", &x, &y);
                 int width, height;
                 scanf("%d %d", &width, &height);

                 img = cortar_imagem(img, x, y, width, height);
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
