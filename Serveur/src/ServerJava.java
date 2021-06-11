import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;
import java.util.Map;

public class ServerJava {

    private ServerSocket servSock;
    private Socket clientConnexion;
    protected HashMap<String, Socket> clients = new HashMap<String, Socket>();
    protected HashMap<String, PrintWriter> clientsWriter = new HashMap<String, PrintWriter>();
    protected HashMap<String, BufferedReader> clientsReader = new HashMap<String, BufferedReader>();

    public ServerJava(int port){
        try {
            this.servSock= new ServerSocket(port);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public HashMap<String, PrintWriter> getClientsWriter() {
        return clientsWriter;
    }

    public HashMap<String, BufferedReader> getClientsReader() {
        return clientsReader;
    }

    public HashMap<String, Socket> getClients() {
        return clients;
    }

    public void getClient(String nom) {
        this.clients.get(nom);
    }

    public void setClient(String nom, Socket sock) {
        this.clients.put(nom, sock);
    }

    public void setConnexion(ServerSocket serverSocket){
        try {
            this.clientConnexion= this.servSock.accept();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public ServerSocket getServSock() {
        return servSock;
    }

    public Socket getClientConnexion() {
        return clientConnexion;
    }

    public PrintWriter initSockWriter(Socket connexion){
        PrintWriter toServer = null;

        try {
            toServer= new PrintWriter(new OutputStreamWriter(connexion.getOutputStream()),true);
        } catch (IOException e) {
            e.printStackTrace();
        }

        return toServer;
    }

    public void sendRequest(String message, PrintWriter writer){
        writer.write(message);
        writer.flush();
    }

    public BufferedReader initSockReader(Socket connexion){
        BufferedReader fromServer = null;
        try {
            fromServer= new BufferedReader(new InputStreamReader(connexion.getInputStream()));
        } catch (IOException e) {
            e.printStackTrace();
        }

        return fromServer;
    }

    public String getAnswer(BufferedReader reader){
        String answer="";
        char c;
        try{
            while((c= (char)reader.read()) != '\0'){
                answer+=c;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return answer;

    }

    public void closeSocketWriter(PrintWriter writer){
        try {
            writer.close();
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }

    public void closeSocketReader(BufferedReader reader){
        try{
            reader.close();
        }catch (IOException e){
            e.printStackTrace();
        }
    }

    public void closeConnexion(){
        try{
            this.clientConnexion.close();
        }catch(IOException e){
            e.printStackTrace();
        }
    }



}
