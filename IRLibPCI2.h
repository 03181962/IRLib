// IRLibPCI2.h

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
