/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class ir_moke_jsysbox_network_JNetwork */

#ifndef _Included_ir_moke_jsysbox_network_JNetwork
#define _Included_ir_moke_jsysbox_network_JNetwork
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     ir_moke_jsysbox_network_JNetwork
 * Method:    setIp
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Lir/moke/jsysbox/network/Ethernet;
 */
JNIEXPORT void JNICALL Java_ir_moke_jsysbox_network_JNetwork_setIp
(JNIEnv *, jclass, jstring, jstring, jstring);

/*
 * Class:     ir_moke_jsysbox_network_JNetwork
 * Method:    updateRoute
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V
 */
JNIEXPORT void JNICALL Java_ir_moke_jsysbox_network_JNetwork_updateRoute
(JNIEnv *, jclass, jstring, jstring, jstring, jstring,jint, jboolean,jboolean);

#ifdef __cplusplus
}
#endif
#endif