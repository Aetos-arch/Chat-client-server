import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;
import java.util.Map;

public class Server {

    private ServerSocket servSock;
    private Socket clientConnection;
    protected HashMap<String, Socket> clients = new HashMap<String, Socket>();
    protected HashMap<String, PrintWriter> clientsWriter = new HashMap<String, PrintWriter>();
    protected HashMap<String, BufferedReader> clientsReader = new HashMap<String, BufferedReader>();

    public ServerJava(int port) {
        try {
            this.servSock = new ServerSocket(port);
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

    public Socket getClient(String name) {
        return this.clients.get(name);
    }

    public void setClient(String name, Socket socket) {
        this.clients.put(name, socket);
    }

    public void setConnection(ServerSocket serverSocket) {
        try {
            this.clientConnection = this.servSock.accept();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public ServerSocket getServSock() {
        return servSock;
    }

    public Socket getClientConnection() {
        return clientConnection;
    }

    public PrintWriter initSocketWriter(Socket connection) {
        PrintWriter toServer = null;

        try {
            toServer = new PrintWriter(new OutputStreamWriter(connection.getOutputStream()), true);
        } catch (IOException e) {
            e.printStackTrace();
        }

        return toServer;
    }

    public void sendRequest(String message, PrintWriter writer) {
        writer.write(message);
        writer.flush();
    }

    public BufferedReader initSocketReader(Socket connection) {
        BufferedReader fromServer = null;
        try {
            fromServer = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        } catch (IOException e) {
            e.printStackTrace();
        }

        return fromServer;
    }

    public String getAnswer(BufferedReader reader) {
        String answer = "";
        char c;
        try {
            while ((c = (char) reader.read()) != '\0') {
                answer += c;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return answer;

    }

    public void closeSocketWriter(PrintWriter writer) {
        try {
            writer.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void closeSocketReader(BufferedReader reader) {
        try {
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void closeConnection() {
        try {
            this.clientConnection.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}