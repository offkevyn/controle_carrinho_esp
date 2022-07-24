/*********
  Criado por: Rui Santos - https://randomnerdtutorials.com/
  Com modificações por: offKevyn
*********/
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>

#define ledPin1 D6
#define ledPin2 D7
#define buzzer D5

#define MOTOR_1_PIN_1 D0
#define MOTOR_1_PIN_2 D1
#define MOTOR_2_PIN_1 D2
#define MOTOR_2_PIN_2 D3

AsyncWebServer server(80);
DNSServer dns;

// Display the HTML web page
static const char PROGMEM  index_html[] = R"rawliteral(
<!DOCTYPE html>

<html lang="pt-br">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Index</title>
    <style>
        * {
            box-sizing: border-box;

            -webkit-touch-callout: none;
            /* iPhone OS, Safari */
            -webkit-user-select: none;
            /* Chrome, Safari 3 */
            -khtml-user-select: none;
            /* Safari 2 */
            -moz-user-select: none;
            /* Firefox */
            -ms-user-select: none;
            /* IE10+ */
            user-select: none;
            /* Possível implementação no futuro */

            --size_buttons: 80px;
            --radius_border_buttons: 50px;
            --cor_font_buttons: #f50056d8;
        }

        body,
        html {
            padding: 0;
            margin: 0;
            font-family: sans-serif;
            background-color: #B0BEC5;
            background-attachment: fixed;
            background-image: linear-gradient(to bottom, #B0BEC5, #B0BEC5, #607D8B);
        }

        body {
            width: 100%;
            height: 100%;

        }

        #main_buttons {
            position: absolute;

            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
        }

        #group1,
        #group2,
        #group3 {
            text-align: center;
            margin: 5px 0;
            min-width: 320px;

        }

        button {


            width: var(--size_buttons);
            height: var(--size_buttons);
            box-shadow: 3px 3px 10px #05628A;
            opacity: 70%;
            cursor: pointer;

            border: 1px solid #05628A;

            color: var(--cor_font_buttons);
            font-weight: 900;
            text-shadow: 1px 1px 15px var(--cor_font_buttons);
            -webkit-text-stroke-width: 1.5px;
            -webkit-text-stroke-color: var(--cor_font_buttons);

            background-color: #9CE1FF;

            transition: all .5s;
        }

        button:hover,
        button:active {
            opacity: 100%;
            box-shadow: 10px 10px 10px #05628A;
        }

        #btn_pressed {
            display: flex;
            align-items: center;
            justify-content: center;

            height: calc(var(--size_buttons) + 25px);
            width: calc(var(--size_buttons) + 25px);

            position: absolute;
            left: 50%;
            top: -100%;
            /* left: 120%; */
            transform: translate(-50%, 20px);

            border: 2.5px solid var(--cor_font_buttons);
            border-radius: 10px;

            /* box-shadow: 1px 1px 15px var(--cor_font_buttons); */

            box-shadow: 1px 1px 10px 0px rgba(245, 0, 86, 0.8);
            animation: pulse 0.6s linear infinite;

            animation-direction: alternate;
            animation-delay: 0s;
            transition: all 0.5s;
        }

        @keyframes pulse {
            0% {
                box-shadow: 1px 1px 5px 0px rgba(245, 0, 86, 0.7);
            }

            70% {
                box-shadow: 1px 1px 10px 5px rgba(245, 0, 86, 0.4);
            }

            100% {
                box-shadow: 2px 2px 10px 5px rgba(245, 0, 86, 0.2);
            }
        }

        #btn_pressed button {
            opacity: 100%;
            box-shadow: 1px 1px 15px #05628A;
        }




        .btn_top {
            border-radius: var(--radius_border_buttons) var(--radius_border_buttons) 0 0;
            height: calc(var(--size_buttons) + 5px);
        }

        .btn_right {
            border-radius: var(--radius_border_buttons) 0 0 var(--radius_border_buttons);
            width: calc(var(--size_buttons) + 5px);
        }

        .btn_led {
            border-radius: var(--radius_border_buttons);
        }

        .btn_left {
            border-radius: 0 var(--radius_border_buttons) var(--radius_border_buttons) 0;
            width: calc(var(--size_buttons) + 5px);
        }

        .btn_botton {
            height: calc(var(--size_buttons) + 5px);
            border-radius: 0 0 var(--radius_border_buttons) var(--radius_border_buttons);
        }

        header {
            position: fixed;
            bottom: 0;
            width: 100%;
            text-align: center;
            padding-bottom: 10px;

            color: #f50056d5;
            font-size: 1.2rem;
        }
    </style>

</head>

<body>
    <div id="btn_pressed"></div>

    <div id="main_buttons">
        <div id="group1">
            <button class="btn btn_top" data-action="forward">↑</button>
        </div>
        <div id="group3">
            <button class="btn btn_right" data-action="right">←</button>
            <button class="btn btn_led" data-action="led">❤</button>
            <button class="btn btn_left" data-action="left">→</button>
        </div>
        <div id="group3">
            <button class="btn btn_botton" data-action="backward">↓</button>
        </div>
    </div>

    <!-- <div id="t">teste: </div> -->

    <header>
        <div>@offkevyn</div>
    </header>

    <script type="text/javascript">
        let list_buttons = document.getElementsByClassName('btn');
        var intervalId = 0;
        var action_pressed;
        let btn_select = document.getElementById("btn_pressed");

        Array.from(list_buttons).forEach(element => {
            element.addEventListener('touchstart', function (e) {
                btn_select.appendChild(e.target.cloneNode(true));
                intervalId = setInterval(btn_pressed, 50);
                action_pressed = e;
            });

            element.addEventListener('click', function (e) {
                //document.getElementById("t").innerHTML += "2";
                ////////
                go(e.target.dataset.action);
                setTimeout(go('stop'), 500);
                window.navigator.vibrate(50);
                action_pressed = e;
            });

            element.addEventListener('touchend', release);
        });

        function btn_pressed() {
            console.log(action_pressed.target.dataset.action);
            //document.getElementById("t").innerHTML += "1";
            ///////
            go(action_pressed.target.dataset.action);
            window.navigator.vibrate(50);
            btn_select.style.top = "0";
        }

        function release() {
            if (intervalId != 0) {
                console.log('Soltou!');
                /////
                go('stop');
                clearInterval(intervalId);
                intervalId = 0;
                btn_select.style.top = "-100%";
                btn_select.innerHTML = '';
            }
        }
        
        function go(move)
        {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/action?go=" + move, true);
            xhr.send();
        }

        document.addEventListener('contextmenu', event => event.preventDefault());

    </script>
</body>
</html>
)rawliteral";

const char* PARAM_INPUT_go = "go";
boolean ledAtivo = false;
String actionMove;

String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(9600);
  
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(MOTOR_1_PIN_1, OUTPUT);
  pinMode(MOTOR_1_PIN_2, OUTPUT);
  pinMode(MOTOR_2_PIN_1, OUTPUT);
  pinMode(MOTOR_2_PIN_2, OUTPUT);
  
  AsyncWiFiManager wifiManager(&server,&dns);
  //wifiManager.resetSettings();
  wifiManager.autoConnect("CarrinhoBraboo");
  Serial.println("Connected.");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/action", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_go)) {
      actionMove = request->getParam(PARAM_INPUT_go)->value();
      mover(actionMove);
    }
    else {
       actionMove = "No action sent";
    }
    request->send(200, "text/plain", "OK");
  });
  
  server.begin();
}

static void mover(String variable){
  
  if(variable == "forward") {
    Serial.println("Forward");
    digitalWrite(MOTOR_1_PIN_1, 0);
    digitalWrite(MOTOR_1_PIN_2, 1);
    digitalWrite(MOTOR_2_PIN_1, 1);
    digitalWrite(MOTOR_2_PIN_2, 0);
  }
  else if(variable == "left") {
    Serial.println("Left");
    digitalWrite(MOTOR_1_PIN_1, 0);
    digitalWrite(MOTOR_1_PIN_2, 1);
    digitalWrite(MOTOR_2_PIN_1, 0);
    digitalWrite(MOTOR_2_PIN_2, 1);
  }
  else if(variable == "right") {
    Serial.println("Right");
    digitalWrite(MOTOR_1_PIN_1, 1);
    digitalWrite(MOTOR_1_PIN_2, 0);
    digitalWrite(MOTOR_2_PIN_1, 1);
    digitalWrite(MOTOR_2_PIN_2, 0);
  }
  else if(variable == "led") {
    Serial.println("Led");
   
  }
  else if(variable == "backward") {
    Serial.println("Backward");
    digitalWrite(MOTOR_1_PIN_1, 1);
    digitalWrite(MOTOR_1_PIN_2, 0);
    digitalWrite(MOTOR_2_PIN_1, 0);
    digitalWrite(MOTOR_2_PIN_2, 1);
  }
  else if(variable == "stop") {
    Serial.println("Stop");
    digitalWrite(MOTOR_1_PIN_1, 0);
    digitalWrite(MOTOR_1_PIN_2, 0);
    digitalWrite(MOTOR_2_PIN_1, 0);
    digitalWrite(MOTOR_2_PIN_2, 0);
  }
}

 
void loop() {
  if (actionMove == "led") {//Ativado
    tone(buzzer, 2050);
    if(ledAtivo)
    {
          digitalWrite(ledPin1, LOW);
          digitalWrite(ledPin2, HIGH);
          ledAtivo = false;
    }
    else
    {
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, LOW);
      ledAtivo = true;
    }
    delay(100);
  } else {//desativado
    noTone(buzzer);
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
  }
    
}
