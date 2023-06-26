import java.io.BufferedReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.HashMap;

public class ServerLauncher {

    public static void main(String[] args) {
        ClientStruct client;
        String message = "";
        JavaServer server = new JavaServer(8000);
        while (true) {
            System.out.println("Waiting for connection to the server");
            server.setConnection(server.getServerSocket());
            FirstThread test = new FirstThread(server);
        }
    }
}
