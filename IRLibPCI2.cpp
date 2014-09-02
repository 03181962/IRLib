// IRLibPCI2.cpp

IRrecvPCI2::IRrecvPCI2(unsigned char inum) {
  Init();
  switch(intrnum = inum) {
#if defined(__AVR_ATmega32U4__) //Assume Arduino Leonardo
    case 0: irparams.recvpin = 3; break;
    case 1: irparams.recvpin = 2; break;
    case 2: irparams.recvpin = 0; break;
    case 3: irparams.recvpin = 1; break;
    case 4: irparams.recvpin = 7; break;
#else //Arduino Uno or Mega 
    case 0: irparams.recvpin = 2; break;
    case 1: irparams.recvpin = 3; break;
  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) //Mega only
    case 2: irparams.recvpin = 21; break;
    case 3: irparams.recvpin = 20; break;
    case 4: irparams.recvpin = 19; break;
    case 5: irparams.recvpin = 18; break;
  #endif
#endif
    //Illegal value to flag you that something is wrong
    default:  irparams.recvpin = 255; 
  }
}

void IRrecvPCI2_Handler() { 
  unsigned long volatile ChangeTime = micros();
  unsigned long DeltaTime = ChangeTime-irparams.timer;
  switch(irparams.rcvstate) {
    case STATE_STOP: return;
    case STATE_RUNNING:
	  do_Blink();
      if (DeltaTime>10000) {
        irparams.rcvstate = STATE_STOP; 
        //Setting gap to 0 is a flag to let you know why we stopped For debugging purposes
        //irparams.rawbuf[0] = 0;
        return;
      };
      break;
    case STATE_IDLE:
       if(digitalRead(irparams.recvpin)) return; else irparams.rcvstate = STATE_RUNNING;
       break;
    // ignore other cases
    default: break;
  };
  irparams.rawbuf[irparams.rawlen] = DeltaTime;
  irparams.timer = ChangeTime;
  if(++irparams.rawlen >= RAWBUF) {
    irparams.rcvstate = STATE_STOP;
    //Setting gap to 1 is a flag to let you know why we stopped For debugging purposes
    //irparams.rawbuf[0] = 1;
  }
}

void IRrecvPCI2::resume(void) {
  irparams.rcvstate = STATE_IDLE;
  IRrecvBase::resume();
  irparams.timer = micros();
  attachInterrupt(intrnum, IRrecvPCI2_Handler, CHANGE);
};

bool IRrecvPCI2::GetResults(IRdecodeBase *decoder) {
  if(irparams.rcvstate == STATE_RUNNING) {
    unsigned long ChangeTime = irparams.timer;
    if( (micros()-ChangeTime) > 10000) {
      irparams.rcvstate = STATE_STOP;
      //Setting gap to 2 is a flag to let you know why we stopped For debugging purposes
      //irparams.rawbuf[0] = 2;
    }
  }
  if (irparams.rcvstate != STATE_STOP) return false;
  detachInterrupt(intrnum);
  IRrecvBase::GetResults(decoder);
  return true;
};
