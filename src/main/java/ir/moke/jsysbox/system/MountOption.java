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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.BitSet;
import java.util.List;

public enum MountOption {
    RDONLY(1),
    NOSUID(2),
    NODEV(4),
    NOEXEC(8),
    SYNCHRONOUS(16),
    REMOUNT(32),
    MANDLOCK(64),
    DIRSYNC(128),
    NOSYMFOLLOW(256),
    NOATIME(1024),
    NODIRATIME(2048),
    BIND(4096),
    MOVE(8192),
    REC(16384),
    SILENT(32768),
    POSIXACL(1 << 16),
    UNBINDABLE(1 << 17),
    PRIVATE(1 << 18),
    SLAVE(1 << 19),
    SHARED(1 << 20),
    RELATIME(1 << 21),
    KERNMOUNT(1 << 22),
    I_VERSION(1 << 23),
    STRICTATIME(1 << 24),
    LAZYTIME(1 << 25),
    ACTIVE(1 << 30),
    NOUSER(1 << 31);

    private final int bit;

    MountOption(int bit) {
        this.bit = bit;
    }

    public int getBit() {
        return this.bit;
    }

    public static List<MountOption> getMountOptions() {
        return Arrays.asList(MountOption.class.getEnumConstants());
    }

    public static MountOption getBit(int bit) {
        return getMountOptions().stream()
                .filter(item -> item.getBit() == bit)
                .findFirst().orElse(null);
    }

    public static List<MountOption> mapToFlags(int bit) {
        List<Integer> integers = getBitNumbers(bit);
        return integers.stream()
                .map(MountOption::getBit)
                .toList();
    }

    public static String getFlagStr(int bit) {
        StringBuilder str = new StringBuilder();
        List<MountOption> routeFlags = mapToFlags(bit);
        for (MountOption flag : routeFlags) {
            str.append(flag);
        }
        return str.toString();
    }

    public static int mapToBit(MountOption... mountOptions) {
        return Arrays.stream(mountOptions).mapToInt(MountOption::getBit).sum();
    }

    public static List<Integer> getBitNumbers(int x) {
        List<Integer> bitList = new ArrayList<>();
        BitSet bs = BitSet.valueOf(new long[]{x});
        for (int i = bs.nextSetBit(0); i >= 0; i = bs.nextSetBit(i + 1)) {
            bitList.add(1 << i);
        }
        return bitList;
    }
}
