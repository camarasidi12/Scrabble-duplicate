package supportGUI;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.io.DataInputStream;
import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;

import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;

import supportGUI.RulesPanel.Eta;

public class Fenetre extends JFrame{
	protected static final int port=2017;
	enum Eta {bvn,plc,tir,scr,phs,tem};
	enum Etabian {bila,mot,vq,scor};
	enum Etatour {tour,plat,tira};
	Ecouteur ecou=new Ecouteur(this);
	String nom;
	boolean end=false;
	Socket clien=null;
	DataInputStream canalecture = null;
	PrintWriter canalecriture=null;

	JPanel container=new JPanel();
	Contentpanel pan;
	RulesPanel rp;
	boolean identifier=false;
	int delaiid=0;
	public Fenetre(){
		this.setSize(1250, 800);
		initecriture();
		rp=new RulesPanel(new Dimension(this.getWidth(),this.getHeight()),this);

		pan=new Contentpanel(1150,750,1,00,this);
		this.setTitle("scrabble");
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		//this.setLocationRelativeTo(null);
		this.setResizable(false);
		this.setContentPane(rp);
		this.setVisible(true);

		//	go();


	}


	public void setPlateu(String pl){
		pan.setPlateau(pl);
	}

	public void setTirage(String tir){
		pan.setTirage(tir);
	}

	public void initecriture(){

		/*	try {


		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}*/

	}

	public void sendplateu(String pl){
		canalecriture.println("TROUVE/"+pl+"/");
		canalecriture.flush();
	
	}

	public void go(String cmd){// pan.getP();
		this.nom="cmd";
		String comande_reponse = "";
		String ligne;
		//verif_reponse("BIENVENUE/placement/tirage/scores/phase/temps/");
		try {

			clien=new Socket(InetAddress.getLocalHost(), port);
			System.out.println("CLIENT etablie : "+clien.getInetAddress()+" port : "+clien.getPort());
			canalecriture=new PrintWriter(clien.getOutputStream());
			canalecture=new DataInputStream(clien.getInputStream());		
			canalecriture.println("CONNEXION/"+cmd+"/");
			canalecriture.flush();
			ecou.start();
		
		}
		catch (IOException e) {System.err.println(e);}
	/*	finally { try {if (clien != null) clien.close();} catch (IOException e2) {}
		}*/


	}
public void ecoute(){
	String comande_reponse = "";
	try {
		comande_reponse=canalecture.readLine();
		verif_reponse(comande_reponse);

	} catch (IOException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	}
}
	public void verif_reponse(String cmr){
		String comande_reponse = cmr;
		char [] cmdrep=comande_reponse.toCharArray();
		int i=0;

		boolean end=false;
		String placement="",
				tirage="",
				scores="",
				phase="",
				temps="";
		Eta eta=Eta.bvn;

		String newclien="";
		String cliendeconect="";

		String bilan="";
		boolean REFUS=cmr.contains("REFUS"),
				CONNECTE=cmr.contains("CONNECTE"),
				DECONNEXION=cmr.contains("DECONNEXION"),
				SESSION=cmr.contains("SESSION"),
				VAINQUEUR=cmr.contains("VAINQUEUR"),
				TOUR=cmr.contains("TOUR"),
				RVALIDE=cmr.contains("RVALIDE"),
				RINVALIDE=cmr.contains("RINVALIDE"),
				RATROUVE=cmr.contains("RATROUVE"),
				RFIN=cmr.contains("RFIN"),
				SVALIDE=cmr.contains("SINVALIDE"),
				SINVALIDE=cmr.contains("SINVALIDE"),
				SFIN=cmr.contains("SFIN"),
				BILAN=cmr.contains("BILAN"),
				TROUVE=cmr.contains("TROUVE");

		String plateau="",
				raison="";

		String mot="",
				vainqueur = "";

		if(cmr.contains("BIENVENUE")){
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

				default:System.out.println("commende incorect dans fct verif commande");
				}

			}
			
			connecter(placement,tirage,scores,phase,temps);
			
		}else{

			if(REFUS){
				this.rp.setMessage();	
			}else{
				if(CONNECTE){

					newclien=cmr.substring(9, cmr.length()-1);
					pan.addJoueur(newclien+" 0");

				}else{
					if(DECONNEXION){
						cliendeconect=cmr.substring(12, cmr.length()-1);
						pan.removjoueur(cliendeconect);
					}else{
						if(VAINQUEUR){
							bilan=cmr.substring(10, cmr.length()-1);
							JOptionPane jop1 = new JOptionPane();
							jop1.showMessageDialog(null, bilan, "Information", JOptionPane.INFORMATION_MESSAGE); 
						}else{
							if(SESSION){
								//on remet les score a zero
								//this.remove(pan);
							//this.setContentPane(pan);
							//	this.revalidate();
								pan.setChrono(180);
							}else{

								if(TOUR){
									String tr="";
									plateau="";
									tirage="";

									boolean fintr=false;
									Etatour etatr=Etatour.tour;
									while(!fintr){
										switch(etatr){
										case tour:
											if(cmdrep[i]=='/'){i++;
											etatr=Etatour.plat;
											continue;   
											}else {
												tr=tr+cmdrep[i++];
												;break;
											}
										case plat:
											if(cmdrep[i]=='/'){i++;
											etatr=Etatour.tira;
											continue;   
											}else {
												plateau=plateau+cmdrep[i++];
												;break;
											}
										case tira:
											if(cmdrep[i]=='/'){i++;
											fintr=true;
											continue;   
											}else {
												tirage=tirage+cmdrep[i++];
												;break;
											}
										default: System.out.println("erreur tour dans verif_reponse");System.exit(1);

										}
									}
									pan.setTour(tr);
									pan.setPlateau(plateau);
									pan.setTirage(tirage);
								}else{
									if(RVALIDE){
										pan.invalidplateau();
									}else{
										if(RINVALIDE){
											raison=cmr.substring(10, cmr.length()-1);
										}else{
											if(RATROUVE){
												String usertrou="";
												usertrou=cmr.substring(9, cmr.length()-1);
												pan.setRaison(usertrou);
												pan.setChrono(120);

											}else{
												if(RFIN){
													pan.invalidplateau();
												}else{
													if(TROUVE){
														String placm=pan.getPlateau();
														String ccmd="TROUVE/"+placm+"/";
														canalecriture.println(ccmd);
														canalecriture.flush();

													}else{
														if(SVALIDE){
															pan.setRaison("Placement valide");
															pan.invalidplateau();

														}else{
															if(SINVALIDE){
																String rais="";
																rais=cmr.substring(10, cmr.length()-1);
																pan.setRaison(rais);
															}else{
																if(SFIN){
																	pan.setRaison("Expiration delai  soumission");
																	pan.invalidplateau();

																}else{
																	if(BILAN){
																		boolean f=false;
																		Etabian etab=Etabian.bila;
																		while(!f){
																			switch(etab){

																			case bila:
																				if(cmdrep[i]=='/'){i++;
																				etab=Etabian.mot;
																				continue;   
																				}else {
																					i++;break;
																				}
																			case mot:
																				if(cmdrep[i]=='/'){i++;
																				etab=Etabian.vq;
																				continue;   
																				}else{
																					mot=mot+cmdrep[i++];
																					break;
																				}
																			case vq:
																				if(cmdrep[i]=='/'){i++;
																				etab=Etabian.scor;
																				continue;   
																				}else{
																					vainqueur=vainqueur+cmdrep[i++];
																					break;
																				}
																			case scor:
																				if(cmdrep[i]=='/'){i++;
																				f=true;
																				continue;   
																				}else{
																					scores=scores+cmdrep[i++];
																					break;
																				}

																			default:System.out.println("commende incorect"+
																					"dans fct verif commande\n pour score final ");System.exit(1);
																			}

																		}



																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}


							}
						}



					}
				}


			}


		}




		//cas d'un refus par le sever


	}



	public void connecter(String placement,String tirage,String scores,String phase,String temps){
		setPlateu(placement);
		setTirage(tirage);
		int tem=Integer.parseInt(temps);
		pan.setChrono(tem);
		String ligne = new String();
		//	while(true){

		
		this.remove(this.rp);
	//	Tmp ff=new Tmp();
	//	ff.setPreferredSize(new Dimension(this.getWidth(),this.getHeight()));
	//	this.setContentPane(ff);
	//	this.remove(ff);
	//	this.revalidate();

		this.setContentPane(pan);
		System.out.println("sssssss");
		//this.identifier=true;
		this.revalidate();
		//}

	}
	public void Deconecter(){
		canalecriture.println("SORT/"+nom+"/");
		canalecriture.flush();
		this.remove(pan);
		this.setContentPane(rp);
		this.revalidate();
		//this.identifier=true;
		this.revalidate();
	}


	public static void main(String args[]){
		//System.out.println(Character.compare('Ø', 'p'));
		//	new Fenetre();

	}

}


class Tmp extends JPanel{
	@Override
	public void paintComponent(Graphics g){
		g.setColor(Color.WHITE);
		g.fillRect(0, 0, this.getWidth(), this.getHeight());
	}
}

class Ecouteur extends Thread{
	Fenetre fen;
	@Override
	public void run(){
		while(true){
			fen.ecoute();
		}
	}
	public Ecouteur(Fenetre fen) {
		this.fen = fen;
	}


}






