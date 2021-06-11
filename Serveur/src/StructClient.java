public class StructClient {
    String expediteur;
    String destinataire;
    String message;


    public String getExpediteur() {
        return expediteur;
    }

    public String getDestinataire() {
        return destinataire;
    }

    public String getMessage() {
        return message;
    }

    @Override
    public String toString() {
        return "StructClient{" +
                "expediteur='" + expediteur + '\'' +
                ", destinataire='" + destinataire + '\'' +
                ", message='" + message + '\'' +
                '}';
    }

    public StructClient(){

    }

    public void setExpediteur(String expediteur) {
        this.expediteur = expediteur;
    }

    public void setDestinataire(String destinataire) {
        this.destinataire = destinataire;
    }

    public void setMessage(String message) {
        this.message = message;
    }
}
