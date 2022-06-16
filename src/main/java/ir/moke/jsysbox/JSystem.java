/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package ir.moke.jsysbox;

public class JSystem {

    static {
        JniNativeLoader.load("jsystem.so");
    }

    public native static void reboot();

    public native static void shutdown();
    public native static boolean mount(String src,String dst,String type,String options);
    public native static boolean umount(String src);
    public native static boolean setEnv(String key,String value);
    public native static boolean unSetEnv(String key);
    public native static String getEnv(String key);

}
