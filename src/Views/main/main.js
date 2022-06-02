import { Button, Progress } from '@chakra-ui/react';
import React, { useState } from 'react';
import { styles } from './styles';
import * as mqtt from 'mqtt/dist/mqtt';

let client = mqtt.connect('ws://test.mosquitto.org:8080/mqtt') // create a client

const MainPage = () => {
  var [count, setCount] = useState(0)
  var caca = 'white'
  const [isStyle, setStyle] = useState('white');
  isStyle && (caca = 'skyblue')
  let valGlobal = 0


      //subscribirse al canal arg, mi canal    
    function EventoConectar() {
      console.log("Conectado a MQTT");
      client.subscribe("domotica/nivel-agua", function (err) {
        if (!err) {
         // client.publish("manu/waterlevel", "jalando al 100");
        }
      });
    }

    //función que recibe los mensajes del broker que son del topic de mi interés
    function EventoMensaje(topic, message) {
      if (topic === "domotica/nivel-agua") {
        
      }
      console.log(topic + " - " + message);
      setCount(parseInt(message))
      // client.end()
    }

    //Conectares al broker
    client.on("connect", EventoConectar);

    //Ponerse a la escucha de la llegada de mensajes del broker
    client.on("message", EventoMensaje);

  const publicar = () =>{
    client.publish("domotica/nivel-agua", 'get')
  }

  return(
    <>
    <div style={styles.container}>
      <div>
        <Progress 
          value={count} //barra nivel
          style={styles.bar} 
          backgroundColor={caca}
          colorScheme={'facebook'}
          transition={'1s'}
          onMouseEnter={() => setStyle(true)}
          onMouseLeave={() => setStyle(false)}
          />
        <p>Yoy click {count}</p>
        <Button 
          style={styles.button} 
          colorScheme={'facebook'} 
          onClick={() => {
            publicar()
            }
          }
          >
          Get The Water Level
          </Button>
      </div>
      <div style={styles.credits}>
        <p style={styles.creditsTitle}>Created by:</p>
        <p>Manuel Eduardo Rios Martinez</p>
        <p>Mezly Zahory Mondragon Delgado</p>
        <p>Angel Abraham Flores Ramirez</p>
        <p>Regina Espinosa Gonzalez</p>
        <p>Carlos Antonio Ramirez</p>
        <p>Javier Anastasio Barreto Martinez</p>
        <p style={styles.creditsTitle}>Designed by:</p>
        <p>Manuel Eduardo Rios Martinez</p>
      </div>
    </div>
    </>
  )
}

const GetValue = () => {
  const RandomValue = Math.floor(Math.random()*[100-0]+0)
  return(
    RandomValue
  )
}

export default MainPage