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

public class Ethernet {
    private String name ;
    private String mac;
    private String ip ;
    private String netmask ;
    private int rxPackets ;
    private int rxBytes;
    private int rxErrors ;
    private int txPackets ;
    private int txBytes;
    private int txErrors ;

    public Ethernet(String name, String mac, String ip, String netmask, int rxPackets, int rxBytes, int rxErrors, int txPackets, int txBytes, int txErrors) {
        this.name = name;
        this.mac = mac;
        this.ip = ip;
        this.netmask = netmask;
        this.rxPackets = rxPackets;
        this.rxBytes = rxBytes;
        this.rxErrors = rxErrors;
        this.txPackets = txPackets;
        this.txBytes = txBytes;
        this.txErrors = txErrors;
    }

    public String getName() {
        return name;
    }

    public String getMac() {
        return mac;
    }

    public String getIp() {
        return ip;
    }

    public String getNetmask() {
        return netmask;
    }

    public int getRxPackets() {
        return rxPackets;
    }

    public int getRxBytes() {
        return rxBytes;
    }

    public int getRxErrors() {
        return rxErrors;
    }

    public int getTxPackets() {
        return txPackets;
    }

    public int getTxBytes() {
        return txBytes;
    }

    public int getTxErrors() {
        return txErrors;
    }

    @Override
    public String toString() {
        return "Ethernet{" +
                "name='" + name + '\'' +
                ", mac='" + mac + '\'' +
                ", ip='" + ip + '\'' +
                ", netmask='" + netmask + '\'' +
                ", rxPackets=" + rxPackets +
                ", rxBytes=" + rxBytes +
                ", rxErrors=" + rxErrors +
                ", txPackets=" + txPackets +
                ", txBytes=" + txBytes +
                ", txErrors=" + txErrors +
                '}';
    }
}
