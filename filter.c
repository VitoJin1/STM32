/*4 order butterworth filter fs=1000 fc=500hz*/
float Numerator[5]={0.0009334986,0.00373399445,0.00560099167,0.00373399,0.0009334986};
float Denominator[5]={1,-2.97684433,3.422309529,-1.7861066,0.355577382};
float ButterWorthFilter( float x )
{
    
    float input_acc;
    float outout_acc;
    int i=0;
    for(i=4;i>0;i--)
    {
        yBuf[i]=yBuf[i-1];
        xBuf[i]=xBuf[i-1];
    }
    xBuf[0]=x;
    input_acc=Numerator[4]*xBuf[4]+Numerator[3]*xBuf[3]+Numerator[2]*xBuf[2]+Numerator[1]*xBuf[1]+Numerator[0]*xBuf[0];
    outout_acc=Denominator[4]*yBuf[4]+Denominator[3]*yBuf[3]+Denominator[2]*yBuf[2]+Denominator[1]*yBuf[1];
    yBuf[0]=input_acc-outout_acc;
    return yBuf[0];
}

first order low pass filter
void LPF_1orderRC_F_init(LPF_1orderRC_F *v)
{
    v->Vi=0.0;
    v->Vo_last=0.0;
    v->Vo=0.0;
    v->Fcutoff=30;
    v->Fs=1000;
}
float LPF_1orderRC_F_FUNC(LPF_1orderRC_F *v)
{
    float RC, Cof1,Cof2;
    RC=(float)1.0/2.0/PI/v->Fcutoff;
    Cof1=1/(1+RC*v->Fs);
    Cof2=RC*v->Fs/(1+RC*v->Fs);
    v->Vo=Cof1*v->Vi+Cof2*v->Vo_last;
    v->Vo_last=v->Vo;
    return v->Vo;
}
typedef struct{
    float Vi;
    float Vo_last;
    float Vo;
    float Fcutoff;
    float Fs;
}LPF_1orderRC_F;

大小端模式测试
int checkSystem( )
{
union check
{
int i;
char ch;
} c;
c.i = 1;
return (c.ch ==1);
}
