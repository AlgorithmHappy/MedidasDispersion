#include <iostream>
#include <cstdlib>
#include <ctime>
#include <glui.h>
#include "Estadistica.h"

using namespace std;

string itoa(int n){ //http://foro.elhacker.net/programacion_cc/iquestcomo_convertir_de_int_a_char_en_c-t346124.0.html
    string s;
    char digit[] = "0123456789";

    do{
        s = digit[n % 10] + s;
    }while ((n /= 10) > 0);

    return s;
}

int tamanyo, tamanyoAleatorio, *conjunto, *conjuntoAleatorio, valorMinimo, valorMaximo, contadorBotonUno = 0, contadorBotonDos = 0;
GLUI_Spinner *spnrCantidad;
GLUI_Rollout *rollManual;
GLUI_EditText *edtxtConjunto;
GLUI_Button *btnDetectarCantidad;
GLUI_Button *btnValor;
GLUI_Spinner *spnrValorMinimo;
GLUI_Spinner *spnrValorMaximo;
GLUI_Spinner *spnrCantidadA;
GLUI_Button *btnMinMax;
GLUI_Rollout *rollAleatorio;

void btnEscucha(int id);
void btnEscuchaDos(int id);
void btnEscuchaTres(int id);
void resultadorConjuntoAleatorio();
void mostrarResultados();
void btnEscuchaCuatro(int id);

int main(int argc, char *argv[]){
    glutInit(&argc, argv);

    //Creacion de ventana
    GLint ventana_id;
    GLUI *ventana = GLUI_Master.create_glui("Medidas de dispersion");
    ventana_id = ventana->get_glut_window_id();
    ventana->set_main_gfx_window(ventana_id);
    //Fin de ventana


    //Creacion del 1er panel
    GLUI_Panel *pnlTitulo;
    pnlTitulo = new GLUI_Panel(ventana, "Autor");
    pnlTitulo->set_font(GLUT_BITMAP_8_BY_13);
        //Creacion del titulo
        GLUI_StaticText *lblTitulo;
        lblTitulo = new GLUI_StaticText(pnlTitulo, "BY: Happy");
        lblTitulo->set_font(GLUT_BITMAP_9_BY_15);
        //Fin del Titulo
    //Fin del 1er panel

    //Creacion del 2do panel
    GLUI_Panel *pnlCuerpo;
    pnlCuerpo = new GLUI_Panel(ventana, "Captura del conjunto Manual/Aleatorio");
        //Creacion de un spinner
        spnrCantidad = new GLUI_Spinner(pnlCuerpo, "Ingrese la cantidad de valores: ", GLUI_SPINNER_INT);
        spnrCantidad->set_int_limits(3, 100);
        spnrCantidad->set_speed(0.02);
        spnrCantidad->set_font(GLUT_BITMAP_9_BY_15);
        //Fin de un spinner

        //Creacion del boton cantidad
        btnDetectarCantidad = new GLUI_Button(pnlCuerpo, "Aceptar", 1, btnEscucha);
        btnDetectarCantidad->set_font(GLUT_BITMAP_9_BY_15);
        //Fin del boton cantidad

        //Creacion del 1er despegable
        rollManual = new GLUI_Rollout(pnlCuerpo, "Captura Manual", false);
        rollManual->set_font(GLUT_BITMAP_8_BY_13);
        rollManual->disable();
            //Creacion de los edittext
            edtxtConjunto = new GLUI_EditText(rollManual, "Presione aceptar para agregar el valor: ", GLUI_EDITTEXT_INT);
            edtxtConjunto->set_font(GLUT_BITMAP_9_BY_15);
            edtxtConjunto->enable();
            //Fin de los edittext

            //Creacion del boton valor
            btnValor = new GLUI_Button(rollManual, "Aceptar", 2, btnEscuchaDos);
            btnValor->set_font(GLUT_BITMAP_9_BY_15);
            btnValor->enable();
            //Fin del boton valor
        //Fin del 1er despegable

        ventana->add_column_to_panel(pnlCuerpo, true); // Agrego columna

        //Creacion del numero minimo
        spnrValorMinimo = new GLUI_Spinner(pnlCuerpo, "Ingrese el valor minimo: ");
        spnrValorMinimo->set_font(GLUT_BITMAP_9_BY_15);
        spnrValorMinimo->set_int_limits(1, 1000);
        spnrValorMinimo->set_speed(0.002);
        //Fin del numero minimo
        //Creacion del numero maximo
        spnrValorMaximo = new GLUI_Spinner(pnlCuerpo, "Ingrese el valor maximo: ");
        spnrValorMaximo->set_font(GLUT_BITMAP_9_BY_15);
        spnrValorMaximo->set_int_limits(1, 1000);
        spnrValorMaximo->set_speed(0.002);
        //Fin del numero maximo
        //Creacion del tamanyo Dos
        spnrCantidadA = new GLUI_Spinner(pnlCuerpo, "Ingresa la cantidad de valores: ");
        spnrCantidadA->set_font(GLUT_BITMAP_9_BY_15);
        spnrCantidadA->set_int_limits(3, 100);
        spnrCantidadA->set_speed(0.02);
        //Fin del tamanyo Dos
        //Creacion del boton aleatorio
        btnMinMax = new GLUI_Button(pnlCuerpo, "Aceptar", 3, btnEscuchaTres);
        btnMinMax->set_font(GLUT_BITMAP_9_BY_15);
        //Fin del boton aleatorio
        //Creacion del 2do despegable
        rollAleatorio = new GLUI_Rollout(pnlCuerpo, "Valores Aleatorios", false);
        rollAleatorio->set_font(GLUT_BITMAP_8_BY_13);
        rollAleatorio->disable();
        //Fin del 2do despegable
    //Fin del 2do panel
    GLUI_Button *btnLimpiar;
    btnLimpiar = new GLUI_Button(ventana, "Crear nuevos conjuntos", 4, btnEscuchaCuatro);
    btnLimpiar->set_font(GLUT_BITMAP_9_BY_15);
    btnLimpiar->set_w(300);

    glutMainLoop();
    return EXIT_SUCCESS;
}

void btnEscucha(int id){
    if(contadorBotonUno == 0){
        tamanyo = spnrCantidad->get_int_val();
        rollManual->enable();
        btnDetectarCantidad->disable();
    }
    contadorBotonUno++;
}

void btnEscuchaDos(int id){
    if(contadorBotonDos == 0)
        conjunto = new int[tamanyo];

    if(contadorBotonDos < tamanyo){
        *(conjunto + contadorBotonDos) = edtxtConjunto->get_int_val();
        cout << '\a';
    }

    if(contadorBotonDos == tamanyo - 1){
        mostrarResultados();
        btnValor->disable();
    }

    contadorBotonDos++;
}

void btnEscuchaTres(int id){
    if(spnrValorMaximo->get_int_val() > spnrValorMinimo->get_int_val()){
        rollAleatorio->enable();
        valorMinimo = spnrValorMinimo->get_int_val();
        valorMaximo = spnrValorMaximo->get_int_val();
        tamanyoAleatorio = spnrCantidadA->get_int_val();
        resultadorConjuntoAleatorio();
        btnMinMax->disable();
    }
}

void mostrarResultados(){
    string strConjuntoUno = "", strConjuntoDos = "", strConjuntoTres = "", strConjuntoCuatro = "", strConjuntoCinco = "";
    Estadistica *miEstadistica;
    miEstadistica = new Estadistica(conjunto, tamanyo);
    GLUI_StaticText *lblSubtitulo, *lblConjuntoUno, *lblConjuntoDos, *lblConjuntoTres, *lblConjuntoCuatro, *lblConjuntoCinco;
    GLUI_EditText *edtxtMedia, *edtxtMediana, *edtxtRango, *edtxtVarianza, *edtxtDesviacionE;

    for(int i = 0; i < tamanyo; i++){
        if(i < 20)
            strConjuntoUno += itoa(*(conjunto + i)) + ' ';

        if(i < 40 && i >= 20)
            strConjuntoDos += itoa(*(conjunto + i))  + ' ';

        if(i < 60 && i >= 40)
            strConjuntoTres += itoa(*(conjunto + i))  + ' ';

        if(i < 80 && i >= 60)
            strConjuntoCuatro += itoa(*(conjunto + i)) + ' ';

        if(i < 100 && i >= 80)
            strConjuntoCinco += itoa(*(conjunto + i)) + ' ';
    }

    const char *caracterConjuntoUno = strConjuntoUno.c_str();
    const char *caracterConjuntoDos = strConjuntoDos.c_str();
    const char *caracterConjuntoTres = strConjuntoTres.c_str();
    const char *caracterConjuntoCuatro = strConjuntoCuatro.c_str();
    const char *caracterConjuntoCinco = strConjuntoCinco.c_str();

    lblSubtitulo = new GLUI_StaticText(rollManual, "El conjunto ingresado es:");
    lblSubtitulo->set_font(GLUT_BITMAP_9_BY_15);

    lblConjuntoUno = new GLUI_StaticText(rollManual, caracterConjuntoUno);
    lblConjuntoUno->set_font(GLUT_BITMAP_9_BY_15);

    lblConjuntoDos = new GLUI_StaticText(rollManual, caracterConjuntoDos);
    lblConjuntoDos->set_font(GLUT_BITMAP_9_BY_15);

    lblConjuntoTres = new GLUI_StaticText(rollManual, caracterConjuntoTres);
    lblConjuntoTres->set_font(GLUT_BITMAP_9_BY_15);

    lblConjuntoCuatro = new GLUI_StaticText(rollManual, caracterConjuntoCuatro);
    lblConjuntoCuatro->set_font(GLUT_BITMAP_9_BY_15);

    lblConjuntoCinco = new GLUI_StaticText(rollManual, caracterConjuntoCinco);
    lblConjuntoCinco->set_font(GLUT_BITMAP_9_BY_15);

    edtxtMedia = new GLUI_EditText(rollManual, "La media es: ", GLUI_EDITTEXT_FLOAT);
    edtxtMedia->set_font(GLUT_BITMAP_9_BY_15);
    edtxtMedia->disable();
    edtxtMedia->set_float_val(miEstadistica->obtenerMedia());

    edtxtMediana = new GLUI_EditText(rollManual, "La mediana es: ", GLUI_EDITTEXT_FLOAT);
    edtxtMediana->set_font(GLUT_BITMAP_9_BY_15);
    edtxtMediana->disable();
    edtxtMediana->set_float_val(miEstadistica->obtenerMediana());

    edtxtRango = new GLUI_EditText(rollManual, "El rango es: ", GLUI_EDITTEXT_FLOAT);
    edtxtRango->set_font(GLUT_BITMAP_9_BY_15);
    edtxtRango->disable();
    edtxtRango->set_float_val(miEstadistica->obtenerRango());

    edtxtVarianza = new GLUI_EditText(rollManual, "La varianza es: ", GLUI_EDITTEXT_FLOAT);
    edtxtVarianza->set_font(GLUT_BITMAP_9_BY_15);
    edtxtVarianza->disable();
    edtxtVarianza->set_float_val(miEstadistica->obtenerVarianza());

    edtxtDesviacionE = new GLUI_EditText(rollManual, "La desviacion estandar es: ", GLUI_EDITTEXT_FLOAT);
    edtxtDesviacionE->set_font(GLUT_BITMAP_9_BY_15);
    edtxtDesviacionE->disable();
    edtxtDesviacionE->set_float_val(miEstadistica->obtenerDesviacionE());
}

void resultadorConjuntoAleatorio(){
    int i;
    string strConjuntoUnoA = "", strConjuntoDosA = "", strConjuntoTresA = "", strConjuntoCuatroA = "", strConjuntoCincoA = "";
    conjuntoAleatorio = new int[tamanyoAleatorio];
    GLUI_StaticText *lblSubtituloA, *lblConjuntoUnoA, *lblConjuntoDosA, *lblConjuntoTresA, *lblConjuntoCuatroA, *lblConjuntoCincoA;
    GLUI_EditText *edtxtMediaA, *edtxtMedianaA, *edtxtRangoA, *edtxtVarianzaA, *edtxtDesviacionEA;
    Estadistica *miEstadistica;

    srand(time(NULL));

    for(i = 0; i < tamanyoAleatorio; i++)
        *(conjuntoAleatorio + i) = valorMinimo + (rand() % ((valorMaximo + 1) - valorMinimo));

    miEstadistica = new Estadistica(conjuntoAleatorio, tamanyoAleatorio);

    for(i = 0; i < tamanyoAleatorio; i++){
        if(i < 20)
            strConjuntoUnoA += itoa(*(conjuntoAleatorio + i)) + ' ';

        if(i < 40 && i >= 20)
            strConjuntoDosA += itoa(*(conjuntoAleatorio + i))  + ' ';

        if(i < 60 && i >= 40)
            strConjuntoTresA += itoa(*(conjuntoAleatorio + i))  + ' ';

        if(i < 80 && i >= 60)
            strConjuntoCuatroA += itoa(*(conjuntoAleatorio + i)) + ' ';

        if(i < 100 && i >= 80)
            strConjuntoCincoA += itoa(*(conjuntoAleatorio + i)) + ' ';
    }

    const char *caracterConjuntoUno = strConjuntoUnoA.c_str();
    const char *caracterConjuntoDos = strConjuntoDosA.c_str();
    const char *caracterConjuntoTres = strConjuntoTresA.c_str();
    const char *caracterConjuntoCuatro = strConjuntoCuatroA.c_str();
    const char *caracterConjuntoCinco = strConjuntoCincoA.c_str();

    lblSubtituloA = new GLUI_StaticText(rollAleatorio, "El conjunto aleatorio es:");
    lblSubtituloA->set_font(GLUT_BITMAP_9_BY_15);

    lblConjuntoUnoA = new GLUI_StaticText(rollAleatorio, caracterConjuntoUno);
    lblConjuntoUnoA->set_font(GLUT_BITMAP_9_BY_15);

    lblConjuntoDosA = new GLUI_StaticText(rollAleatorio, caracterConjuntoDos);
    lblConjuntoDosA->set_font(GLUT_BITMAP_9_BY_15);

    lblConjuntoTresA = new GLUI_StaticText(rollAleatorio, caracterConjuntoTres);
    lblConjuntoTresA->set_font(GLUT_BITMAP_9_BY_15);

    lblConjuntoCuatroA = new GLUI_StaticText(rollAleatorio, caracterConjuntoCuatro);
    lblConjuntoCuatroA->set_font(GLUT_BITMAP_9_BY_15);

    lblConjuntoCincoA = new GLUI_StaticText(rollAleatorio, caracterConjuntoCinco);
    lblConjuntoCincoA->set_font(GLUT_BITMAP_9_BY_15);

    edtxtMediaA = new GLUI_EditText(rollAleatorio, "La media es: ", GLUI_EDITTEXT_FLOAT);
    edtxtMediaA->set_font(GLUT_BITMAP_9_BY_15);
    edtxtMediaA->disable();
    edtxtMediaA->set_float_val(miEstadistica->obtenerMedia());

    edtxtMedianaA = new GLUI_EditText(rollAleatorio, "La mediana es: ", GLUI_EDITTEXT_FLOAT);
    edtxtMedianaA->set_font(GLUT_BITMAP_9_BY_15);
    edtxtMedianaA->disable();
    edtxtMedianaA->set_float_val(miEstadistica->obtenerMediana());

    edtxtRangoA = new GLUI_EditText(rollAleatorio, "El rango es: ", GLUI_EDITTEXT_FLOAT);
    edtxtRangoA->set_font(GLUT_BITMAP_9_BY_15);
    edtxtRangoA->disable();
    edtxtRangoA->set_float_val(miEstadistica->obtenerRango());

    edtxtVarianzaA = new GLUI_EditText(rollAleatorio, "La varianza es: ", GLUI_EDITTEXT_FLOAT);
    edtxtVarianzaA->set_font(GLUT_BITMAP_9_BY_15);
    edtxtVarianzaA->disable();
    edtxtVarianzaA->set_float_val(miEstadistica->obtenerVarianza());

    edtxtDesviacionEA = new GLUI_EditText(rollAleatorio, "La desviacion estandar es: ", GLUI_EDITTEXT_FLOAT);
    edtxtDesviacionEA->set_font(GLUT_BITMAP_9_BY_15);
    edtxtDesviacionEA->disable();
    edtxtDesviacionEA->set_float_val(miEstadistica->obtenerDesviacionE());
}

void btnEscuchaCuatro(int id){
    /*delete conjunto;
    delete conjuntoAleatorio;
    tamanyo = 0;
    tamanyoAleatorio = 0;
    contadorBotonUno = 0;
    contadorBotonDos = 0;
    rollManual->disable();
    rollManual->close();
    rollAleatorio->disable();
    rollAleatorio->close();
    btnDetectarCantidad->enable();
    btnMinMax->disable();*/
}














