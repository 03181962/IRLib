// IRLibPCI2.h

#ifndef IRLibPCI2_h
#define IRLibPCI2_h

class IRrecvPCI2: public IRrecvBase
{
public:
  //Note this is interrupt number not pin number
  IRrecvPCI2(unsigned char inum);
  bool GetResults(IRdecodeBase *decoder);
  void resume(void);
private:
  unsigned char intrnum;
};

#endif //IRLibPCI2_h
