package supportGUI;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.PrintWriter;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;


public class RulesPanel extends JPanel{
	enum Eta {bvn,plc,tir,scr,phs,tem};
	boolean end=false;

	protected Font comics30 = new Font("Comics Sans MS", Font.BOLD, 30);
	protected Font comics40 = new Font("Comics Sans MS", Font.BOLD, 40);
	protected Font comics20 = new Font("Comics Sans MS", Font.BOLD, 20);
	protected Font arial = new Font("Arial", Font.BOLD, 15);
	protected Font dialog = new Font("Dialog", Font.BOLD + Font.ITALIC, 15);
	JTextField name;
	Fenetre parent;
	Message message=new Message();
	JButton submit=new JButton("connecte");
	JPanel conn=new JPanel();
	public RulesPanel(Dimension dim,Fenetre par){
	
		this.parent=par;
		name=new JTextField(15);
		name.setPreferredSize(new Dimension(20, 30));

		this.name.setEnabled(true);
		this.setPreferredSize(dim);
		initPanel(dim);
	}

	public void initPanel(Dimension dim){	
		JLabel titre = new JLabel();
		titre.setFont(comics30);
		titre.setText("Les Regles du jeu  scrabble");
		titre.setHorizontalAlignment(JLabel.CENTER);
		titre.setPreferredSize(new Dimension(dim.width,40));
		titre.setBorder(BorderFactory.createLineBorder(Color.black));
		this.add(titre, BorderLayout.NORTH);
		JTextArea accueil = new JTextArea();
		accueil.setBackground(Color.white);
		accueil.setPreferredSize(new Dimension(dim.width-700, dim.height-50));
		conn.setPreferredSize(new Dimension(300, dim.height-50));
		message.setPreferredSize(new Dimension(300, 100));
		accueil.setText("\n\n\Point  attribuer au lettres:"+
				"A:1\nB:1\nC:1\nD:1\nE:1\nF:1\nG:1\nH:1\nI:1\nJ:1\nK:1\nL:1"+
				"\nM:1\nN:1\nO:1\nP:1\nQ:1\nR:1\nS:1\nT:1\nU:1\nV:1\nW:1\nX:1\nY:1\nZ:1");
		accueil.setFont(arial);
		accueil.setEditable(false);
		submit.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				connexion(name.getText());
			}
		});

		this.add(accueil, BorderLayout.CENTER);
		JLabel l=new JLabel("Saisissez votre Nom pour Connecté \n");
		l.setPreferredSize(new Dimension(300,50));
		conn.add(l, BorderLayout.NORTH);
		conn.add(name, BorderLayout.CENTER);
		conn.add(submit, BorderLayout.SOUTH);
		
		conn.add(message, BorderLayout.SOUTH);

		this.add(conn, BorderLayout.EAST);

	}
	
	public void setMessage(){
		this.message.setMessage();
	}

	public void connexion(String nom){
		/*String placement="",
				tirage="",
				scores="",
				phase="",
				temps="";

		Eta eta=Eta.bvn;
		String commande_connexion="CONNEXION/"+nom+"/";
		canalecriture.println(commande_connexion);
		canalecriture.flush();
		String comande_reponse = "";
		canalecriture.println("dans rule");

		try {
		
			System.out.println(comande_reponse+"avant");
			comande_reponse=canalecture.readLine();
			System.out.println("apres");
			if(comande_reponse.contains("BIENVENUE")){
				//BIENVENUE/placement/tirage/scores/phase/temps/
				char [] cmdrep=comande_reponse.toCharArray();
				int i=0;
				while(!end){
					switch(eta){

					case bvn:
						if(cmdrep[i]=='/'){i++;
						eta=Eta.plc;
						continue;   
						}else {
							i++;break;
						}
					case plc:
						if(cmdrep[i]=='/'){i++;
						eta=Eta.tir;
						continue;   
						}else{
							placement=placement+cmdrep[i++];
							break;
						}
					case tir:
						if(cmdrep[i]=='/'){i++;
						eta=Eta.scr;
						continue;   
						}else{
							tirage=tirage+cmdrep[i++];
							break;
						}
					case scr:
						if(cmdrep[i]=='/'){i++;
						eta=Eta.phs;
						continue;   
						}else{
							scores=scores+cmdrep[i++];
							break;
						}
					case phs:
						if(cmdrep[i]=='/'){i++;
						eta=Eta.tem;
						continue;   
						}else{
							phase=phase+cmdrep[i++];
							break;
						}
					case tem:
						if(cmdrep[i]=='/'){i++;
						end=true;
						continue;   
						}else{
							temps=temps+cmdrep[i++];
							break;
						}





					}

				}

				this.parent.connecter(placement,tirage,scores,phase,temps);



			}
			else{
               //cas d'un refus par le sever
				this.message.setMessage();
				

		}

		} catch (IOException e) {
		e.printStackTrace();
		}
	}*/
	
this.parent.go(nom);
}
}
class Message extends JPanel{
	String msg="";
	String msg1="";

	@Override
	public void paintComponent(Graphics g){
		g.setColor(Color.red);
		g.drawString(msg, 15, 10);
		g.drawString(msg1, 15, 40);
	}
	public void setMessage(){
		
		msg="connexion refuser par le server:";
		msg1="utiliser un autre nom";
		repaint();

	}
}



