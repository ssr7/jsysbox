#include <stdio.h> 
#include <jni.h>

JNIEXPORT void JNICALL Java_ir_moke_jsysbox_System_reboot (JNIEnv *env, jobject obj) {

	printf("Hello dear !\n") ;
}

JNIEXPORT void JNICALL Java_ir_moke_jsysbox_System_shutdown (JNIEnv *env, jobject obj) {

}
