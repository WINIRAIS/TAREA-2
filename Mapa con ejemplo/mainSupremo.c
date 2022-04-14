#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.c"
#include  <stdbool.h>
typedef struct{
    char *nombre;
    char *tipo;
    char *marca;
    size_t stock;
    size_t precio;
}Producto;

const char *get_csv_field (char * tmp, int k) {
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){

        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }


    return NULL;
}

int is_equal_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}
int lower_than_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2) < 0) return 1;
    return 0;
}

void chomp(char *s) {
    while(*s && *s != '\n' && *s != '\r') s++;
    *s = 0;
}

Producto * crearProducto(){
    Producto * new = (Producto*)malloc(sizeof(Producto));
    new->nombre = (char*)calloc(100,sizeof(char));
    new->tipo = (char*)calloc(30,sizeof(char));
    new->marca = (char*)calloc(30,sizeof(char));
    new->stock = 0;
    new->precio = 0;
    return new;
}

void agregarProducto(Map* mapa){
    Producto * new = crearProducto();

    printf("Ingrese nombre del producto: ");
    fgets(new->nombre, 100, stdin);
    chomp(new->nombre);
    if(searchMap(mapa, new->nombre) != NULL){
        printf("PRODUCTO EXISTENTE");
        return;
    }
    

    printf("\nIngrese tipo del producto: ");
    fgets(new->tipo, 30, stdin);
    chomp(new->tipo);

    printf("\nIngrese marca del producto: ");
    fgets(new->marca, 30, stdin);
    chomp(new->marca);

    printf("\nIngrese stock del producto: ");
    scanf("%ld",&new->stock);

    printf("\nIngrese precio del producto: ");
    scanf("%ld", &new->precio);
    printf("\n\n");

    insertMap(mapa, new->nombre, new);
}

void importarCSV(Map* mapaNombre){
    FILE *fp = fopen ("Archivo_100productos.csv", "r");

    char linea[1024];

    fgets (linea, 1023, fp);
    int k=0;

    while (fgets (linea, 1023, fp) != NULL) {
        Producto * new = crearProducto();
        
        strcpy(new->nombre,get_csv_field(linea, 0));
        chomp(new->nombre);
        strcpy(new->marca,get_csv_field(linea, 1));
        chomp(new->marca);
        strcpy(new->tipo,get_csv_field(linea, 2));
        chomp(new->tipo);
        new->stock = atoi(get_csv_field(linea, 3));
        new->precio = atoi(get_csv_field(linea, 4));
        insertMap(mapaNombre,(new->nombre),new);
        k++;
    }
    fclose(fp);
}

void mostrarMapa(Map * mapa){
    Producto* aux = firstMap(mapa);
    while (aux) {
        printf("[%s] / %s / %s\n", aux->nombre, aux->marca, aux->tipo);
        printf("Stock: %ld\n$%ld\n\n", aux->stock, aux->precio);
        aux = nextMap(mapa);
    }
}

bool menu(Map * mapaNombre){
    int respuesta;
    printf("Que desea hacer?\nAgregar producto (1)\nMostrar Todos Los Datos (2)\nEXIT (3)\n");
    scanf("%d", &respuesta);

    if (respuesta == 1){
      agregarProducto(mapaNombre);
      return true;  
    } 
    if (respuesta == 2) {
        mostrarMapa(mapaNombre);
        return true;
    }
    if(respuesta == 3) return false;

}

int main() {

    
    Map * mapaNombre = createMap(is_equal_string);
    setSortFunction(mapaNombre,lower_than_string);

    importarCSV(mapaNombre);

    while(menu(mapaNombre));

    return 0;
}
