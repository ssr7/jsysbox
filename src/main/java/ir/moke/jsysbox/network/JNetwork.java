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

import ir.moke.jsysbox.JNetworkException;
import ir.moke.jsysbox.JniNativeLoader;

import java.io.IOException;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.time.Duration;
import java.time.Instant;
import java.util.ArrayList;
import java.util.List;

public class JNetwork {
    private static final Path ROUTE_TABLE_PATH = Paths.get("/proc/net/route");
    private static final int DEFAULT_METRICS = 600;
    private static final int DEFAULT_TTL = 600;

    static {
        JniNativeLoader.load("jnetwork.so");
    }

    public native static Ethernet[] ethernetList();

    public native static Ethernet ethernet(String ethName);

    public native static Ethernet setIp(String ethName, String ipAddress, String netmask) throws JNetworkException;

    public native static void updateRoute(String destination, String netmask, String gateway, String ethernet, int metrics, boolean isHost, boolean delete);

    public static void setDefaultGateway(String gateway) {
        updateRoute("0.0.0.0", "0.0.0.0", gateway, null, DEFAULT_METRICS, false, false);
    }

    public static void deleteRoute(int id) {
        route().stream()
                .filter(item -> item.getId() == id)
                .findFirst()
                .ifPresent(item -> updateRoute(item.getDestination(), item.getNetmask(), item.getGateway(), item.getIface(), item.getMetrics(), false, true));
    }

    public static List<Route> route() {
        List<Route> routeList = new ArrayList<>();
        try {
            List<String> lines = Files.readAllLines(ROUTE_TABLE_PATH);
            for (int i = 1; i < lines.size(); i++) {
                routeList.add(mapToRoute(lines.get(i), i));
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        return routeList;
    }

    public static void ping(String destination, String iface, Integer ttl, Integer count, Integer timeout,Integer interval) {
        if (count == null || count < 1) count = 4;
        if (timeout == null || timeout <= 1000) timeout = 1000;
        if (ttl == null || ttl <=0) ttl = DEFAULT_TTL;
        if (interval == null || interval < 800) interval = 800 ;
        try {
            InetAddress addr = InetAddress.getByName(destination);
            System.out.printf("Ping %s\n",destination);
            int reachableCount = 0;
            for (int i = 0; i < count; i++) {
                Instant startTime = Instant.now();
                boolean reachable;
                if (iface != null && !iface.isEmpty()) {
                    NetworkInterface networkInterface = NetworkInterface.getByName(iface);
                    reachable = addr.isReachable(networkInterface, ttl,timeout);
                } else {
                    reachable = addr.isReachable(timeout);
                }
                if (reachable) reachableCount++;
                long diff = Duration.between(startTime, Instant.now()).toMillis();
                System.out.printf("%d: from %s ttl=%d time=%d ms\n",reachableCount,destination,ttl,diff);
                sleep(interval);
            }
        } catch (IOException e) {
            System.out.println("ping: " + e.getMessage());
        }
    }

    private static Route mapToRoute(String line, int id) {
        String[] s = line.split("\\s+");
        String iface = s[0];
        String destination = hexToIp(s[1]);
        String gateway = hexToIp(s[2]);
        String netmask = hexToIp(s[7]);
        int flags = Integer.parseInt(s[3]);
        int refcnt = Integer.parseInt(s[4]);
        int use = Integer.parseInt(s[5]);
        int metrics = Integer.parseInt(s[6]);
        int mtu = Integer.parseInt(s[8]);
        int window = Integer.parseInt(s[9]);
        int irtt = Integer.parseInt(s[10]);
        return new Route(id, destination, netmask, gateway, iface, flags, use, metrics, mtu, window, irtt, refcnt);
    }

    private static String hexToIp(String hex) {
        StringBuilder ip = new StringBuilder();
        for (int i = 0; i < hex.length(); i = i + 2) {
            ip.insert(0, Integer.valueOf(hex.substring(i, i + 2), 16) + ".");
        }
        return ip.deleteCharAt(ip.length() - 1).toString();
    }


    private static void sleep(int mills) {
        try {
            Thread.sleep(mills);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }
}
