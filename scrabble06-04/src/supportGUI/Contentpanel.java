package supportGUI;

import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.LayoutManager;
import java.awt.PaintContext;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Vector;

import javax.sound.midi.Soundbank;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.sound.sampled.LineUnavailableException;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;

public class Contentpanel extends JPanel {
	Fenetre parent;
	plateau p;
	int height,widht,min = 1,sec = 0;
	Haut hau;
	plateau centre=new plateau();
	Tirage ba;
	JPanel est=new JPanel();
	JPanel tirage=new JPanel();
	JPanel score=new JPanel();
	Joeur joueur;
	Raison raison;
	Tm ch;
	JButton deconec=new JButton("deconecté");
	ActionListener dec=new ActionListener() {
		
		@Override
		public void actionPerformed(ActionEvent e) {
			parent.Deconecter();
			
		}
	};
	Chrono tem;

	public Contentpanel(int heig,int widh,int min,int sec, Fenetre pa) {
		deconec.addActionListener(dec );
		this.parent=pa;
		this.height=heig;
		this.widht=widh;
		this.min=min;
		this.sec=sec;
		tem=new Chrono(min, sec);
		hau=new Haut();
		ba=new Tirage(this);
		joueur=new Joeur();
		raison=new Raison();
		this.setPreferredSize(new Dimension(heig,widh));
		hau.setPreferredSize(new Dimension(heig,40));
		hau.add(deconec,BorderLayout.EAST);
		ba.setPreferredSize(new Dimension(heig,30));
		est.setPreferredSize(new Dimension(300,widh-100));
		tem.setPreferredSize(new Dimension(280,20));
		joueur.setPreferredSize(new Dimension(280,widh-200));
		raison.setPreferredSize(new Dimension(280,20));
		centre.setPreferredSize(new Dimension(heig-300,widh-100));
		score.add(new JLabel("points: "),BorderLayout.WEST);
		score.add(new JTextField(5),BorderLayout.CENTER);
		score.add(new JLabel("ordre: "),BorderLayout.EAST);
		score.add(new JTextField("1 "),BorderLayout.EAST);
		//	ba.add(new JLabel("Tirage"), BorderLayout.WEST);
		//	ba.add(tirage, BorderLayout.CENTER);
		//	ba.add(score, BorderLayout.EAST);
		//tirage.setPreferredSize(new Dimension(heig-300,40));
		Font police = new Font("while", Font.BOLD, 20);
		for(int i=1;i<=7;++i){
			JTextField tf=new JTextField(2);
			tf.setDragEnabled(true);
			tf.setFont(police);
			tirage.add(tf);

		}

		tirage.add(new JButton("submit"));
		tem.setFont(police);
		tem.setBorder(BorderFactory.createLineBorder(Color.blue));
		JLabel sidi=new JLabel("sidi 10  156");
		JLabel laly=new JLabel("laly 10  156");
		JLabel mou=new JLabel("moussa 10  156");
		joueur.setBorder(BorderFactory.createLineBorder(Color.blue));

		sidi.setPreferredSize(new Dimension(280,20));
		laly.setPreferredSize(new Dimension(280,20));
		mou.setPreferredSize(new Dimension(280,20));
		//joueur.addJoeur("sidi");
		//joueur.addJoeur("laly");
		//joueur.add(mou);

		est.add(tem, BorderLayout.NORTH);
		est.add(new JScrollPane(joueur), BorderLayout.CENTER);
		est.add(raison, BorderLayout.SOUTH);


		//hau.setBorder(BorderFactory.createLineBorder(Color.blue));
		ba.setBorder(BorderFactory.createLineBorder(Color.black));
		centre.setBorder(BorderFactory.createLineBorder(Color.blue));
		est.setBorder(BorderFactory.createLineBorder(Color.blue));
	//	centre.setText(1);
		this.add(hau,BorderLayout.NORTH);
		this.add(centre,BorderLayout.CENTER);
		this.add(new JScrollPane(est),BorderLayout.EAST);
		this.add(ba,BorderLayout.SOUTH);
		//tem.go();
		ch=new Tm(tem);
		ch.start();
		//this.setLayout(new GridLayout(3, 3));
	}

	public void drawtextarea(){
		/*	this.add(new TextArea("1"));
	this.add(new TextArea("2"));
	this.add(new TextArea("3"));
	this.add(new TextArea("4"));
	this.add(new TextArea("5"));
	this.add(new TextArea("6"));
	this.add(new TextArea("7"));*/
	}
	
	
	@Override
	public void paintComponent(Graphics g2d) {




		//System.out.println("o\n");

	}

	public void setChrono(int t){
		tem.setChrono(t/60, t %60);
	}

	public int getMin() {
		return min;
	}

	public void setMin(int min) {
		this.min = min;
	}

	public int getSec() {
		return sec;
	}

	public void setSec(int sec) {
		this.sec = sec;
	}

	public void addJoueur(String jr){
		this.joueur.addJoeur(jr);
		joueur.repaint();
	}
	public void addMin(){
		//JLabel tem=new JLabel(this.min+":"+this.sec,JLabel.CENTER);
		//JLabel l=new JLabel("sidi");
		//l.setPreferredSize(new Dimension(280,20));
		//joueur.add(l);
	}

	public void decTem() {
		this.tem.decChrono();
	}

	public void setTem(Chrono tem) {
		this.tem = tem;
	}

	public String getPlateau() {
		return centre.getPlateau();
	}

	public void setPlateau(String p) {
		this.centre.setPlateau(p);

	}

	public Tirage getTirage() {
		return ba;
	}

	public void setTirage(String tirag) {
		this.ba.setTirage(tirag);
	}
	public void setRaison(String r){
		this.raison.setRaison(r);
	}

	public void sendPlateu(){
		String pl=getPlateau();
		parent.sendplateu(pl);
	}
	public void invalidplateau(){
		centre.invalidplateau();
	}
	public void removjoueur(String jr){
		this.joueur.removjoueur(jr);
	}
	public void setTour(String i){
		hau.setTour(i);
	}
}

class plateau extends JPanel{

	public plateau() {
		this.setLayout(new GridLayout(15, 15));
		Font police = new Font("while", Font.BOLD, 20);
		for(int i=1;i<=225;++i){ 
			JTextField t=new JTextField("");
			t.setFont(police);
			//	t.setBackground(new Color(255, 250, 255));
			t.setDragEnabled(true);
			//t.setPreferredSize(new Dimension(100, 100));
			this.add(t);
			//Font police = new Font("while", Font.BOLD, 20);

		}
		JTextField t=(JTextField) this.getComponent(15*7+7);
		t.setBackground(Color.MAGENTA);
	}

	public void invalidplateau(){
		for(int i=1;i<=225;++i){ 
			JTextField t=(JTextField) this.getComponent(i);
			if(t!=null)
				t.disable();
		}
	}
/*	public void setText(int i){
		JTextField t=(JTextField) this.getComponent(15*7+7);
		if(t!=null)
			//t.setText("sidi");
		//Font police = new Font("red", Font.BOLD, 20);
		t.setBackground(Color.magenta);
	}*/

	public String getPlateau(){
		String pl="";
		for(int i=0;i<224;i++){
			JTextField t=(JTextField) this.getComponent(i);
			if(t!=null){
				String tx=t.getText();
				if(t.getText().equals("Ø")){

					pl=pl+"Ø";
				}else{
					pl=pl+tx;

				}
			
			}
		}
		//System.out.println(pl);

		return pl;
	}

	public void setPlateau(String plateau){
		char[] p=plateau.toCharArray();
		//Font police = new Font("red", Font.BOLD, 20);
		if(p.length==225)
		{


			for(int i=0;i<224;i++){System.out.println(p[i]);
				JTextField t=(JTextField) this.getComponent(i);
				if(t!=null){
					if(Character.compare('0', p[i])==0){
						t.setText("");//t.setFont(police);
					}else{
						t.setText(Character.toString(p[i]));
						t.setBackground(Color.magenta);
						t.disable();
					}
				}
			}
		}else{
			
		}
	}



}

class Joeur extends JPanel{
	int y=20;
	boolean debut=true,removjr=false;
	Vector<String> tab_joeur=new Vector<String>();
	HashMap<String, Point> mapjoueur=new HashMap<String, Point>();
	String joeur="";
	Point jouerremov;
	public Joeur() {
		debut=false;
	}
	public Joeur(String j) {
		this.joeur = j;
	}
	@Override
	public void paintComponent(Graphics g){
		g.setColor(Color.black);
		if(debut){
			for(String j:tab_joeur)
				g.drawString(j,15, (y=y+20));
		}else{
			g.drawString(joeur,15, y);
			this.y=y+20;
		}
		g.setColor(Color.white);
		if(removjr)
			g.drawString("joeur deconecté", jouerremov.x,jouerremov.y);
	}

	public void addJoeur(String joeur){
		mapjoueur.put(joeur, new Point(15,y+20));
		this.joeur=joeur;
		this.repaint();
		tab_joeur.add(joeur);
	}
	public void removjoueur(String jr){
		if(mapjoueur.containsKey(jr)){
			jouerremov=mapjoueur.get(jr);
			removjr=true;
		}
	}
}

class Chrono extends JPanel{
	int min,sec;

	public Chrono(int min, int sec) {
		this.min = min;
		this.sec = sec;

	}
	@Override
	public void paintComponent(Graphics g){
		g.setColor(Color.white);
		g.fillRect(0, 0, this.getWidth(), this.getHeight());
		g.setColor(Color.red);
		g.drawString(""+min+":"+sec,30,17);
	}
	public void decChrono(){

		boolean ok=true;
		this.getToolkit().beep();
		if(min!=0 ||sec!=0){
			if(sec==0){
				this.min=this.min-1;
				this.sec=59;
				if(min==0)
					ok=false;
			}
			else
				this.sec=sec-1;
			this.repaint();
		}

	}
	public void setChrono(int m,int s){
		this.min=m;
		this.sec=s;

	}
	public void  go(){// pan.getP();


		for(;;){
			//int m=pan.getMin();
			//pan.addJoueur("sidi"+i);
			decChrono();


			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}	

		}

	}


}

class Tirage extends JPanel{
	Contentpanel parent;
	public Tirage(Contentpanel parent) {
		this.parent=parent;
		GridLayout g=new GridLayout(1, 13);
		g.setHgap(5);
		this.setLayout(g);
		this.add(new JLabel("tirage"));
		Font police = new Font("white", Font.BOLD, 20);
		for(int i=1;i<=7;++i){
			JTextField tf=new JTextField(1);
			tf.setDragEnabled(true);
			tf.setFont(police);
			this.add(tf);
		}
		JButton submit=new JButton("submit");
		submit.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				parent.sendPlateu();
			}
		});
		this.add(submit);
		this.add(new JLabel("points:"));
		this.add(new JTextField(5));
		this.add(new JLabel("ordre:"));
		this.add(new JTextField(1));

	}

	public void setTirage(String tir){
		char[] tira=tir.toCharArray();int ind=0;
		if(tira.length==7)
		{
			for(int i=1;i<8;++i){
				JTextField t =(JTextField)this.getComponent(i);
				if(t!=null)
					t.setText(Character.toString(tira[ind++]));
			}
		}else{
			System.out.println("tirage incorecte dans class tirage(settirage");
		}
	}
}


class Raison extends JPanel{
	String raison="";
	@Override
	public void paintComponent(Graphics g){
		g.setColor(Color.white);
		g.fillRect(0, 0, this.getWidth(), this.getHeight());
		g.setColor(Color.blue);
		Font police = new Font("white", Font.BOLD, 17);
		g.setFont(police);
		g.drawString("raison:", 5, 12);
		g.setColor(Color.red);
		g.drawString(raison, 70, 12);


	}

	public void setRaison(String r){
		this.raison=r;
		this.repaint();
	}
}

class Tm extends Thread{
	Chrono tem;

	public Tm(Chrono tem) {
		this.tem = tem;
	}

	@Override
	public void run(){
		while(true){
			tem.decChrono();
			try {
				this.sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}

class Haut extends JPanel{
	String tour="1";
	String tr="tour "+tour;
	@Override
	public void paintComponent(Graphics g){
		g.setColor(Color.WHITE);
		g.fillRect(0, 0, this.getWidth(), this.getHeight());
		Font police = new Font("white", Font.BOLD, 19);
		g.setFont(police);
		g.setColor(Color.BLACK);
		g.drawString(tr,15, this.getHeight()/2);
	}
	public void setTour(String i){
		this.tour=i;
		repaint();
	}
}










