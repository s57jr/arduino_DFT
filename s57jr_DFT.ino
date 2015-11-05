#include <avr/io.h>
#include <glcd.h>
#include <fonts/allFonts.h>
#include <avr/interrupt.h>



#define FS 50.0
#define N 128


double tds[N+(N/2)]={'\0'};
double tds1[N+(N/2)]={'\0'};
int i,j,maximum,c,minimum;
double hw[N]={0};
int32_t re[(N/2)]={'\0'};
int32_t im[(N/2)]={'\0'};
float maxx,tez,suma;
int nmax,obseg;
int maxi,mini;
int cc,wc=0;
int DCoffset;

void setup(){
   Serial.begin(115200);
   analogReference(INTERNAL2V56);
  
    GLCD.Init();
    GLCD.SelectFont(System5x7);
    GLCD.CursorTo(0,0);
    for(i=0;i<N;i++){hw[i]=(0.54-(0.46*cos(2*3.14*i/(N-1))));}
    for(i=0;i<N;i++){GLCD.DrawLine(i,63,i,63-hw[i]*10);}
    first_fill();

}


void offset(int ff){
  int off;
  mini=maxi=tds[ff];
  
  for(off=ff;off<N+ff;off++){
     if(maxi<tds[off]){
       maxi=tds[off];
     }
     if(mini>tds[off]){
       mini=tds[off];
     }
 }
}


void first_fill(){
  for(i=0;i<N;i++) {           
    tds[i] = analogRead(A0);
    //GLCD.DrawLine(i,63,i,63-(tds[i]/14));            
    delay(10);
  }
}


void dft(int dd){
  
  memset(re, 0, N/2);
  memset(im, 0, N/2);
  wc=0;
  
  for(cc=dd;cc<N+dd;cc++){
    tds1[cc]=tds1[cc]*hw[wc];
    wc++;
//    tds[cc]=tds[cc]*hw[wc];
  }
  maxx=0;

  nmax=0;

  GLCD.ClearScreen();
  GLCD.print(tez*50.0/(float)N);
  GLCD.CursorTo(10,0);
  GLCD.print(nmax);
  
  for(cc=0;cc<=N/2;cc++){
    for(j=dd;j<N+dd;j++){
      
      re[cc]=re[cc]+((tds[j]-DCoffset)*(cos(2*3.14159*cc*j/N))*40);
      im[cc]=im[cc]-((tds[j]-DCoffset)*(sin(2*3.14159*cc*j/N))*40);
   
    }
    re[cc]=re[cc]/(200*N);
    im[cc]=im[cc]/(200*N);
    if(re[cc]<0){re[cc]*=-1;}
    if(im[cc]<0){im[cc]*=-1;}
    Serial.print((im[cc]+re[cc]));Serial.print(' ');
    GLCD.DrawLine(cc*2,63,(cc)*2,63-((im[cc]+re[cc])));
    if ((im[cc]+re[cc])>maxx) {maxx=(im[cc]+re[cc]); nmax=cc;}
  }
  Serial.println(' ');

//izracunamo tezisce
  obseg=3;	//koliko binov pod/nad max uporabmo
  tez=0.0; suma=0.0;

  for (cc=nmax-obseg; cc<=nmax+obseg; cc++){

    tez+=(im[cc]+re[cc])*(float)cc;

    suma+=(im[cc]+re[cc]);
  }

  tez/=suma;
  GLCD.print(tez*50.0/(float)N);
  GLCD.CursorTo(10,0);
  GLCD.print(nmax);
}


void rewrite(){
  for(i=N/4;i<N;i++){
    tds[i-N/4]=tds[i];              
  }
}



void loop(){
  
    rewrite();
      
    for(i=N*(3/4);i<N;i++) {            
      tds[i] = analogRead(A0);
      //GLCD.DrawLine(i,63,i,63-(tds[i]/14));
      delay(1000/FS);     
    }
    offset(0);
    DCoffset=(mini+maxi)/2;
//    for(c=0;c<N;c++){
//      tds1[c]=tds[c]-DCoffset;
//    }
    dft(0);
//    rewrite();    
//    offset(0);
//    DCoffset=(mini+maxi)/2;
//    for(i=0;i<N;i++){
//      tds1[i]=tds[i]-DCoffset;

//    }
//    rewrite();
//    dft(0);
//   
 
//        for(i=0;i<N;i++) {
//            
//            GLCD.SetDot(i,(tds1[i]/14)+16, BLACK);
//            
//        }
  
  
//  for(i=0;i<(15);i+=1){
//    if(re[i]<0){re[i]*=-1;}
//    if(im[i]<0){im[i]*=-1;}
//    Serial.print((im[i]+re[i]));
//    GLCD.DrawLine(75+(i*3),63,75+(i*3),63-(im[i]+re[i]));
//    Serial.print(' ');
//  }

}
