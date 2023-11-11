#if defined(ESP8266)

void startLD1115H(){
  if (dataStart){
    //Sending a request to receive sensor settings

    String s3 = "get_all \n";

    byte bufC[s3.length() + 1];
    s3.getBytes(bufC, s3.length() + 1);
    msSerial.write(bufC, s3.length() + 1);

    #ifdef DEBUG
    Serial.println("get_all");
    #endif

    dataStart = false;
  }
  //Getting the sensor settings at startup
}

void saveSetLD1115H(){

  String s3 = "save \n";
  int s3Int = s3.length() + 1;
  byte bufC3[s3Int];
  s3.getBytes(bufC3, s3Int);

  String s2 = "th1="+String(numTh1Sens)+" \n";

  int s2Int = s2.length() + 1;

  byte bufC1[s2Int];
  s2.getBytes(bufC1, s2Int);
  msSerial.write(bufC1, s2Int);
  msSerial.write(bufC3, s3Int);

  s2 = "th2="+String(numTh2Sens)+" \n";
  s2Int = s2.length() + 1;
  byte bufC2[s2Int];
  s2.getBytes(bufC2, s2Int);
  msSerial.write(bufC2, s2Int);
  msSerial.write(bufC3, s3Int);

  butSave = false;
  dataStart = true; //Reading the settings from the sensor

}

void defSetLD1115H(){
  // default settings
  // th1=120
  // th2=250
  // dtime=5
  // mov_sn=3
  // occ_sn=5

  if (butDefault){
    //We send a request to reset the sensor settings
    String s2 = "initial \n";

    #ifdef DEBUG
    Serial.println("initial \n");
    #endif

    byte bufC[s2.length() + 1];
    s2.getBytes(bufC, s2.length() + 1);
    msSerial.write(bufC, s2.length() + 1);

    String s3 = "save \n";

    #ifdef DEBUG
    Serial.println("save \n");
    #endif

    byte bufC3[s3.length() + 1];
    s3.getBytes(bufC3, s3.length() + 1);
    msSerial.write(bufC3, s3.length() + 1);

    numTh1Sens = 120;
    numTh2Sens = 250;

    numTh1.setState(numTh1Sens);
    numTh2.setState(numTh2Sens);

    saveSetLD1115H();
    butDefault = false;
    dataStart = true; //Reading the settings from the sensor
  }
}

void dataLD1115H(){

  int dataNum = msSerial.available(); // number of bytes available in Serial buffer

  startLD1115H();

  if (dataNum > 0) {
    char dataBuf[dataNum];
    // read the incoming bytes:
    msSerial.readBytes(dataBuf, dataNum);
    String endMarker = "\n";

    for(int i = 0; i < dataNum; i++){

      char dataCh = dataBuf[i];
      String dataSt = String(dataCh);

      if (dataSt != endMarker){
        //Assembling the string while waiting for the termination character 
        dataRadar = dataRadar + dataSt;

        int strParsN = dataRadar.length();

        if (strParsN == 4){
          if (dataRadar == "mov,"){
            #ifdef DEBUG
            Serial.println("mov");
            #endif
            lastStateMotionN = true;
            lastStateOccupancyN = true;
          }else{ 
            lastStateMotionN = false;
            if(dataRadar == "occ,"){
              #ifdef DEBUG
              Serial.println("occ");
              #endif
              lastStateOccupancyN = true;
            }else{
              if(dataRadar == "null"){
                #ifdef DEBUG
                Serial.println("null");
                #endif
                lastStateMotionN = false;
                lastStateOccupancyN = false;
              }
            }
          }
        }else{
          if(strParsN == 7){
            if(dataRadar == "th1 is "){
              th1Is = true;
            }else{
              if(dataRadar == "th2 is "){
                th2Is = true;
              }
            }
          }
          // else{//
          //   if(strParsN == 10){
          //     if(dataRadar == "mov_sn is "){
          //       movSnIs = true;
          //     }else{//
          //       if(dataRadar == "occ_sn is "){
          //         occSnIs = true;
          //       }
          //     }
          //   }
          // }
        }
      }else{
      //The string is completely assembled, you can parse it

        #ifdef DEBUG
        Serial.println(dataRadar);
        #endif

        //Data that comes
        // th1 is 120
        // th2 is 250
        // dtime is 5
        // mov_sn is 3
        // occ_sn is 5
        // mov, 12 3456
        // occ, 12 34566
        // null
        // th1 is 130
        // th2 is 250

        //?         char dataRadarS = dataRadar.charAt(3);
        // take a string, find a character "="
        // Find out the symbol number
        // Compare characters before "=" and find out what the command is
        // And transfer the remaining characters to the command variable

        // String Mov = "mov";
        // String Occ = "occ";
        // String Nul = "nul";

        //Sending movement and presence status to HA
        if (tmr1.tick()){
          if(lastStateMotion != lastStateMotionN){
            lastStateMotion = lastStateMotionN;
            motionSensor.setState(lastStateMotion);
            lastStateMotion = motionSensor.getCurrentState();
            redLed();
          }
          
          if(lastStateOccupancy != lastStateOccupancyN){
            lastStateOccupancy = lastStateOccupancyN;
            occupancySensor.setState(lastStateOccupancy);
            lastStateOccupancy = occupancySensor.getCurrentState();
            redLed();
          }
        }
        //---------------------------------
        //-
        // th1Is
        // th2Is
        // movSnIs
        // occSnIs
        //-Checking the sensor parameters

        if (th1Is){
          String dataRadarTh1 = dataRadar;
          dataRadarTh1.remove(0, 6);
          numTh1Sens = dataRadarTh1.toInt();
          numTh1.setState(numTh1Sens);
          #ifdef DEBUG
          Serial.println("th1 is "+numTh1Sens);//
          #endif
          th1Is = false;
        }
        if (th2Is){
          String dataRadarTh2 = dataRadar;
          dataRadarTh2.remove(0, 6);
          numTh2Sens = dataRadarTh2.toInt();
          numTh2.setState(numTh2Sens);
          th2Is = false;
          #ifdef DEBUG
          Serial.println("th2 is "+dataRadarTh2);
          #endif
        }
        // if (movSnIs){   
        //   String dataRadarmovSnIs = dataRadar;      
        //   dataRadarmovSnIs.remove(0, 10);
        //   numMovsnSens = dataRadarmovSnIs.toInt();
        //   numMovsn.setState(numMovsnSens);
        //   movSnIs = false;
        //   #ifdef DEBUG
        //   Serial.println("Mov_sn is "+dataRadarmovSnIs);
        //   #endif
        // }
        // if (occSnIs){      
        //   String dataRadaroccSnIs = dataRadar;   
        //   dataRadaroccSnIs.remove(0, 10);
        //   numOccsnSens = dataRadaroccSnIs.toInt();
        //   numOccsn.setState(numOccsnSens);
        //   movSnIs = false;
        //   #ifdef DEBUG
        //   Serial.println("Occ_sn is "+dataRadaroccSnIs);
        //   #endif
        // }

        dataRadar = ""; //Clearing the line
      

      }
    }
  }
}

#endif