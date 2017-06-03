
(*  Compilation:ocamlc nums.cma ternaire.ml 
*  compilation du fichier dot;dot -Tpdf -O file_abr.dot 
*)
open Printf
open Big_int;;


type abr= Nil|Nd1 of int*abr|Nd2 of abr*int*abr|Nd3 of abr*int*abr*abr

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
	   	   
let min3 a b c =
    let min=ref 0 in
    if a< b then
     begin
      min:=a;
      end
      else
      begin
       min:=b;
      end; 
	 if  c <(!min) then
     begin
      min:=c;
	 end;
	 !min
	 
let max3 a b c =
    let max=ref 0 in
    if  a< b then
     begin
      max:=b;
      end
      else
      begin
       max:=a;
      end; 
	 if (!max) <c then
     begin
      max:=c;
	 end;
	 !max

let bn n=
 let namebn="calcul/tn/t0.txt" in
    let writbn = open_out namebn  in
    fprintf writbn "1\n";
    close_out writbn;
    
      for i=1 to n do
          let namesumfil="calcul/tnsum/prodt"^string_of_int i^".txt" in 
          let  osum= open_out namesumfil  in
          let sum=ref Big_int.zero_big_int in
          let ind=ref 0 in 
          let readb=open_in ("calcul/tn/t"^string_of_int (i-1)^".txt") in
          sum:=big_int_of_string (input_line readb) ;
          fprintf osum "%s\n" (string_of_big_int (!sum));
          ind:=!ind+1;
         for j=0 to i-1 do
             let readbn1=open_in ("calcul/tn/t"^string_of_int j^".txt") and 
             readbn2=open_in ("calcul/tn/t"^string_of_int (i-1-j) ^".txt")  and
             prodbn=open_out ("calcul/tmpprod/t"^string_of_int (i-1-j) ^"t"^string_of_int (j)^".txt")  in
             let valbn1=big_int_of_string (input_line readbn1) and
             valbn2=big_int_of_string (input_line readbn2)  in
             close_in readbn1; 
             close_in readbn2; 
            let tmp=Big_int.mult_big_int valbn1 valbn2 in
            fprintf osum "%s\n" (string_of_big_int (tmp)); 
            fprintf prodbn "%s\n" (string_of_big_int (tmp)); 
            close_out prodbn;
            sum:=Big_int.add_big_int !sum tmp;
            ind:=!ind+1;
         done;  
         
         for j=0 to i-1 do
            for k=0 to i-1-j do
                 let min =min3 k j (i-1-j-k) and max=max3 k j (i-1-j-k) in
                 let autre=k+j+(i-1-j-k)-min-max in 
                 let prodbn=open_in ("calcul/tmpprod/t"^string_of_int max^"t"^string_of_int autre^".txt") and 
                     minbn=open_in ("calcul/tn/t"^string_of_int min^".txt") in 
                    
                     let  pbn=big_int_of_string (input_line prodbn) and
                         minn=big_int_of_string (input_line minbn) in
                    
		                 close_in prodbn; 
					     close_in minbn; 
           
               let tmp= Big_int.mult_big_int minn pbn in
                (*  printf "tmp=%s b1.b2.b3=%s\n" (string_of_big_int tmp)
                  (Big_int.string_of_big_int( Big_int.mult_big_int minn pbn)) ;*)

                fprintf osum "%s\n" (string_of_big_int (tmp)); 
                sum:=Big_int.add_big_int !sum tmp;
                ind:=!ind+1;

            done;
         done;  
        close_out osum;
        let namebn="calcul/tn/t"^string_of_int i^".txt" in
        let writbn = open_out namebn  in
        fprintf writbn "%s\n" (string_of_big_int !sum);
         close_out writbn;
        done 
   
let rec ternaire n nbtirer v  =
  v:=!v+1;
  let oldv=(!v) in
  let k= ref (-1)  and j=ref (-1) and i=ref 0 in
  let sub=ref zero_big_int in 
  if n>0 then
   begin
     let namebn="calcul/tnsum/prodt"^string_of_int n^".txt" in 
	 let  bnprod= open_in namebn  in
	 let produi= ref (big_int_of_string(input_line bnprod)) in
     sub:=sub_big_int ( nbtirer) (!produi);
     while gt_big_int  (!sub) zero_big_int do
     	 produi:=big_int_of_string(input_line bnprod);
         sub:=sub_big_int (!sub) (!produi) ;
         k:=!k+1;
         if !k>n-1 then
          begin
            j:=!j+1;
            if !j>n-1-(!i) then
             begin
               i:=!i+1;
               j:=0;
             end
          end
	 done;
	 sub:=add_big_int (!sub) (!produi);
	 close_in bnprod;
  end;
	 match n with
	     |0 ->(*let rand=Big_int.mod_big_int (Big_int.add_int !sub matr.(n).(!k)) bn.(n-1) and v:=!v+1 in*)
	          Nil
	     |t-> if !k==(-1) then
	            begin
	               let filbn="calcul/tn/t"^string_of_int (n-1)^".txt" in 
	               let  bn= open_in filbn  in
	               let rand=Big_int.mod_big_int !sub (big_int_of_string(input_line bn)) in
	               close_in bn;
	               v:=!v+1;
	               Nd1(oldv,ternaire (n-1) rand v)
                end	           
	          else  if !k<n then
	            begin
	            let filbn="calcul/tn/t"^string_of_int (!k)^".txt" in 
	            let  bn= open_in filbn  in
	            let valbn=big_int_of_string (input_line bn) in close_in bn;
	             let rand1=Big_int.mod_big_int !sub valbn and 
	                rand2=Big_int.div_big_int  !sub valbn  in
	                v:=!v+1;
	               let abr1=ternaire (!k) rand1 v in
	               v:=!v+1;
	                let abr2=ternaire (n-(!k)-1) rand2 v  in
	               Nd2(abr1,oldv,abr2)
	                
	            end
	            else
	            begin
	              
	                let filbn1=open_in ("calcul/tn/t"^string_of_int (!i)^".txt") in 
	                let filbn2=open_in("calcul/tn/t"^string_of_int (!j)^".txt" )in 
	               (* let filbn3=open_in ("calcul/bn/b"^string_of_int (n-1-(!i)-(!j))^".txt" )in *)
	                let b1=big_int_of_string (input_line filbn1) and
	                    b2=big_int_of_string (input_line filbn2)in
	                   (* b3=big_int_of_string (input_line filbn3)in*)
		            let rand1=Big_int.mod_big_int !sub b1 in
		            let p=Big_int.div_big_int !sub b1 in
		            let rand2=Big_int.mod_big_int p b2 and
		                  rand3=Big_int.div_big_int p b2 in
		              close_in filbn1;
	                  close_in filbn2;
	                 (* close_in filbn3;*)
		                  v:=!v+1;
		                  let abr1=ternaire (!i) rand1 v in
		                  v:=!v+1;
		                  let abr2=ternaire (!j) rand2 v  in
		                  v:=!v+1;
	                      let abr3=ternaire (n-(!i)-(!j)-1) rand3 v in
	                      Nd3(abr1,oldv,abr2,abr3)
		         end
	   

  let rec print_in_file arb n file_abr enligne=
     
      match arb with
	 |Nil->  n:=!n+1  ;
            let h="nil"^string_of_int (!n)   in 
	        fprintf file_abr "%s \n %s [label= \"Lp\"]\n" h h;fprintf enligne "()"
	 |Nd1(v,Nil)-> fprintf file_abr "%d--" v; fprintf enligne "(())";
	                n:=!n+1  ;
                   let h="nil"^string_of_int (!n)   in 
	               fprintf file_abr "%s \n " h ;
	               fprintf file_abr "\n %d  [label= \"Nd\"]\n" v;
	               fprintf file_abr "\n %s  [label= \"Lp\"]\n" h;
	 |Nd1(v,chld)-> fprintf file_abr "%d--" v; fprintf enligne "(";
	                n:=!n+1;
	                print_in_file chld n file_abr enligne;
	                fprintf enligne ")";
	                fprintf file_abr "\n %d  [label= \"Nd\"]\n" v;
	 |Nd2(Nil,v,Nil)->fprintf file_abr "%d--" v;fprintf enligne "(()())";
	                   n:=!n+1  ;
                   let h="nil"^string_of_int (!n)   in 
	                 fprintf file_abr "%s\n" h;
	                 fprintf file_abr "%d--" v;
	                 n:=!n+1;
	                 let g="nil"^string_of_int !n  in
	                 fprintf file_abr "%s" g;
	                 fprintf file_abr "\n %d  [label= \"Nd\"]\n" v;
	                 fprintf file_abr "\n %s  [label= \"Lp\"]\n" h;
	                 fprintf file_abr "\n %s  [label= \"Lp\"]\n" g;
	 |Nd2(l,v,Nil)-> fprintf file_abr "%d--" v;
	                n:=!n+1;
	                 fprintf enligne "(";
	                print_in_file l n file_abr enligne;
	                 fprintf enligne "())";
				    fprintf file_abr "\n%d--" v;
				     n:=!n+1  ;
                    let h="nil"^string_of_int (!n)   in 
				    fprintf file_abr "%s" h;
				    fprintf file_abr "\n %d  [label= \"Nd\"]\n" v;
				    fprintf file_abr "\n %s  [label= \"Lp\"]\n" h
                    
	(* |Nd2(l,v,Nil)-> fprintf file_abr "%d--" v;
	                n:=!n+1;
	                print_in_file l n file_abr;
				    fprintf file_abr "\n%d--" v;
				    fprintf file_abr "%s" h;
				    fprintf file_abr "\n %d  [label= \"Nd\"]\n" v;
				    fprintf file_abr "\n %s  [label= \"Lp\"]\n" h*)
	 |Nd2(Nil,v,r)-> fprintf file_abr "%d--" v;
	               n:=!n+1  ;
	                fprintf enligne "(()";
                   let h="nil"^string_of_int (!n)   in 
	                fprintf file_abr "%s" h;
	                fprintf file_abr "\n%d--" v;
	                n:=!n+1;
	                print_in_file r n file_abr enligne;
	                fprintf enligne ")";
				    fprintf file_abr "\n %d  [label= \"Nd\"]\n" v;
				    fprintf file_abr "\n %s  [label= \"Lp\"]\n" h
	 |Nd2(l,v,r)-> fprintf file_abr "%d--" v;
	               n:=!n+1;
	                fprintf enligne "(";
	              print_in_file l n file_abr enligne;
	               n:=!n+1;
	              fprintf file_abr "\n%d--" v;
		          print_in_file r n  file_abr enligne;
		           fprintf enligne ")";
	              fprintf file_abr "\n %d  [label= \"Nd\"]\n" v;
	 |Nd3(Nil,v,Nil,Nil)->fprintf file_abr "%d--" v; 
	 	                fprintf enligne "(()()())";
	                       n:=!n+1  ;
                         let h="nil"^string_of_int (!n)   in 
		                 fprintf file_abr "%s\n" h;
		                 fprintf file_abr "%d--" v;
		                 n:=!n+1;
		                 let g="nil"^string_of_int !n  in
		                 n:=!n+1;
		                 let e="nil"^string_of_int !n in
		                 fprintf file_abr "%s\n" g;
		                 fprintf file_abr "%d--" v;
		                 fprintf file_abr "%s\n" e;
		                 fprintf file_abr "\n %d  [label= \"Nd\"]\n" v;
		                 fprintf file_abr "\n %s  [label= \"Lp\"]\n" h;
		                 fprintf file_abr "\n %s  [label= \"Lp\"]\n" g;
		                 fprintf file_abr "\n %s  [label= \"Lp\"]\n" e;
	 |Nd3(l,v,Nil,Nil)->fprintf file_abr "%d--" v; 
	                     n:=!n+1;
	                     fprintf enligne "(";
		                 print_in_file l n  file_abr enligne;
		                 fprintf enligne "()())";
		                 fprintf file_abr "\n%d--" v;
		                 n:=!n+1;
		                 let g="nil"^string_of_int !n  in
		                 n:=!n+1;
		                 let e="nil"^string_of_int !n in
		                 fprintf file_abr "%s\n" g;
		                 fprintf file_abr "%d--" v;
		                 fprintf file_abr "%s\n" e;
		                 fprintf file_abr "\n %d  [label= \"Nd\"]\n" v;
		                 fprintf file_abr "\n %s  [label= \"Lp\"]\n" g;
		                 fprintf file_abr "\n %s  [label= \"Lp\"]\n" e;
	 |Nd3(Nil,v,r1,Nil)->fprintf file_abr "%d--" v; 
	                     n:=!n+1  ;
	                     fprintf enligne "(()";
                        let h="nil"^string_of_int (!n)   in 
	                    fprintf file_abr "%s\n" h;
	                    fprintf file_abr "%d--" v; 
	                     n:=!n+1;
		                 print_in_file r1 n  file_abr enligne;
		                 fprintf enligne "())";
		                 fprintf file_abr "\n%d--" v;
		                 n:=!n+1;
		                 let g="nil"^string_of_int !n  in
		                 fprintf file_abr "%s\n" g;
		                 fprintf file_abr "\n %d  [label= \"Nd\"]\n" v;
		                 fprintf file_abr "\n %s  [label= \"Lp\"]\n" g;
		                 fprintf file_abr "\n %s  [label= \"Lp\"]\n" h;
	 |Nd3(Nil,v,Nil,r1)->fprintf file_abr "%d--" v; 
	                      fprintf enligne "(()()";
	                      n:=!n+1  ;
                         let h="nil"^string_of_int (!n)   in 
	                    fprintf file_abr "%s\n" h;
	                    fprintf file_abr "%d--" v; 
	                     n:=!n+1;
		                 let g="nil"^string_of_int !n  in
		                 fprintf file_abr "%s\n" g;
		                 fprintf file_abr "%d--" v; 
		                 n:=!n+1;
		                 print_in_file r1 n  file_abr enligne;
		                 fprintf enligne ")";
		                 fprintf file_abr "\n %d  [label= \"Nd\"]\n" v;
		                 fprintf file_abr "\n %s  [label= \"Lp\"]\n" g;
		                 fprintf file_abr "\n %s  [label= \"Lp\"]\n" h;
	 |Nd3(l,v,r1,Nil)->fprintf file_abr "%d--" v; 
	                    fprintf enligne "(";
	                    n:=!n+1;
	                    print_in_file l n  file_abr enligne;
	                    fprintf file_abr "\n%d--" v; 
	                    n:=!n+1;
	                    print_in_file r1 n  file_abr enligne;
	                    fprintf enligne "())";
	                    n:=!n+1  ;
                        let h="nil"^string_of_int (!n)   in 
	                    fprintf file_abr "\n%d--" v; 
	                    fprintf file_abr "%s\n" h;
		                fprintf file_abr "\n %d  [label= \"Nd\"]\n" v;
		                fprintf file_abr "\n %s  [label= \"Lp\"]\n" h;
	 |Nd3(l,v,Nil,r1)->fprintf file_abr "%d--" v; 
	                   n:=!n+1;
	                   fprintf enligne "(";
	                   print_in_file l n  file_abr enligne;
	                   fprintf enligne "()";
	                   fprintf file_abr "\n%d--" v; 
	                   n:=!n+1  ;
                       let h="nil"^string_of_int (!n)   in 
	                   fprintf file_abr "%s\n" h;
	                   fprintf file_abr "\n%d--" v; 
	                   n:=!n+1;
	                   print_in_file r1 n  file_abr enligne;
	                   fprintf enligne ")";
		               fprintf file_abr "\n %d  [label= \"Nd\"]\n" v;
		               fprintf file_abr "\n %s  [label= \"Lp\"]\n" h;
	 |Nd3(Nil,v,r1,r2)->fprintf file_abr "%d--" v; 
	                     fprintf enligne "(()";
	                    n:=!n+1  ;
                        let h="nil"^string_of_int (!n)   in 
	                   fprintf file_abr "%s\n" h;
	                   fprintf file_abr "%d--" v; 
	                   n:=!n+1;
	                   print_in_file r1 n  file_abr enligne;
	                   fprintf file_abr "\n%d--" v; 
	                   n:=!n+1;
	                   print_in_file r2 n  file_abr enligne;
	                   fprintf enligne ")";
		               fprintf file_abr "\n %d  [label= \"Nd\"]\n" v;
		               fprintf file_abr "\n %s  [label= \"Lp\"]\n" h;
	 |Nd3(l,v,r1,r2)->fprintf file_abr "%d--" v; 
	                   n:=!n+1;
	                   fprintf enligne "(";
	                   print_in_file l n  file_abr enligne;
	                   fprintf file_abr "\n%d--" v; 
	                   n:=!n+1;
	                   print_in_file r1 n  file_abr enligne;
	                   fprintf file_abr "\n%d--" v; 
	                   n:=!n+1;
	                   print_in_file r2 n  file_abr enligne;
	                   fprintf enligne ")";
		               fprintf file_abr "\n %d  [label= \"Nd\"]\n" v
	

let print_tree_infile arb n=
     let file_abr=open_out ("file_abr.dot") and enlign=open_out ("en_ligne.dot")in 
     let _=fprintf file_abr "graph {\n  node [color=blue, shape=circle] \n" in
     print_in_file arb n file_abr enlign;
     let _= fprintf file_abr "\n}" in
     close_out file_abr
      
let main ()=
    
     Random.self_init ();
    print_string "donner la taille de l'arbre ";
    let n=read_int () in     
     let _= bn n in  
      let filbn=open_in ("calcul/tn/t"^string_of_int n^".txt") in 
    let valbn=big_int_of_string (input_line filbn) in
    close_in filbn;
    let  rand=random_big_int (sub_big_int valbn unit_big_int) and v=ref 0 in
    let abr=ternaire n rand v in
    let v=ref 0 in
   print_tree_infile abr v;
    Printf.printf "\ntemps d'execution: %f secondes\n" ((Sys.time ()))
   ;;
    
    
      
      
main ()
      
      
      
      
