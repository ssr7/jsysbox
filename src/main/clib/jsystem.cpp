/*
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <jni.h>
#include "jsystem.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/reboot.h>
#include <sys/mount.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;


JNIEXPORT void JNICALL Java_ir_moke_jsysbox_system_JSystem_reboot (JNIEnv *env, jobject obj) {
	sync() ;
	reboot(RB_AUTOBOOT);
}

JNIEXPORT void JNICALL Java_ir_moke_jsysbox_system_JSystem_shutdown (JNIEnv *env, jobject obj) {
    sync() ;
	reboot(RB_POWER_OFF);
}

JNIEXPORT jboolean JNICALL Java_ir_moke_jsysbox_system_JSystem_mount (JNIEnv *env, jclass clazz, jstring src, jstring dst,jstring file_system_type,jstring options) {
    const char *src_path = env->GetStringUTFChars(src,0);
    const char *dst_path = env->GetStringUTFChars(dst,0);
    const char *fs_type = env->GetStringUTFChars(file_system_type,0);
    const char *mnt_opt = env->GetStringUTFChars(options,0);

    return mount(src_path,dst_path,fs_type,0,mnt_opt) == 0 ;
}

JNIEXPORT jboolean JNICALL Java_ir_moke_jsysbox_system_JSystem_umount (JNIEnv *env, jclass clazz, jstring target) {
    const char *target_path = env->GetStringUTFChars(target,0);
    return umount(target_path) == 0 ;
}

JNIEXPORT jboolean JNICALL Java_ir_moke_jsysbox_system_JSystem_setEnv (JNIEnv *env, jclass clazz, jstring key, jstring value) {
    const char *k = env->GetStringUTFChars(key,0);
    const char *v = env->GetStringUTFChars(value,0);
    int r = setenv(k, v, 1);
    return r == 0;
}

JNIEXPORT jboolean JNICALL Java_ir_moke_jsysbox_system_JSystem_unSetEnv (JNIEnv *env, jclass clazz, jstring key) {
    const char *k = env->GetStringUTFChars(key,0);
    int r = unsetenv(k);
    return r == 0;
}

JNIEXPORT jstring JNICALL Java_ir_moke_jsysbox_system_JSystem_getEnv (JNIEnv *env, jclass clazz, jstring key) {
    const char *k = env->GetStringUTFChars(key,0);
    char *v = getenv(k);
    return env -> NewStringUTF(v);
}
