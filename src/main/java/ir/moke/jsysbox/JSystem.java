package ir.moke.jsysbox;

public class JSystem {

    static {
        System.loadLibrary("hello");
    }
    public native static void reboot();

    public native static void shutdown();

}
