#include<stdio.h>
#include<unistd.h>

int main(int argc, char **argv)
{
printf("Welcome to OS Lab Course\n");
printf("Argc=%d",argc);
int i;
for(i=0;i<argc;i++)
{
printf("Argc[%d]=%s\n",i,argv[i]);
}
return 0;
}
