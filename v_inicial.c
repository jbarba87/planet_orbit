#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>



// Estructura que contendrá los datos de salida de la funcion
// y-> vector al valor de la funcion
// t-> tiempo
struct	datos	{
	double	*y;
	float	t;
	};

// Puntero a una funcion
double	*(*funcion) (float , double*, int);

// Declaracion a la Funcion Euler y rk4
// *func es el puntero a la funcion diferencial (ODE)
// *y0 es el puntero a los valores iniciales
// b el valor final del tiempo
// h es el paso
// N es la cantidad de variables (o cantidad de estados )
struct	datos	euler( double	*(*func) (float , double*, int), double *y0, int  b, float h, int N);
struct	datos	rk4( double	*(*func) (float , double*, int), double *y0, int  b, float h, int N);


// Funciones diferenciales (ODE's)
double	*f(float  t, double *y, int N);
double	*f2(float  t, double *y, int N);

int main(){

	float h = 0.01;			// Paso

	struct	datos	result;		// Declaro la estructura
	funcion = f;

	double	*y0;

	double v_inicial = 1.0;
	y0 = &v_inicial;

	result	= euler(funcion, y0, 2, h, 1);
//	printf("\n%f, %f euler\n", result.t, *result.y );
	result	= rk4(funcion, y0, 2, h, 1);
//	printf("\n%f, %f rk4\n", result.t, *result.y );
	return 0;
}


struct	datos	euler( double	*(*func) (float , double*, int), double *y0, int  b, float h, int N){

	FILE	*archivo;
	archivo = fopen("salida_euler.txt", "w");

	int	n = (int) b/h;
	int	i, j;

	double	*y;
	double	*buffer;
	
	y = calloc(N, sizeof(double));
	buffer = calloc(N, sizeof(double));

	for(j = 0; j < N; j++ ){
		*(y+j) = *(y0+j);
	}
	fprintf(archivo, "%f\n", *y);
	float	t = 0.0;

	struct	datos	resultados;

	for(i = 1; i<=n; i++){
		t = t + h;
		buffer = func(t, y, N);

		for (j = 0; j < N; j++ ){
					
			*(y+j) = *(y+j) + h*( *(buffer + j) );
			
		}
		fprintf(archivo, "%f\n", *y);
	}
	resultados.y = y;
	resultados.t = t;
	fclose(archivo);
	return	resultados;
}



struct	datos	rk4( double	*(*func) (float , double*, int), double *y0, int  b, float h, int N){

	FILE	*archivo;
	archivo = fopen("salida_rk4.txt", "w");

	int	n = (int) b/h;

	int	i, j, l, m;
	float	t = 0.0;
	
	double	*y;
	y = calloc(N, sizeof(double));

	double	*buffer;
	buffer = calloc(N, sizeof(double));
	
	double	*buffer2;
	buffer2 = calloc(N, sizeof(double));

	double	*buffer3;
	buffer3 = calloc(N, sizeof(double));

	for(j = 0; j < N; j++ ){
		*(y+j) = *(y0+j);
	}
	fprintf(archivo, "%f\n", *y);

	double	k[4][N];

	struct	datos	resultados;

	for(i = 1; i<=n; i++){

		t = t + h;

		for (l = 0; l < N; l++ ){
			*(buffer + l) = *(y + l);
		}
		buffer2 = func( t, buffer, N);	
		for (l = 0; l < N; l++ ){
			k[0][l] = h* ( *(buffer2 + l ) );
		}
	
		for (l = 0; l < N; l++ ){
			*(buffer + l) = *(y + l) + 0.5*k[0][l];
		}
		buffer2 = func( t + 0.5*h, buffer, N);
		for (l = 0; l < N; l++ ){
			k[1][l] = h* ( *(buffer2 + l ) );
		}


		for (l = 0; l < N; l++ ){
			*(buffer + l) = *(y + l) + 0.5*k[1][l];
		}
		buffer2 = func( t + 0.5*h, buffer, N);
		for (l = 0; l < N; l++ ){
			k[2][l] = h* ( *(buffer2 + l ) );
		}


		for (l = 0; l < N; l++ ){
			*(buffer + l) = *(y + l) + k[2][l];
		}
		buffer2 = func( t + h, buffer, N);
		for (l = 0; l < N; l++ ){
			k[3][l] = h* ( *(buffer2 + l ) );
		}


		for (l = 0; l < N; l++ ){
			*(y+l) = *(y+l) + ( ( k[0][l] + 2*k[1][l] + 2*k[2][l] + k[3][l] )/6.0 );
			
		}
fprintf(archivo, "%f\n", *y);
	}

	resultados.y = y;
	resultados.t = t;
	fclose(archivo);

	return	resultados;
}




//	y' = y			y = exp^x
double	*f(float  t, double *y, int N){

	return	y;
}

// 	y' = y^2		y = 1/(1-t)
double	*f2(float  t, double *y, int N){

	int	i;
	// Se eleva al cuadrado todos los valores a los que apunta y
	for(i = 0; i<N; i++){
		*(y+i) = ( *(y+i) )*( *(y+i) );
	}

	return	y;
}











