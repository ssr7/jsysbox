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

package ir.moke.jsysbox.system;

import ir.moke.jsysbox.JniNativeLoader;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;

public class JSystem {

    static {
        JniNativeLoader.load("jsystem.so");
    }

    public native static void reboot();

    public native static void shutdown();

    public native static boolean mount(String src, String dst, String type, int flags, String options);

    public native static boolean umount(String src);

    public native static boolean setEnv(String key, String value);

    public native static boolean unSetEnv(String key);

    public native static String getEnv(String key);

    private native static String[] envList();

    public static List<String> environments() {
        String[] envList = envList();
        return Arrays.stream(envList).filter(Objects::nonNull)
                .filter(item -> !item.isEmpty())
                .toList();
    }

    public static List<String> mounts() {
        try {
            return Files.readAllLines(Paths.get("/proc/mounts"));
        } catch (IOException e) {
            return null;
        }
    }

    public static boolean checkMountPoint(String mountpoint) {
        List<String> mounts = JSystem.mounts();
        if (mounts == null) return false;
        return mounts.stream().anyMatch(item -> item.contains(mountpoint));
    }
}
