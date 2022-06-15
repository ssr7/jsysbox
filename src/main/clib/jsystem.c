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

#include <stdio.h>
#include <jni.h>
#include <unistd.h>
#include <sys/reboot.h>
#include <sys/mount.h>


JNIEXPORT void JNICALL Java_ir_moke_jsysbox_JSystem_reboot (JNIEnv *env, jobject obj) {
	sync() ;
	reboot(RB_AUTOBOOT);
}

JNIEXPORT void JNICALL Java_ir_moke_jsysbox_JSystem_shutdown (JNIEnv *env, jobject obj) {
    sync() ;
	reboot(RB_POWER_OFF);
}

JNIEXPORT jboolean JNICALL Java_ir_moke_jsysbox_JSystem_mount (JNIEnv *env, jclass clazz, jstring src, jstring dst,jstring file_system_type) {
    const char *src_path = (*env)->GetStringUTFChars(env,src,0);
    const char *dst_path = (*env)->GetStringUTFChars(env,dst,0);
    const char *fs_type = (*env)->GetStringUTFChars(env,file_system_type,0);

    return mount(src_path,dst_path,fs_type,0,"") == 0 ;
}

JNIEXPORT jboolean JNICALL Java_ir_moke_jsysbox_JSystem_umount (JNIEnv *env, jclass clazz, jstring target) {
    const char *target_path = (*env)->GetStringUTFChars(env,target,0);
    return umount(target_path) == 0 ;
}
