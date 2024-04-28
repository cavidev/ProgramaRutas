#include <iostream>
#include <stdio.h>
#include <string>
#include <list>
#include <fstream>
#include <string.h>
#include <stdlib.h>
using namespace std;

list<string>unSoloCamino;
fstream archivoCaminos("ArchivoCaminos.txt",ios::in|ios::out|ios::binary);//| ios::trunc//Tiene que crearlo primero
fstream archivoConductor("ArchivoConductores.txt",ios::in | ios::out | ios::binary);//| ios::trunc
fstream archivoRutasConductor("ArchivoRutas.txt",ios::in | ios::out | ios::binary);//| ios::trunc

void menuPrincipal();
void menuCiudades();
void menuCaminos();
void menuConsultas();
void menuArchivos();

struct Ciudades *  buscar(string);
void InsertarCiudad(string);
void InsertarDatosCiudad();
void VerCiudades();
void BorrarCiudad();
void desmarcar();
void ConductorNuevo();

void InsertarCaminos(struct Ciudades, struct Ciudades, int, int,bool);
void InsertarDatosCaminos();
void VerCaminosCiudad();
void AuxVerCaminosCiudad();
void BorrarCamino(struct Ciudades,struct Ciudades);
void ModificarCamino(struct Ciudades,struct Ciudades);
void ruta(struct Ciudades,string);
int contarRutas();

bool AveriguarConexo();
void auxAveriguarConexo();
void recorrerEulen(struct Ciudades);
void CicloEulen();
void recorrerUnSoloCamino(struct Ciudades);
void RecorridoCaminos();
bool Apunta(struct Ciudades,string);
bool LoApuntan(struct Ciudades,string);
void InicioPintar();
void rutaMasCorta(struct Ciudades, string,int,list<string>);
void rutaMasCortaDatos();
void rutaMasRapida(struct Ciudades, string,int,list<string>);
void rutaMasRapidaDatos();
void CerrarRuta();
void RutasAlternas(struct Ciudades,string,list<string>);
void inicioRutasAlternas();
void ImprimirRutaConductor(int);
void ImprimirRutaConductorDatos();


void ImprimirArchivoRutas();
void imprimirArchivoConductor();
void imprimirArchivoRutasConductor();

void escribirEnArchivoCaminos(char *,char *,int,int,bool);
void cargarRutas();
void cargarCiudad();

long fin_archivo();
long buscarConductor(int id);
void guardarArchivoConductor(int,char*,char*,int);

long fin_archivoRC();
void guardarArchivoRutasConductor(struct archivoRutasC, int);

void guardarAarchivos();
/************************************* Estructuras *************************************/
/**********************************************************************************/
struct Ciudades{
    /**< Lista Simple de ciudades, representa un grafo de las ciudades */
    string nombreCiudad;
    string colorCiudad;
    bool visitado;
    struct Ciudades * sigCiudad;
    struct Caminos * subListaCaminos;
    Ciudades(string nombreLugar)
    {
        nombreCiudad = nombreLugar;
        colorCiudad = "";
        visitado = false;//false;
        sigCiudad = NULL;
        subListaCaminos = NULL;
    }
}*mapaCiudades;

struct Caminos{
    /**< Sub-Lista de los caminos, son los arcos  */
    int distanciaKm;
    int velocidad;
    bool dirigido;
    bool paso;
    bool cerrado;
    struct Ciudades * CiudadDestino;
    struct Caminos * sigCamino;
    Caminos(int distancia, int kph, bool siesdirigido, struct Ciudades * destino)
    {
        distanciaKm = distancia;
        velocidad = kph;
        dirigido = siesdirigido;
        sigCamino = NULL;
        CiudadDestino = destino;
        paso = 0;
        cerrado = 0;
    }
};
/************************************* Estructuras Archivos *************************************/
/**********************************************************************************/
struct rutasArchivo{
    char origen[20];
    char destino[20];
    int distancia;
    int velocidad;
    bool dirigido;
}registroCaminos;

struct archivoConductores{
    int id;
    char nombreChofer[15];
    char tipoVeiculo[15];
    long enlaceRutas;
    long izq;
    long der;
}registroConductores;

struct ciudad{
    char nombre[10];
};

struct archivoRutasC{
    struct ciudad lugares[10];
    long sig;
}registroRutasC;

/************************************* Código *************************************/
/**********************************************************************************/
/******************************* Código de Ciudadades *****************************/
/**********************************************************************************/
struct Ciudades * buscar(string nombreLugar)
{
    /** \brief Funcion de busqueda de ciudades
     *
     * \param nombreLugar... nombre de la ciudad que va a buscar
     * \return si encuetra la ciudad, el nodo. Sino NULL
     *
     */
    struct Ciudades * tempCiudades = mapaCiudades;
    while(tempCiudades != NULL)
    {
        if(tempCiudades->nombreCiudad == nombreLugar)
            return tempCiudades;
        tempCiudades  = tempCiudades->sigCiudad;
    }
    return NULL;
}

void InsertarCiudad(string nombreLugar)
{
    /** \brief Inserta la ciudades nuevas a la lista mapasCiudades..
     *
     * \param nombreLugar: es el nombre del nuevo lugar por crear, si ya existe no lo agregar
     * \return NULL
     *
     */
    struct Ciudades * nnC = new Ciudades(nombreLugar);
    nnC->sigCiudad = mapaCiudades;
    mapaCiudades = nnC;
}
void InsertarDatosCiudad()
{
    /** \brief Funcion que pide al usuario el nombre de la nueva ciudad y llama a la funcion para insertarla
    en la lista de ciudades..
     *
     * \param NULL
     * \return NULL
     *
     */
    string nombreLugar;
    string colorLugar;
    cout<<"\n\tDigite el nombre de la ciudad Nueva"<<endl;
    cout<<"***";
    cin>>nombreLugar;
    if(buscar(nombreLugar) != NULL){
        cout<<"\n\t\t**********La ciudad con ese nombre ya existe..**********\n"<<endl;
        return;
    }
    else
    {
        InsertarCiudad(nombreLugar);
        //cout<<"\n\t\t**********Se agrego con exito la ciuadad..**********\n"<<endl;
		return;
    }
}
void VerCiudades()
{
    /** \brief Imprime todas las ciudades que se encuentra  en el mapa con sus respectivos destinos
     *
     * \param NULL
     * \param NULL
     * \return NULL
     *
     */

    int contador = 1;
    struct Ciudades * tempCiudades = mapaCiudades;
    struct Caminos * tempCaminos;
    cout<<"\n\tLa ciudades en el mapa son:\n "<<endl;
    while(tempCiudades != NULL)
    {
        cout<<"\t  "<<contador<<" "<<tempCiudades->nombreCiudad<<endl;
        tempCaminos = tempCiudades->subListaCaminos;
        cout<<"\t\t  Sus rutas son: "<<endl;
        while(tempCaminos != NULL)
        {
            cout<<"\t\t\t  "<<tempCiudades->nombreCiudad<<" a "<<tempCaminos->CiudadDestino->nombreCiudad<<endl;
            tempCaminos = tempCaminos->sigCamino;
        }
        contador++;
        tempCiudades = tempCiudades->sigCiudad;
    }
    cout<<"\n\n";
}
void BorrarCiudad()
{
    /** \brief Borra la ciudad deseada, primero borra los caminos que vallas hacia ella y luego la borra
    de la lista de ciudades...
     *
     * \param
     * \param
     * \return
     *
     */

    string nombreCiudad;
    cout<<"Digite el nombre de la Ciudad a borrar"<<endl;
    cout<<"***";
    cin>>nombreCiudad;
    struct Ciudades * ciudadEncontrada = buscar(nombreCiudad);
    if(ciudadEncontrada == NULL)
        cout<<"Ciudad no Existe"<<endl;
    else
        {
            struct Ciudades * tempCiudades = mapaCiudades;
            struct Caminos * tempCaminoAnterior;///Guarda el camino antes de el que se va a eliminado
            ///Borra las rutas que van hacia la ciudad que se quiere borrar...
            while(tempCiudades != NULL)
            {
            	if (tempCiudades->subListaCaminos == NULL){
            		continue;
				}
                if(tempCiudades->subListaCaminos->CiudadDestino->nombreCiudad == nombreCiudad)
                {
                    tempCaminoAnterior = tempCiudades->subListaCaminos->sigCamino;
                    delete tempCiudades->subListaCaminos;
                    tempCiudades->subListaCaminos = tempCaminoAnterior;
                }
                else
                {
                    struct Caminos * tempCamino = tempCiudades->subListaCaminos;
                    tempCaminoAnterior = tempCamino;
                    while(tempCamino != NULL)
                    {
                        if(tempCamino->CiudadDestino->nombreCiudad == nombreCiudad)
                        {
                            tempCaminoAnterior->sigCamino = tempCamino->sigCamino;
                            delete tempCamino;

                        }
                        tempCaminoAnterior = tempCamino;
                        tempCamino = tempCamino->sigCamino;
                    }
                }
                tempCiudades = tempCiudades->sigCiudad;
            }
            ///Borra la ciudad de la lista Ciudades
            tempCiudades = mapaCiudades;
            struct Ciudades * ciudadAnterior;

            if(mapaCiudades->nombreCiudad == nombreCiudad)
            {
                ciudadAnterior = mapaCiudades->sigCiudad;
                delete mapaCiudades;
                mapaCiudades = ciudadAnterior;
            }
            else
            {
                ciudadAnterior = mapaCiudades;
                while(tempCiudades != NULL)
                {
                    if(tempCiudades->nombreCiudad == nombreCiudad)
                    {
                        if(tempCiudades->sigCiudad == NULL)
                        {
                           ciudadAnterior->sigCiudad = NULL;
                           delete tempCiudades;
                        }
                        ciudadAnterior->sigCiudad = tempCiudades->sigCiudad;
                        delete tempCiudades;
                    }
                    ciudadAnterior = tempCiudades;
                    tempCiudades = tempCiudades->sigCiudad;
                }

            }
        }
}
void desmarcar()
{
    /** \brief La función recorre todo el grafo de Ciudades y cada uno de sus caminos y desmarcar todo
    dejendolo en false o 0 para poder reiniciar los recorridos.
     *
     * \param
     * \param
     * \return
     *
     */

    struct Ciudades * tempCiudades = mapaCiudades;
    struct Caminos * tempCaminos;
    while(tempCiudades != NULL)
    {
        tempCaminos = tempCiudades->subListaCaminos;
        while(tempCaminos != NULL)
        {
            tempCaminos->paso = 0;
            tempCaminos = tempCaminos->sigCamino;
        }
        tempCiudades->visitado = false;
        tempCiudades = tempCiudades->sigCiudad;
    }
}
void ConductorNuevo()
{
    /** \brief Funcion de que Inserta un Nuevo conductor en el archivo de Conductor... Lo inserta
    con el enlace -1, despues se le agregan las rutas..
     *
     * \param
     * \param
     * \return
     *
     */

    int id;
    char vehiculo[15];
    char nombre[15];
    cout<<"\n***Digite el nombre del nuevo conductor\n***";
    cin>>nombre;
    cout<<"\n***Digite el id del nuevo conductor\n***";
    cin>>id;
    cout<<"\n***Digite el tipo de vehiculo\n***";
    cin>>vehiculo;
    int aparece = buscarConductor(id);
    if(aparece == -1)
    {
        guardarArchivoConductor(id,nombre,vehiculo,-1);
        cout<<"\n\t****Se agrego con EXITO el conductor****\n"<<endl;
    }

    else{cout<<"\n\t****El id no sirve, ya aparece un conductor con él****\n";}
}
void AgregarRutaNuevaConductor()
{
    /** \brief Inicializa la estructura de Caminos para el archivo recoje todas las rutas con un maximo de 10 por recorrido..
     *
     * \param
     * \param
     * \return
     *
     */

    int cantidad;
    cout<<"\t\t***SOLO SE PUEDE AGREGAR UN MAXIMO DE 10 CIUDADES***"<<endl;
    cout<<"\n***Digite la Cantidad de Rutas a Agregar\n***";
    cin>>cantidad;

    struct archivoRutasC reg2;
    for(int i = 0; i< 10; i++)
		strcpy(reg2.lugares[i].nombre,"");

	char nombreCiudad[15];

    if(cantidad > 10){cout<<"\t\n*** Son demaciadas Rutas ***\n"<<endl;return;}
	for(int i = 0; i< cantidad; i++)
    {
        cout<<"Digite la "<<i+1<<" ciudad\n***";
        cin>>nombreCiudad;
        strcpy(reg2.lugares[i].nombre,nombreCiudad);
    }
    int id;
    cout<<"\n***Digite el id del Conductor\n***";
    cin>>id;


	int aparece = buscarConductor(id);
	if(aparece != -1)
    {
        reg2.sig = -1;
        guardarArchivoRutasConductor(reg2,id);
        cout<<"\n\t****Se agrego con EXITO el conductor****\n"<<endl;
    }

    else{cout<<"\n\t****El Conductor no EXISTE****\n";}

}
/********************************** Código de Caminos *****************************/
/**********************************************************************************/
void InsertarCaminos(struct Ciudades * cOrigen, struct Ciudades * cDestino, int distancia, int velocidad,bool dirigido)
{
    /**\brief Inserta los nuevos caminos de una ciudad a otra.. Si no se encuentra, no agregara nada
     *
     * \param cudadOrigen: el nombre de la ciudad de Origen;
            ciudadDestino: el nombre de la ciudad hacia donde el camino va;
            distancia: kilometros que posee esa carretera
            velociadad: la velocidad a la que puede ir un automovil.
     * \return NULL
     *
     */
    if(cOrigen == NULL || cDestino == NULL){
        cout<<"\n\t\t**********Datos Inexistentes..**********"<<endl;
        return;
    }
    else{
	    struct Caminos * nnCamino = new Caminos(distancia,velocidad,dirigido,cDestino);
	    nnCamino->sigCamino = cOrigen->subListaCaminos;
	    cOrigen->subListaCaminos = nnCamino;
	    //cout<<"\n\t****Se inserto con exito****"<<endl;
	}
}

void InsertarDatosCaminos()
{
    /** \brief Inserta los datos de los caminos, si el camino origen tiene el mismo color que el camino destino no se
    inserta.....
     *
     * \param
     * \param
     * \return
     *
     */

    char ciudadDestino[20];
    char ciudadOrigen[20];
    int distancia;
    int velocidad;
    string dirigido;
    cout<<"Digite la ciudad Origen\n***";
    cin>>ciudadOrigen;
    cout<<"Digite la ciudad Destino\n***";
    cin>>ciudadDestino;
    struct Ciudades * COrigen = buscar(ciudadOrigen);
    struct Ciudades * CDestino = buscar(ciudadDestino);
    cout<<"Digite la velocidad en k/h permitida\n***";
    cin>>velocidad;
    cout<<"Digite la distancia\n***";
    cin>>distancia;
    cout<<"Digite si el camino es Ida y Vuelta\n(si/no)\n***";
    cin>>dirigido;

    if(dirigido == "si")
        {
            InsertarCaminos(CDestino,COrigen,distancia,velocidad, 1);
            InsertarCaminos(COrigen,CDestino,distancia,velocidad,1);
            escribirEnArchivoCaminos(ciudadOrigen,ciudadDestino,distancia,velocidad,1);
            escribirEnArchivoCaminos(ciudadDestino,ciudadOrigen,distancia,velocidad,1);
        }
    else
        {
            InsertarCaminos(COrigen,CDestino,distancia,velocidad,0);
            escribirEnArchivoCaminos(ciudadOrigen,ciudadDestino,distancia,velocidad,0);
        }
}
void VerCaminosCiudad(struct Ciudades * inicio)
{
    /** \brief Imprime los caminos de una ciudad ingresada por el usuario, hacias los destinos que este tenga...
     *
     * \param estructura del camino buscado
     * \param
     * \return
     *
     */

    if(inicio == NULL)
        return;
    struct Caminos * tempCaminos= inicio->subListaCaminos;
    if (tempCaminos == NULL){
    	cout<<"Esta ciudad no posee caminos"<<endl;
    	return;
	}
    while(tempCaminos != NULL)
    {
        cout<<"\t\t"<<inicio->nombreCiudad<<" a "<<tempCaminos->CiudadDestino->nombreCiudad<<endl;
        tempCaminos = tempCaminos->sigCamino;
    }
    cout<<"\n\n";
}
void AuxVerCaminosCiudad()
{
    /** \brief Es la funcion que ayuda a la busquedad de un camino y sus rutas
     *
     * \param
     * \param
     * \return
     *
     */

    string nombreCiudad;
    cout<<"Digite el nombre de la Ciudad"<<endl;
    cin>>nombreCiudad;
    struct Ciudades * temp = buscar(nombreCiudad);
    if(temp == NULL)
        cout<<"Ciudad no Existe"<<endl;
    else{VerCaminosCiudad(temp);}
}

void BorrarCamino(struct Ciudades * tempO,struct Ciudades * tempD){
    /** \brief Borra un camino entre dos ciudades. Las ciudades se reciben en una
    funcion aparte para la recepcion de datos y utilizan la funcion buscar
     *
     * \param tempO= Origen,,, tempD = Destino.
     * \param
     * \return
     *
     */

	if (tempO == NULL or tempD == NULL){
		cout<<"Datos Incorrectos"<<endl;
		return;
	}
	else{
		if (tempO->subListaCaminos == NULL){
				cout<<"La Ciudad origen no tiene caminos...";
				return;
			}
		else if(tempO->subListaCaminos->CiudadDestino->nombreCiudad == tempD->nombreCiudad){
			struct Caminos * tempCamino = tempO->subListaCaminos;
			tempO->subListaCaminos = tempO->subListaCaminos->sigCamino;
			tempCamino->sigCamino = NULL;
			delete tempCamino;

			}
		else{
		struct Caminos * tempCamino = tempO-> subListaCaminos;
		struct Caminos * tempAnterior = tempO-> subListaCaminos;
		while (tempCamino != NULL){
			if (tempCamino->	CiudadDestino->nombreCiudad == tempD->nombreCiudad){
				tempAnterior->sigCamino = tempCamino->sigCamino;
				tempCamino->sigCamino = NULL;
				delete tempCamino;
				}
			tempAnterior = tempCamino;
			tempCamino = tempCamino->sigCamino;
			}

		}
	}
}

void ModificarCamino(struct Ciudades * tempO,struct Ciudades * tempD){
    /** \brief Modifica los caminos de una ciudad recoje los nuevos datos,
     *
     * \param
     * \param
     * \return
     *
     */

if (tempO == NULL or tempD == NULL){
		cout<<"Datos Incorrectos"<<endl;
		return;
	}
	else{
		if (tempO->subListaCaminos == NULL){
				cout<<"La Ciudad origen no tiene caminos...";
				return;
			}
		else{
			struct Caminos * tempCamino = tempO-> subListaCaminos;
			while (tempCamino!= NULL){
				if(tempCamino->CiudadDestino->nombreCiudad == tempD->nombreCiudad){
					int nVelocidad;
					int nDistancia;
					cout<<"Digite la nueva distancia a asignar"<<endl;
					cin>>nDistancia;
					cout<<"Digite la nueva velocidad en k/h a asignar"<<endl;
					cin>>nVelocidad;
					tempCamino->distanciaKm = nDistancia;
					tempCamino->velocidad = nVelocidad;
					cout<<"**** Modificado con exito ****"<<endl;
					return;
				}
				tempCamino = tempCamino->sigCamino;
			}
		}
	}
}

bool siHayRuta = false;

void ruta(struct Ciudades * inicio,string destino)
{
    /** \brief Encuentra una ruta indicada,,,, Es recursivo
     *
     * \param
     * \param
     * \return
     *
     */

    if((inicio == NULL) or (inicio->visitado == true))
        return;

    if(destino == inicio->nombreCiudad)
    {
        siHayRuta = true;
        return;
    }
    inicio->visitado = true;
    struct Caminos * tempCaminos = inicio->subListaCaminos;
    while(tempCaminos != NULL)
    {
        ruta(tempCaminos->CiudadDestino, destino);
        tempCaminos = tempCaminos->sigCamino;
    }
}

int contarRutas()
{
    /** \brief Si el camino es dirigido lo cuenta una ves nada mas
     *
     * \param
     * \param
     * \return El contador
     *
     */

    int cont = 0;
    struct Ciudades * tempCiudades = mapaCiudades;
    struct Caminos * tempCaminos;
    while(tempCiudades != NULL )
    {
        tempCaminos = tempCiudades->subListaCaminos;
        while(tempCaminos != NULL)
        {
            if(tempCaminos->dirigido == 1)
                cont--;
            cont++;
            tempCaminos = tempCaminos->sigCamino;
        }
        tempCiudades = tempCiudades->sigCiudad;
    }
	return cont;
}

/********************************** Código de Consultas *****************************/
/**********************************************************************************/
bool AveriguarConexo(){
/** \brief Consulta Si cada uno posee un camino hacia los otros...
 *
 * \param
 * \param
 * \return 1 si es true o 0 si es false
 *
 */

    struct Ciudades * tempCiudadOrigen = mapaCiudades;
    struct Ciudades * tempCiudadDestino = mapaCiudades;

    while(tempCiudadOrigen != NULL)
    {
        while(tempCiudadDestino != NULL)
        {
            if(tempCiudadOrigen->nombreCiudad == tempCiudadDestino->nombreCiudad)
            {
                tempCiudadDestino = tempCiudadDestino->sigCiudad;
                continue;
            }
            else
            {
                desmarcar();
                ruta(tempCiudadOrigen,tempCiudadDestino->nombreCiudad);
                if(siHayRuta == false)
                {
                    return 0;
                }
                siHayRuta = false;
            }

            tempCiudadDestino = tempCiudadDestino->sigCiudad;
        }
        tempCiudadDestino = mapaCiudades;
        tempCiudadOrigen = tempCiudadOrigen->sigCiudad;
    }
    return 1;
}

void auxAveriguarConexo(){

    cout<<"\n\t****Espere un momento****"<<endl;
    bool averigua = AveriguarConexo();

    if(averigua == 1)
    {
        cout<<"\n\t\t*****Esta Ciudad es Conexa*****\n"<<endl;
    }
    else
    {
        cout<<"\n\t\t*****Esta Ciudad NO es Conexa*****\n"<<endl;
    }
}
int contidadCiudades()
{
    int cantidad = 0;
    struct Ciudades * tempCiudad = mapaCiudades;
    while(tempCiudad != NULL)
    {
        cantidad++;
        tempCiudad = tempCiudad->sigCiudad;
    }
    return cantidad;
}
list<string> unSoloVertice;
int sostine = 0;
void recorrerEulen(struct Ciudades * ciudad, string destino,list<string> local,int contCiudad)
{
    /** \brief Es el recorrido de la consulta 2, mete las ciudades en la lista que viene como parametro
    solo retorna cuando la cantidad de ciudades visitadas es mayor a 1 que las que hay en el grafo
     *
     * \param
     * \param
     * \return
     *
     */

    local.push_front(ciudad->nombreCiudad);
    contCiudad++;
    if((ciudad->nombreCiudad == destino) and (sostine != 0))
    {
        sostine++;
        int verificar = contidadCiudades();
        ciudad->visitado=false;
        if(contCiudad == (verificar+1))
        {
            unSoloVertice = local;
            return;

        }
        return;
    }
    if((ciudad == NULL) or (ciudad->visitado == 1))
        return;

    sostine++;
    ciudad->visitado = true;
    struct Caminos * camino = ciudad->subListaCaminos;
    while(camino != NULL)
    {
        recorrerEulen(camino->CiudadDestino,destino,local,contCiudad);
        camino = camino->sigCamino;
    }
    ciudad->visitado = false;
}
void CicloEulen()
{
    int cont = contarRutas();
    bool conexa = AveriguarConexo();
    cout<<cont<<endl;
    if(conexa == 1 and cont % 2 == 0)
    {
        desmarcar();
        unSoloVertice.clear();
        list<string>local;
        recorrerEulen(mapaCiudades,mapaCiudades->nombreCiudad,local,0);
        if(unSoloVertice.empty())
        {
            cout<<"\n\t****NO se encontro el camino1****\n"<<endl;
        }
        else
        {
            while(unSoloVertice.size() != 0)
            {
                cout<<"\t"<<unSoloVertice.back()<<endl;
                unSoloVertice.pop_back();
            }
            cout<<"\n"<<endl;
        }
    }
    else
    {
        if(cont%2 !=0)
        {
            cout<<"\n\t***No se puede hacer este recorrido, la ciudad no tiene rutas pares***\n"<<endl;
        }
        else
        {
            cout<<"\n\t***No se puede hacer este recorrido, la ciudad no es Conexa***\n"<<endl;
        }
    }
}
void recorrerUnSoloCamino(struct Ciudades * ciudadActual)
{
    /** \brief Recorre el grafo buscando solo una camino disponible sin tocar los otros que ya a recorrido.
     *
     * \param
     * \param
     * \return
     *
     */

    if((ciudadActual == NULL))
		return;
	struct Caminos * tempCaminos = ciudadActual->subListaCaminos;
	while ( tempCaminos != NULL)
    {
	    if(tempCaminos->paso == 1)
            {tempCaminos = tempCaminos->sigCamino;continue;}
	    else
        {
            tempCaminos->paso = 1;
            unSoloCamino.push_front(ciudadActual->nombreCiudad + " a "+tempCaminos->CiudadDestino->nombreCiudad);
            recorrerUnSoloCamino(tempCaminos->CiudadDestino);
            tempCaminos = tempCaminos->sigCamino;
        }
    }

}
void RecorridoCaminos()
{
    desmarcar();
    bool conexa = AveriguarConexo();
    if(conexa == 1)
    {
        unSoloCamino.clear();
        recorrerUnSoloCamino(mapaCiudades);
        while(unSoloCamino.size() != 0)
        {
            cout<<"\t"<<unSoloCamino.back()<<endl;
            unSoloCamino.pop_back();
        }
        cout<<"\n"<<endl;
    }
    else{cout<<"\n\t***No se puede recorrer toda la ciudad\n\t   tocando un solo camino ya que no es conexa***\n"<<endl;}

}
bool Apunta(struct Ciudades * Ciudad,string color)
{
    /** \brief Busca si el apunta a una ciudad con el color que se le mando,
     *
     * \param
     * \param
     * \return
     *
     */

    struct Caminos * camino = Ciudad->subListaCaminos;
    while(camino != NULL)
    {
        if(camino->CiudadDestino->colorCiudad == color)
        {
            return 1;
        }

        camino = camino->sigCamino;
    }
    return 0;
}
bool LoApuntan(struct Ciudades * nombreCiudad,string color)
{
    /** \brief Buscar si alguna ciudad lo apunta a el con el color que se le mando.
     *
     * \param
     * \param
     * \return
     *
     */

    struct Ciudades * ciudadPintar = mapaCiudades;
    struct Caminos * camino;
    while(ciudadPintar != NULL)
    {
        camino = ciudadPintar->subListaCaminos;
        while(camino != NULL)
        {
            if(camino->CiudadDestino->nombreCiudad == nombreCiudad->nombreCiudad)
            {
                if(ciudadPintar->colorCiudad == color)
                {
                    return 1;
                }
            }
            camino = camino->sigCamino;
        }
        ciudadPintar = ciudadPintar->sigCiudad;
    }
    return 0;
}
void InicioPintar()
{
    string verde = "Verde";
    string rojo = "Rojo";
    string amarillo = "Amarillo";
    struct Ciudades * ciudadAPintar = mapaCiudades;
    while(ciudadAPintar != NULL)
    {
        int conVerdeApunta = Apunta(ciudadAPintar,verde);
        int conVerdeApuntaLoApuntan = LoApuntan(ciudadAPintar,verde);

        int conRojoApunta = Apunta(ciudadAPintar,rojo);
        int conRojoLoApuntan = LoApuntan(ciudadAPintar,rojo);

        int conAmarilloApunta = Apunta(ciudadAPintar,amarillo);
        int conAmarilloApuntaLoApuntan = LoApuntan(ciudadAPintar,amarillo);

        if((conAmarilloApunta == 0) and (conAmarilloApuntaLoApuntan == 0))
        {
            ciudadAPintar->colorCiudad = amarillo;
        }

        else if((conVerdeApunta == 0) and (conVerdeApuntaLoApuntan == 0))
        {
            ciudadAPintar->colorCiudad = verde;
        }
        else if((conRojoApunta == 0) and (conRojoLoApuntan == 0))
        {
            ciudadAPintar->colorCiudad = rojo;
        }
        else{ciudadAPintar->colorCiudad = "Azul";}

        ciudadAPintar = ciudadAPintar->sigCiudad;
    }
    struct Ciudades * ciudad = mapaCiudades;
    while(ciudad != NULL)
    {
        cout<<"\n\t"<<ciudad->nombreCiudad <<" su color "<<ciudad->colorCiudad<<endl;

        ciudad = ciudad->sigCiudad;
    }
    cout<<"\n"<<endl;
}

int mayor = 1000000;
list<string>rutaCortaImp;

void rutaMasCorta(struct Ciudades * ciudad, string destino,int contLocal,list<string> local)/**<Recorrido Profundidad \param Estructura de tipo Vertice */
{
    /** \brief Encuentra la ruta mas corta de una ciudad, toma en cuenta las distancias..
     *
     * \param
     * \param
     * \return
     *
     */

    local.push_front(ciudad->nombreCiudad);
    if((ciudad == NULL) or (ciudad->visitado == true)or (contLocal>mayor))
        return;
    if(ciudad->nombreCiudad == destino)
    {
        if(contLocal < mayor)
        {
            mayor = contLocal;
            rutaCortaImp.clear();
            rutaCortaImp = local;
        }
        return;
    }
    ciudad->visitado = true;
    struct Caminos * camino = ciudad->subListaCaminos;
    while(camino != NULL)
    {
        rutaMasCorta(camino->CiudadDestino,destino,contLocal + camino->distanciaKm,local);
        camino = camino->sigCamino;

    }
    ciudad->visitado = false;
}

void rutaMasCortaDatos()
{
    string origen;
    string destino;
    list<string>rutaCorta;
    cout<<"\n\tDigite la ciudad Origen"<<endl;
    cin>>origen;
    cout<<"\n\tDigite la ciudad Destino"<<endl;
    cin>>destino;
    mayor = 100000000;
    if(buscar(origen) == NULL || buscar(destino) == NULL){cout<<"\n\t****Lugar no valido en este mapa****\n"<<endl;return;}
    rutaMasCorta(buscar(origen),destino,0,rutaCorta);
    if(rutaCortaImp.empty()){cout<<"\n\t****NO existe una ruta hacia ese lugar****\n"<<endl;}
    else
    {
        cout<<"\n"<<endl;
        while(rutaCortaImp.size() != 0)
        {
            cout<<"\t"<<rutaCortaImp.back()<<endl;
            rutaCortaImp.pop_back();
        }
        cout<<"\n"<<endl;
    }
}

list<string>rutaRapidaImp;
float tiempoGeneral = 100000.0;

void rutaMasRapida(struct Ciudades * inicio, string destino,list<string> local,float tiempo)
{
    /** \brief Encuentra la ruta mas rapida, sumando las distancias y dividiendo entre las suma de velocidades
     *
     * \param
     * \param
     * \return
     *
     */

    if((inicio == NULL) or (inicio->visitado == true))
        return;
    local.push_front(inicio->nombreCiudad);//
    if(inicio->nombreCiudad == destino)
    {
        if(tiempo < tiempoGeneral)
            {rutaRapidaImp.clear();rutaRapidaImp = local;
            tiempoGeneral = tiempo;
            }
        return;
    }
    inicio->visitado = true;
    struct Caminos * tempA = inicio->subListaCaminos;
    float tiempo1 = float(tempA->distanciaKm)/float(tempA->velocidad);
    while(tempA != NULL)
    {
        rutaMasRapida(tempA->CiudadDestino,destino,local,tiempo+tiempo1);
        tempA = tempA->sigCamino;

    }
    inicio->visitado = false;
}
void rutaMasRapidaDatos()
{
    string origen;
    string destino;
    list<string>rutaRapida;
    cout<<"\n\tDigite la ciudad Origen"<<endl;
    cin>>origen;
    cout<<"\n\tDigite la ciudad Destino"<<endl;
    cin>>destino;
    desmarcar();
    tiempoGeneral = 100000.0;
    rutaMasRapida(buscar(origen),destino,rutaRapida,0.0);

    if(100000.0 == tiempoGeneral)
    {
        cout<<"NO se pudo Calcular"<<endl;
        return;
    }

    while(rutaRapidaImp.size() != 0)
    {
        cout<<"\t"<<rutaRapidaImp.back()<<endl;
        rutaRapidaImp.pop_back();
    }
    cout<<"\n***El tiempo que dura es: ";
    printf("%.2f", tiempoGeneral);
    cout<<" minutos\n"<<endl;
}
void CerrarRuta()
{
    string origen;
    string destino;
    cout<<"Digite el nombre del Origen"<<endl;
    cin>>origen;
    cout<<"Digite la ciudad destino a cerrar"<<endl;
    cin>>destino;
    struct Ciudades * ciudad = buscar(origen);
    struct Caminos * camino = ciudad->subListaCaminos;
    while(camino != NULL)
    {
        if(camino->CiudadDestino->nombreCiudad == destino)
        {
            camino->cerrado = 1;
        }
        camino = camino->sigCamino;
    }
}
void imprimir(list<string> imprimir)
{
    while(imprimir.size() != 0)
    {
        cout<<"\t"<<imprimir.back()<<endl;
        imprimir.pop_back();
    }
    cout<<"\n"<<endl;
}

void RutasAlternas(struct Ciudades * inicio,string destino,list<string> rutas)
{
/** \brief No toma en cuenta el camino cerrado previante antes, se manda por las alternas.
 *
 * \param
 * \param
 * \return
 *
 */

    if((inicio == NULL) or (inicio->visitado == true))
        return;
    rutas.push_front(inicio->nombreCiudad);
    if(destino == inicio->nombreCiudad)
    {
        imprimir(rutas);
        return;
    }
    inicio->visitado = true;
    struct Caminos * tempA = inicio->subListaCaminos;
    while(tempA != NULL)
    {
        if(tempA->cerrado == 1)
        {
            tempA = tempA->sigCamino;
            continue;
        }
        RutasAlternas(tempA->CiudadDestino, destino,rutas);
        inicio->visitado = false;
        tempA = tempA->sigCamino;
    }
    //inicio->visitado = false;
}
void UsuariosConDestinosX(string ciudadDestino){
/** \brief Busca en el archivo de rutas los Usuario que viajan hacia ese destino.
 *
 * \param
 * \param
 * \return
 *
 */

	struct archivoConductores regTemp;

	int cantidadUsuarios= 0;
    fstream archivoRutasTemp("ArchivoConductores.txt",ios::in | ios::out | ios::binary );

    archivoRutasTemp.seekg(0, ios::beg);
    archivoRutasTemp.read(reinterpret_cast<char *>(&regTemp),sizeof(regTemp));

 	int numeroRegistroRuta = regTemp.enlaceRutas;

    while(!archivoRutasTemp.eof()){
    	if(regTemp.enlaceRutas != -1){

		numeroRegistroRuta= regTemp.enlaceRutas;
		struct archivoRutasC regTemp2;

	    fstream archivoRutasTemp("ArchivoRutas.txt",ios::in | ios::out | ios::binary );

	    bool fin= false;
	    while(fin != true){

	    	archivoRutasTemp.seekg(numeroRegistroRuta*sizeof(regTemp2), ios::beg);
	    	archivoRutasTemp.read(reinterpret_cast<char *>(&regTemp2),sizeof(regTemp2));
	    	if(regTemp2.sig == -1){
	    		fin= true;
	        	for(int i = 0; i< 10; i++){
		        	string nombre = regTemp2.lugares[i].nombre;
		            if(strcmp(regTemp2.lugares[i].nombre,"")==0){
		            	break;
					}
					if(nombre == ciudadDestino and strcmp(regTemp2.lugares[i+1].nombre,"")==0)	{
						cantidadUsuarios++;
					}
		        }
			}
			else{
		        numeroRegistroRuta = regTemp2.sig;
				}
			}
		}
        archivoRutasTemp.read(reinterpret_cast<char *>(&regTemp),sizeof(regTemp));

    }
	cout<<"\n\t***Conductores hacia esa Ciudad: "<<cantidadUsuarios<<"\n"<<endl;
}
void InicioUsuariosConDestinosX(){
	string ciudadN;
	cout<<"\n***Digite la ciudad destino\n***: ";
	cin>>ciudadN;
	UsuariosConDestinosX(ciudadN);
}
void inicioRutasAlternas()
{
    string origen;
    string destino;
    cout<<"\n***Digite el nombre del Origen\n***";
    cin>>origen;
    cout<<"\n***Digite la ciudad Destino\n***";
    cin>>destino;
    list<string> empezar;
    cout<<"\n"<<endl;
    RutasAlternas(buscar(origen),destino,empezar);
}
void ImprimirRutaConductor(int id)
{
    /** \brief realiza la impresion del archivo Conductor...
     *
     * \param
     * \param
     * \return
     *
     */

    fstream archivoConductorTemp("ArchivoConductores.txt",ios::in | ios::out | ios::binary);

    long posicionC = buscarConductor(id);

    struct archivoConductores tempConductor;

    archivoConductorTemp.seekg(posicionC * sizeof(tempConductor));
    archivoConductorTemp.read(reinterpret_cast<char *>(&tempConductor),sizeof(tempConductor));

     if(tempConductor.enlaceRutas == -1)
    {
        cout<<"\n\t****El Conductor no posee rutas Asignadas****\n"<<endl;
        return;
    }
    else
    {
        cout<<"\n\tLas rutas del conductor "<<tempConductor.nombreChofer<<" son: \n"<<endl;
        int enlace = tempConductor.enlaceRutas;

        fstream archivoRutasConductorTemp("ArchivoRutas.txt",ios::in | ios::out | ios::binary);

        struct archivoRutasC tempArchivo;

        archivoRutasConductorTemp.seekg(enlace * sizeof(tempArchivo));
        archivoRutasConductorTemp.read(reinterpret_cast<char *>(&tempArchivo),sizeof(tempArchivo));

        int cont = 1;

        while(!archivoRutasConductorTemp.eof())
        {
            cout<<"\nRuta: "<<cont<<endl;
            for(int i = 0; i< 10; i++)
            {
                if(strcmp(tempArchivo.lugares[i].nombre,"") == 0)
                    continue;
                cout<<tempArchivo.lugares[i].nombre<<" ";
            }
            cout<<"\n"<<endl;
            if(tempArchivo.sig == -1)
            {
                cout<<"\n\t****No posee mas rutas****"<<endl;
                break;
            }
            cont++;
            archivoRutasConductorTemp.seekg(tempArchivo.sig * sizeof(tempArchivo));
            archivoRutasConductorTemp.read(reinterpret_cast<char *>(&tempArchivo),sizeof(tempArchivo));
        }
    }
}
void ImprimirRutaConductorDatos()
{
    int idConductor;
    cout<<"***Digite el ID del Conductor\n***";
    cin>>idConductor;
    ImprimirRutaConductor(idConductor);
    cout<<"\n"<<endl;

}
/********************************* Reportes **************************/
/**********************************************************************************/
void ImprimirArchivoRutas()
{
    struct rutasArchivo regTempCaminos;

	fstream archivoRutasTemp("ArchivoCaminos.txt",ios::in | ios::out | ios::binary);

	archivoRutasTemp.seekg(0, ios::beg);
	archivoRutasTemp.read(reinterpret_cast<char *>(&regTempCaminos),sizeof(regTempCaminos));

	cout<<"\n\nOrigen         "<<"Destino        "<<"Distancia "<<"Velocidad "<<"Dirigido\n"<<endl;

	while(!archivoRutasTemp.eof())
    {
        printf("%-15s%-19s%-10i%-8i%-3i\n",regTempCaminos.origen,regTempCaminos.destino,regTempCaminos.distancia,regTempCaminos.velocidad,regTempCaminos.dirigido);
		archivoRutasTemp.read(reinterpret_cast<char *>(&regTempCaminos),sizeof(regTempCaminos));
	}
	cout<<"\n\n";

}

void imprimirArchivoConductor()
{

	struct archivoConductores regTemp;

	fstream archivoConductorTemp("ArchivoConductores.txt",ios::in | ios::out | ios::binary );

	archivoConductorTemp.seekg(0);
	archivoConductorTemp.read(reinterpret_cast<char *>(&regTemp),sizeof(regTemp));
    cout<<"\nizq  "<<"ID "<<" Nombre Chofer "<<" Tipo de vehiculo "<<" Enlace "<<" der "<<endl;
	while(!archivoConductorTemp.eof())
        {
            printf("%-5i%-9i%-15s%-15s%-7i%-10i\n",regTemp.izq,regTemp.id,regTemp.nombreChofer,regTemp.tipoVeiculo,regTemp.enlaceRutas,regTemp.der);
            archivoConductorTemp.read(reinterpret_cast<char *>(&regTemp),sizeof(regTemp));
		}
    cout<<"\n\n";


}
void imprimirArchivoRutasConductor(){

	struct archivoRutasC regTemp;

	fstream archivoRutasTemp("ArchivoRutas.txt",ios::in | ios::out | ios::binary);

	archivoRutasTemp.seekg(0,ios::beg);//,
	archivoRutasTemp.read(reinterpret_cast<char *>(&regTemp),sizeof(regTemp));

	while(!archivoRutasTemp.eof())
    {
		cout<<"Ruta: ";
		for(int i = 0; i< 10; i++)
        {
			if(strcmp(regTemp.lugares[i].nombre,"") == 0)
				continue;
			cout<<regTemp.lugares[i].nombre<<" ";

		}
		cout<<regTemp.sig<<"\n"<<endl;
		archivoRutasTemp.read(reinterpret_cast<char *>(&regTemp),sizeof(regTemp));
	}
}
/********************************* Codigo sobre archivos **************************/
/**********************************************************************************/

void escribirEnArchivoCaminos(char origen[20],char destino[20],int distancia1,int velocidad1,bool dirigido1)
{
    /** \brief Escribe un registro nuevo en el archivo de rutas..
     *
     * \param
     * \param
     * \return
     *
     */

    struct rutasArchivo registroCam;

    strcpy(registroCam.origen,origen);
    strcpy(registroCam.destino,destino);
    registroCam.distancia = distancia1;
    registroCam.velocidad = velocidad1;
    registroCam.dirigido = dirigido1;

    archivoCaminos.seekg(0, ios::end);
    archivoCaminos.write(reinterpret_cast<char*>(&registroCam),sizeof(registroCam));
}
void cargarRutas()
{
    struct rutasArchivo regTempCaminos;

	fstream archivoRutasTemp("ArchivoCaminos.txt",ios::in | ios::out | ios::binary);

	archivoRutasTemp.seekg(0, ios::beg);
	archivoRutasTemp.read(reinterpret_cast<char *>(&regTempCaminos),sizeof(regTempCaminos));

	while(!archivoRutasTemp.eof()){

		InsertarCaminos(buscar(regTempCaminos.origen),buscar(regTempCaminos.destino),regTempCaminos.distancia,
                  regTempCaminos.velocidad,regTempCaminos.dirigido);

		archivoRutasTemp.read(reinterpret_cast<char *>(&regTempCaminos),sizeof(regTempCaminos));
	}
}
void cargarCiudad()
{
    struct rutasArchivo regTempCaminos;

	fstream archivoRutasTemp("ArchivoCaminos.txt",ios::in | ios::out | ios::binary);

	archivoRutasTemp.seekg(0, ios::beg);
	archivoRutasTemp.read(reinterpret_cast<char *>(&regTempCaminos),sizeof(regTempCaminos));

	while(!archivoRutasTemp.eof()){
        struct Ciudades * encontrada = buscar(regTempCaminos.origen);
        if(encontrada == NULL)
        {
            InsertarCiudad(regTempCaminos.origen);
        }

		archivoRutasTemp.read(reinterpret_cast<char *>(&regTempCaminos),sizeof(regTempCaminos));
	}

    struct rutasArchivo regTempCaminos2;

    fstream archivoRutasTemp2("ArchivoCaminos.txt",ios::in | ios::out | ios::binary );

	archivoRutasTemp2.seekg(0, ios::beg);
	archivoRutasTemp2.read(reinterpret_cast<char *>(&regTempCaminos2),sizeof(regTempCaminos2));

	while(!archivoRutasTemp2.eof()){
        struct Ciudades * encontrada = buscar(regTempCaminos2.origen);
        if(encontrada == NULL)
        {
            InsertarCiudad(regTempCaminos2.destino);
        }

		archivoRutasTemp2.read(reinterpret_cast<char *>(&regTempCaminos2),sizeof(regTempCaminos2));
	}

}

long fin_archivo()
{
    long n;
    archivoConductor.seekg(0,ios::end);
    n = archivoConductor.tellg();
    n = (n/sizeof(registroConductores))-1;

    return n;
}
long buscarConductor(int id){

    struct archivoConductores temp;

    fstream archivoTemp("ArchivoConductores.txt",ios::in | ios::out | ios::binary);

    int indice = 0;

    while(true)
    {
        archivoTemp.seekg(indice * sizeof(temp));
        archivoTemp.read(reinterpret_cast<char *>(&temp),sizeof(temp));

        if(temp.id<id)
        {
            if(temp.id == id)
            {
                return indice;
            }
            else if(temp.der != -1)
            {
                indice = temp.der;
            }
            else
            {
                break;
            }
        }
        else
        {
            if(temp.id == id)
            {
                return indice;
            }
            else if(temp.izq != -1)
            {
                indice = temp.izq;
            }
            else
            {
                break;
            }
        }

    }return -1;
}
void guardarArchivoConductor(int id,char nombre[15],char vehiculo[15],int enlaceR){
    int indice = 0;

    if(!archivoConductor)
        cout<<"Error abrir archivo"<<endl;
    registroConductores.id = id;
    registroConductores.enlaceRutas = enlaceR;
    strcpy(registroConductores.nombreChofer,nombre);
    strcpy(registroConductores.tipoVeiculo,vehiculo);
    registroConductores.der = -1;
    registroConductores.izq = -1;
    archivoConductor.seekg(0,ios::end);
    archivoConductor.write(reinterpret_cast<char *>(&registroConductores),sizeof(registroConductores));
    while(true)
    {
        archivoConductor.seekg(indice * sizeof(registroConductores));
        archivoConductor.read(reinterpret_cast<char *>(&registroConductores),sizeof(registroConductores));

        if(registroConductores.id == id)
        {
            cout<<"Esta Repetido"<<endl;
            break;
        }
        else if(registroConductores.id < id)
        {
            if(registroConductores.der != -1)
                indice = registroConductores.der;
            else
            {
                registroConductores.der = fin_archivo();
                archivoConductor.seekg(indice*sizeof(registroConductores));
                archivoConductor.write(reinterpret_cast<char *>(&registroConductores),sizeof(registroConductores));
                break;

            }
        }
        else
        {
            if(registroConductores.izq != -1)
                indice = registroConductores.izq;
            else
            {
                registroConductores.izq = fin_archivo();
                archivoConductor.seekg(indice * sizeof(registroConductores));
                archivoConductor.write(reinterpret_cast<char *>(&registroConductores),sizeof(registroConductores));
                break;
            }
        }
    }
}
long fin_archivoRC()
{
    fstream archivoRutasConductorTemp("ArchivoRutas.txt",ios::in | ios::out | ios::binary);
    long n;
    archivoRutasConductorTemp.seekg(0,ios::end);
    n = archivoRutasConductorTemp.tellg();
    n = (n/sizeof(registroRutasC));

    return n;
}


void guardarArchivoRutasConductor(struct archivoRutasC rutas, int id){

    fstream archivoRutasConductorTemp("ArchivoRutas.txt",ios::in | ios::out | ios::binary);

    archivoRutasConductorTemp.seekg(0,ios::end);
    archivoRutasConductorTemp.write(reinterpret_cast<char *>(&rutas),sizeof(rutas));

    long posicionConductor = buscarConductor(id);

    struct archivoConductores tempConductor;

    archivoConductor.seekg(posicionConductor * sizeof(tempConductor));
    archivoConductor.read(reinterpret_cast<char *>(&tempConductor),sizeof(tempConductor));

    cout<<"\n*** Nombre Conductor: "<<tempConductor.nombreChofer<<endl;

    if(tempConductor.enlaceRutas == -1)
    {
        tempConductor.enlaceRutas = fin_archivoRC();
        archivoConductor.seekg(posicionConductor * sizeof(tempConductor));
        archivoConductor.write(reinterpret_cast<char *>(&tempConductor),sizeof(tempConductor));
        return;
    }
    else
    {
        int enlace = tempConductor.enlaceRutas;

        fstream archivoRutasConductorTemp2("ArchivoRutas.txt",ios::in | ios::out | ios::binary);

        while(true)
        {

            archivoRutasConductorTemp2.seekg(enlace * sizeof(registroRutasC));
            archivoRutasConductorTemp2.read(reinterpret_cast<char *>(&registroRutasC),sizeof(registroRutasC));

            if(registroRutasC.sig != -1)
            {
                enlace = registroRutasC.sig;

            }
            else
            {
                registroRutasC.sig = fin_archivoRC();
                archivoRutasConductorTemp2.seekg(enlace * sizeof(registroRutasC));
                archivoRutasConductorTemp2.write(reinterpret_cast<char *>(&registroRutasC),sizeof(registroRutasC));
                break;
            }
        }
    }
}


void guardarAarchivos()
{
    escribirEnArchivoCaminos("Los_Chiles","Pavon",2,30,1);
    escribirEnArchivoCaminos("Pavon","Los_Chiles",2,30,1);
    escribirEnArchivoCaminos("Los_Chiles","Sarapiqui",5,60,0);
    escribirEnArchivoCaminos("Sarapiqui","Pavon",4,40,0);
    escribirEnArchivoCaminos("Sarapiqui","San_Carlos",3,30,0);
    escribirEnArchivoCaminos("San_Carlos","Pavon",3,30,0);
    escribirEnArchivoCaminos("Pavon","Florencia",2,40,0);
    escribirEnArchivoCaminos("Florencia","Los_Chiles",6,50,0);
    escribirEnArchivoCaminos("Santa_Clara","Los_Chiles",4,40,0);
    escribirEnArchivoCaminos("El_Parque","Los_Chiles",2,60,0);
    escribirEnArchivoCaminos("El_Parque","Santa_Clara",5,20,1);
    escribirEnArchivoCaminos("Santa_Clara","El_Parque",5,20,1);
    escribirEnArchivoCaminos("Santa_Clara","Florencia",1,20,1);
    escribirEnArchivoCaminos("Santa_Rosa","El_Parque",2,25,0);
    escribirEnArchivoCaminos("Santa_Rosa","Los_Chiles",2,25,0);
    escribirEnArchivoCaminos("Santa_Rosa","Sarapiqui",6,30,0);
    escribirEnArchivoCaminos("Fortuna","El_Parque",5,30,0);
    escribirEnArchivoCaminos("Fortuna","Santa_Clara",15,40,0);

    guardarArchivoConductor(30,"Juan","Camion",-1);
    guardarArchivoConductor(20,"Marco","Bus",-1);
    guardarArchivoConductor(40,"Ana","Automovil",-1);
    guardarArchivoConductor(25,"Carmen","Bus",-1);
    guardarArchivoConductor(38,"Critiam","Automovil",-1);


    struct archivoRutasC reg2;

	for(int i = 0; i< 10; i++)
		strcpy(reg2.lugares[i].nombre,"");

	strcpy(reg2.lugares[0].nombre,"Los_Chiles");
	strcpy(reg2.lugares[1].nombre,"Pavon");
	strcpy(reg2.lugares[2].nombre,"Florencia");
	strcpy(reg2.lugares[3].nombre,"San_Carlos");
	reg2.sig = -1;
	guardarArchivoRutasConductor(reg2,30);

    for(int i = 0; i< 10; i++)
		strcpy(reg2.lugares[i].nombre,"");

	strcpy(reg2.lugares[0].nombre,"Fortuna");
	strcpy(reg2.lugares[1].nombre,"El_Parque");
	reg2.sig = -1;
	guardarArchivoRutasConductor(reg2,20);

	for(int i = 0; i< 10; i++)
		strcpy(reg2.lugares[i].nombre,"");

	strcpy(reg2.lugares[0].nombre,"Los_Chiles");
	strcpy(reg2.lugares[1].nombre,"Pavon");
	strcpy(reg2.lugares[2].nombre,"Florencia");
	strcpy(reg2.lugares[3].nombre,"San_Carlos");
	reg2.sig = -1;
	guardarArchivoRutasConductor(reg2,40);

	for(int i = 0; i< 10; i++)
		strcpy(reg2.lugares[i].nombre,"");

	strcpy(reg2.lugares[0].nombre,"Fortuna");
	strcpy(reg2.lugares[1].nombre,"Santa_Clara");
	strcpy(reg2.lugares[2].nombre,"Florencia");
	reg2.sig = -1;
	guardarArchivoRutasConductor(reg2,25);

	for(int i = 0; i< 10; i++)
		strcpy(reg2.lugares[i].nombre,"");

	strcpy(reg2.lugares[0].nombre,"Santa_Rosa");
	strcpy(reg2.lugares[1].nombre,"Sarapiqui");
	strcpy(reg2.lugares[2].nombre,"Pavon");
	reg2.sig = -1;
	guardarArchivoRutasConductor(reg2,38);

	for(int i = 0; i< 10; i++)
		strcpy(reg2.lugares[i].nombre,"");

	strcpy(reg2.lugares[0].nombre,"San_Carlos");
	strcpy(reg2.lugares[1].nombre,"Florencia");
	strcpy(reg2.lugares[2].nombre,"Pavon");
	strcpy(reg2.lugares[3].nombre,"Los_Chiles");
	reg2.sig = -1;
	guardarArchivoRutasConductor(reg2,30);

	for(int i = 0; i< 10; i++)
		strcpy(reg2.lugares[i].nombre,"");

	strcpy(reg2.lugares[0].nombre,"Florencia");
	strcpy(reg2.lugares[1].nombre,"San_Carlos");
	strcpy(reg2.lugares[2].nombre,"Sarapiqui");
	strcpy(reg2.lugares[3].nombre,"Los_Chiles");
	reg2.sig = -1;
	guardarArchivoRutasConductor(reg2,40);
}

/**********************************************************************************/
/**********************************************************************************/

void menuCaminos()
{
    while(true)
    {
        int opcion;
        cout <<"\t\tCAMINOS\n"<< endl;
        cout <<"\t1: Insertar Caminos"<< endl;
        cout <<"\t2: CerrarRuta"<< endl;
        cout <<"\t3: "<< endl;
        cout <<"\t4: "<< endl;
        cout <<"\t0: Regresar"<<endl;
        cout <<"****:" ;
        cin>>opcion;
        if(opcion == 0)
            menuPrincipal();
        else if(opcion == 1)
            InsertarDatosCaminos();
        else if(opcion == 2)
            CerrarRuta();
        else if(opcion == 3)
            VerCiudades();
        else if(opcion == 4)
            AuxVerCaminosCiudad();
    }
}

void menuCiudades()
{
   while(true)
    {
        int opcion;
        cout <<"\t\tCIUDADES\n"<< endl;
        cout <<"\t1: Insertar Nueva Ciudad"<< endl;
        cout <<"\t2: Borrar Ciudad"<< endl;
        cout <<"\t3: Ver Ciudades con sus rutas"<< endl;
        cout <<"\t4: Ver Ciudad y sus rutas"<< endl;
        cout <<"\t5: Averiguar si es Conexa"<< endl;
        cout <<"\t6: Guardar nuevo Conductor"<< endl;
        cout <<"\t7: Guardar nueva Ruta"<< endl;
        cout <<"\t0: Regresar"<<endl;
        cout <<"****:" ;
        cin>>opcion;
        if(opcion == 0)
            menuPrincipal();
        else if(opcion == 1)
            InsertarDatosCiudad();
        else if(opcion == 2)
            BorrarCiudad();
        else if(opcion == 3)
            VerCiudades();
        else if(opcion == 4)
            AuxVerCaminosCiudad();
        else if(opcion == 5)
            auxAveriguarConexo();
        else if(opcion == 6)
            ConductorNuevo();
        else if(opcion == 7)
            AgregarRutaNuevaConductor();
    }
}
void menuConsultas()
{
    while(true)
    {
        int opcion;
        cout<<"\t\tMENU CONSULTAS\n"<<endl;
        cout<<"\t1: Si la ciudad es conexa"<<endl;
        cout<<"\t2: Imprime CicloEulen"<<endl;
        cout<<"\t3: Recorrido un solo camino"<<endl;
        cout<<"\t4: Colores"<<endl;
        cout<<"\t5: Ruta mas Corta"<<endl;
        cout<<"\t6: Ruta mas Rapida"<<endl;
        cout<<"\t7: Rutas Alternas"<<endl;
        cout<<"\t8: Cantidad Usuario con destino X"<<endl;
        cout<<"\t9: Imprimir ruta de Usuario X"<<endl;
        cout<<"\t10: Desmarcar"<<endl;
        cout<<"\t0: Salir"<<endl;
        cout <<"****:" ;
        cin>>opcion;
        if(opcion == 0)
            return;
        else if(opcion == 1)
            auxAveriguarConexo();
        else if(opcion == 2)
            CicloEulen();
        else if(opcion == 3)
            RecorridoCaminos();
        else if(opcion == 4)
            InicioPintar();
        else if(opcion == 5)
            rutaMasCortaDatos();
        else if(opcion == 6)
            rutaMasRapidaDatos();
        else if(opcion == 7)
            inicioRutasAlternas();
        else if(opcion == 8)
            InicioUsuariosConDestinosX();
        else if(opcion == 9)
            ImprimirRutaConductorDatos();
        else if(opcion == 10)
            desmarcar();
    }
}
void menuReportes()
{
    while(true)
    {
        int opcion;
        cout<<"\t   MENU REPORTES\n"<<endl;
        cout<<"\t1: Imprimir Archivo Conductor"<<endl;
        cout<<"\t2: Imprimir Archivo Rutas Mapa"<<endl;
        cout<<"\t3: Imprimir Archivo Rutas Conductores"<<endl;
        cout<<"\t"<<endl;
        cout<<"\t0: Salir"<<endl;
        cin>>opcion;
        if(opcion == 0)
            return;
        else if(opcion == 1)
            imprimirArchivoConductor();
        else if(opcion == 2)
            ImprimirArchivoRutas();
        else if(opcion == 3)
            imprimirArchivoRutasConductor();

    }

}
void menuArchivos()
{
    while(true)
    {
        int opcion;
        cout<<"\t   MENU ARCHIVOS\n"<<endl;
        cout<<"\t1: Cargar Ciudad"<<endl;
        cout<<"\t2: Cargar Rutas"<<endl;
        cout<<"\t3: Cargar a Archivos"<<endl;
        cout<<"\t0: Salir"<<endl;
        cin>>opcion;
        if(opcion == 0)
            return;
        else if(opcion == 1)
            cargarCiudad();
        else if(opcion == 2)
            cargarRutas();
        else if(opcion == 3)
            guardarAarchivos();

    }

}
void menuPrincipal()
{
    while(true)
    {
        int opcion;
        cout<<"   MENU PRINCIPAL\n"<<endl;
        cout<<"1: Menu Ciudades"<<endl;
        cout<<"2: Menu Caminos"<<endl;
        cout<<"3: Menu Consultas"<<endl;
        cout<<"4: Menu Reportes"<<endl;
        cout<<"5: Menu Archivos"<<endl;
        cout<<"0: Salir"<<endl;
        cin>>opcion;
        if(opcion == 0)
            break;
        else if(opcion == 1)
            menuCiudades();
        else if(opcion == 2)
            menuCaminos();
        else if(opcion == 3)
            menuConsultas();
        else if(opcion == 4)
            menuReportes();
        else if(opcion == 5)
            menuArchivos();
    }
}
int main()
{
    /** \brief Fecha de inicio: 29 de Octubre 2015
     *        Fecha Finalidad: 09 de Noviembre 201
     *
     * \param
     * \param
     * \return
     *
     */

    system("color fc" );
    /**< Inserciones de las ciudades por defecto.... */
    /*InsertarCiudad("Los_Chiles");
    InsertarCiudad("Sarapiqui");
    InsertarCiudad("San_Carlos");
    InsertarCiudad("Santa_Rosa");
    InsertarCiudad("Pavon");
    InsertarCiudad("El_Parque");
    InsertarCiudad("Florencia");
    InsertarCiudad("Santa_Clara");
    InsertarCiudad("Fortuna");*/
    /**< Insertar Caminos */

   /*
    InsertarCaminos(buscar("Los_Chiles"),buscar("Pavon"),20,30,1);
    InsertarCaminos(buscar("Pavon"),buscar("Los_Chiles"),20,30,1);
    InsertarCaminos(buscar("Los_Chiles"),buscar("Sarapiqui"),50,60,0);
    InsertarCaminos(buscar("Sarapiqui"),buscar("Pavon"),43,40,0);
    InsertarCaminos(buscar("Sarapiqui"),buscar("San_Carlos"),30,30,0);
    InsertarCaminos(buscar("San_Carlos"),buscar("Pavon"),30,30,0);
    InsertarCaminos(buscar("Pavon"),buscar("Florencia"),20,40,0);
    InsertarCaminos(buscar("Florencia"),buscar("Los_Chiles"),60,50,0);
    InsertarCaminos(buscar("Santa_Clara"),buscar("Los_Chiles"),40,40,0);
    InsertarCaminos(buscar("El_Parque"),buscar("Los_Chiles"),25,60,0);
    InsertarCaminos(buscar("El_Parque"),buscar("Santa_Clara"),50,20,1);
    InsertarCaminos(buscar("Santa_Clara"),buscar("El_Parque"),50,20,1);
    InsertarCaminos(buscar("Santa_Clara"),buscar("Florencia"),10,20,1);
    InsertarCaminos(buscar("Santa_Rosa"),buscar("El_Parque"),15,25,0);
    InsertarCaminos(buscar("Santa_Rosa"),buscar("Los_Chiles"),15,25,0);
    InsertarCaminos(buscar("Santa_Rosa"),buscar("Sarapiqui"),60,30,0);
    InsertarCaminos(buscar("Fortuna"),buscar("El_Parque"),50,30,0);
    InsertarCaminos(buscar("Fortuna"),buscar("Santa_Clara"),150,40,0);*/
    //Para hacer el grafo conexo....
    /*InsertarCaminos(buscar("Fortuna"),buscar("Santa_Rosa"),450,40,0);
    InsertarCaminos(buscar("Santa_Clara"),buscar("Fortuna"),450,40,0);
    InsertarCaminos(buscar("Florencia"),buscar("Fortuna"),450,40,0);*/

    cout<<"\t\t\t\t\t\t*******  ******  ******"<<endl;
    cout<<"\t\t       SISTEMA WAZE \t\t*  *  *  *    *  *   "<<endl;
    cout<<"\t\t\t*TEC 2015*\t\t   *     *       *"<<endl;
    cout<<"\t\t\t\t\t\t   *     ****    *"<<endl;
    cout<<"\t\t\t\t\t\t   *     *       *   "<<endl;
    cout<<"\t\t\t\t\t\t   *     *    *  *"<<endl;
    cout<<"\t\t\t\t\t\t   *     ******  ******"<<endl;
    menuPrincipal();
    return 0;
}

