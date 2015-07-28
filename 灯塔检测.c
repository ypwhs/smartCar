    int test = 0;
    int last = 0;
    
    while(1)
    {
        DelayUs(500);
        
        if(PBin(3)){
            if(test>0)test--;
        }
        else {
            test = 50;
        }
        
        PDout(10) = test>0;
        
        if((test>0)!=last){
            last = test>0;
            if(last){
                ENABLE_SERVO = 0;
                ENABLE_DRIVE = 0;
            }else{
                ENABLE_SERVO = 1;
                ENABLE_DRIVE = 1;
                DelayMs(1000);
            }
        }
        
    }