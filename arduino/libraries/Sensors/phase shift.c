float pshift(float amps[], float volts[]){
float maxv[3], maxa[3], minv[3], mina[3]; //index 0 = index, 1 = max, 2 = follow
maxv[1]=maxa[1]=0;
minv[1]=mina[1]=1000;
maxv[2]=minv[2]=maxa[2]=mina[2]=0;
int i;

for(i=0;i<100;i++){
if(volts[i]>maxv[1]){
maxv[0]=i;
maxv[1]=volts[i];
maxv[2]=1;
}
else if(volts[i]<minv[1]){
minv[0]=i;
minv[1]=volts[i];
minv[2]=1;
}
else if(maxv[2]==1) maxv[2]=volts[i];
else if(maxv[2]!=1 && maxv[2]!=0){
if(volts[i]<maxv[2]) maxv[2]=0;
else maxv[1]=volts[i];
}
else if(minv[2]==1) minv[2]=volts[i];
else if(minv[2]!=1 && minv[2]!=0){
if(volts[i]>minv[2]) minv[2]=0;
else minv[1]=volts[i];
}

if(amps[i]>maxa[1]){
maxa[0]=i;
maxa[1]=amps[i];
maxa[2]=1;
}
else if(amps[i]<mina[1]){
mina[0]=i;
mina[1]=amps[i];
mina[2]=1;
}
else if(maxa[2]==1) maxa[2]=volts[i];
else if(maxa[2]!=1 && maxa[2]!=0){
if(volts[i]<maxa[2]) maxa[2]=0;
else maxa[1]=volts[i];
}
else if(mina[2]==1) mina[2]=volts[i];
else if(mina[2]!=1 && mina[2]!=0){
if(volts[i]>mina[2]) mina[2]=0;
else mina[1]=volts[i];
}

}

float period = (maxa[0]-mina[0]+maxv[0]-minv[0])/2;
float phaseshift = (maxa[0]-maxv[0])*3.14159265359/period;
return phaseshift;
}