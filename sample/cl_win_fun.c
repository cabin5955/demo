
//---------------------------------------------------------------
U32 cl_get_time_us()
{
	static int first=1;
	static LARGE_INTEGER  t2;
	LARGE_INTEGER   t1,feq;     
	if(first)
	{
	    first=0;
	    QueryPerformanceCounter(&t2);//测前跳动次数  
	    
	}
	QueryPerformanceFrequency(&feq);//每秒跳动次数  
	QueryPerformanceCounter(&t1);//测前跳动次数  
	//info[1]=t2-t1;
	return  (U32)(((double)(t1.QuadPart-t2.QuadPart))/((double)feq.QuadPart/1000000));
}

U32 cl_get_ms();

U32 cl_get_days(int year,int month,int date)
{
	int i;
	int month_table[]={ 31,28,31,30,31,30,
						31,31,30,31,30,31};
	int days;
	int m2_days;
	int mouths_days=0;
	//------------------------------------
	if(year<2013)
	{
		return 0;
	}
	year-=2013;
	m2_days = (year+1)>>2;
	if(((year+1)&0xfffffc) == 0 && month<=2)
	{
		m2_days--;
	}
	//------------------------------------
	for(i=0;i<month-1;i++)
	{
		mouths_days+=month_table[i];
	}
	//------------------------------------
	days=year*365 + mouths_days + date+m2_days;
	return days;
}
U32 cl_get_date()
{
    SYSTEMTIME st, lt;
    //GetSystemTime(&st);
    GetLocalTime(&lt);
	return cl_get_days(lt.wYear,lt.wMonth,lt.wDay);
}
//--
U32 cl_get_date_form()
{
    SYSTEMTIME st, lt;
    //GetSystemTime(&st);
    GetLocalTime(&lt);
	return lt.wYear*10000 + lt.wMonth*100 + lt.wDay;
}
U32 cl_get_time_form()
{
    SYSTEMTIME st, lt;
    //GetSystemTime(&st);
    GetLocalTime(&lt);
	return  ((((lt.wMonth*00
		+ lt.wDay)*0
		+ lt.wHour)*100
		+ lt.wMinute)*100 
		+ lt.wSecond)*1000
		+ lt.wMilliseconds;
}
void cl_get_time_form_str(char *time)
{
    SYSTEMTIME st, lt;
    //GetSystemTime(&st);
    GetLocalTime(&lt);
	sprintf(time,"[%d-%d-%d][%d:%d:%2d-%3d]",
		lt.wYear,lt.wMonth,lt.wDay,
		lt.wHour,lt.wMinute,
		lt.wSecond,lt.wMilliseconds
		);
}
//--
U32 cl_get_second()
{
    SYSTEMTIME st, lt;
    //GetSystemTime(&st);
    GetLocalTime(&lt);
	int days,second;
	days = cl_get_days(lt.wYear,lt.wMonth,lt.wDay);
	second = days*60*60*24 + lt.wHour*60*60 + lt.wMinute*60 + lt.wSecond;
	return second;
}
U32 cl_get_minute()
{
    SYSTEMTIME st, lt;
    //GetSystemTime(&st);
    GetLocalTime(&lt);
	int days,minute;
	days = cl_get_days(lt.wYear,lt.wMonth,lt.wDay);
	minute = days*60*24 + lt.wHour*60 + lt.wMinute;
	return minute;
}
U32 cl_get_hour()
{
    SYSTEMTIME st, lt;
    //GetSystemTime(&st);
    GetLocalTime(&lt);
	int days,hour;
	days = cl_get_days(lt.wYear,lt.wMonth,lt.wDay);
	hour = days*24 + lt.wHour;
	return hour;
}

static int days_bf=0;
//--49 days--
U32 cl_get_ms()
{
    SYSTEMTIME st, lt;
    //GetSystemTime(&st);
    GetLocalTime(&lt);
	int days,second,ms;
	if(days_bf==0)
	{
		days_bf = cl_get_days(lt.wYear,lt.wMonth,lt.wDay);
	}
	days = cl_get_days(lt.wYear,lt.wMonth,lt.wDay);
	second = (days-days_bf)*60*60*24 + lt.wHour*60*60 + lt.wMinute*60 + lt.wSecond;
	ms = second*1000+lt.wMilliseconds;
	return ms;
}
U32 cl_get_10ms()
{
    SYSTEMTIME st, lt;
    //GetSystemTime(&st);
    GetLocalTime(&lt);
	int days,second,ms;
	if(days_bf==0)
	{
		days_bf = cl_get_days(lt.wYear,lt.wMonth,lt.wDay);
	}
	days = cl_get_days(lt.wYear,lt.wMonth,lt.wDay);
	second = (days-days_bf)*60*60*24 + lt.wHour*60*60 + lt.wMinute*60 + lt.wSecond;
	ms = second*100+lt.wMilliseconds/10;
	return ms;
}
