import java.io.BufferedReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.HashMap;

public class LanceurServeur {

    public static void main(String[] args) {
        StructClient client;
        String message = "";
        ServerJava server = new ServerJava(8000);
        while(true) {
            System.out.println("En attente de connexion au serveur");
            server.setConnexion(server.getServSock());
            premierThread test = new premierThread(server);
        }
    }
}
