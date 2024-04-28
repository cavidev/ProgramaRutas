#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

void menuPrincipal();
void menuCiudades();

struct Ciudades *  buscar(string);
void InsertarCiudad(string);
void InsertarDatosCiudad();
void VerCiudades();
void BorrarCiudadad();

void InsertarCaminos(struct Ciudades, struct Ciudades, int, string ,bool );
void InsertarDatosCaminos();
void VerCaminosCiudad();
void AuxVerCaminosCiudad();

/************************************* Estructuras *************************************/
/**********************************************************************************/
struct Ciudades{
    /**< Lista Simple de ciudades, representa un grafo de las ciudades */
    /**< Pasar a char los string... y no se le envia el color */
    string nombreCiudad;
    string colorCiudad;
    bool visitado;
    struct Ciudades * sigCiudad;
    struct Caminos * subListaCaminos;
    Ciudades(string nombreLugar,string colorLugar1)
    {
        nombreCiudad = nombreLugar;
        colorCiudad = colorLugar1;
        visitado = false;
        sigCiudad = NULL;
        subListaCaminos = NULL;
    }
}*mapaCiudades;

struct Caminos{
    /**< Sub-Lista de los caminos, son los arcos  */
    int distanciaKm;
    string velocidad;
    string dirigido;/**< No tengo entendido a que se refiere con esto.. */
    struct Ciudades * CiudadDestino;
    struct Caminos * sigCamino;
    ///no lleva contructor...
    Caminos(int distancia, string kph, string siesdirigido, struct Ciudades * destino)
    {
        distanciaKm = distancia;
        velocidad = kph;
        dirigido = siesdirigido;
        sigCamino = NULL;
        CiudadDestino = destino;
    }
};
/************************************* Archivos *************************************/
/**********************************************************************************/




/************************************* Código *************************************/
/**********************************************************************************/
/******************************* Código de Ciudadades *****************************/
/**********************************************************************************/
struct Ciudades * buscar(string nombreLugar)
{
    /** \brief Funcion de busqueda de ciudades
     *
     * \param nombreLugar... nombre de la ciudad que va a buscar
     * \return si encuetra la ciudad el nodo sino NULL
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

void InsertarCiudad(string nombreLugar, string colorLugar)
{
    /** \brief Inserta la ciudades nuevas a la lista mapasCiudades..
     *
     * \param nombreLugar: es el nombre del nuevo lugar por crear, si ya existe no lo agregar
     * \return NULL
     *
     */
    struct Ciudades * nnC = new Ciudades(nombreLugar,colorLugar);
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
    cout<<"\n\tDigite el nombre de la ciuadad Nueva"<<endl;
    cout<<"***";
    cin>>nombreLugar;
    cout<<"\n\tDigite el color de la ciuadad Nueva"<<endl;
    cout<<"***";
    cin>>colorLugar;
    if(buscar(nombreLugar) != NULL)
        cout<<"\n\t\t**********La ciudad con ese nombre ya existe..**********\n"<<endl;
    else
    {
        InsertarCiudad(nombreLugar,colorLugar);
        cout<<"\n\t\t**********Se agrego con exito la ciuadad..**********\n"<<endl;

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
void BorrarCiudadad()
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
/*
void modificaCiudad()
{
    string nombreCiudadModificar;
    cout<<"Digite el nombre de la Ciudad a Modificar\n***";
    cin>>nombreCiudadModificar;
    struct Ciudades * ciudadEncontrada = buscar(nombreCiudad);
    if(ciudadEncontrada == NULL)
        cout<<"Ciudad no Existe"<<endl;
    else
    {
        cout<<"\n\tLa ciudad se llama: "<<ciudadEncontrada->nombreCiudad<<endl;
        cout<<"\tEl color de la ciudad es: "<<ciudadEncontrada->colorCiudad<<endl;
        cout<<"\n\t1: Modificar nombre\n\t2: Modificar Color"<<endl;
        int opcion;
        cin>>opcion;
        struct Ciudades * tempCiudad = mapaCiudades;
        if(opcion == 1)
        {
            string nombreNuevo;
            cout<<"Digite el nuevo nombre\n***";
            cin>>nombreNuevo;
           if(nombreNuevo == ciudadEncontrada->nombreCiudad)
           {
               cout<<"****La ciudad ya se llama Asi****"<<endl;
           }
           else
           {
               while(tempCiudad != NULL)
               {
                   if(tempCiudad->nombreCiudad == nombreNuevo)
                        cout<<"****Ya hay una ciudad asociada a ese nombre****"<<endl;
                   tempCiudad = tempCiudad->sigCiudad;
               }
               ciudadEncontrada->nombreCiudad = nombreNuevo;
               cout<<"****Se modifico con exito****"<<endl;
           }
        }
        else
        {

        }
    }
}*/
/********************************** Código de Caminos *****************************/
/**********************************************************************************/
void InsertarCaminos(struct Ciudades * cOrigen, struct Ciudades * cDestino, int distancia, string velocidad,string dirigido)
{
    /**\brief Inserta los nuevos caminos de una ciudad a otra.. Si no se encuentra, no agregara nada
     *
     * \param cudadOrigen: el nombre de la ciudad de Origen;
            ciudadDestino: el nombre de la ciudad hacia donde el camino va;
            distancia: kilometros que posee esa carretera
            velociadad: la velociadad a la que puede ir un automovil.
     * \return NULL
     *
     */
    if(cOrigen == NULL || cDestino == NULL)
        cout<<"\n\t\t**********Datos Inexistente..**********"<<endl;
    struct Caminos * nnCamino = new Caminos(distancia,velocidad,dirigido,cDestino);
    nnCamino->sigCamino = cOrigen->subListaCaminos;
    cOrigen->subListaCaminos = nnCamino;
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

    string ciudadDestino;
    string ciudadOrigen;
    int distancia;
    string velocidad;
    string dirigido;
    cout<<"Digite la ciudad Origen\n***";
    cin>>ciudadOrigen;

    cout<<"Digite la ciudad Destino\n***";
    cin>>ciudadDestino;
    struct Ciudades * COrigen = buscar(ciudadOrigen);
    struct Ciudades * CDestino = buscar(ciudadDestino);
    if(COrigen->colorCiudad == CDestino->colorCiudad)
    {
       cout<<"\n\t****Las Ciudades poseen el mismo color, no se pueden asociar****"<<endl;
    }
    else
    {
        cout<<"Digite la velocidad permitida\n***";
        cin>>velocidad;

        cout<<"Digite la distancia\n***";
        cin>>distancia;

        cout<<"Digite si el camino es Ida y Vuelta\n(si/no)\n***";
        cin>>dirigido;

        if(dirigido == "si")
        {
            InsertarCaminos(CDestino,COrigen,distancia,velocidad + "k/h",dirigido);
            InsertarCaminos(COrigen,CDestino,distancia,velocidad + "k/h",dirigido);
            cout<<"\n\t****Se inserto con exito****"<<endl;
        }
        else
        {
            InsertarCaminos(COrigen,CDestino,distancia,velocidad + "k/h",dirigido);
            cout<<"\n\t****Se inserto con exito****"<<endl;
        }

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

    if(inicio == NULL || inicio->visitado == true)
        return;
    inicio->visitado = true;
    struct Caminos * tempCaminos= inicio->subListaCaminos;
    while(tempCaminos != NULL)
    {
        cout<<"\t\t"<<inicio->nombreCiudad<<" a "<<tempCaminos->CiudadDestino->nombreCiudad<<endl;
        VerCaminosCiudad(tempCaminos->CiudadDestino);
        tempCaminos = tempCaminos->sigCamino;
    }
    cout<<"\n\n";

}
void AuxVerCaminosCiudad()
{
    /** \brief Es la funcion que ayuda a la busquedad de la busquedad de un camino y sus rutas
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
    else
        VerCaminosCiudad(temp);
}

void menuCaminos()
{
    while(true)
    {
        int opcion;
        cout <<"\t\tCAMINOS\n"<< endl;
        cout <<"\t1: Insertar Caminos"<< endl;
        cout <<"\t2: "<< endl;
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
            BorrarCiudadad();
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
        cout <<"\t4: Ver Ciudadad y sus rutas"<< endl;
        cout <<"\t0: Regresar"<<endl;
        cout <<"****:" ;
        cin>>opcion;
        if(opcion == 0)
            menuPrincipal();
        else if(opcion == 1)
            InsertarDatosCiudad();
        else if(opcion == 2)
            BorrarCiudadad();
        else if(opcion == 3)
            VerCiudades();
        else if(opcion == 4)
            AuxVerCaminosCiudad();
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
        cout<<"0: Salir"<<endl;
        cin>>opcion;
        if(opcion == 0)
            return;
        else if(opcion == 1)
            menuCiudades();
        else if(opcion == 2)
            menuCaminos();
    }

}
int main()
{
    /**< Inserciones de las ciudades por defecto.... */
    InsertarCiudad("Los_Chiles","Rojo");
    InsertarCiudad("Sarapiqui","Verde");
    InsertarCiudad("San_Carlos","Rojo");
    InsertarCiudad("Santa_Rosa","Amarillo");
    InsertarCiudad("Pavon","Amarillo");
    InsertarCiudad("El_Parque","Verde");
    InsertarCiudad("Florencia","Verde");
    InsertarCiudad("Santa_Clara","Amarillo");
    InsertarCiudad("Fortuna","Rojo");
    /**< Insertar Caminos */

    InsertarCaminos(buscar("Los_Chiles"),buscar("Pavon"),200,"30k/h","si");
    InsertarCaminos(buscar("Pavon"),buscar("Los_Chiles"),200,"30k/h","si");
    InsertarCaminos(buscar("Los_Chiles"),buscar("Sarapiqui"),500,"60k/h","no");
    InsertarCaminos(buscar("Sarapiqui"),buscar("Pavon"),439,"40k/h","no");
    InsertarCaminos(buscar("Sarapiqui"),buscar("San_Carlos"),300,"30k/h","no");
    InsertarCaminos(buscar("San_Carlos"),buscar("Pavon"),300,"30k/h","no");
    InsertarCaminos(buscar("Pavon"),buscar("Florencia"),200,"40k/h","no");
    InsertarCaminos(buscar("Florencia"),buscar("Los_Chiles"),600,"50k/h","no");
    InsertarCaminos(buscar("Santa_Clara"),buscar("Los_Chiles"),400,"40k/h","no");
    InsertarCaminos(buscar("El_Parque"),buscar("Los_Chiles"),250,"60k/h","no");
    InsertarCaminos(buscar("El_Parque"),buscar("Santa_Clara"),50,"20k/h","si");
    InsertarCaminos(buscar("Santa_Clara"),buscar("El_Parque"),50,"20k/h","si");
    InsertarCaminos(buscar("Santa_Clara"),buscar("Florencia"),100,"20k/h","si");
    InsertarCaminos(buscar("Santa_Rosa"),buscar("El_Parque"),150,"25k/h","no");
    InsertarCaminos(buscar("Santa_Rosa"),buscar("Los_Chiles"),150,"25k/h","no");
    InsertarCaminos(buscar("Santa_Rosa"),buscar("Sarapiqui"),200,"30k/h","no");
    InsertarCaminos(buscar("Fortuna"),buscar("El_Parque"),500,"30k/h","no");
    InsertarCaminos(buscar("Fortuna"),buscar("Santa_Clara"),450,"40k/h","no");

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

