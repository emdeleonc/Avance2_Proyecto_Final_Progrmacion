#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <iomanip>

using namespace std;

struct Producto{
    int codigo;
    char nombre[50];
    float precio;
    int stock;
    int vendidos;
    bool activo;
};

struct DetalleVenta{
    int codigoProducto;
    char nombre[50];
    int cantidad;
    float precio;
    float subtotal;
};

struct Venta{
    int numeroVenta;
    int cantidadProductos;
    DetalleVenta detalles[20];
    float subtotal;
    float iva;
    float descuento;
    float total;
    int dia;
    int mes;
};

void mostrarEncabezado(){
    cout<<"\n--------Sistema de Ventas e Inventarios--------";
}

void pausarSistema(){
    cout<<"\nPresione Enter para continuar...";
    cin.ignore();
    cin.get();
}

bool existeCodigo(int codigoBuscado){
    Producto producto;
    ifstream archivo("productos.dat", ios::binary);
    if(!archivo){
        return false;
    }

    while(archivo.read((char*)&producto,sizeof(producto))){
        if(producto.codigo==codigoBuscado && producto.activo==true){
            archivo.close();
            return true;
        }
    }
    archivo.close();
    return false;
}

void registrarProducto(){
    try{
        Producto producto;
        ofstream archivo("productos.dat", ios::binary | ios::app);
        if(!archivo){
            throw 1;
        }
        cout<<"\n-----Registro de Producto-----\n";
        
        do{
            cout<<"Codigo: ";
            cin>>producto.codigo;
            if(cin.fail()){

                cin.clear();
                cin.ignore(1000,'\n');

                producto.codigo=-1;
            }

            if(producto.codigo<=0){
                cout<<"\nCodigo invalido\n";
            }

            else if(existeCodigo(producto.codigo)){
                cout<<"\nEl codigo ya existe\n";
                producto.codigo=-1;
            }

        }while(producto.codigo<=0);
        cin.ignore();

        cout<<"Nombre: ";
        cin.getline(producto.nombre,50);

        do{
            cout<<"Precio: ";
            cin>>producto.precio;

            if(cin.fail()){
                cin.clear();
                cin.ignore(1000,'\n');

                producto.precio=-1;
            }

            if(producto.precio<=0){
                cout<<"\nPrecio invalido\n";
            }

        }while(producto.precio<=0);
        do{

            cout<<"Stock: ";
            cin>>producto.stock;
            if(cin.fail()){
                cin.clear();
                cin.ignore(1000,'\n');

                producto.stock=-1;
            }

            if(producto.stock<0){

                cout<<"\nStock invalido\n";
            }

        }while(producto.stock<0);
        producto.vendidos=0;
        producto.activo=true;

        archivo.write((char*)&producto,sizeof(producto));
        archivo.close();

        cout<<"\nProducto registrado correctamente\n";
    }

    catch(...){
        cout<<"\nSe produjo un error inesperado\n";
    }

    pausarSistema();
}

void listarProductos(){
    Producto producto;
    ifstream archivo("productos.dat", ios::binary);

    if(!archivo){
        cout<<"\nNo hay productos registrados\n";
        pausarSistema();
        return;
    }

    cout<<"\n--------Productos--------\n";
    while(archivo.read((char*)&producto,sizeof(producto))){
        if(producto.activo==true){
            cout<<"\nCodigo: "<<producto.codigo;
            cout<<"\nNombre: "<<producto.nombre;
            cout<<"\nPrecio: Q"<<producto.precio;
            cout<<"\nStock: "<<producto.stock;
            cout<<"\nVendidos: "<<producto.vendidos;
            cout<<"\n-------------------------";
        }
    }

    archivo.close();
    pausarSistema();
}

void buscarProductoCodigo(){
    Producto producto;
    int codigoBuscado;
    bool encontrado=false;

    ifstream archivo("productos.dat", ios::binary);
    if(!archivo){
        cout<<"\nNo hay productos\n";
        return;
    }
    cout<<"\nIngrese codigo: ";
    cin>>codigoBuscado;

    while(archivo.read((char*)&producto,sizeof(producto))){
        if(producto.codigo==codigoBuscado && producto.activo==true){
            cout<<"\nProducto encontrado";
            cout<<"\nNombre: "<<producto.nombre;
            cout<<"\nPrecio: Q"<<producto.precio;
            cout<<"\nStock: "<<producto.stock;
            encontrado=true;
            break;
        }
    }
    
    if(!encontrado){
        cout<<"\nProducto no encontrado\n";
    }
    archivo.close();
    pausarSistema();
}

void buscarProductoNombre(){
    Producto producto;
    char nombreBuscado[50];
    bool encontrado=false;
    ifstream archivo("productos.dat", ios::binary);

    if(!archivo){
        cout<<"\nNo hay productos\n";
        return;
    }

    cin.ignore();

    cout<<"\nIngrese nombre: ";
    cin.getline(nombreBuscado,50);

    while(archivo.read((char*)&producto,sizeof(producto))){
        if(strcmp(producto.nombre,nombreBuscado)==0 && producto.activo==true){
            cout<<"\nProducto encontrado";
            cout<<"\nCodigo: "<<producto.codigo;
            cout<<"\nPrecio: Q"<<producto.precio;
            cout<<"\nStock: "<<producto.stock;
            encontrado=true;
        }
    }

    if(!encontrado){
        cout<<"\nProducto no encontrado\n";
    }
    archivo.close();
    pausarSistema();
}

void modificarPrecio(){
    Producto producto;
    int codigoBuscado;
    float nuevoPrecio;
    bool encontrado=false;
    fstream archivo("productos.dat", ios::binary | ios::in | ios::out);

    if(!archivo){
        cout<<"\nError al abrir archivo\n";
        return;
    }

    cout<<"\nCodigo del producto: ";
    cin>>codigoBuscado;

    while(archivo.read((char*)&producto,sizeof(producto))){
        if(producto.codigo==codigoBuscado && producto.activo==true){
            do{
                cout<<"Nuevo precio: ";
                cin>>nuevoPrecio;

                if(nuevoPrecio<=0){
                    cout<<"\nPrecio invalido\n";
                }

            }while(nuevoPrecio<=0);
            producto.precio=nuevoPrecio;
            archivo.seekp(archivo.tellg()-(long)sizeof(producto));
            archivo.write((char*)&producto,sizeof(producto));

            encontrado=true;

            cout<<"\nPrecio actualizado\n";
            break;
        }
    }

    if(!encontrado){
        cout<<"\nProducto no encontrado\n";
    }
    archivo.close();
    pausarSistema();
}

void actualizarStock(){
    Producto producto;
    int codigoBuscado;
    int nuevoStock;
    bool encontrado=false;

    fstream archivo("productos.dat", ios::binary | ios::in | ios::out);

    if(!archivo){
        cout<<"\nError al abrir archivo\n";
        return;
    }
    cout<<"\nCodigo del producto: ";
    cin>>codigoBuscado;

    while(archivo.read((char*)&producto,sizeof(producto))){
        if(producto.codigo==codigoBuscado && producto.activo==true){
            do{
                cout<<"Nuevo stock: ";
                cin>>nuevoStock;

                if(nuevoStock<0){
                    cout<<"\nStock invalido\n";
                }

            }while(nuevoStock<0);
            producto.stock=nuevoStock;
            archivo.seekp(archivo.tellg()-(long)sizeof(producto));
            archivo.write((char*)&producto,sizeof(producto));

            encontrado=true;

            cout<<"\nStock actualizado\n";

            break;
        }
    }

    if(!encontrado){

        cout<<"\nProducto no encontrado\n";
    }
    archivo.close();
    pausarSistema();
}

void desactivarProducto(){
    Producto producto;
    int codigoBuscado;
    bool encontrado=false;

    fstream archivo("productos.dat", ios::binary | ios::in | ios::out);

    if(!archivo){
        cout<<"\nError al abrir archivo\n";
        return;
    }

    cout<<"\nCodigo del producto: ";
    cin>>codigoBuscado;

    while(archivo.read((char*)&producto,sizeof(producto))){
        if(producto.codigo==codigoBuscado && producto.activo==true){
            producto.activo=false;
            archivo.seekp(archivo.tellg()-(long)sizeof(producto));
            archivo.write((char*)&producto,sizeof(producto));

            encontrado=true;
            cout<<"\nProducto desactivado\n";

            break;
        }
    }

    if(!encontrado){
        cout<<"\nProducto no encontrado\n";
    }

    archivo.close();
    pausarSistema();
}

void procesoVenta(){
    Venta venta;
    Producto producto;
    int codigoBuscado;
    int cantidad;
    int opcion;
    int contador=0;
    int totalArticulos=0;

    venta.subtotal=0;
    venta.iva=0;
    venta.descuento=0;
    venta.total=0;

    cout<<"\nNumero de venta: ";
    cin>>venta.numeroVenta;
    do{
        cout<<"Dia: ";
        cin>>venta.dia;

        if(venta.dia<1 || venta.dia>31){
            cout<<"\nDia invalido\n";
        }
    }while(venta.dia<1 || venta.dia>31);
    do{
        cout<<"Mes: ";
        cin>>venta.mes;

        if(venta.mes<1 || venta.mes>12){
            cout<<"\nMes invalido\n";
        }

    }while(venta.mes<1 || venta.mes>12);
    do{
        if(contador>=20){
            cout<<"\nLimite de productos alcanzado\n";
            break;
        }

        bool encontrado=false;
        fstream archivo("productos.dat", ios::binary | ios::in | ios::out);

        cout<<"\nCodigo del producto: ";
        cin>>codigoBuscado;

        while(archivo.read((char*)&producto,sizeof(producto))){
            if(producto.codigo==codigoBuscado && producto.activo==true){
                encontrado=true;
                cout<<"\nProducto: "<<producto.nombre;
                cout<<"\nPrecio: Q"<<producto.precio;
                cout<<"\nStock: "<<producto.stock;

                do{
                    cout<<"\nCantidad: ";
                    cin>>cantidad;

                    if(cantidad<=0){
                        cout<<"\nCantidad invalida\n";
                    }

                    else if(cantidad>producto.stock){
                        cout<<"\nStock insuficiente\n";
                    }

                }while(cantidad<=0 || cantidad>producto.stock);
                venta.detalles[contador].codigoProducto=producto.codigo;
                strcpy(venta.detalles[contador].nombre,producto.nombre);
                venta.detalles[contador].cantidad=cantidad;
                venta.detalles[contador].precio=producto.precio;
                venta.detalles[contador].subtotal=producto.precio*cantidad;
                venta.subtotal=venta.subtotal+venta.detalles[contador].subtotal;

                totalArticulos=totalArticulos+cantidad;
                producto.stock=producto.stock-cantidad;
                producto.vendidos=producto.vendidos+cantidad;
                archivo.seekp(archivo.tellg()-(long)sizeof(producto));
                archivo.write((char*)&producto,sizeof(producto));
                contador++;

                break;
            }
        }

        archivo.close();
        if(!encontrado){
            cout<<"\nProducto no encontrado\n";
        }
        cout<<"\n1. Agregar otro producto";
        cout<<"\n2. Finalizar venta";
        cout<<"\nOpcion: ";
        cin>>opcion;

    }while(opcion==1);
    venta.cantidadProductos=contador;
    venta.iva=venta.subtotal*0.12;

    if(venta.subtotal>500){
        venta.descuento=venta.subtotal*0.05;
    }

    if(totalArticulos%5==0){
        venta.descuento=venta.descuento+25;
    }

    venta.total=venta.subtotal+venta.iva-venta.descuento;
    venta.total=round(venta.total*100)/100;
    cout<<fixed<<setprecision(2);

    cout<<"\n--------FACTURA--------";
    cout<<"\nSubtotal: Q"<<venta.subtotal;
    cout<<"\nIVA: Q"<<venta.iva;
    cout<<"\nDescuento: Q"<<venta.descuento;
    cout<<"\nTotal: Q"<<venta.total;

    ofstream archivoVentas("ventas.dat", ios::binary | ios::app);
    archivoVentas.write((char*)&venta,sizeof(venta));
    archivoVentas.close();
    cout<<"\n\nVenta guardada correctamente\n";
    pausarSistema();
}

void productosMenorStock(){
    Producto producto;
    ifstream archivo("productos.dat", ios::binary);

    if(!archivo){
        cout<<"\nNo hay productos\n";
        return;
    }
    cout<<"\n-----Productos con Menor Stock-----\n";

    while(archivo.read((char*)&producto,sizeof(producto))){

        if(producto.stock<=5 && producto.activo==true){
            cout<<"\n"<<producto.nombre;
            cout<<"\nStock: "<<producto.stock;
            cout<<"\n-------------------";
        }
    }

    archivo.close();
    pausarSistema();
}

void productosMasVendidos(){
    Producto productos[100];
    Producto aux;
    int contador=0;
    ifstream archivo("productos.dat", ios::binary);

    while(archivo.read((char*)&productos[contador],sizeof(Producto))){
        contador++;
    }

    archivo.close();

    for(int i=0;i<contador-1;i++){
        for(int j=0;j<contador-1;j++){
            if(productos[j].vendidos<productos[j+1].vendidos){

                aux=productos[j];
                productos[j]=productos[j+1];
                productos[j+1]=aux;
            }
        }
    }

    cout<<"\n-----Productos Mas Vendidos-----\n";
    for(int i=0;i<contador;i++){
        if(productos[i].activo==true){
            cout<<"\n"<<productos[i].nombre;
            cout<<"\nVendidos: "<<productos[i].vendidos;
            cout<<"\n--------------------";
        }
    }
    pausarSistema();
}

void ventasTotalesDia(){
    Venta venta;
    int diaBuscado;
    float totalDia=0;
    ifstream archivo("ventas.dat", ios::binary);

    if(!archivo){
        cout<<"\nNo hay ventas\n";
        return;
    }

    cout<<"\nIngrese dia: ";
    cin>>diaBuscado;

    while(archivo.read((char*)&venta,sizeof(venta))){
        if(venta.dia==diaBuscado){
            totalDia=totalDia+venta.total;
        }
    }

    archivo.close();
    cout<<"\nVentas del dia: Q"<<totalDia;
    pausarSistema();
}

void ventasPorMes(){
    Venta venta;
    float matrizVentas[12][31]={0};
    ifstream archivo("ventas.dat", ios::binary);

    if(!archivo){
        cout<<"\nNo hay ventas\n";
        return;
    }

    while(archivo.read((char*)&venta,sizeof(venta))){
        if(venta.mes>=1 && venta.mes<=12 && venta.dia>=1 && venta.dia<=31){

            matrizVentas[venta.mes-1][venta.dia-1]+=venta.total;
        }
    }

    archivo.close();

    cout<<"\n-----Ventas por Mes-----\n";
    for(int i=0;i<12;i++){
        float suma=0;
        for(int j=0;j<31;j++){
            suma=suma+matrizVentas[i][j];
        }
        cout<<"\nMes "<<i+1<<": Q"<<suma;
    }
    pausarSistema();
}

void exportarReporte(){
    Producto producto;
    ifstream archivo("productos.dat", ios::binary);
    ofstream reporte("reporte.txt");

    if(!archivo || !reporte){
        cout<<"\nError al generar reporte\n";
        return;
    }

    reporte<<"Reporte de productos\n\n";
    while(archivo.read((char*)&producto,sizeof(producto))){
        if(producto.activo==true){
            reporte<<"Codigo: "<<producto.codigo<<endl;
            reporte<<"Nombre: "<<producto.nombre<<endl;
            reporte<<"Precio: "<<producto.precio<<endl;
            reporte<<"Stock: "<<producto.stock<<endl;
            reporte<<"Vendidos: "<<producto.vendidos<<endl;
            reporte<<"----------------------"<<endl;
        }
    }
    archivo.close();
    reporte.close();

    cout<<"\nReporte exportado correctamente\n";

    pausarSistema();
}

void reiniciarInventario(){
    char opcion;
    cout<<"\nSeguro que desea reiniciar inventario? (s/n): ";
    cin>>opcion;

    if(opcion=='s'||opcion=='S'){
        ofstream archivo("productos.dat", ios::binary | ios::trunc);
        archivo.close();
        cout<<"\nInventario reiniciado\n";
    }else{
        cout<<"\nOperacion cancelada\n";
    }
    pausarSistema();
}

void estadisticasGenerales(){
    Producto producto;

    int totalProductos=0;
    int stockTotal=0;
    int vendidosTotal=0;

    ifstream archivo("productos.dat", ios::binary);

    if(!archivo){
        cout<<"\nNo hay productos\n";
        return;
    }

    while(archivo.read((char*)&producto,sizeof(producto))){
        if(producto.activo==true){
            totalProductos++;
            stockTotal=stockTotal+producto.stock;
            vendidosTotal=vendidosTotal+producto.vendidos;
        }
    }

    archivo.close();

    cout<<"\n-----Estadísticas-----";
    cout<<"\nProductos activos: "<<totalProductos;
    cout<<"\nStock total: "<<stockTotal;
    cout<<"\nProductos vendidos: "<<vendidosTotal;
    pausarSistema();
}

void menuReportes(){
    int opcion;
    do{
        mostrarEncabezado();
        cout<<"\n1. Productos con menor stock";
        cout<<"\n2. Productos mas vendidos";
        cout<<"\n3. Ventas totales del dia";
        cout<<"\n4. Ventas por mes";
        cout<<"\n5. Regresar";
        cout<<"\n\nSeleccione una opcion: ";
        cin>>opcion;
        switch(opcion){
            case 1:
                productosMenorStock();
                break;
            case 2:
                productosMasVendidos();
                break;
            case 3:
                ventasTotalesDia();
                break;
            case 4:
                ventasPorMes();
                break;
            case 5:
                break;
            default:
                cout<<"\nOpcion invalida\n";
        }

    }while(opcion!=5);
}

void menuUtilidades(){
    int opcion;
    do{
        mostrarEncabezado();
        cout<<"\n1. Exportar reporte a TXT";
        cout<<"\n2. Reiniciar inventario";
        cout<<"\n3. Estadisticas generales";
        cout<<"\n4. Regresar";
        cout<<"\n\nSeleccione una opcion: ";
        cin>>opcion;

        switch(opcion){
            case 1:
                exportarReporte();
                break;
            case 2:
                reiniciarInventario();
                break;
            case 3:
                estadisticasGenerales();
                break;
            case 4:
                break;
            default:
                cout<<"\nOpcion invalida\n";
        }
    }while(opcion!=4);
}

void menuProductos(){
    int opcion;

    do{
        mostrarEncabezado();
        cout<<"\n1. Registrar producto";
        cout<<"\n2. Listar productos";
        cout<<"\n3. Buscar producto por codigo";
        cout<<"\n4. Buscar producto por nombre";
        cout<<"\n5. Modificar precio";
        cout<<"\n6. Actualizar stock";
        cout<<"\n7. Desactivar producto";
        cout<<"\n8. Regresar";
        cout<<"\n\nSeleccione una opcion: ";
        cin>>opcion;

        switch(opcion){
            case 1:
                registrarProducto();
                break;
            case 2:
                listarProductos();
                break;
            case 3:
                buscarProductoCodigo();
                break;
            case 4:
                buscarProductoNombre();
                break;
            case 5:
                modificarPrecio();
                break;
            case 6:
                actualizarStock();
                break;
            case 7:
                desactivarProducto();
                break;
            case 8:
                break;
            default:
                cout<<"\nOpcion invalida\n";
        }
    }while(opcion!=8);
}

void menu(){
    int opcion;
    do{
        mostrarEncabezado();
        cout<<"\n1. Gestion de productos";
        cout<<"\n2. Proceso de venta";
        cout<<"\n3. Reportes";
        cout<<"\n4. Utilidades";
        cout<<"\n5. Salir";
        cout<<"\n\nSeleccione una opcion: ";
        cin>>opcion;

        switch(opcion){
            case 1:
                menuProductos();
                break;
            case 2:
                try{

                    procesoVenta();
                }
                catch(...){

                    cout<<"\nError en el proceso de venta\n";
                }
                break;
            case 3:
                menuReportes();
                break;
            case 4:
                menuUtilidades();
                break;
            case 5:
                cout<<"\nGracias por usar el sistema\n";
                break;
            default:
                cout<<"\nOpcion invalida\n";
        }
    }while(opcion!=5);
}

int main(){
    cout<<"\nDesarrollado por Emily de Leon 9941-25-13659\n";
    menu();

    return 0;
}
