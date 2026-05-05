=Vaga Inteligente com ESP32=

📌 Descrição

Sistema de monitoramento de vaga de estacionamento utilizando ESP32, capaz de identificar se a vaga está livre, ocupada ou em estado de erro.

O projeto utiliza sensores e validações para garantir maior confiabilidade na detecção, evitando falsos positivos.

🧠 Como funciona

O sistema utiliza:

📡 Sensor ultrassônico (HC-SR04) para detectar a presença de veículos

🌗 LDR para monitorar a luminosidade do ambiente

🔴🟢 LEDs para indicar o estado da vaga

🔄 Estados do sistema

🟢 Vaga Livre
Nenhum objeto detectado
LED verde piscando

🔴 Vaga Ocupada
Objeto detectado entre 5–20 cm por 5 segundos
LED vermelho piscando

⚠️ Erro
Sensor desconectado ou leitura instável
Ambos os LEDs piscando

⏱️ Regras importantes
Leitura a cada 500 ms
Confirmação de presença: 5 segundos
Timeout do sensor: 5 segundos

🛠️ Tecnologias utilizadas
ESP32
Arduino (C/C++)
Sensor HC-SR04
LDR
Simulação no Wokwi

🔗 Simulação no Wokwi
👉(https://wokwi.com/projects/462129500532142081)

📸 Testes do projeto
=Simulação do projeto=

https://github.com/user-attachments/assets/49c3bcf2-d92f-437e-9031-56eb51d2cddf

https://github.com/user-attachments/assets/93bbfc13-7f76-4647-bc0a-c2fc6110c523

https://github.com/user-attachments/assets/6bc5c723-b6c5-4799-bd6f-a6a5c6cd37ad



📊 Logs do sistema
Exemplos de saída no Serial Monitor:

[INFO] Vaga livre
[INFO] Carro confirmado -> vaga ocupada
[ERRO_01] Sensor desconectado
[ERRO_02] Leitura instável
[ERRO_03] Sensor sem resposta

✅ Status do projeto

✔️ Funcional
✔️ Testado em simulação
✔️ Tratamento de falhas implementado

👨‍💻 Autores(as): 

Equipe: ParkSense

->Enzo,Amanda,Melanie,Matheus,Flávia

=Projeto desenvolvido para fins acadêmicos.

