const char PAGE_MAIN[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>PetFeederConnect</title>
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Dancing+Script:wght@500&family=Fira+Sans:ital,wght@0,500;1,400&display=swap" rel="stylesheet">
    <link href="https://fonts.googleapis.com/css2?family=Poppins:wght@400;700&display=swap" rel="stylesheet">
</head>

<!-- CSS -->
<style>
    :root {
    --white: #FFFFFF;
    --gray: #F3F3F5;
    --dark-gray: #858383;
    --main: #FECF00;
    --black: #3E3D3F;
    --green: #08BC08;
    --red: #F00E0E;

    --logo-font: 'Dancing Script', cursive;
    --text-font: 'Poppins', sans-serif;
    }

    html {
        font-size: 62.5%;
        box-sizing: border-box;
    }

    *, *:before, *:after {
        margin: 0;
        padding: 0;
        box-sizing: border-box;
    }

    body {
        position: relative;
        background-color: var(--gray);
        display: block;
        flex-direction: column;
        width: 100vw;
        height: 100vh;
    }

    .screen-msg {
        display: flex;
        position: absolute;
        top: -10%;
        left: 50%;
        transform: translate(-50%, 10%);
        padding: 1rem 2rem;
        border-radius: 2rem;
        color: var(--white);
        align-items: center;
        justify-content:space-around;
        transition: top 0.2s ease, transform 0.2s ease;
    }

    .screen-msg h1 {
        font-size: 1.5rem;
        font-family: var(--text-font);
        font-weight: 600;
        margin-right: 2rem;
    }

    .screen-msg img {
        filter: invert(100%) sepia(100%) saturate(0%) hue-rotate(258deg) brightness(101%) contrast(102%);
        width: 3rem;
    }

    .screen-msg.active {
        top: 5%;
        left: 50%;
        transform: translate(-50%, -5%);
        
    }

    #msg-succeeded {
        background-color: var(--green);
    }

    #msg-failed {
        background-color: var(--red);
    }

    .main-container {
        position: absolute;
        display: block;
        border-radius: 2rem;
        top: 50%;
        left: 50%;
        transform: translate(-50%, -50%);
        padding: 3rem;
        box-shadow:  9px 9px 18px #d8d8da, -9px -9px 18px #ffffff;
    }

    .main-container h1 {
        margin: 2rem 0;
        font-family: var(--logo-font);
        font-size: 5rem;
    }

    .logo {
        display: flex;
        justify-content: center;
    }

    .tag1 {
        color: var(--main);
    }

    .tag2 {
        color: var(--dark-gray);
    }

    .form-container {
        display: flex;
        flex-direction: column;
    }

    .text-container {
        display: flex;
        align-items: center;
        justify-content: space-between;
        margin: 1rem 0;
    }

    label {
        font-family: var(--text-font);
        font-size: 1.5rem;
        font-weight: 400;
        color: var(--main);
    }

    input {
        border-color: transparent;
        border-radius: 2rem;
        outline: 0.2rem solid transparent;
        width: 30rem;
        height: 3.5rem;
        font-size: 1.5rem;
        font-family: var(--text-font);
        margin: 0 0 0 2rem;
        padding: 1rem;
        transition: outline 0.3s ease;
    }

    input:focus {
        outline: 0.2rem solid var(--main);
    }

    ::placeholder {
        color: #A3A3A3;
    }

    .button-container {
        display: flex;
        justify-content: space-between;
        width: 100%;
    }

    .connect-status {
        display: flex;
        visibility: hidden;
        align-items: center;
    }

    .connect-status.visible {
        visibility: visible;
    }

    .loader {
        position: relative;
        width: 4rem;
        height: 4rem;
        border-radius: 50%;
    }

    .loader::after,
    .loader::before {
        content: '';
        animation: loader 2s linear infinite;
        border: 3px solid var(--main);
        border-color: inherit;
        width: 4rem;
        height: 4rem;
        position: absolute;
        border-radius: 50%;
        border-color: var(--main);
    }

    .loader::after {
        opacity: 0;
        animation-delay: 1s;
    }

    .connect-message {
        margin-left: 1rem;
        color: var(--black);
        font-family: var(--text-font);
        font-size: 1.5rem;
    }

    @keyframes loader {
        0% {
            transform: scale(0);
            opacity: 1;
        }

        100% {
            transform: scale(1);
            opacity: 0;
        }
    }


    button {
        background: linear-gradient(145deg, #ffdd00, #e5ba00);
        border: none;
        border-radius: 1rem;
        height: 4rem;
        width: 16rem;
        margin-top: 1rem;
        padding: .5rem 2rem;
        font-family: var(--text-font);
        font-size: 2rem;
        font-weight: 500;
        box-shadow:  5px 5px 10px #c5c5c6, -5px -5px 10px #ffffff;
        transition: font-size 0.2s ease, background 0.5s ease, width 0.2s ease, margin-right 0.2s ease;
    }

    button:hover {
        cursor: pointer;
        font-size: 1.9rem;
        background: linear-gradient(145deg, #e5ba00, #ffdd00);
    }

    button:active {
        font-size: 1.8rem;
        width: 14rem;
        margin-right: 1rem;
    }

    @media (max-width: 700px) {

        .text-container {
            flex-direction: column;
            justify-content: left;
            margin: .5rem 0;
        }

        label {
            width: 100%;
            margin: .5rem 0;
        }

        input {
            margin: 0;
        }

        .button-container {
            margin-top: 1.5rem;
            flex-direction: column;
            align-items: center;
        }

        button {
            margin-top: 2rem;
        }
    }

    @media (max-width: 500px) {

        .main-container {
            box-shadow:  none;
        }
    }
</style>

<!-- HTML -->
<body onload="process()">
    <div class="screen-msg" id="msg-succeeded">
        <h1>Connection Successful!</h1>
        <img src="/icons/circle-check-solid.svg" alt="Successful">
    </div>

    <div class="screen-msg" id="msg-failed">
        <h1>Connection Failed!</h1>
        <img src="/icons/circle-xmark-solid.svg" alt="Failed">
    </div>

    <!-- Main Container -->
    <div class="main-container">
        <!-- Logo -->
        <div class="logo">
            <h1 class="tag1">PetFeeder</h1>
            <h1 class="tag2">Connect</h1>
        </div>

        <div id="network-list">

        </div>

        <!-- Form -->
        <div class="form-container">
            <div class="text-container">
                <label for="username-tb">Network:</label>
                <input type="text" id="username-tb" placeholder="Your Network Name">
            </div>
    
            <div class="text-container">
                <label for="password-tb">Password:</label>
                <input type="password" id="password-tb" placeholder="Your Network Password">
            </div>
        </div>

        <!-- Button  -->
        <div class="button-container">
            <div class="connect-status" id="connect-status">
                <span class="loader" id="loader"></span>
                <span class="connect-message" id="connect-message">Connecting</span>
            </div>
            <button class="connect-button" id="connect-button" onclick="clickConnect(document.getElementById('username-tb').value, document.getElementById('password-tb').value)">Connect</button>
        </div>
    </div>
</body>

<!-- JavaScript -->
<script>
    const button = document.getElementById("connect-button");
    const connect_status = document.getElementById("connect-status");
    const msg_failed = document.getElementById("msg-failed");
    const msg_succeeded = document.getElementById("msg-succeeded");
    const screen_msgs = document.getElementsByClassName("screen-msg");

    var xmlHttp=createXmlHttpObject();
    var connection_status = 1;

    function response(){
      var message;
      var xmlResponse;
      var xmldoc;

      xmlResponse=xmlHttp.responseXML;
      if (xmlResponse) {
        xmldoc = xmlResponse.getElementsByTagName("C");
        if (xmldoc.length > 0) {
          message = xmldoc[0].firstChild.nodeValue;
          if (message == "Connected successfully") {
            connect_status.classList.remove("visible");
            msg_succeeded.classList.add("active");
            xhttp.open("PUT", "Connected?VALUE=true", true);
            xhttp.send();
          } else if (message == "Connection failed"){
            connect_status.classList.remove("visible");
            msg_failed.classList.remove("active");
          } else if (message == "Connecting..."){
            connect_status.classList.add("visible");
            screen_msgs.classList.remove("active");
          } else {
            connect_status.classList.remove("visible");
            screen_msgs.classList.remove("active");
          }

          if (message == "Connecting...") {
            button.style.background = "#adadad";
          } else {
            button.style.background = "var(--main)";
          }
        }
      }
    }

    function createXmlHttpObject(){
      if(window.XMLHttpRequest){
          xmlHttp=new XMLHttpRequest();
      }
      else {
          xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
      }
      return xmlHttp;
    }

    function clickConnect(network, password) {
        WiFiNetwork(network);
        WiFiPassword(password);
        
        response();
    }

    function WiFiNetwork(network) {
    var xhttp = new XMLHttpRequest();
    xhttp.open("PUT", "WiFiNetwork?VALUE="+network, true);
    xhttp.send();
    }

    function WiFiPassword(password) {
    var xhttp = new XMLHttpRequest();
    xhttp.open("PUT", "WiFiPassword?VALUE="+password, true);
    xhttp.send();
    }

    function process(){
        if(xmlHttp.readyState==0 || xmlHttp.readyState==4) {
            xmlHttp.open("PUT","xml",true);
            xmlHttp.onreadystatechange=response;
            xmlHttp.send(null);
        }       
        setTimeout("process()",200);
    }

</script>


</html>
)=====";