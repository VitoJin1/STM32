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
