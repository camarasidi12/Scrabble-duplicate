package echange;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.ServerSocket;
import java.net.Socket;

class Connexion extends Thread {
	protected Socket client; protected DataInputStream in; protected PrintStream out;
	public Connexion(Socket client_soc){
		client=client_soc;
		try {				

			in=new DataInputStream(client.getInputStream());
			out=new PrintStream(client.getOutputStream()); }
		catch (IOException e){
			try {client.close();} catch (IOException e1){}
			System.err.println(e.getMessage());
			return;}
		this.start();
	}
	public void run(){
		try {int i=3;
			while (i>1) {i--;

				BufferedReader clav=new BufferedReader(new InputStreamReader(System.in));
				String ll=in.readLine();

				System.out.println("\nVIENT DU CLIENT: "+ll);System.out.flush();
				
				System.out.println("saisir");System.out.flush();

				String ligne="BIENVENUE/placement/tirage/scores/phase/temps/";
				//if (ligne.toUpperCase().compareTo("FIN")==0) break;
				//System.out.println(ligne.toUpperCase());
				out.println(ligne.toUpperCase()); out.flush();
				//System.out.println("\nVIENT DU CLIENT: "+in.readLine());
				//System.out.println(ligne.toUpperCase()+"on vera");
			}}
		catch (IOException e) {System.out.println("connexion : "+e.toString());}
		finally {try {client.close();} catch (IOException e) {}}
	}
}

public class Serveur extends Thread {
	protected static final int PORT =2017;
	protected ServerSocket ecoute;
	Serveur () {
		try {ecoute = new ServerSocket(PORT);}
		catch (IOException e){
			System.err.println(e.getMessage());
			System.exit(1);
		}
		System.out.println("Serveur en ecoute sur le port : "+PORT);
		this.start();
	}
	public void run (){
		try {
			while (true){ Socket client=ecoute.accept();
			Connexion c = new Connexion(client);
			}}
		catch (IOException e){System.err.println(e.getMessage());System.exit(1);}
	}
	public static void main (String[] args){new Serveur();}
}







