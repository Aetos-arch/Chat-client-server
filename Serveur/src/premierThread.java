import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.security.Principal;

public class premierThread implements Runnable{
    ServerJava serv;
    BufferedReader reader;
    PrintWriter writer;

    public premierThread(ServerJava server){
        serv = server;
        new Thread(this).start();
    }

    @Override
    public void run() {
        // Création Reader / Writer pour l'expéditeur
        reader = serv.initSockReader(serv.getClientConnexion());
        writer = serv.initSockWriter(serv.getClientConnexion());
        StructClient client = new StructClient();
        boolean marche = true;


        String message = serv.getAnswer(reader);
        //Return -2 si le client existe déjà
        if (serv.getClients().containsKey(message)) {
            serv.sendRequest("-2\0", writer);
        }else {
            serv.sendRequest("[Server] Bien recu\0", writer);
            client.setExpediteur(message);
            preparationExpediteur(message);
        while(marche) {

                message = serv.getAnswer(reader);
                if (message.equals("end")){
                    marche = preparationExit(client);
                }else if (!serv.getClients().containsKey(message)) {
                    System.out.println("[server] Le Client n'est pas dans la liste message refusé");
                    serv.getAnswer(reader);
                } else {
                    client.setDestinataire(message);
                    client.setMessage(serv.getAnswer(reader));
                    System.out.println(client);
                    sendMessage(client);
                }
            }
        }
    }

    //On ajoute le Reader / Writer / socket dans des Hashmaps présentent dans le serverJava pour que les autres
    //threads puissent y avoir accès
    public void preparationExpediteur(String message){
        serv.getClientsReader().put(message, reader);
        serv.getClientsWriter().put(message, writer);
        serv.setClient(message, serv.getClientConnexion());
    }

    // Cette méthode prépare la sortie du thread avec des suppressions de liste etc
    public boolean preparationExit(StructClient client){
        serv.getClientsWriter().remove(client.getExpediteur());
        serv.getClientsReader().remove(client.getExpediteur());
        serv.getClients().remove(client.getExpediteur());
        closeAll(serv,reader,writer);
        System.out.println("[server] Déconnexion de " + client.getExpediteur());
        return false;
    }

    // Cette méthode permet l'envoi d'un message
    public void sendMessage(StructClient client) {
        System.out.println("Les clients sauvegardés dans le serveur : " + serv.getClients());
        serv.sendRequest(client.getDestinataire()+"\0", serv.getClientsWriter().get(client.getDestinataire()));
        serv.sendRequest(client.getExpediteur()+"\0", serv.getClientsWriter().get(client.getDestinataire()));
        serv.sendRequest(client.getMessage()+"\0", serv.getClientsWriter().get(client.getDestinataire()));
    }

    // Cette méthode ferme les readers / writer d'un user
    public static void closeAll(ServerJava server, BufferedReader reader, PrintWriter writer){
        server.closeSocketReader(reader);
        server.closeSocketWriter(writer);
    }
}
