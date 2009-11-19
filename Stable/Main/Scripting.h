//#include "stdafx.h"
#include "../StateMachine/FSM.h"
#include "../StateMachine/states.h"
#include "../StateMachine/ScriptedState.h"
#include "../AIController/Actor.h"
#include "Vector3D.h"
//#include "MockEntity.cpp"

#include <iostream>
#include <fstream>  
#include <string>
#include <string.h>
#include <iostream>
#include <map>
#include <utility> // make_pair
#include <conio.h>
#include <windows.h>


using namespace std;

class Scripting{
	//void inicializaEdos();
	//void execute(int);

	FILE *archivo;
	char* nombre;
	Actor* owner;
	char *c;
	string param1;
	string param2;
	string param3;
	int contEdos;
	char palabra[200];
	std::map<string, ScriptedState<Actor>, ltstr> m;
	FSM<Actor>* m_pFSM;

Scripting(char* archivo, Actor* objeto){
	int contEdos = 0;
	nombre = archivo;
	owner = objeto;
}
void mainPrueba()
{	
	int i = 0;
	// abrimos el archivo C:\datos.dat en modo lectura
	archivo = fopen ( nombre, "r" ); 
	while (feof(archivo)==0)
	{
	   
	   fgets(palabra, 200, archivo);/* Obtiene una linea del archivo */
	   //printf("%s", palabra); 
		c = strtok (palabra," ");
		while(c != NULL)
		{
			if( strcmp(c,"State") ==  0 )
			{
				c = strtok (NULL, " ");
				if( i != 0)
				{
					printf("El estado termina en la linea: %d\n",i-1);
					//getch();
				}
				printf("En la linea %d comienza el edo de %s",i,c);
				contEdos = contEdos + 1;
				i = i+1;
				//getch();
			}
			else if(strcmp(c,"MOVP") == 0)
			{
				c = strtok (NULL, " ");
				printf("El estado se movera al punto: ");
				while(strcmp(c,")") != 0)
				{
					c = strtok (NULL, " ");
					if( !(strcmp(c,"(")== 0 || strcmp(c,",")== 0 || strcmp(c,")")== 0) )
						printf("%s", c);
				}
				printf("\n");

				i = i+1;
				//getch();
			}
			else if(strcmp(c,"MOVD") == 0)
			{
				c = strtok (NULL, " ");
				printf("El estado se movera en la direccion: ");
				while(strcmp(c,")") != 0)
				{
					c = strtok (NULL, " ");
					if( !(strcmp(c,"(")== 0 || strcmp(c,",")== 0 || strcmp(c,")")== 0) )
						printf("%s ", c);
				}
				printf("\n");

				i = i+1;
				//getch();
			}
			else if(strcmp(c,"MOVO") == 0)
			{
				c = strtok (NULL, " ");
				printf("El estado se movera al objeto: ");
				while(strcmp(c,")") != 0)
				{
					c = strtok (NULL, " ");
					if( !(strcmp(c,"(")== 0 || strcmp(c,",")== 0 || strcmp(c,")")== 0) )
						printf("%s ", c);
				}
				printf("\n");

				i = i+1;
				//getch();
			}
			else if(strcmp(c,"ROTX") == 0)
			{
				c = strtok (NULL, " ");
				printf("El estado rotara en el eje X: ");
				while(strcmp(c,")") != 0)
				{
					c = strtok (NULL, " ");
					if( !(strcmp(c,"(")== 0 || strcmp(c,",")== 0 || strcmp(c,")")== 0) )
						printf("%s ", c);
				}
				printf("\n");

				i = i+1;
				//getch();
			}
			else if(strcmp(c,"ROTY") == 0)
			{
				c = strtok (NULL, " ");
				printf("El estado rotara en el eje Y: ");
				while(strcmp(c,")") != 0)
				{
					c = strtok (NULL, " ");
					if( !(strcmp(c,"(")== 0 || strcmp(c,",")== 0 || strcmp(c,")")== 0) )
						printf("%s ", c);
				}

				printf("\n");

				i = i+1;
				//getch();
			}
			else if(strcmp(c,"ROTZ") == 0)
			{
				c = strtok (NULL, " ");
				printf("El estado rotara en el eje Z: ");
				while(strcmp(c,")") != 0)
				{
					c = strtok (NULL, " ");
					if( !(strcmp(c,"(")== 0 || strcmp(c,",")== 0 || strcmp(c,")")== 0) )
						printf("%s ", c);
				}

				printf("\n");

				i = i+1;
				//getch();
			}
			else if(strcmp(c,"ILU") == 0)
			{
				c = strtok (NULL, " ");
				printf("Iluminar el objecto: ");
				while(strcmp(c,")") != 0)
				{
					c = strtok (NULL, " ");
					if( !(strcmp(c,"(")== 0 || strcmp(c,",")== 0 || strcmp(c,")")== 0) )
						printf("%s ", c);
				}
				printf("\n");

				i = i+1;
				//getch();
			}
			c = strtok (NULL, " ");

	   }
		
		
	}

	fclose (archivo);
	inicializaEdos();
	execute(4);
	//getch();
	
}

void inicializaEdos()
{
	//Actor* actor = new Actor("Actor 1");

	printf("\n");
	printf("\n");
	printf("Los estados son: %d",contEdos);
	printf("\n");

	contEdos = 0;
	int contLineas = 0;
	int lineaInicial = 0;
	string ultimo;
	int lineaFinal;
	archivo = fopen ( nombre, "r" ); 
	while (feof(archivo)==0)
	{	   
	   fgets(palabra, 200, archivo);/* Obtiene una linea del archivo */	
		c = strtok (palabra," ");
	   while(c != NULL)
	   {
			if( strcmp(c,"State") ==  0)
			{
				c = strtok (NULL, " ");
				lineaFinal = contLineas - 1 ;
				if(contLineas != 0)
				{
					m[ultimo] = *(new ScriptedState<Actor>(ultimo, lineaInicial, lineaFinal));
					lineaInicial = contLineas;
				}
				ultimo = c;
			}
			c = strtok (NULL, " ");
	   }
	   contLineas = contLineas + 1; 	   
	}
	m[ultimo] = *(new ScriptedState<Actor>(ultimo, lineaInicial, contLineas));	
	this->owner->getController()->m_pFSM = new FSM<Actor>(m, "caminar", this->owner);
	//actor -> m_pFSM = new FSM<Actor>(m, "caminar", actor);
	fclose (archivo);
}

void execute(int linea)
{
	int contParam = 1;
	printf("\n");
	printf("\n");
	printf("La linea a ejecutarse es: ");
	printf("\n");
	int i;
	archivo = fopen ( nombre, "r" ); 
	for(i=0; i<=linea; i++)
	{
	   fgets(palabra, 200, archivo);/* Obtiene una linea del archivo */	   
	}
	printf("%s", palabra);
	printf("\n");

	c = strtok (palabra," ");

	if(strcmp(c,"MOVP") == 0)
	{
		c = strtok (NULL, " ");
		printf("Se ejecutara el comando Mover hacia el punto: ");
		
		while(strcmp(c,")") != 0)
		{
			c = strtok (NULL, " ");
			if( !(strcmp(c,"(")== 0 || strcmp(c,",")== 0 || strcmp(c,")")== 0) )
			{
				if(contParam == 1)
					param1 = c;
				else if(contParam == 2)
					param2 = c;
				else if(contParam == 3)
					param3 = c;

				printf("%s", c);
			}

			contParam += 1;
		}
		/* aqui ira el metodo del engine*/
		printf("\n");
		this->owner->move(Vector3D(10.0,0.0,0.0));

		i = i+1;
		//getch();
	}
	else if(strcmp(c,"MOVD") == 0)
	{
		c = strtok (NULL, " ");
		printf("Se ejecutara el comando Mover en la direccion: ");
		while(strcmp(c,")") != 0)
		{
			c = strtok (NULL, " ");
			if( !(strcmp(c,"(")== 0 || strcmp(c,",")== 0 || strcmp(c,")")== 0) )
				printf("%s ", c);
		}
		printf("\n");
		/* aqui ira el metodo del engine*/

		i = i+1;
		//getch();
	}
	else if(strcmp(c,"MOVO") == 0)
	{
		c = strtok (NULL, " ");
		printf("Se ejecutara el comando Mover hacia el objeto: ");
		while(strcmp(c,")") != 0)
		{
			c = strtok (NULL, " ");
			if( !(strcmp(c,"(")== 0 || strcmp(c,",")== 0 || strcmp(c,")")== 0) )
				printf("%s ", c);
		}

		printf("\n");
		/* aqui ira el metodo del engine*/

		i = i+1;
		//getch();
	}
	else if(strcmp(c,"ROTX") == 0)
	{
		c = strtok (NULL, " ");
		printf("Se ejecutara el comando Rotar en X: ");
		while(strcmp(c,")") != 0)
		{
			c = strtok (NULL, " ");
			if( !(strcmp(c,"(")== 0 || strcmp(c,",")== 0 || strcmp(c,")")== 0) )
				printf("%s ", c);
		}
		printf("\n");
		/* aqui ira el metodo del engine*/

		i = i+1;
		//getch();
	}
	else if(strcmp(c,"ROTY") == 0)
	{
		c = strtok (NULL, " ");
		printf("Se ejecutara el comando Rotar en Y: ");
		while(strcmp(c,")") != 0)
		{
			c = strtok (NULL, " ");
			if( !(strcmp(c,"(")== 0 || strcmp(c,",")== 0 || strcmp(c,")")== 0) )
				printf("%s ", c);
		}

		printf("\n");
		/* aqui ira el metodo del engine*/


		i = i+1;
		//getch();
	}
	else if(strcmp(c,"ROTZ") == 0)
	{
		c = strtok (NULL, " ");
		printf("Se ejecutara el comando Rotar en Z: ");
		while(strcmp(c,")") != 0)
		{
			c = strtok (NULL, " ");
			if( !(strcmp(c,"(")== 0 || strcmp(c,",")== 0 || strcmp(c,")")== 0) )
				printf("%s ", c);
		}

		printf("\n");
		/* aqui ira el metodo del engine*/

		i = i+1;
		//getch();
	}
	else if(strcmp(c,"ILU") == 0)
	{
		c = strtok (NULL, " ");
		printf("Se ejecutara el comando Iluminar en el objeto: ");
		while(strcmp(c,")") != 0)
		{
			c = strtok (NULL, " ");
			if( !(strcmp(c,"(")== 0 || strcmp(c,",")== 0 || strcmp(c,")")== 0) )
				printf("%s ", c);
		}
		printf("\n");
		/* aqui ira el metodo del engine*/

		i = i+1;
		//getch();
	}
	else
	{
		printf("Esa linea no existe ...");
	}

	fclose (archivo);
}

};