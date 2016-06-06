#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <queue>          
#include <list> 
#include <chrono>

using namespace std;
using namespace std::chrono;

static long long int nodosExpandidos=0;
static long long int tamColaNodosVivos=0;
static long long int numPodas=0;

vector<int> solucionGreedy(const vector<vector<int> > &matrizDistancias,vector<bool> &cvistas,int &distancia_total, int numCiudades){

   int ciudad_actual;
   //Donde vamos a guardar el minimo de las ciudades que rodean a la actual.
   map<int, int> minimo;

   //La secuencia de solucion:
   vector<int> solucion;

   //Empezamos a recorrer desde el 0:
   ciudad_actual = 0;
   int ciudad_inicial = ciudad_actual;
   cvistas[ciudad_actual]=true;

   //En total vamos a pasar por n-1 ciudades partiendo de la ciudad origen
   solucion.push_back(ciudad_actual+1);
   
   for(int n=0; n< numCiudades-1; n++){

    // Nos disponemos a calcular la ciudad mas cercana por la que no hemos pasado ya:
      for(int i = 0; i < numCiudades; i++){

        //ignoramos las ciudades por donde hemos pasado:
        if(!cvistas[i]){
          minimo.insert(pair<int,int> (matrizDistancias[ciudad_actual][i], i));
        }
      }

      //actualizamos la ciudad actual a la que se encuentra más cerca:
      if(!minimo.empty()){
        distancia_total+= matrizDistancias[ciudad_actual][minimo.begin()->second];
        ciudad_actual = minimo.begin()->second;
        
        solucion.push_back(ciudad_actual+1);//sumamos 1 porque las ciudades empiezan desde 1
        
      }
      else{ //Nos encontramos en el ultimo
        distancia_total+= matrizDistancias[ciudad_actual][ciudad_inicial];
          solucion.push_back(ciudad_actual+1);
         }

      //limpiamos el set:
      cvistas[ciudad_actual] = true;
      minimo.clear();
   }

   distancia_total+= matrizDistancias[ciudad_actual][ciudad_inicial];

   return solucion;
}

//Clase Solucion que representarán nuestros nodos
class Solucion{


	private:

		vector<int> ruta;
		int distancia; // Distancia recorrida desde el principio hasta la ciudad actual	
		int ciudadActual; //Ciudad actual en la que me encuentro
		int cpActual; // Costo posible acumulado del tramo de ruta que llevamos
		list<int> ciudadesRestantes; //Ciudades restantes por visitar


	public:

		//Constructor
		Solucion(int numCiudades){

			ciudadActual=0;
			distancia=0;
			cpActual=0;

			ruta.push_back(1);

			//Inicializar la lista de ciudades restantes sin contar la primera
			ciudadesRestantes.assign(numCiudades-1,-1);

			//Numeramos las ciudades restantes (empezando por la segunda)
			int i=2;

      for (auto it=ciudadesRestantes.begin(); it !=ciudadesRestantes.end(); ++it,i++)
      	*it=i;
		}
		
		vector<int> getSolucion(){
			return ruta;
		}

		int getDistancia(){
			return distancia;

		}

		int getCotaLocal() const{
			return cpActual;
		}

		vector<int> getCiudadesRestantes(){

			vector<int> aux;
        	for (auto it=ciudadesRestantes.begin(); it!=ciudadesRestantes.end(); ++it)
            	aux.push_back(*it);
	        return aux;
		}

		//Devuelve si hemos recorrido todas las ciudades
		bool esSolucion(int numCiudades){
			return (ruta.size()==numCiudades);
		}

		int distanciaCiudades(int ciudad1,int ciudad2, const vector< vector<int> > & matrizDistancias) { //restar uno ya que la ciduad n, en la matriz es la n-1
    		return matrizDistancias[ciudad1-1][ciudad2-1];

    }

		void aniadirCiudad(int nuevaCiudad, const vector< vector<int> >  &matrizDistancias){

			distancia+=distanciaCiudades(ruta.back(),nuevaCiudad, matrizDistancias);
			ruta.push_back(nuevaCiudad);

		}

		void eliminarCiudad(const vector< vector<int> >  &matrizDistancias){

			distancia-=distanciaCiudades(ruta[ruta.size()-2],ruta.back(),matrizDistancias);
			ruta.pop_back();

		}

		void aniadirCiudadRestante(int nuevaCiudadR) {
        ciudadesRestantes.push_back(nuevaCiudadR);
    }

    void eliminarCiudadRestante(int nuevaCiudadR) {

    	  bool encontrado = false;
        for (auto it=ciudadesRestantes.begin(); it!=ciudadesRestantes.end() && !encontrado; ++it) {
            if(*it==nuevaCiudadR) {
                ciudadesRestantes.erase(it);
                encontrado = true;
            }
        }
    }

		void calcularCotaLocal(const vector<int> & menorDeCadaFila) { //En menorDeCadaFila en la posicion 0, tenemos la ciudad 1.
        
        cpActual=distancia;

        for (auto it=ciudadesRestantes.begin(); it!=ciudadesRestantes.end(); ++it)
            cpActual+=menorDeCadaFila[(*it)-1];

        cpActual+=menorDeCadaFila[0];
    }

    int calculaDistancia(const vector< vector<int> > & matriz) { //devuelve la distnacia y añade la distancia para ir a la ciudad 1.
        distancia+=distanciaCiudades(ruta.back(),1,matriz);
        return distancia;
    }


		//Sobrecarga del operador < para la priority_queue
		bool operator<( const Solucion & otro) const { //Ordenados de menor a mayor
			return otro.cpActual < cpActual;
    }


    void Greedy(const vector<vector<int> > &matrizDistancias, vector<bool> &cvistas,int &distancia_total, int numCiudades){

    	ruta=solucionGreedy(matrizDistancias,cvistas,distancia_total,numCiudades);

      cout << "\nDistancia recorrida con Greedy: " << distancia_total + distanciaCiudades(ruta[0],ruta[ruta.size()-1],matrizDistancias) << endl;
      
      for (auto it=ruta.begin(); it!=ruta.end(); ++it)
        cout << *it << " ";

      cout << endl;
    
    	distancia=distancia_total;
    	ciudadesRestantes.clear();
    }



};

    vector<int> calcularMenorDeCadaFila(const vector< vector<int> > & matriz) {

        vector<int> salida(matriz.size());
        int min=0,j;

        for(int i=0; i<matriz.size(); i++) {
            min=matriz[i][0];
            j=1;
            if(min==-1) {
                min=matriz[i][1];
                j++;
            }
            for(; j<matriz[i].size(); j++) {
                if(matriz[i][j]<min && matriz[i][j]!=-1)  min=matriz[i][j];
            }

            salida[i]=min;
        }
        return salida;
    }

vector<pair<double,double> > leerDatos(char * archivo){

	//Abrimos el archivo de entrada:
   ifstream entrada;
   entrada.open(archivo);

   //ignoramos la primera palabra:
   string palabra;
   entrada >> palabra;

   //leemos el num. de ciudades:
   int numCiudades;
   entrada >> numCiudades;

   vector<pair<double, double> > coordenadas;

   //Empezamos a almacenar las ciudades apartir de la posición 1 del vector:
   for(int i = 0; i < numCiudades; i++){
   	

   	pair<double, double> p;
   	double coordenada;

   	//ignoramosel noombre de la ciudad:
   	entrada >> coordenada;

   	entrada >> coordenada;
   	p.first = coordenada;
   	entrada >> coordenada;
   	p.second = coordenada;

   	coordenadas.push_back(p);

   }

   return coordenadas;


}

void calcularMatrizDistancias(const vector<pair<double,double> > &coordenadas,
vector<vector<int> > &matrizDistancias, int numCiudades){


	//Calculamos la matriz de distancias:
   for (int i = 0; i < numCiudades; i++){
   	for(int j = 0; j < numCiudades; j++){

   		if (j == i)
   			matrizDistancias[i][j] = 0;
   		else{
   			matrizDistancias[i][j] = 
   			round(sqrt( ( (coordenadas[j].first - coordenadas[i].first) * (coordenadas[j].first - coordenadas[i].first) ) +
   			( (coordenadas[j].second - coordenadas[i].second) * (coordenadas[j].second - coordenadas[i].second) ) ));
   		}
   	}

   }

}


Solucion BranchAndBound(vector<vector<int> > &matrizDistancias, const vector<int> & menorDeCadaFila, int numCiudades ){

   vector<bool> cvistas(numCiudades);

   //Inicializamos a false:
   for (int i = 0; i < numCiudades; i++)
   	cvistas[i] = false;

   //La que sera nuestra primera cota global
   int distancia_total = 0;
   //Dos nodos, el primero es aquel en el que vamos a hacer las expansiones en cada nivel,
   // el segundo representa la primera cota global (solución greedy)
   Solucion nodoEnExpansion(numCiudades), nodoSolucionGreedy(numCiudades);
   nodoSolucionGreedy.Greedy(matrizDistancias,cvistas,distancia_total,numCiudades);

   //PQ para ir seleccionando el nodo cuyo coste sea el menor:
   priority_queue<Solucion> colaNodosVivos;

   //Devuelve la distancia de ese camino (cota global)
   int cotaGlobal= nodoSolucionGreedy.getDistancia();
   int distanciaActual=0;

   //Metemos en los vivos el nodo inicial a expandir (ciudad 1):
   colaNodosVivos.push(nodoEnExpansion);
   nodosExpandidos++;
   tamColaNodosVivos++;

   vector<int> siguienteExpansion;

   // Mientras haya nodos vivos y nos queden nodos vivos con una cota local menor que la cota global,
   // continuamos buscando la ruta con menor cota local posible.
   while( (!colaNodosVivos.empty()) && (colaNodosVivos.top().getCotaLocal() < cotaGlobal) ) {

      int tamQueue = colaNodosVivos.size();
      if (tamColaNodosVivos < tamQueue)
        tamColaNodosVivos = tamQueue;

   		nodoEnExpansion = colaNodosVivos.top(); // Cogemos el nodo con la menor cota local
   		colaNodosVivos.pop();

   		siguienteExpansion = nodoEnExpansion.getCiudadesRestantes();

   		for(int i=0; i<siguienteExpansion.size(); i++) {

            nodoEnExpansion.aniadirCiudad(siguienteExpansion[i],matrizDistancias);//añadimos ciudad y le sumamos distancia
            nodoEnExpansion.eliminarCiudadRestante(siguienteExpansion[i]);//quietamos la ciudad de ciudadrestante
            
            //Comprobamos si ya hemos encontrado una solución parcial (ruta == num_ciudades)
            if ( nodoEnExpansion.esSolucion(numCiudades) ) {

                //Actualizamos la distancia entre todas las ciudades
                distanciaActual = nodoEnExpansion.calculaDistancia(matrizDistancias);

                //Si es menor que la cota global, actualiza esta
                if (distanciaActual < cotaGlobal) {

                    cotaGlobal = distanciaActual;
                    nodoSolucionGreedy = nodoEnExpansion;
                } 
                else   //en caso contrario podamos (aumentamos el numero de podas)
                    numPodas++;
            }
            //Si no hemos encontrado una solución parcial
            else {

                nodoEnExpansion.calcularCotaLocal(menorDeCadaFila); //calculamos cota local 

                //si es menor que la global expandimos
                if (nodoEnExpansion.getCotaLocal()<cotaGlobal ) {
                    colaNodosVivos.push(nodoEnExpansion );
                    nodosExpandidos++;
                }
            }
            //eliminamos la que ha sido expandida
            nodoEnExpansion.eliminarCiudad(matrizDistancias);//la ultima que se ha añadido
  
            nodoEnExpansion.aniadirCiudadRestante(siguienteExpansion[i]);//añadimos la ultma que se quito
      }
      
   }
  
  //nodoSolucionGreedy.aniadirCiudad(1,matrizDistancias);

   return nodoSolucionGreedy;
}


int main(int argc, char ** argv){
	
	 if (argc != 3) {
     cout << "Error Formato:  <archivo_ciudades.tsp> <ciudad de partida>" << endl;
     exit(1);
   }

   high_resolution_clock::time_point t1,t2;

   //declaramos un vector para almacenar las ciudades:
   vector<pair<double, double> > coordenadas= leerDatos(argv[1]);

   int numCiudades= coordenadas.size();
 
   //Creamos la matriz y la rellenamos
   vector<vector<int> > matrizDistancias(numCiudades,vector<int>(numCiudades));
   calcularMatrizDistancias(coordenadas,matrizDistancias,numCiudades);

   //Calculamos la primera cota local inferior seleccionando el menor coste de salir de cada una
   //de las ciudades
   vector<int> menorDeCadaFila = calcularMenorDeCadaFila(matrizDistancias);

   //Llamamos a nuestro algoritmo B&B para obtener la solucion

   //MEDICION DE TIEMPO: INICIO
   t1= high_resolution_clock::now();

   Solucion sol = BranchAndBound(matrizDistancias,menorDeCadaFila,numCiudades);

   t2= high_resolution_clock::now();
   //MEDICION DE TIEMPO: FIN

   duration<double> tiempoTotal = duration_cast<duration<double>>(t2 - t1);

   vector<int> ruta = sol.getSolucion();

   cout << "\nDistancia recorrida con B&B: " << sol.getDistancia() << endl;

   for (auto it=ruta.begin(); it!=ruta.end(); ++it)
   	  cout << *it << " " << coordenadas[(*it)-1].first<< " " << coordenadas[(*it)-1].second << endl;

   cout << "1 " << coordenadas[0].first<< " " << coordenadas[0].second << endl;

   cout << "\nNodos Expandidos: " << nodosExpandidos << endl;
   cout << "Numero de Podas:  " << numPodas << endl;
   cout << "Tamaño maximo de la cola de nodos vivos: " << tamColaNodosVivos << endl;
   cout << "Tiempo de ejecucion del algoritmo: " <<tiempoTotal.count() <<endl;
   
	return 0;
}
