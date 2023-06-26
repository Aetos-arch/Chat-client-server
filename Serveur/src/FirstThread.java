import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.security.Principal;

public class FirstThread implements Runnable {
    JavaServer server;
    BufferedReader reader;
    PrintWriter writer;

    public FirstThread(JavaServer server) {
        this.server = server;
        new Thread(this).start();
    }

    @Override
    public void run() {
        reader = server.initSocketReader(server.getClientConnection());
        writer = server.initSocketWriter(server.getClientConnection());
        ClientStruct client = new ClientStruct();
        boolean running = true;

        String message = server.getResponse(reader);
        if (server.getClients().containsKey(message)) {
            server.sendResponse("-2\0", writer);
        } else {
            server.sendResponse("[Server] Message received\0", writer);
            client.setSender(message);
            prepareSender(message);
            
            while (running) {
                message = server.getResponse(reader);
                if (message.equals("end")) {
                    running = prepareExit(client);
                } else if (!server.getClients().containsKey(message)) {
                    System.out.println("[Server] Client not in the list, message rejected");
                    server.getResponse(reader);
                } else {
                    client.setRecipient(message);
                    client.setMessage(server.getResponse(reader));
                    System.out.println(client);
                    sendMessage(client);
                }
            }
        }
    }

    public void prepareSender(String message) {
        server.getClientsReader().put(message, reader);
        server.getClientsWriter().put(message, writer);
        server.setClient(message, server.getClientConnection());
    }

    public boolean prepareExit(ClientStruct client) {
        server.getClientsWriter().remove(client.getSender());
        server.getClientsReader().remove(client.getSender());
        server.getClients().remove(client.getSender());
        closeAll(server, reader, writer);
        System.out.println("[Server] Disconnected: " + client.getSender());
        return false;
    }

    public void sendMessage(ClientStruct client) {
        System.out.println("Clients stored in the server: " + server.getClients());
        server.sendResponse(client.getRecipient() + "\0", server.getClientsWriter().get(client.getRecipient()));
        server.sendResponse(client.getSender() + "\0", server.getClientsWriter().get(client.getRecipient()));
        server.sendResponse(client.getMessage() + "\0", server.getClientsWriter().get(client.getRecipient()));
    }

    public static void closeAll(JavaServer server, BufferedReader reader, PrintWriter writer) {
        server.closeSocketReader(reader);
        server.closeSocketWriter(writer);
    }
}