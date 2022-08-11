#include <jni.h>
#include <sys/time.h>
#include <stdlib.h>
#include <iostream>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>


#include "jdatetime.h"

using namespace std;


static char RTC_FILE[] =  "/dev/rtc" ;

void throwException(JNIEnv *env, string err) {
	jclass jexception = env->FindClass("ir/moke/jsysbox/JSysboxException");
	env->ThrowNew(jexception,err.data());
}

void get_system_dt(struct tm& tm) {
    time_t t = time(NULL);
    tm = *localtime(&t) ;
}

int set_system_dt(timeval tv) {
    int r = settimeofday(&tv,NULL);
    if (r != 0) {
        perror("SET_SYSTEM_DT");
        return 1 ;
    }
    return 0 ;
}

int get_hardware_dt(struct rtc_time rt) {
    int fd = open (RTC_FILE,O_RDONLY);
    ioctl(fd,RTC_RD_TIME,&rt);
    close(fd);
    return 0 ;
}

int set_hardware_dt() {
    struct tm tm ;
    get_system_dt(tm);

    struct rtc_time rt ; 

    rt.tm_year = tm.tm_year ;
    rt.tm_min = tm.tm_mon ;
    rt.tm_yday = tm.tm_yday ;
    rt.tm_mday = tm.tm_mday ;
    rt.tm_hour = tm.tm_hour ;
    rt.tm_wday = tm.tm_wday ;
    rt.tm_min = tm.tm_min ;
    rt.tm_sec = tm.tm_sec ;
    rt.tm_isdst = tm.tm_isdst ;

    int fd = open (RTC_FILE,O_RDONLY);
    ioctl(fd,RTC_SET_TIME,&rt);
    close(fd);
    return 0 ;
}

JNIEXPORT void JNICALL Java_ir_moke_jsysbox_time_JDateTime_setTimezone (JNIEnv *env, jclass clazz, jstring jtz) {
    string tz = env->GetStringUTFChars(jtz,0);
    int r = setenv("TZ",tz.data(),1);
    if (r !=0) {
        throwException(env,"Failed to set timezone") ;
    }
}

JNIEXPORT jobject JNICALL Java_ir_moke_jsysbox_time_JDateTime_getZonedDateTime (JNIEnv *env, jclass clazz) {
    struct tm tm ;
    get_system_dt(tm);

    /*
        Initialize LocalDateTime
    */
    jclass ldtClass = env->FindClass("java/time/LocalDateTime");
    jmethodID ldtMethod = env->GetStaticMethodID(ldtClass,"of","(IIIIII)Ljava/time/LocalDateTime;");
    jobject localDateTime = env->CallStaticObjectMethod(ldtClass,ldtMethod,tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);

    /*
        Initialize TimeZone
    */
    const char *tz = getenv("TZ");
    if (tz == NULL) {
	    tz = "UTC";
    }
    jclass zClass = env->FindClass("java/time/ZoneId");
    jmethodID zMethod = env->GetStaticMethodID(zClass,"of","(Ljava/lang/String;)Ljava/time/ZoneId;");
    if (zMethod == NULL) {
	   env->FatalError("Method not found") ; 
    }
    jobject zoneId = env->CallStaticObjectMethod(zClass,zMethod,env->NewStringUTF(tz));

    /*
        Initialize ZonedDateTime
    */
    jclass zdtClass = env->FindClass("java/time/ZonedDateTime");
    jmethodID zdtMethod = env->GetStaticMethodID(zdtClass,"of","(Ljava/time/LocalDateTime;Ljava/time/ZoneId;)Ljava/time/ZonedDateTime;");
    jobject zonedDateTime = env->CallStaticObjectMethod(zdtClass,zdtMethod,localDateTime,zoneId);

    return zonedDateTime;
}

JNIEXPORT void JNICALL Java_ir_moke_jsysbox_time_JDateTime_setDateTime (JNIEnv *env, jclass clazz, jlong timestamp) {
    struct timeval tv;
    tv.tv_sec = (long) timestamp ;
    tv.tv_usec = 00000;
    int r = set_system_dt(tv) ;
    if (r != 0) throwException(env,"Failed to set date and time"); 
}

JNIEXPORT void JNICALL Java_ir_moke_jsysbox_time_JDateTime_syncSystemToHardware (JNIEnv *env, jclass clazz) {
    int r = set_hardware_dt();
    if (r != 0) throwException(env,"Failed to sync system to hardware");
}

JNIEXPORT void JNICALL Java_ir_moke_jsysbox_time_JDateTime_syncHardwareToSystem (JNIEnv *env, jclass clazz) {
    struct rtc_time rt ;
    int r ;
    r = get_hardware_dt(rt);
    if (r != 0) return ;
    struct tm tm ;
    struct timeval tv;

    tm.tm_hour = rt.tm_hour ;
    tm.tm_min = rt.tm_min;
    tm.tm_sec = rt.tm_sec ;
    tm.tm_year = rt.tm_year ;
    tm.tm_mon = rt.tm_mon;
    tm.tm_wday = rt.tm_wday ;
    tm.tm_mday = rt.tm_mday ;
    tm.tm_yday = rt.tm_yday ;
    tm.tm_isdst = rt.tm_isdst;
    tv.tv_sec = mktime(&tm);
    tv.tv_usec = 00000;

    r = set_system_dt(tv);
    if (r != 0) throwException(env,"Failed to sync hardware to system");
}
