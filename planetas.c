#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

FILE	*archivo;
// Sistema Solar
// Unidades en Kg., metros y segundos

#define Nsphere 	10

// Cte. gravitacional
#define G 		6.67259e-11

// Masas
#define M_Mercurio 	0.33e24			// Masa Mercurio
#define M_Venus		4.87e24			// Masa Venus
#define M_Tierra	5.97e24			// Masa Tierra
#define M_Marte		0.642e24		// Masa Marte
#define M_Jupiter	1899e24			// Masa Jupiter
#define M_Saturno	568e24			// Masa Saturno
#define M_Urano		86.8e24			// Masa Urano
#define M_Neptuno	102e24			// Masa Neptuno
#define M_Pluton	0.0125e24		// Masa Pluton
#define M_Sol		1047.3486*M_Jupiter		// Masa Sol


// Posicion (se asume que los planetas esta alineados con y=0)
#define P_Mercurio 	57.9e9
#define P_Venus 	108.2e9
#define P_Tierra 	149.6e9
#define P_Marte 	227.9e9
#define P_Jupiter 	778.6e9
#define P_Saturno 	1433.5e9
#define P_Urano		2872.5e9
#define P_Neptuno	4495.1e9
#define P_Pluton	5870.0e9
#define P_Sol 		0


// Velocidad Inicial (solo en la direcion Y)
#define V_Mercurio	47.89e3
#define V_Venus		35.03e3
#define V_Tierra	29.79e3
#define V_Marte		24.13e3
#define V_Jupiter	13.06e3
#define V_Saturno	9.64e3
#define V_Urano		6.80e3
#define V_Neptuno	5.40e3
#define V_Pluton	4.70e3
#define V_Sol		0

struct	datos	euler( double	*(*funcion) (float , double*, int), double *y0, int  b, float h, int N);

struct	datos	rk4( double	*(*func) (float , double*, int), double *y0, int  b, float h, int N);

double	*f_planetas(float  t, double *y, int N);

double	*(*funcion) (float , double*, int);		// declaro puntero a funcion

struct	datos	{
	double	*y;
	float	t;
	};

int main(){

	double	y[4*Nsphere];
	
	//double	*dy;

	double	*y0;

	y0 = &y[0];

	*(y0) = P_Mercurio;
	*(y0+1) = 0;
	*(y0+2) = 0;
	*(y0+3) = V_Mercurio;

	*(y0+4) = P_Venus;
	*(y0+5) = 0;
	*(y0+6) = 0;
	*(y0+7) = V_Venus;

	*(y0+8) = P_Tierra;
	*(y0+9) = 0;
	*(y0+10) = 0;
	*(y0+11) = V_Tierra;

	*(y0+12) = P_Marte;
	*(y0+13) = 0;
	*(y0+14) = 0;
	*(y0+15) = V_Marte;

	*(y0+16) = P_Jupiter;
	*(y0+17) = 0;
	*(y0+18) = 0;
	*(y0+19) = V_Jupiter;

	*(y0+20) = P_Saturno;
	*(y0+21) = 0;
	*(y0+22) = 0;
	*(y0+23) = V_Saturno;

	*(y0+24) = P_Urano;
	*(y0+25) = 0;
	*(y0+26) = 0;
	*(y0+27) = V_Urano;

	*(y0+28) = P_Neptuno;
	*(y0+29) = 0;
	*(y0+30) = 0;
	*(y0+31) = V_Neptuno;

	*(y0+32) = P_Pluton;
	*(y0+33) = 0;
	*(y0+34) = 0;
	*(y0+35) = V_Pluton;

	*(y0+36) = P_Sol;
	*(y0+37) = 0;
	*(y0+38) = 0;
	*(y0+39) = V_Sol;

	struct	datos	result;		// declaro la estructura

	int	i, j;

	funcion = f_planetas;
	int	h = 500;

	// 90 dias -> 90*24*60*60 = 7776000 seg.
	int	b = 364*24*60*60;	// seg.
	int	N = Nsphere*4;

	// Aqui se llaman las funciones
//	result	= euler(funcion, y0, b, h, N);
	result	= rk4(funcion, y0, b, h, N);

archivo = fopen("planetas_rk4_364.txt", "w");

for(i = 0; i<N; i++){
	fprintf(archivo, "%f\n", *(result.y+i));
}

	fclose(archivo);
	return 0;
}





struct	datos	euler( double	*(*func) (float , double*, int), double *y0, int  b, float h, int N){

	int	n = (int) b/h;
	int	i, j, l;

	double	*y;
	double	*buffer;
	
	y = calloc(N, sizeof(double));
	buffer = calloc(N, sizeof(double));

	for(j = 0; j < N; j++ ){
		*(y+j) = *(y0+j);
	}

	float	t = 0.0;

	struct	datos	resultados;

	for(i = 1; i<=n; i++){
		t = t + h;
		buffer = func(t, y, N);

		for (j = 0; j < N; j++ ){
			*(y+j) = *(y+j) + h*( *(buffer + j) );
		}

	}
	resultados.y = y;
	resultados.t = t;
	return	resultados;
}










struct	datos	rk4( double	*(*func) (float , double*, int), double *y0, int  b, float h, int N){

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

	}

	resultados.y = y;
	resultados.t = t;

	return	resultados;
}



double	*f_planetas(float  t, double *y, int N){

	int	i, j;
	double	M[Nsphere];

	M[0] = M_Mercurio;
	M[1] = M_Venus;
	M[2] = M_Tierra;
	M[3] = M_Marte;
	M[4] = M_Jupiter;
	M[5] = M_Saturno;
	M[6] = M_Urano;
	M[7] = M_Neptuno;
	M[8] = M_Pluton;
	M[9] = M_Sol;

	double	*dy;
	dy = calloc(Nsphere*4, sizeof(double));

	double	distancias[Nsphere][Nsphere];

	for (i = 0; i<Nsphere; i++){
		for (j = 0; j<Nsphere; j++){
			distancias[i][j] = sqrt( pow( ( *(y + 4*i) - *(y + 4*j) ), 2) + pow(( *(y + 4*i +1 ) - *(y + 4*j + 1) ), 2) );
		}
	}

	for (i = 0; i < Nsphere; i++ ){
		*(dy + 4*i) = *(y + 4*i + 2);
		*(dy + 4*i + 1) = *(y + 4*i + 3);
	}

	double accu_x = 0.0, accu_y = 0.0;

	for (i = 0; i < Nsphere; i++ ){

		accu_x = 0.0;
		accu_y = 0.0;

		for (j = 0; j < Nsphere; j++ ){
			if (distancias[i][j] == 0.0){
				continue;
			}
			accu_x = accu_x + M[i]*M[j]* ( *(y + 4*j) - *(y + 4*i) ) / ( pow( distancias[i][j], 3)) ;
			accu_y = accu_y + M[i]*M[j]* ( *(y + 4*j + 1) - *( y + 4*i + 1) ) / ( pow( distancias[i][j], 3)) ;
		}

		*(dy + 4*i + 2) = accu_x*G/M[i];
		*(dy + 4*i + 3) = accu_y*G/M[i];
	}

	return	dy;
}










