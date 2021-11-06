//
// Tiny slcan for attiny1614                    // AUGUST 2021
//
// arduino ide                                  // 1.8.13 windows
// megatinycore                                 // version 2.4.0
// BOD                  1.8v
// BOD mode (sleep)     disabled
// BOD mode (active)    disabled
// Clock speed          20MHz
// millis()/micros()    enabled
// v for UART           closer to 3.3v
// UPDI/reset           UPDI
// UART for bootloader  TX:5,RX:4
//

#include <mcp_can.h>                            // version 25/09/17 from https://github.com/coryjfowler/MCP_CAN_lib
#include <SPI.h>                                // version 1.0.0

boolean       working   = false;
boolean       timestamp = false;
boolean       cr        = false;

int           can_speed = 500;
long          ser_speed = 500000;         

long unsigned int   rxId;
unsigned char       len = 0;
unsigned char       rxBuf[8];

#define             LED2      7                 // PB0 SLCAN active
#define             LED3      6                 // PB1 SPARE
#define             LED4      2                 // PA6 TX message
#define             LED5      3                 // PA7 RX message

#define             CAN0_INT  1                 // Set INT to pin 1
MCP_CAN             CAN0(0);                    // Set CS to pin 0

static uint8_t hexval[17] = "0123456789ABCDEF";

//----------------------------------------------------------------

void slcan_ack()
{
  Serial.write('\r');
} // slcan_ack

//----------------------------------------------------------------

void slcan_nack()
{
  Serial.write('\a');
} // slcan_nack

//----------------------------------------------------------------

void pars_slcancmd(char *buf)
{                                               // LAWICEL PROTOCOL
  switch (buf[0]) {
    case 'O':                                   // OPEN CAN
      working=true;
      PORTB.OUTSET = PIN0_bm;                   // digitalWrite(LED2,true);
      switch (can_speed) {
        case 100:
          if(CAN0.begin(MCP_ANY, CAN_100KBPS, MCP_8MHZ) == CAN_OK)
          {
            CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.
            slcan_ack();
          } else {
            slcan_nack();
          }
          break;
        case 125:
          if(CAN0.begin(MCP_ANY, CAN_125KBPS, MCP_8MHZ) == CAN_OK)
          {
            CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.
            slcan_ack();
          } else {
            slcan_nack();
          }
          break;
        case 250:
          if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK)
          {
            CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.
            slcan_ack();
          } else {
            slcan_nack();
          }
          break;
        case 500:
          if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
          {
            CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.
            slcan_ack();
          } else {
            slcan_nack();
          }
          break;
        case 1000:
          if(CAN0.begin(MCP_ANY, CAN_1000KBPS, MCP_8MHZ) == CAN_OK)
          {
            CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.
            slcan_ack();
          } else {
            slcan_nack();
          }
          break;
        default:
          slcan_nack();
          break;
      }    
      break;
    case 'C':                                   // CLOSE CAN
      working=false;
      PORTB.OUTCLR = PIN0_bm;                   // digitalWrite(LED2,false);
      delay(1000);
      asm volatile ("  jmp 0"::);
      slcan_ack();
      break;
    case 't':                                   // SEND STD FRAME
      send_canmsg(buf,false,false);
      slcan_ack();
      break;
    case 'T':                                   // SEND EXT FRAME
      send_canmsg(buf,false,true);
      slcan_ack();
      break;
    case 'r':                                   // SEND STD RTR FRAME
      send_canmsg(buf,true,false);
      slcan_ack();
      break;
    case 'R':                                   // SEND EXT RTR FRAME
      send_canmsg(buf,true,true);
      slcan_ack();
      break;
    case 'Z':                                   // ENABLE TIMESTAMPS
      switch (buf[1]) {
        case '0':                               // TIMESTAMP OFF  
          timestamp = false;
          slcan_ack();
          break;
        case '1':                               // TIMESTAMP ON
          timestamp = true;
          slcan_ack();
          break;
        default:
          break;
      }
      break;
    case 'M':                                   // set ACCEPTANCE CODE ACn REG
      slcan_ack();
      break;
    case 'm':                                   // set ACCEPTANCE CODE AMn REG
      slcan_ack();
      break;
    case 's':                                   // CUSTOM CAN bit-rate
      slcan_nack();
      break;
    case 'S':                                   // CAN bit-rate
      switch (buf[1]) {
        case '0':                               // 10k  
          slcan_nack();
          break;
        case '1':                               // 20k
          slcan_nack();
          break;
        case '2':                               // 50k
          slcan_nack();
          break;
        case '3':                               // 100k
          can_speed = 100;
          slcan_ack();
          break;
        case '4':                               // 125k
          can_speed = 125;
          slcan_ack();
          break;
        case '5':                               // 250k
          can_speed = 250;
         slcan_ack();
          break;
        case '6':                               // 500k
          can_speed = 500;
          slcan_ack();
          break;
        case '7':                               // 800k
          slcan_nack();
          break;
        case '8':                               // 1000k
          can_speed = 1000;
          slcan_ack();
          break;
        default:
          slcan_nack();
          break;
      }
      break;
    case 'F':                                   // STATUS FLAGS
      Serial.print("F00");
      slcan_ack();
      break;
    case 'V':                                   // VERSION NUMBER
      Serial.print("V082021-PLUG");
      slcan_ack();
      break;
    case 'N':                                   // SERIAL NUMBER
      Serial.print("N2208");
      slcan_ack();
      break;
    case 'l':                                   // (NOT SPEC) TOGGLE LINE FEED ON SERIAL
      cr = !cr;
      slcan_nack();
      break;
    case 'H':                                   // (NOT SPEC) CAN UTILS INIT SCRIPT
      Serial.println(F("stty -F /dev/ttyUSB0 speed 500000 raw"));
      Serial.println(F("stty -F /dev/ttyUSB0 speed 500000 raw"));
      Serial.println(F("echo 'C\r' > /dev/ttyUSB0"));
      Serial.println(F("echo 'h\r' > /dev/ttyUSB0"));
      Serial.println(F("echo 'h\r' > /dev/ttyUSB0"));
      Serial.println(F("echo 'h\r' > /dev/ttyUSB0"));
      Serial.println(F("echo 'C\r' > /dev/ttyUSB0"));
      Serial.println(F("echo 'Press any key'"));
      Serial.println(F("slcand -o -c -s6 -S 500000 /dev/ttyUSB0 can0"));
      Serial.println(F("read -n1"));
      Serial.println(F("ip link set up can0"));
      Serial.println(F("ifconfig can0 txqueuelen 1000"));
      Serial.println(F("pkill slcand"));
      Serial.println(F("ip link set can0 down"));
      Serial.println(F("stty -F /dev/ttyUSB0 speed 500000 raw"));
      Serial.println(F("echo 'C\r' > /dev/ttyUSB0"));
      Serial.println(F("echo 'h\r' > /dev/ttyUSB0"));
      Serial.println(F("echo 'h\r' > /dev/ttyUSB0"));
      Serial.println(F("echo 'h\r' > /dev/ttyUSB0"));
      Serial.println(F("echo 'C\r' > /dev/ttyUSB0"));
      Serial.println(F("stty -F /dev/ttyUSB0 speed 500000 raw"));
      Serial.println(F("stty -F /dev/ttyUSB0 speed 500000 raw"));
      Serial.println(F("echo 'C\r' > /dev/ttyUSB0"));
      Serial.println(F("echo 'h\r' > /dev/ttyUSB0"));
      Serial.println(F("echo 'h\r' > /dev/ttyUSB0"));
      Serial.println(F("echo 'h\r' > /dev/ttyUSB0"));
      Serial.println(F("echo 'C\r' > /dev/ttyUSB0"));
      Serial.println(F("slcand -o -c -s6 -S 500000 /dev/ttyUSB0 can0"));
      Serial.println(F("read -n1"));
      Serial.println(F("echo 'Press any key'"));
      Serial.println(F("ip link set up can0"));
      Serial.println(F("ifconfig can0 txqueuelen 1000"));
      slcan_nack();
      break;
    case 'h':                                   // (NOT SPEC) HELP SERIAL
      Serial.println();
      Serial.println(F("mintynet.com - slcan nano"));
      Serial.println();
      Serial.println(F("O\t=\tStart slcan"));
      Serial.println(F("C\t=\tStop slcan (reboots device)"));
      Serial.println(F("t\t=\tSend std frame"));
      Serial.println(F("r\t=\tSend std rtr frame"));
      Serial.println(F("T\t=\tSend ext frame"));
      Serial.println(F("R\t=\tSend ext rtr frame"));
      Serial.println(F("Z0\t=\tTimestamp Off"));
      Serial.println(F("Z1\t=\tTimestamp On"));
      Serial.println(F("snn\t=\tSpeed 0xnnk N/A"));
      Serial.println(F("S0\t=\tSpeed 10k N/A"));
      Serial.println(F("S1\t=\tSpeed 20k N/A"));
      Serial.println(F("S2\t=\tSpeed 50k N/A"));
      Serial.println(F("S3\t=\tSpeed 100k"));
      Serial.println(F("S4\t=\tSpeed 125k"));
      Serial.println(F("S5\t=\tSpeed 250k"));
      Serial.println(F("S6\t=\tSpeed 500k"));
      Serial.println(F("S7\t=\tSpeed 800k N/A"));
      Serial.println(F("S8\t=\tSpeed 1000k"));
      Serial.println(F("F\t=\tFlags N/A"));
      Serial.println(F("N\t=\tSerial No"));
      Serial.println(F("V\t=\tVersion"));
      Serial.println(F("-----NOT SPEC-----"));
      Serial.println(F("H\t=\tCAN UTILS INIT SCRIPT"));
      Serial.println(F("h\t=\tHelp"));
      Serial.print(F("l\t=\tToggle CR "));
      if (cr) {
        Serial.println(F("ON"));
      } else {
        Serial.println(F("OFF"));
      }
      Serial.print(F("CAN_SPEED:\t"));
      switch(can_speed) {
        case 100:
          Serial.print(F("100"));
          break;
        case 125:
          Serial.print(F("125"));
          break;
        case 250:
          Serial.print(F("250"));
          break;
        case 500:
          Serial.print(F("500"));
          break;
        case 800:
          Serial.print(F("800"));
          break;
        case 1000:
          Serial.print(F("1000"));
          break;
        default:
          break;
      }
      Serial.print(F("kbps"));
      if (timestamp) {
        Serial.print(F("\tT"));
      }
      if (working) {
        Serial.print(F("\tON"));
      } else {
        Serial.print(F("\tOFF"));
      }
      Serial.println();
      Serial.println();
      Serial.println("-----EXAMPLES for 208-----");
      Serial.println("t0f68000007b1a0000000\t31337 odometer");
      Serial.println("t0b6800001e0000000000\t50mph speedo");
      Serial.println("t0b687e00000000000000\t4000rpm revcounter");
      Serial.println("t2606800000000000\tchange to kmh");
      Serial.println("t2606ffffffffffff\tchange to mph");
      Serial.println();
      slcan_nack();
      break;
    default:
      slcan_nack();
      break;
  }
} // pars_slcancmd()

//----------------------------------------------------------------

void transfer_tty2can()
{
  int ser_length;
  static char cmdbuf[32];
  static int cmdidx = 0;
  if ((ser_length = Serial.available()) > 0) {
    for (int i = 0; i < ser_length; i++) {
      char val = Serial.read();
      cmdbuf[cmdidx++] = val;
      if (cmdidx == 32)
      {
        slcan_nack();
        cmdidx = 0;
      } else if (val == '\r')
      {
        cmdbuf[cmdidx] = '\0';
        pars_slcancmd(cmdbuf);
        cmdidx = 0;
      }
    }
  }
} // transfer_tty2can()

//----------------------------------------------------------------

void transfer_can2tty()
{
  long time_now = 0;
  //receive next CAN frame from queue
  if(!digitalReadFast(CAN0_INT)) {
    //do stuff!
    if(working) {
  PORTA.OUTSET = PIN6_bm;
      PORTA.OUTTGL = PIN7_bm;                    // led5_status = !led5_status; digitalWrite(LED5,led5_status);
      CAN0.readMsgBuf(&rxId, &len, rxBuf);
      if((rxId & 0x80000000) == 0x80000000) {
        if ((rxId & 0x40000000) == 0x40000000) {
          Serial.printf("R");
        } else {
          Serial.printf("T");
        }
          Serial.printHex(rxId & 0x1fffffff);
          //Serial.printf("%c",char(hexval[ (rxId>>28)&1]));
          //Serial.printf("%c",char(hexval[ (rxId>>24)&15]));
          //Serial.printf("%c",char(hexval[ (rxId>>20)&15]));
          //Serial.printf("%c",char(hexval[ (rxId>>16)&15]));
          //Serial.printf("%c",char(hexval[ (rxId>>12)&15]));
          //Serial.printf("%c",char(hexval[ (rxId>>8)&15]));
          //Serial.printf("%c",char(hexval[ (rxId>>4)&15]));
          //Serial.printf("%c",char(hexval[ rxId&15]));
          Serial.printf("%c",char(hexval[ len ]));
      } else {
        if ((rxId & 0x40000000) == 0x40000000) {
          Serial.printf("r");
        } else {
          Serial.printf("t");
        }
          Serial.printf("%c",char(hexval[ (rxId>>8)&15]));
          Serial.printf("%c",char(hexval[ (rxId>>4)&15]));
          Serial.printf("%c",char(hexval[ rxId&15]));
          Serial.printf("%c",char(hexval[ len ]));
      }
      for(int i = 0; i < len; i++){
        Serial.printHex(rxBuf[i]);
        //Serial.printf("%c",char(hexval[ rxBuf[i]>>4 ]));
        //Serial.printf("%c",char(hexval[ rxBuf[i]&15 ]));
        //printf("%c\t", (char)rx_frame.data.u8[i]);
      }
      if (timestamp) {
        time_now = millis() % 60000;
        Serial.printHex(uint16_t (time_now));
        //Serial.printf("%c",char(hexval[ (time_now>>12)&15 ]));
        //Serial.printf("%c",char(hexval[ (time_now>>8)&15 ]));
        //Serial.printf("%c",char(hexval[ (time_now>>4)&15 ]));
        //Serial.printf("%c",char(hexval[ time_now&15 ]));
      }
        Serial.printf("%c",'\r');
      if (cr) Serial.println("");
  PORTA.OUTCLR = PIN6_bm;
    }
  }
} // transfer_can2tty()

//----------------------------------------------------------------

void send_canmsg(char *buf, boolean rtr, boolean ext) {
  if (!working) {

  } else {
    PORTA.OUTTGL = PIN6_bm;                    // led4_status = !led4_status; digitalWrite(LED4,led4_status);
    int msgID = 0;
    int msgDLC = 0;
    boolean msgEXT = false;
    int msg_id = 0;
    int msg_ide = 0;
    if (rtr) {
      if (ext) {
        sscanf(&buf[1], "%04x%04x", &msg_ide, &msg_id);
        msg_ide = msg_ide | 0x4000;
        msgEXT = true;
      } else {
        sscanf(&buf[1], "%03x", &msg_id);
        msg_ide = msg_ide | 0x4000;
        msgEXT = false;
      }
    } else {
      if (ext) {
        sscanf(&buf[1], "%04x%04x", &msg_ide, &msg_id);
        msgEXT = true;
      } else {
        sscanf(&buf[1], "%03x", &msg_id);
        msgEXT = false;
      }
    }
    msgID = msg_ide*65536 + msg_id;
    int msg_len = 0;
    if (ext) {
      sscanf(&buf[9], "%01x", &msg_len);
    } else {
      sscanf(&buf[4], "%01x", &msg_len);
    }
    msgDLC = msg_len;
    int candata = 0;
    byte data[8] = {0,0,0,0,0,0,0,0};
    if (ext) {
      for (int i = 0; i < msg_len; i++) {
        sscanf(&buf[10 + (i*2)], "%02x", &candata);
        data[i] = candata;
      }
    } else {
      for (int i = 0; i < msg_len; i++) {
        sscanf(&buf[5 + (i*2)], "%02x", &candata);
        data[i] = candata;
      }
    }
/*    Serial.print("msgID:");
    Serial.print(msgID,HEX);
    Serial.print(" len:");
    Serial.print(msgDLC);
    Serial.print(" ");
    if (data[0]<16) Serial.print("0");
    Serial.print(data[0],HEX);
    if (data[1]<16) Serial.print("0");
    Serial.print(data[1],HEX);
    if (data[2]<16) Serial.print("0");
    Serial.print(data[2],HEX);
    if (data[3]<16) Serial.print("0");
    Serial.print(data[3],HEX);
    if (data[4]<16) Serial.print("0");
    Serial.print(data[4],HEX);
    if (data[5]<16) Serial.print("0");
    Serial.print(data[5],HEX);
    if (data[6]<16) Serial.print("0");
    Serial.print(data[6],HEX);
    if (data[7]<16) Serial.print("0");
    Serial.print(data[7],HEX);
    Serial.println();*/
    byte sndStat = CAN0.sendMsgBuf(msgID, msgEXT, msgDLC, data);
    if(sndStat == CAN_OK){
      slcan_ack();
    } else {
      slcan_nack();
    }
  }
} // send_canmsg()

//----------------------------------------------------------------

void setup()
{
  Serial.begin(ser_speed);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(LED5,OUTPUT);
  //pars_slcancmd("O\0");
} //setup()

//----------------------------------------------------------------

void loop()
{
  transfer_tty2can();
  transfer_can2tty();
  //PORTA.OUTTGL = PIN6_bm;
  //send_canmsg("t0b687e00000000000000\0",false,false);
} //loop()
