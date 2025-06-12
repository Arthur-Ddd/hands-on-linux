// Defina os pinos de LED e LDR
// Defina uma variável com valor máximo do LDR (4000)
// Defina uma variável para guardar o valor atual do LED (10)
int ledPin = 25;
int ledValue = 10;
int ldrPin = 34;

// Faça testes no sensor ldr para encontrar o valor maximo e atribua a variável ldrMax
int ldrMax = 4000;

void setup() {
    Serial.begin(9600);
    
    pinMode(ledPin, OUTPUT);
    pinMode(ldrPin, INPUT);
    
    Serial.printf("SmartLamp Initialized.\n");


}

// Função loop será executada infinitamente pelo ESP32
void loop() {
    //Obtenha os comandos enviados pela serial 
    //e processe-os com a função processCommand

    if(Serial.available()){
      String command = Serial.readStringUntil('\n');
      Serial.println(command.substring(0,7));
      processCommand(command);
    }

    
}


void processCommand(String command) {
    // compare o comando com os comandos possíveis e execute a ação correspondente
    // SET_LED X - onde X representa a intensidade de luminosidade do led entre 0 e 100, qualquer valor fora desse intervalo deve ser ignorado
    // GET_LED - deve retornar o valor da intensidade atual do led
    // GET_LDR - deve retornar o valor da leitura do ldr atual
    // RES SET_LED 1 - quando o valor inserido estiver no intervalo 0 a 100
    // RES SET_LED -1- qualquer entrada inválida do comando SET_LED
    // ERR Unknown command. - resposta para qualquer comando inválido
    // RES GET_LED Y - resposta para o comando GET_LED onde Y é o valor atual do led
    // RES GET_LDR Z - resposta para o comando GET_LDR onde Z é o valor atual do ldr

    if (command.substring(0,7) == "SET_LED"){
        //String newValue = command.substring(8,11);
        int newValue =  command.substring(8,11).toInt();
        if(newValue < 0 || newValue > 100){
            Serial.println("RES SET_LED -1");
        }
        else{
          Serial.println("RES SET_LED 1");
          ledUpdate(newValue);
        }
    }

    else if(command.substring(0,7) == "GET_LED"){
        Serial.printf("RES GET_LED %d\n", ledValue);
    }

    else if(command.substring(0,7) == "GET_LDR"){
        Serial.printf("RES GET_LDR %d\n", ldrGetValue());
    }

    else{
      Serial.println("ERR Unknown command.");
    }
    
}

// Função para atualizar o valor do LED
void ledUpdate(int newLedValue) {
    // Valor deve convertar o valor recebido pelo comando SET_LED de 0 a 100 para 0 e 255
    // Normalize o valor do LED antes de enviar para a porta correspondente
    ledValue = (newLedValue * 255) / 100;
    analogWrite(ledPin, ledValue);
}

// Função para ler o valor do LED
int ledGetValue() {
    // Normaliza o valor de output do led de 0 a 255 e retorna um valor entre 0 e 100
    if(ledValue < 0){
      return 0;
    }
    if(ledValue > 255){
      return 100;
    }
    return (int)(ledValue * 100) / 255;
}

// Função para ler o valor do LDR
int ldrGetValue() {
    // Leia o sensor LDR de 0 a 255 e retorne o valor normalizado entre 0 e 100
    // faça testes para encontrar o valor maximo do ldr (exemplo: aponte a lanterna do celular para o sensor)       
    // Atribua o valor para a variável ldrMax e utilize esse valor para a normalização
    int ldrValue = analogRead(ldrPin);
    if(ldrValue < 0){
      return 0;
    }
    if(ldrValue > ldrMax){
      return 100;
    }
    return (int)(ldrValue * 100) / 4000;
}
