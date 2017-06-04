package echange;

import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.util.HashMap;
import java.util.Scanner;
import java.util.Vector;

import javax.imageio.stream.FileImageInputStream;
import javax.swing.JOptionPane;

import supportGUI.Fenetre;

public class Client {
	static char [][] pla= new char[15][15];


	public static void setPlateau(String pl){
		char[] p=pl.toCharArray();
		int ind=0;
		for(int i=0;i<15;i++){
			for(int j=0;j<15;j++){
				pla[i][j]=p[ind++];
			}
		}
	}
	public static char[][] setNewPlateau(String pl){
		char[] p=pl.toCharArray();
		char [][] nwpl=new char[15][15];
		int ind=0;
		for(int i=0;i<15;i++){
			for(int j=0;j<15;j++){
				nwpl[i][j]=p[ind++];
			}
		}
		return nwpl;
	}

	public boolean verifpacement(String newpla){
		char [][] nwpl=setNewPlateau(newpla);
		String mot=new String();
		mot="";
		Vector<int[]> chg=new Vector<int[]>();
		for(int i=0;i<15;i++){
			for(int j=0;j<15;j++){
				if(pla[i][j]!=nwpl[i][j]){
					int [] t={i,j};
					chg.add(t);

				}
			}
		}

		if(chg.size()==0)

			return false;
		if(chg.size()==1){
			for(int i=chg.get(0)[1];i<=0;i--){
				char m=pla[chg.get(0)[0]][i];
				if(m!='Ø'){
					mot=m+mot;
				}else{
					break;
				}
			}

			for(int i=chg.get(0)[1]+1;i<15;i++){
				char m=pla[chg.get(0)[0]][i];
				if(m!='Ø'){
					mot=mot+m;
				}else{
					break;
				}
			}
		}
		else{
			//le placement etai t fait haurizontalement ou vertical
			boolean vert=true;
			if(chg.get(0)[0]==chg.get(0)[1])
				vert=false;

			//verification que tout etai fai horizontalement
			if(!vert){
				for(int i=0;i<chg.size();i++){
					if(chg.get(0)[0]!=chg.get(i)[0])
						return false;
				}

				for(int i=chg.get(0)[1];i<=0;i--){
					char m=pla[chg.get(0)[0]][i];
					if(m!='Ø'){
						mot=m+mot;
					}else{
						break;
					}
				}

				for(int i=chg.get(0)[1]+1;i<15;i++){
					char m=pla[chg.get(0)[0]][i];
					if(m!='Ø'){
						mot=mot+m;
					}else{
						break;
					}
				}

				//dernier carac du mot doi etre egal dernier case modif
				int i=chg.lastElement()[0];int j=chg.lastElement()[1];
				if(!(mot.charAt(mot.length()-1)==pla[i][j]))
					return false;

				if(mot.length()<=chg.size())
					return false;
				File dic=new File("dic");
				try {
					Scanner scanner = new Scanner (dic);
					String m;
					//	boolean b= scanner.findInLine(mot);
					//	scanner.

				} catch (FileNotFoundException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}



			}
			else{//vertical
				for(int i=0;i<chg.size();i++){
					if(chg.get(0)[1]!=chg.get(i)[1])
						return false;
				}
			}


		}
		return true;
	}
	public static void main(String args[]){
	//	String c="CONNECTE/user/gg/";

		//String newclien=c.substring(14, c.length()-1);
	//	System.out.println(newclien);
		  
		Fenetre fen=new Fenetre();
		String 
		user
		,typediscusion
		,message
		,plateau
		,tirage
		,scores
		,mot;
		HashMap<String, String> joueurs=new HashMap<>();
		
	}
}




















