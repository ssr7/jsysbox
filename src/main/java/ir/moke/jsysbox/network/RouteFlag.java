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

package ir.moke.jsysbox.network;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.BitSet;
import java.util.List;

public enum RouteFlag {
    U(1),
    G(2),
    H(4),
    R(8),
    D(16),
    M(32),
    A(64),
    C(128),
    Z(256);
    private final int bit ;

    RouteFlag(int bit) {
        this.bit = bit;
    }

    public int getBit() {
        return this.bit;
    }

    public static List<RouteFlag> getRouteFlags() {
        return Arrays.asList(RouteFlag.class.getEnumConstants());
    }

    public static RouteFlag getBit(int bit) {
        return getRouteFlags().stream()
                .filter(item -> item.getBit() == bit)
                .findFirst().orElse(null);
    }

    public static List<RouteFlag> mapToFlags(int bit) {
        List<Integer> integers = getBitNumbers(bit);
        return integers.stream()
                .map(RouteFlag::getBit)
                .toList();
    }

    public static String getFlagStr(int bit) {
        StringBuilder str = new StringBuilder();
        List<RouteFlag> routeFlags = mapToFlags(bit);
        for (RouteFlag flag : routeFlags) {
            str.append(flag);
        }
        return str.toString();
    }

    public static int mapToBit(RouteFlag... routeFlags) {
        return Arrays.stream(routeFlags).mapToInt(RouteFlag::getBit).sum();
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
