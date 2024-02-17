include<BOSL2/std.scad>
include<BOSL2/threading.scad>


z = 28;
xi = 60;
xa = 100;
yi = 80;
widthbase = 32;


widthx = (xa-xi);
widthy = widthbase*2;
ya = yi + widthy/2;
wall = 1.5;
clearance = 0.2;
cutheight = 4;
d1 = 2;
$fn=50;
lolinspace = 7.4;
dLED = 5.1;
//color("green")translate([5.1,5.2,-6.3])rotate([-180,0,0])import("lolin32_lite.stl");


module rodmount(mount = true){
      difference(){
      cuboid([15,15,8],rounding=2,except=[TOP,BOTTOM],anchor=BOTTOM);
      if(mount){
         threaded_rod(d=10, pitch = 2, $slop=0.2,internal=true,l = 30,orient=TOP,anchor=BOTTOM);
      }
      }//end difference
}//end rodmount



module oled_bezel(){
   module screwshaft(h=4,d1=2,d2=6){
      difference(){
      cylinder(d=d2,h=h);
      cylinder(d=d1,h=h+1);
     }
}
    th = 1.6;//thickness of panel
    difference(){
        down(th/2)cuboid([26.5+2.4,27.5+2.4,th],anchor=BOTTOM);
        //cuboid([33,33,th]);
        back(1)down(th/2)prismoid([24,14],[26,16], chamfer=1, h=th);
    }
    a = 11;
    b = 11;
    h=1.9; //heigth of screwshaft
    down(h+th/2-0.1)for(i=[[a,b,0],[a,-b,0],[-a,b,0],[-a,-b,0]]){
        translate(i)screwshaft(h,2,4);
    }
    //down(th/2+4)rect_tube(isize=[26.5,27.5], wall=1.2, h=4);
    //if(modules)down(4.6)color("green")oled();
}

module buttoncase(cap = true){
   x = 14;
   y = 17;
   z = 14;
   xi = 12;
   yi = 14.2;
   tol = 0.8;
   difference(){
      cuboid([x,y,z],anchor=TOP);
      up(0.1)cuboid([xi,yi,10],anchor=TOP);
      fwd(1.4)down(6)cuboid([7,y,6],anchor=TOP);
      down(6)cuboid([xi,1.5,6],anchor=TOP);
   }
   // button cap
   if(cap){
      down(0.25)difference(){
         cuboid([xi-tol,yi-tol,4],anchor = BOTTOM);
         down(0.05)cylinder(d=7,h=3.5/2-0.1,anchor = BOTTOM);
         }//end difference
      for(i=[1:90:279]){
         rotate([0,0,i])translate([-4/2-0.75/2,0,0])union(){
            cuboid([0.75,2.7,3.5]);
            translate([-0.75/2,0,0.25])cuboid([1,1,3]);
            translate([0.75/2,2.7/2,-3.5/2+0.7])rotate([90,90,0])linear_extrude(2.7)polygon([[0,0],[0.7,0.5],[0.7,0]]);
         }//end union
      }//end for
      }
  }
module SnapFemale(l =yi, angy = 0,angz = 0,m=0){
   mirror([0,0,m])rotate([0,angy,angz])difference(){
               prismoid(size1=[d1*2,l-clearance*2], size2=[d1*3,l-clearance*2], h=d1,anchor=TOP);
               translate([-1+d1/8,0,d1/4])ycyl(d = d1+clearance*2, h = l-clearance*2);
               cuboid([clearance,l,d1*4/2],anchor = BOTTOM);
               }//end difference
               }//end SnapFemale


module Body(){
    difference(){
        cuboid([xa,ya,z],chamfer = 2, except = [LEFT,RIGHT],anchor=FRONT);
        translate([0,widthy/2,0])cuboid([xi,yi+0.1,z+1],chamfer = 2, except = [TOP,BOTTOM,BACK],anchor=FRONT);
        for(i=[-1,1]){
            translate([i*(xa/2-(widthx/2-wall)/2),wall,0])cuboid([widthx/2-wall+0.01,ya-2*wall,z-2*wall],chamfer = 2, except = [LEFT,RIGHT],anchor=FRONT);
            translate([i*(xa/2-wall),wall/2,0])cuboid([wall+clearance,ya-wall+clearance,z-wall+clearance],chamfer = 2, except = [LEFT,RIGHT],anchor=FRONT);
            }//end for
         translate([0,wall,0])cuboid([xa,widthy/2-2*wall,z-2*wall],chamfer = 2, edges = FRONT,anchor = FRONT);
        
        }//end difference
}//end body        
module Sidepanel(assemble = 0){
   translate(assemble*[(xa/2-wall),wall/2,0])cuboid([wall,ya-wall,z-wall],chamfer = 2, except = [LEFT,RIGHT],anchor=FRONT);

 }
 
 module leftpanel(button=false){
   difference(){
      Sidepanel();
      if(button)translate([-wall/3,15,0])cuboid([wall*2,17,14]);
   }//end difference
   if(button)translate([-wall/2,15,0])rotate([0,90,180])buttoncase(false);
}//end leftpanel
 
 
 module rightpanel(){
   difference(){
      Sidepanel();
      translate([-0.75,70,0])rotate([90,0,-90])switchcase(true);
      translate([wall,15.5,-1.8])rotate([0,0,90]){
       //translate([-7.7,-0.1,-0.4])cuboid([7,3,5.5],rounding=1,except=[FRONT,BACK],anchor=FRONT);
       translate([7.3,-0.1,-2.0])cuboid([10.5,3,4.5],rounding=1,except=[FRONT,BACK],anchor=FRONT);
       }
      translate([+wall+1,ya/2.5,0])rotate([0,-90,0])rodmount(false);

      }
      translate([+wall/2,ya/2.5,0])rotate([0,-90,0])rodmount();
      translate([-0.75,70,0])rotate([90,0,-90])switchcase(false);

      }
 module Top(oled=1){
    difference(){
        Body();
        translate([0,-0.01,cutheight])cuboid([xa+1,ya+1,z],anchor = FRONT+TOP);
        if(oled){
         translate([0,17,z/2-wall])cuboid([26.5+2.4,27.5+2.4,wall*2],anchor=BOTTOM);
        }
        //status LED
        //translate([xa/2-widthx/4,ya-20,0])zcyl(h=xi*3+2,d=dLED);
    }//end difference
    for(i=[-1,1]){
      translate([i*(xi/2+wall-d1/5),ya-(yi-clearance*2)/2-wall,cutheight+d1*0.6])ycyl(h=yi,d=d1);
      
      translate([i*(xa/2-(widthx/2-3*wall)/2-wall*1.5-clearance/2),ya-wall+0.01+d1/5,cutheight+d1*0.6])xcyl(h=widthx/2-3*wall,d=d1);

      }//end for
            translate([0,wall-d1/5,cutheight+d1*0.6])xcyl(h=xa-3*wall,d=d1);
            translate([0,widthy/2-wall+d1/5,cutheight+d1*0.6])xcyl(h=xi-3*wall,d=d1);
     if(oled){
      color("green")translate([0,17,z/2-0.8])oled_bezel();       
}

 }
 module switchcase(hole = false){
   a = [4.5,4.4,3];//top cut
   b=[5,6,2];
   c= [3.5,4.4,0.5];
   if(hole){
   cuboid([7,3.7,2],anchor=TOP);
   }else{
   left(10.7/2)difference(){
      cuboid(b,anchor=RIGHT+BOTTOM);
      cuboid(a,anchor=RIGHT+BOTTOM);
      cuboid(c,anchor=RIGHT+BOTTOM);
      }
    right(10.7/2)mirror([1,0,0])difference(){
      cuboid(b,anchor=RIGHT+BOTTOM);
      cuboid(a,anchor=RIGHT+BOTTOM);
      cuboid(c,anchor=RIGHT+BOTTOM);
      }  
   left(12.5/2+1.25/2+0.4)cyl(d=2.3,h=2,anchor=BOTTOM);
   right(12.5/2+1.25/2+0.4)cyl(d=2.3,h=2,anchor=BOTTOM);
   }
}

 module Bottom(){
  difference(){
        union(){
         Body();
         for(i=[-1,1]){
            translate([i*(xi/2+2*wall),ya-6,-z/2])cuboid([5,8,z/2+cutheight],anchor=BOTTOM+BACK);
         }//end for
         }//end union
        translate([0,-0.01,cutheight])cuboid([xa+1,ya+1,z],anchor = FRONT+BOTTOM);
        translate([0,ya-10,0])xcyl(h=xi*3+2,d=dLED);
        for(i=[-1,1]){
         translate([i*(xi/2+wall/6),ya-10,0])cuboid([wall/3,10,1],anchor=FRONT);
         }//end for
    }//end difference
    
    //female snaps
    for(i=[-1,1]){
            translate([i*(-xi/2-wall+0.01),widthy-2*wall,d1-d1/4+cutheight-1])SnapFemale(l =yi/1.5,angy=i*90,m=-1+i);
            translate([i*(xa/2-(widthx/2-3*wall)/2-wall*1.5),ya-wall+0.01,d1-d1/4+cutheight-1])SnapFemale(l = widthx/2-3*wall,angy=90,angz = 90);
            }//end for
    translate([0,wall-0.01,d1-d1/4+cutheight-1])SnapFemale(l = xa-3*wall,angy=-90,angz = 90,m=1);
    //translate([0,widthx/2-wall+0.01,d1-d1/4+cutheight-1])SnapFemale(l = xi-3*wall,angy=90,angz = 90);
    translate([0,widthy/2-wall+0.01,d1-d1/4+cutheight-1])SnapFemale(l = xi-3*wall,angy=90,angz = 90);
    
    //lolin mount
    translate([21.8,16.5,0])rotate([0,0,90])difference(){
        union(){
          translate([0,-23,-z/2])cuboid([26,8,lolinspace],anchor = BOTTOM+FRONT);
          translate([0,24,-z/2])cuboid([26,5.5,lolinspace],anchor = BOTTOM+BACK);
           }//end union
           translate([10.8,-17.6,-z/2])cyl(d=2,h=lolinspace+1,anchor = BOTTOM);
           translate([-10.3,21,-z/2])cyl(d=2,h=lolinspace+1,anchor = BOTTOM);
    }//end difference
           
 }
difference(){
    union(){
   up(10)up(0.0)Top(false);
   *down()Bottom();
   }//end union
   *down(5)fwd(1)cube(40);
   }
translate([-(xa/2-1.5*wall),wall/2,0])leftpanel();
translate([xa/2-1.5*wall,wall/2,0])rightpanel();



