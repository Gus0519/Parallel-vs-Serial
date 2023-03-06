#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<time.h>
FILE *arch;
int generar_numero(int x);
void simulacion(int array[],int numero_entradas);
void escribir_archivo(int entradas,double tiempo);
void reparar_datos(int array[],int tamano_array);

int main(){
    srand(time(NULL));
    arch = fopen("datos_paralelo.csv","a+");
    int i;
    double tiempo,t_i,t_f;
    for (i=0;i<=2095000;i+=1){
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




void simulacion(int array[],int numero_entradas){
    int i;
    for (i=0;i<numero_entradas;i++){
        array[i] = generar_numero(i);
    }
    for (i=0;i<numero_entradas/10;i++){ //Al 10% de los datos 
        int indice_random = rand()%numero_entradas;
        array[indice_random] += 10; // Le suma para descuadrarlo
    }
}

void escribir_archivo(int entradas,double tiempo){
    fprintf(arch,"%d,%.8f\n",entradas,tiempo);    
}

int generar_numero(int x){
    return x*x;
}

void reparar_datos(int array[],int tamano_array){
    int tamano_intervalo=0,residuo=0;
    tamano_intervalo = omp_get_num_threads();
    residuo =  tamano_array%omp_get_num_threads(); 
    #pragma omp parallel
    {
        int inicio_intervalo=0,fin_intervalo=0;
        inicio_intervalo = tamano_intervalo * omp_get_thread_num();
        fin_intervalo =  tamano_intervalo * omp_get_thread_num() + tamano_intervalo;

        if (omp_get_thread_num() == omp_get_num_threads() - 1){
            fin_intervalo += residuo;
        }

        int i,media;
        for(i=inicio_intervalo;i<=fin_intervalo;i++){
            if (array[i] != generar_numero(i)){
                if (i==0){
                    media = 0;
                    array[i] = media;
                }else{
                    media = (array[i-1] + array[i+1]) / 2;
                    array[i] = media;
                }
            }
        }
    
    }
}
