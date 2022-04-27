#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.c"
#include  <stdbool.h>
#include "list_answer.c"

typedef struct{
    char *nombre;
    char *tipo;
    char *marca;
    size_t stock;
    size_t precio;
}Producto;

typedef struct{
    Producto * producto;
    char nombreCarro[100];
    int cantidad;
}ProductoCarrito;

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
    
    fflush(stdin);
    printf("Ingrese nombre del producto: \n > ");
    fgets(new->nombre, 100, stdin);
    chomp(new->nombre);
    printf("\n");
    if(searchMap(mapa, new->nombre) != NULL){
        Producto * copiaProd = searchMap(mapa, new->nombre);

        printf("\nProducto existente\n");
        printf("No agregar (1)\nAgregar de todas formas (2)\n > ");
        int resultado;
        scanf("%d", &resultado);
        printf("\n");
        if(resultado == 2){ 
            int cantidadAgregada;
            eraseMap(mapa, new->nombre);
            printf("Ingrese la cantidad: \n > ");
            scanf("%d", &cantidadAgregada);
            printf("\n");
            copiaProd->stock += cantidadAgregada;
            insertMap(mapa,new->nombre,copiaProd);
            printf("\n\n");
            return;
        } else{ free(new); return; }
    }
    fflush(stdin);
    printf("Ingrese tipo del producto: \n > ");
    fgets(new->tipo, 30, stdin);
    chomp(new->tipo);
    printf("\n");

    printf("Ingrese marca del producto: \n > ");
    fgets(new->marca, 30, stdin);
    chomp(new->marca);
    printf("\n");

    printf("Ingrese stock del producto: \n > ");
    scanf("%ld",&new->stock);
    printf("\n");

    printf("Ingrese precio del producto: \n > ");
    scanf("%ld", &new->precio);
    printf("\n\n");

    insertMap(mapa, new->nombre, new);
}

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
    printf("Nombre: %s\nMarca: %s\nTipo: %s\n", prod->nombre, prod->marca, prod->tipo);
    printf("Stock: %ld\n$%ld\n\n", prod->stock, prod->precio);
}

void mostrarMapa(Map * mapa){
    Producto* aux = firstMap(mapa);
    while (aux) {
        mostrarProducto(aux);
        aux = nextMap(mapa);
    }
}

void MostrarPorNombre(Map * mapa){
    char key[100];
    printf("Ingrese Nombre de producto a buscar: \n > ");
    fflush(stdin);
    fgets(key, 100, stdin);
    chomp(key);
    printf("\n\n");

    Producto* aux = searchMap(mapa,key);

    if(aux == NULL){
        int res;
        printf("Nombre no existente");
        printf("\nPara agregar (1)\nVolver al Menu Principal (2)\n > ");
        scanf("%d", &res);
        printf("\n\n");
        if(res == 1) agregarProducto(mapa);
        else return;
    } 
    mostrarProducto(aux);
    printf("\n");
}

void MostrarPorTipo(Map * mapa){
    char key[30];
    printf("Ingrese Tipo del producto a buscar: \n > ");
    fflush(stdin);
    fgets(key, 30, stdin);
    chomp(key);
    printf("\n\n");

    Producto * aux = firstMap(mapa);// esta ordenado asi que deberia de darme el primer dato en relacion a la key
    int k = 0;
    while(aux != NULL){
        if(strcmp(aux->tipo,key) == 0){
            mostrarProducto(aux);
            k++;
        }
        aux = nextMap(mapa);
    }
    if (!k) printf("Tipo no existente\n\n");
}

void MostrarPorMarca(Map * mapa){
    char key[30];
    printf("Ingrese Marca del producto a buscar: \n > ");
    fflush(stdin);
    fgets(key, 30, stdin);
    chomp(key);
    printf("\n\n");

    Producto * aux = firstMap(mapa);// esta ordenado asi que deberia de darme el primer dato en relacion a la key
    int k = 0;
    while(aux != NULL){
        if(strcmp(aux->marca,key) == 0){
            mostrarProducto(aux);
            k++;
        }
        aux = nextMap(mapa);
    }
    if (!k) printf("Marca no existente\n\n");
}

void crearCarrito(Map * carrito, char *nombreC, ProductoCarrito * new){
    List * listaP = createList();
    insertMap(carrito, nombreC, listaP);
    pushBack(searchMap(carrito, nombreC), new);
    //printf("agsdgasgasgsagsagsagsgsfgasgas");
}

void agregarDataCarrito(Map * mapa, Map* carrito){
    char nombreProd[100];
    fflush(stdin);
    printf("Ingrese nombre del producto: \n > ");
    fgets(nombreProd, 100, stdin);
    chomp(nombreProd);
    printf("\n");

    Producto * prod = searchMap(mapa, nombreProd);
    if (prod == NULL){
        printf("Producto no existente\n\n");
        return;
    }

    ProductoCarrito * new = (ProductoCarrito*)malloc(sizeof(ProductoCarrito));
    new->producto = prod;
    printf("Ingrese cantidad\n > ");
    scanf("%d", &(new->cantidad));
    printf("\n");
    

    char nombreCarrito[100];
    fflush(stdin);
    printf("Ingrese nombre del carrito: \n > ");
    fgets(nombreCarrito, 100, stdin);
    chomp(nombreCarrito);
    printf("\n");
    strcpy(new->nombreCarro, nombreCarrito); 

    List * aux = searchMap(carrito, nombreCarrito);
    if (aux == NULL){
        crearCarrito(carrito, nombreCarrito, new);
        return;
    }
    pushBack(aux, new);
}

void mostrarProductosCarrito(Map * carrito){
    List * aux = firstMap(carrito);
    int k = 1;
    
    if(aux==NULL){
    printf("No hay carritos\n\n");
    return;
    }
    while (aux != NULL){
        ProductoCarrito *auxiliar = firstList(aux);
        while(auxiliar!=NULL){
            printf("(%s) / %s / %d\n",auxiliar->nombreCarro, auxiliar->producto->nombre, auxiliar->cantidad);
            auxiliar = nextList(aux);
        }
        k++;
        printf("\n");
        aux = nextMap(carrito);
    }
    
    printf("\n");
}

bool menu(Map * mapa, Map * carrito){
    int res;
    printf ("Que desea hacer?\nCatalogo(1)\nCarritos(2)\nEXIT(3)\n > ");
    scanf("%d", &res);
    printf("\n");
    if(res == 1){ 
        int respuesta;
        printf("Agregar producto (1)\nMostrar Producto por Nombre (2)\n");
        printf("Mostrar Por Tipo (3)\nMostrar Por Marca (4)\nMostrar Todos Los Datos (5) \nVolver al Menu(6)\n");
        printf(" > ");

        scanf("%d", &respuesta);
        printf("\n");

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
        if(respuesta == 6) menu(mapa, carrito);
        exit(0);
    }
    if (res == 2){
        int respuesta;
        printf("Agregar al Carrito(1)\nMostrar productos Carrito(2)\n > ");
        scanf("%d", &respuesta);
        printf("\n");
        if (respuesta == 1){
            agregarDataCarrito(mapa, carrito);
            return true;
        }
        if (respuesta == 2){
            mostrarProductosCarrito(carrito);
            return true;
        }
    }
    exit(0);
}

int main() {

    
    Map * mapa = createMap(is_equal_string);
    Map * carrito = createMap(is_equal_string);

    setSortFunction(carrito, lower_than_string);
    setSortFunction(mapa,lower_than_string);

    importarCSV(mapa);
    while(menu(mapa, carrito));

    return 0;
}
