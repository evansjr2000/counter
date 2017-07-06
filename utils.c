#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define LF 10
#define CR 13

void getword(char *word, char *line, char stop) {
  int x = 0,y;

  for(x=0;((line[x]) && (line[x] != stop));x++)
    word[x] = line[x];

  word[x] = '\0';
  if(line[x]) ++x;
  y=0;

  while((line[y++] = line[x++]));
}

char *makeword(char *line, char stop) {
  int x = 0,y;
  char *word = (char *) malloc(sizeof(char) * (strlen(line) + 1));

  for(x=0;((line[x]) && (line[x] != stop));x++)
    word[x] = line[x];

  word[x] = '\0';
  if(line[x]) ++x;
  y=0;

  while((line[y++] = line[x++]));
  return word;
}

char *fmakeword(FILE *f, char stop, int *cl) {
  int wsize;
  char *word;
  int ll;

  wsize = 102400;
  ll=0;
  word = (char *) malloc(sizeof(char) * (wsize + 1));

  while(1) {
    word[ll] = (char)fgetc(f);
    if(ll==wsize) {
      word[ll+1] = '\0';
      wsize+=102400;
      word = (char *)realloc(word,sizeof(char)*(wsize+1));
    }
    --(*cl);
    if((word[ll] == stop) || (feof(f)) || (!(*cl))) {
      if(word[ll] != stop) ll++;
      word[ll] = '\0';
      return word;
    }
    ++ll;
  }
}


void escape_url(char *url) {
  register int x,y,l;
  char c;
  char code[10];
  char new[256];

  l=strlen(url);
  for(y=0,x=0;x<l;++x) {
    if (isalnum(c=url[x])||c=='.'|| c=='/' || c=='-') {
      new[y]=url[x];y++;
    }
    else {
      sprintf(code,"%%%02X",c);
      strncpy(&new[y],code,3);
      y+=3;
    }
  }
  new[y] = '\0';
  strcpy(url,new);
}

/*
main (int argc,char **argv)
{
  char koko[512];
  int i=0;
  int c;

  koko[0]=0;
  if (argc!=2) {
    for (i=0;(c=getchar())!=EOF;i++) {
      koko[i]=c;
    }
    koko[i-1]=0;
  }
  else {
    strncpy(koko,argv[1],512);
    i=1;
  }
  if (!i)
    exit(1);
  (void)unescape_url(koko);
  printf("%s\n",koko);
  
}

*/

char x2c(char *what) {
  register char digit;

  digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
  digit *= 16;
  digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));
  return(digit);
}

void unescape_url(char *url) {
  register int x,y;

  for(x=0,y=0;url[y];++x,++y) {
    if((url[x] = url[y]) == '%') {
      url[x] = x2c(&url[y+1]);
      y+=2;
    }
  }
  url[x] = '\0';
}

void plustospace(char *str) {
  register int x;

  for(x=0;str[x];x++) if(str[x] == '+') str[x] = ' ';
}

int rind(char *s, char c) {
  register int x;
  for(x=strlen(s) - 1;x != -1; x--)
    if(s[x] == c) return x;
  return -1;
}

void tobasename(char *s) {
  register int i;
  int l;
  char buf[128];

  l=strlen(s)-1;

  for(i=l;i>0 && s[i]!='/'; i--);
  strcpy(buf,&s[i+1]);
  strcpy(s,buf);
}

void cutlast(char *s) {
  register int x,i;
  int l;

  l=strlen(s)-1;

  for(i=l;i>0 && s[i]=='/'; i--);
  for(x=i;x>0 && s[x]!='/'; x--) {
    if(s[x-1] == '/') {
      s[x]='\0';
      break;
    }
  }
}

int getline2(char *s, int n, FILE *f) {
  register int i=0;

  while(1) {
    s[i] = (char)fgetc(f);

    if(s[i] == CR)
      s[i] = fgetc(f);

    if((s[i] == 0x4) || (s[i] == LF) || (i == (n-1)) ||s[i]==EOF ) {
      s[i] = '\0';
      return (feof(f) ? 1 : 0);
    }
    ++i;
  }
}

void send_fd(FILE *f, FILE *fd)
{
  char c;

  while (1) {
    c = fgetc(f);
    if(feof(f))
      return;
    fputc(c,fd);
  }
}

void spacetoplus(char *str) {
  register int x;
  for(x=0;str[x];x++) if(str[x] == ' ') str[x] = '+';
}

void spacetounderscore(char *str) {
  register int x;
  for(x=0;str[x];x++) if(str[x] == ' ') str[x] = '_';
}
