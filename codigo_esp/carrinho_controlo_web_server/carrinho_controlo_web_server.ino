/*********
  Criado por: Rui Santos
  Com modificações por: offKevyn
*********/
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFiManager.h>


const char* ssid = "Backdoor Longeee";
const char* password = "Foda-se a senha";

AsyncWebServer server(80);

// Display the HTML web page
            const char index_html[] = R"rawliteral(
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
            <button class="btn btn_top" data-action="top">↑</button>
        </div>
        <div id="group3">
            <button class="btn btn_right" data-action="right">←</button>
            <button class="btn btn_led" data-action="led">❤</button>
            <button class="btn btn_left" data-action="left">→</button>
        </div>
        <div id="group3">
            <button class="btn btn_botton" data-action="botton">↓</button>
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
                window.navigator.vibrate(50);
                action_pressed = e;
            });

            element.addEventListener('touchend', release);
        });

        function btn_pressed() {
            console.log(action_pressed.target);
            //document.getElementById("t").innerHTML += "1";
            ///////
            window.navigator.vibrate(50);
            btn_select.style.top = "0";
        }

        function release() {
            if (intervalId != 0) {
                console.log('Soltou!');
                /////
                clearInterval(intervalId);
                intervalId = 0;
                btn_select.style.top = "-100%";
                btn_select.innerHTML = '';
            }
        }

        document.addEventListener('contextmenu', event => event.preventDefault());

    </script>
</body>



</html>
            )rawliteral";

            
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(9600);

  Serial.print("Connecting to ");
  /*Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("Connected.");*/

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  
  server.begin();
}

 
void loop() {
  
}
