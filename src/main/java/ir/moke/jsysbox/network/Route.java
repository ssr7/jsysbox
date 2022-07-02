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

public class Route {
    private int id ;
    private String destination ;
    private String netmask ;
    private String gateway ;
    private String iface ;
    private int flags ;
    private int use ;
    private int metrics ;
    private int mtu ;
    private int window ;
    private int irtt ;
    private int refcnt;

    public Route(int id ,String destination, String netmask, String gateway, String iface, int flags, int use, int metrics, int mtu, int window, int irtt, int refcnt) {
        this.id = id;
        this.destination = destination;
        this.netmask = netmask;
        this.gateway = gateway;
        this.iface = iface;
        this.flags = flags;
        this.use = use;
        this.metrics = metrics;
        this.mtu = mtu;
        this.window = window;
        this.irtt = irtt;
        this.refcnt = refcnt;
    }

    public int getId() {
        return id;
    }

    public String getDestination() {
        return destination;
    }

    public String getNetmask() {
        return netmask;
    }

    public String getGateway() {
        return gateway;
    }

    public String getIface() {
        return iface;
    }

    public int getFlags() {
        return flags;
    }

    public int getUse() {
        return use;
    }

    public int getMetrics() {
        return metrics;
    }

    public int getMtu() {
        return mtu;
    }

    public int getWindow() {
        return window;
    }

    public int getIrtt() {
        return irtt;
    }

    public int getRefcnt() {
        return refcnt;
    }

    public String getFlagStr() {
        int flags = getFlags();
        return RouteFlag.getFlagStr(flags);
    }

    @Override
    public String toString() {
        return "Route{" +
                "id=" + id + '\'' +
                "destination='" + destination + '\'' +
                ", netmask='" + netmask + '\'' +
                ", gateway='" + gateway + '\'' +
                ", iface='" + iface + '\'' +
                ", flags=" + getFlagStr() +
                ", use=" + use +
                ", metrics=" + metrics +
                ", mtu=" + mtu +
                ", window=" + window +
                ", irtt=" + irtt +
                ", refcnt=" + refcnt +
                '}';
    }
}
