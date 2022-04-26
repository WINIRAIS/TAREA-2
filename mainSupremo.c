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

//Listo
Producto * crearProducto(){
    Producto * new = (Producto*)malloc(sizeof(Producto));
    new->nombre = (char*)calloc(100,sizeof(char));
    new->tipo = (char*)calloc(30,sizeof(char));
    new->marca = (char*)calloc(30,sizeof(char));
    new->stock = 0;
    new->precio = 0;
    return new;
}

//Listo
void agregarProducto(Map* mapa){
    Producto * new = crearProducto();
    
    fflush(stdin);
    printf("Ingrese nombre del producto: ");
    fgets(new->nombre, 100, stdin);
    chomp(new->nombre);
    if(searchMap(mapa, new->nombre) != NULL){
        Producto * copiaProd = searchMap(mapa, new->nombre);

        printf("\nPRODUCTO EXISTENTE\n");
        printf("\nNo Agregar (1)\nAgrgar de Todas Formas (2)\n");
        int resultado;
        scanf("%d", &resultado);
        if(resultado == 2){ 
            int cantidadAgregada;
            eraseMap(mapa, new->nombre);
            printf("INGRESE CANTIDAD \n");
            scanf("%d", &cantidadAgregada);
            copiaProd->stock += cantidadAgregada;
            insertMap(mapa,new->nombre,copiaProd);
            return;
        } else{ free(new); return; }
    }
    fflush(stdin);
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

//Listo
void importarCSV(Map * mapaNombre){
    FILE *fp = fopen ("Archivo_100productos.csv", "r");

    char linea[1024];

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

void mostrarProducto(Producto* prod){
    printf("[%s] / %s / %s\n", prod->nombre, prod->marca, prod->tipo);
    printf("Stock: %ld\n$%ld\n\n", prod->stock, prod->precio);
}

//Listo
void mostrarMapa(Map * mapa){
    Producto* aux = firstMap(mapa);
    while (aux) {
        mostrarProducto(aux);
        aux = nextMap(mapa);
    }
}
void MostrarPorNombre(Map * mapa){
    char key[100];
    printf("INGRESE NOMBRE\n");
    
    fflush(stdin);
    fgets(key, 100, stdin);
    chomp(key);

    Producto* aux = searchMap(mapa,key);

    if(aux == NULL){
        int res;
        printf("NOMBRE NO EXISTE\n");
        printf("\nPara Agregar (1)\nVolver al Menu Principal (2)\n");
        scanf("%d", &res);
        if(res == 1) agregarProducto(mapa);
        if(res == 2) return;
        else return;
    } 
    mostrarProducto(aux);
}
void MostrarPorTipo(Map * mapa){
    char key[30];
    printf("INGRESE TIPO DEL PRODUCTO\n");
    
    fflush(stdin);
    fgets(key, 30, stdin);
    chomp(key);

    Producto * aux = firstMap(mapa);// esta ordenado asi que deberia de darme el primer dato en relacion a la key
    while(aux != NULL){
        if(strcmp(aux->tipo,key) == 0){
            mostrarProducto(aux);
        }
        aux = nextMap(mapa);

    }
}
void MostrarPorMarca(Map * mapa){
    char key[30];
    printf("INGRESE MARCA DEL PRODUCTO\n");
    
    fflush(stdin);
    fgets(key, 30, stdin);
    chomp(key);

    Producto * aux = firstMap(mapa);// esta ordenado asi que deberia de darme el primer dato en relacion a la key
    while(aux != NULL){
        if(strcmp(aux->marca,key) == 0){
            mostrarProducto(aux);
        }
        aux = nextMap(mapa);

    }
}
bool menu(Map * mapa){
    int respuesta;
    printf("Que desea hacer?\n\nAgregar producto (1)\nMostrar Producto por Nombre (2)\n");
    printf("Mostrar Por Tipo (3)\nMostrar Por Marca (4)\nMostrar Todos Los Datos (5) \nEXIT (6)\n");

    scanf("%d", &respuesta);

    if (respuesta == 1){
      agregarProducto(mapa);
      return true;  
    } 
    if (respuesta == 2) {
        MostrarPorNombre(mapa);
        return true;
    }
    if (respuesta == 3){
        MostrarPorTipo(mapa);
        return true;
    }
    if(respuesta == 4){
        MostrarPorMarca(mapa);
        return true;
    }
    if(respuesta == 5){ 
        printf("\n");
        mostrarMapa(mapa);
        return true;
    }
    if(respuesta == 6) return false;

}

int main() {

    
    Map * mapa = createMap(is_equal_string);

    setSortFunction(mapa,lower_than_string);

    importarCSV(mapa);

    while(menu(mapa));

    return 0;
}
