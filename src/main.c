extern void scrollup(unsigned int);
extern void print(char *);

extern kY;
extern kattr;

void main()
{
  kattr = 0x5E;
  print("Here is the main !\n");

  kattr = 0x4E;
  print("Hello world\n");
  
  while (1);
}

