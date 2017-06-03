open Printf
open Pervasives
open Big_int;;
(* 
*compilation: ocamlc nums.cma pstl.ml
*compilation fichier dot 
* dot -Tpng -O file.dot  pour voir l'ABR avec graphiz
*)

let file="calcul/bn.txt"
let filsum="sum.txt"
  
type abr=Nil|Nd of abr*int*abr

(**** fonction Random pour big_int ************)
   
let random_big_int  bigint=
    let nbit=Big_int.num_bits_big_int bigint  in
          let ok=ref false and res=ref Big_int.zero_big_int in
        while not(!ok) do    
          res:=Big_int.zero_big_int;                     
         for i=1 to nbit do   
            if Random.bool () then
              begin        
                 res:=Big_int.add_big_int !res  (Big_int.power_big_int_positive_int (Big_int.big_int_of_int 2) i);
               end;          
         done;
         
          if Big_int.le_big_int  !res  bigint then
          begin
            ok:=true;
            end   ;           
  
        done; 
         !res    
	   
(**************calcules des bn**********************)
let bn n=
 let namebn="calcul/bn/b0.txt" in
    let writbn = open_out namebn  in
    fprintf writbn "1\n";
    close_out writbn;
    
    (*b: un tableau contenant les Bn et array_sum : une matrice contenant les Bk*B(n-k) *)
  for i=1 to n do
    let namesumfil="calcul/prodb"^string_of_int i^".txt" in 
    let  osum= open_out namesumfil  in
    let sum=ref zero_big_int  in
    for j=0 to i-1 do
      let readbn1=open_in ("calcul/bn/b"^string_of_int j^".txt") and 
       readbn2=open_in ("calcul/bn/b"^string_of_int (i-1-j) ^".txt")  in
       let valbn1=big_int_of_string (input_line readbn1) and
           valbn2=big_int_of_string (input_line readbn2)  in
        close_in readbn1; 
        close_in readbn2; 
      let tmp=mult_big_int valbn1 valbn2 in
       fprintf osum "%s\n" (string_of_big_int (tmp)); 
      sum:= add_big_int !sum tmp
    done;
    let namebn="calcul/bn/b"^string_of_int i^".txt" in
    let writbn = open_out namebn  in
    fprintf writbn "%s\n" (string_of_big_int (!sum));
    close_out writbn;
    close_out osum;
  done
  
 
(***** calcul taille arbre gauche et droite *******)
let size_left_and_right n nbtirer  =
	   let leftindex= ref 0 in
	   let namesumfil="calcul/prodb"^string_of_int n^".txt" in 
       let  osum= open_in namesumfil  in	       
       	     let prod=ref (big_int_of_string (input_line osum)) in
	         let sub=ref (sub_big_int ( nbtirer) !prod )in
	         while gt_big_int  (!sub)  zero_big_int do
		    leftindex:= (!leftindex)+1;
		    prod:=big_int_of_string (input_line osum);
		    sub:=sub_big_int (!sub) (!prod);	            
		 done;	  
		 let nbt=add_big_int (!sub) (!prod) in  
		let namebn="calcul/bn/b"^string_of_int (!leftindex)^".txt" in 
		let  b= open_in namebn  in
	    let (q,r)=Big_int.quomod_big_int nbt (big_int_of_string (input_line b)) in
	    close_in osum;
	    close_in b;
     ((!leftindex,r),(n-(!leftindex)-1,q))
     
(***********construction de l'arbre ****************)
               
let rec consabr l r v   =
      let (lf,rlf)=l and (rt,rrt)=r in
     match  (lf,rt) with
       |(0,0)->Nd(Nil,!v,Nil)
       |(l1,0)->let (lf1,rt1)=size_left_and_right  l1 rlf   in 
         let encv= !v in
         v:=!v+1;
	    Nd(consabr lf1 rt1  v ,encv,Nil)
       |(0,r1)->let (rt1,rrt1)=size_left_and_right  r1 rrt   in
         let encv= !v in
         v:=!v+1;
		 Nd(Nil,encv,consabr rt1 rrt1 v )
       |(l1,r1)->let (lf1,rt1)=size_left_and_right  l1 rlf   and (lf2,rt2)=size_left_and_right r1 rrt   in
         let encv= !v  in
         v:=!v+1;
         let left=consabr lf1 rt1 v   in
           v:=!v+1;
         let right=consabr lf2 rt2 v   in
		     Nd(left,encv,right)



   (*******printage********)
   
      
      let rec print_in_file arb n  oc=
       n:=!n+1  ;
      let h="nil"^string_of_int (!n)   in 
      match arb with
	 |Nil-> fprintf oc "%s \n %s [label= \"Lp\"]\n" h h
	 |Nd(Nil,v,Nil)->fprintf oc "%d--" v; fprintf oc "%s\n" h;fprintf oc "%d--" v; 
	   let g="nil"^string_of_int (!n+1)  in
	   fprintf oc "%s" g;
	   fprintf oc "\n %d  [label= \"Nd\"]\n" v;
	   fprintf oc "\n %s  [label= \"Lp\"]\n" h;
	   fprintf oc "\n %s  [label= \"Lp\"]\n" g;
	 |Nd(l,v,Nil)-> fprintf oc "%d--" v;print_in_file l (n) oc;
	   fprintf oc "\n %d  [label= \"Nd\"]\n" v;
	   fprintf oc "\n%d--" v;fprintf oc "%s" h;
	   fprintf oc "\n %s  [label= \"Lp\"]\n" h
	 |Nd(Nil,v,r)-> fprintf oc "%d--" v;
	                fprintf oc "%s" h;
	                fprintf oc "\n%d--" v;
	                print_in_file r (n) oc;
	                fprintf oc "\n %d  [label= \"Nd\"]\n" v;
	                fprintf oc "\n %s  [label= \"Lp\"]\n" h

	 |Nd(l,v,r)-> fprintf oc "%d--" v;print_in_file l (n) oc;
	   fprintf oc "\n %d  [label= \"Nd\"]\n" v;
	   fprintf oc "\n";n:=!n+1;
		          fprintf oc "%d--" v;print_in_file r (n)  oc
	
  
  let rec print_online abr enlign =
       match abr with
       |Nil->fprintf enlign "()"
       |Nd(Nil,v,Nil)->fprintf enlign "(()())"
       |Nd(l,v,Nil)->fprintf enlign "(" ;print_online l enlign;fprintf enlign "())"
       |Nd(Nil,v,r)->fprintf enlign "(()"; print_online r enlign; fprintf enlign ")"
       |Nd(l,v,r)->fprintf enlign "(" ;print_online l enlign ;print_online r enlign;fprintf enlign ")" 
  
  
   
     let print_abr ab n=
      if Array.length Sys.argv<3  then
       begin
       
       end;
     let file = "file.dot"  and online="en_ligne.txt" in
      let oc = open_out file  and onl= open_out online  in
      print_online ab onl;
       close_out onl;

      let _=fprintf oc "graph {\n  node [color=blue, shape=circle] \n" in
           print_in_file ab n oc;
       let _=fprintf oc "\n}"in
       close_out oc
    
      
   let main ()=
     Random.self_init ();

     print_string "donner la taille de l'arbre ";
     let n=read_int () in
    bn n ;
   let namebn="calcul/bn/b"^string_of_int n^".txt" in 
    let b= open_in namebn  in
    let valeur=big_int_of_string (input_line b) in
    close_in b;
    let nbtirer=random_big_int (sub_big_int valeur (Big_int.big_int_of_int 1)) in
    let  (l,r)=size_left_and_right n nbtirer   and v=ref 0 in

    let abr= consabr l r v   and va=ref 0 in
    print_abr abr va;
    Printf.printf "\ntemps d'execution: %f secondes\n" (Sys.time ())

   ;;


  main ()
 
 
 
 
 
