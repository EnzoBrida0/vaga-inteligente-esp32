#include "esp_system.h"

#define TRIG_PIN 13
#define ECHO_PIN 12
#define LED_VERMELHO 26
#define LED_VERDE 27
#define LDR_PIN 34

// true = garagem aberta
// false = garagem fechada
#define GARAGEM_ABERTA true

#define ESTADO_ERRO -1
#define ESTADO_LIVRE 0
#define ESTADO_OCUPADA 1

#define DIST_MIN_CARRO 5
#define DIST_MAX_CARRO 20
#define TEMPO_CONFIRMACAO 5000
#define TIMEOUT_SENSOR 5000
#define INTERVALO_LEITURA 500

#define LIMITE_LUZ 500

unsigned long ultimaRespostaSensor = 0;
unsigned long ultimaLeitura = 0;
unsigned long tempoInicioDeteccao = 0;
unsigned long ultimoPiscar = 0;

bool objetoDetectado = false;
int ultimoEstadoLogado = -999;

void logInfo(const char* msg){
    Serial.print("[INFO] ");
    Serial.println(msg);
}

void logErro(const char* codigo,const char* msg,const char* acao){
    Serial.print("[");
    Serial.print(codigo);
    Serial.print("] ");
    Serial.println(msg);

    Serial.print("[AÇÃO] ");
    Serial.println(acao);
}

void testarLEDs(){
    digitalWrite(LED_VERMELHO,HIGH);
    digitalWrite(LED_VERDE,HIGH);

    delay(300);

    digitalWrite(LED_VERMELHO,LOW);
    digitalWrite(LED_VERDE,LOW);

    logInfo("Teste de LEDs concluído");
}

float medirDistancia(){

    digitalWrite(TRIG_PIN,LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN,HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN,LOW);

    long duracao = pulseIn(ECHO_PIN,HIGH,30000);

    if(duracao == 0){
        return -1;
    }

    return duracao * 0.034 / 2;
}

// FUNÇÃO ATUALIZADA
void verificarLuminosidade(){

    int valorLDR = analogRead(LDR_PIN);

    Serial.print("[LUMINOSIDADE]: ");
    Serial.println(valorLDR);

    if(valorLDR < LIMITE_LUZ){

        if(GARAGEM_ABERTA){
            logInfo("Ambiente escuro externo - funcionamento normal");
        }
        else{
            logErro(
                "ERRO_06",
                "Baixa luminosidade detectada",
                "Verificar iluminacao da garagem"
            );
        }
    }
}

void verificarTimeout(){
    if(millis() - ultimaRespostaSensor > TIMEOUT_SENSOR){
        logErro(
            "ERRO_03",
            "Sensor sem resposta",
            "Verificar alimentação/cabos"
        );
    }
}

void controlarLED(int estado){

    if(millis() - ultimoPiscar >= 500){
        ultimoPiscar = millis();

        if(estado == ESTADO_OCUPADA){
            digitalWrite(LED_VERDE,LOW);
            digitalWrite(LED_VERMELHO,!digitalRead(LED_VERMELHO));
        }

        else if(estado == ESTADO_LIVRE){
            digitalWrite(LED_VERMELHO,LOW);
            digitalWrite(LED_VERDE,!digitalRead(LED_VERDE));
        }

        else{
            digitalWrite(LED_VERMELHO,!digitalRead(LED_VERMELHO));
            digitalWrite(LED_VERDE,!digitalRead(LED_VERDE));
        }
    }
}

int verificarSensor(){

    float leitura1 = medirDistancia();
    float leitura2 = medirDistancia();
    float leitura3 = medirDistancia();

    if(leitura1 == -1 || leitura2 == -1 || leitura3 == -1){
        logErro(
            "ERRO_01",
            "Sensor desconectado",
            "Verifique os fios"
        );
        return ESTADO_ERRO;
    }

    if(abs(leitura1-leitura2) > 5 || abs(leitura2-leitura3) > 5){
        logErro(
            "ERRO_02",
            "Leitura instável",
            "Verifique interferências"
        );
        return ESTADO_ERRO;
    }

    ultimaRespostaSensor = millis();

    float media = (leitura1 + leitura2 + leitura3)/3;

    Serial.print("[DISTANCIA]: ");
    Serial.print(media);
    Serial.println(" cm");

    if(media >= DIST_MIN_CARRO && media <= DIST_MAX_CARRO){

        if(!objetoDetectado){
            objetoDetectado = true;
            tempoInicioDeteccao = millis();

            logInfo("Objeto detectado -> validando...");
        }

        if(millis() - tempoInicioDeteccao >= TEMPO_CONFIRMACAO){
            return ESTADO_OCUPADA;
        }
    }
    else{
        objetoDetectado = false;
    }

    return ESTADO_LIVRE;
}

void logarMudancaEstado(int estado){

    if(estado != ultimoEstadoLogado){

        ultimoEstadoLogado = estado;

        switch(estado){

            case ESTADO_OCUPADA:
                logInfo("Carro confirmado -> vaga ocupada");
                break;

            case ESTADO_LIVRE:
                logInfo("Vaga livre");
                break;

            case ESTADO_ERRO:
                logInfo("Sistema em estado de erro");
                break;
        }
    }
}

void setup(){

    Serial.begin(115200);

    pinMode(TRIG_PIN,OUTPUT);
    pinMode(ECHO_PIN,INPUT);

    pinMode(LED_VERMELHO,OUTPUT);
    pinMode(LED_VERDE,OUTPUT);

    pinMode(LDR_PIN,INPUT);

    logInfo("Sistema iniciado");

    testarLEDs();

    if(esp_reset_reason() != ESP_RST_POWERON){
        logErro(
            "ERRO_05",
            "Reboot inesperado",
            "Verificar fonte do ESP32"
        );
    }

    ultimaRespostaSensor = millis();
}

void loop(){

    if(millis() - ultimaLeitura >= INTERVALO_LEITURA){

        ultimaLeitura = millis();

        verificarTimeout();

        verificarLuminosidade();

        int estadoAtual = verificarSensor();

        logarMudancaEstado(estadoAtual);

        controlarLED(estadoAtual);
    }
}
