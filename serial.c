#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
FILE* arch;
void escribir_archivo(int entradas,double tiempo);
int generar_numero(int x);
void simulacion(int array[],int numero_entradas);
void imprimir_arreglo(int array[],int tamano_arreglo);
void reparar_datos(int array[],int tamano_array);

int main(){
    srand(time(NULL));
    arch = fopen("datos_serial_2.csv","a+");
    int i;
    double tiempo,t_i,t_f;
    for (i=0;i<=500;i+=1){
        int array[i];
        simulacion(array,i);
        t_i = omp_get_wtime();
        reparar_datos(array,i);
        t_f = omp_get_wtime();
        tiempo = t_f-t_i;
        escribir_archivo(i,tiempo);
    }
    
    fclose(arch);
}



int generar_numero(int x){
    return x*x;
}

void simulacion(int array[],int numero_entradas){
    int i;
    for (i=0;i<numero_entradas;i++){
        array[i] = generar_numero(i);
    }
    for (i=0;i<numero_entradas/10;i++){
        int indice_random = rand()%numero_entradas;
        array[indice_random] += rand()%10; 
    }
}

void imprimir_arreglo(int array[],int tamano_arreglo){
    int j=0;
    while(j<tamano_arreglo){
        printf("%d\n",array[j]);
        j++;
    }
}

void reparar_datos(int array[],int tamano_array){
    int media,i;
    for (i=0;i<tamano_array;i++){
        if (array[i] != generar_numero(i)){
            if (i == 0){
                media = 0; 
                array[i] = media;
            }else{
                media = (array[i-1] + array[i+1]) / 2;
                array[i] = media;
            }
        }
    }
}

void escribir_archivo(int entradas,double tiempo){
    fprintf(arch,"%d,%.8f\n",entradas,tiempo);    
}
