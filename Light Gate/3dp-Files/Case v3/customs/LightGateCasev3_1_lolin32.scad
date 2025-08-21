include<BOSL2/std.scad>
include<BOSL2/threading.scad>


z = 28;
xi = 80;//60
xa = 140;//100
yi = 85;//80
widthbase = 37; //Minimum for lolin lite 32
rod = 1;

widthx = (xa-xi);
widthy = widthbase*2;
ya = yi + widthy/2;
wall = 2;
clearance = 0.2;
cutheight = 4;
d1 = 2;
$fn=50;
lolinspace = 17;
dLED = 5.1;


*color("green")translate([-38,18,-29+29])rotate([0,0,-90])import("Wemos-Lolin32.stl");
//


//module int_thread(){
//   d = 11;
//   threaded_rod(d = d, l = 15, pitch = 3, bevel = 1,internal = true);
//}
//
//module screw(l = 15,knob_a = 20, knob_c = 10){
//    d = 11;
//    knob_b = 6.2;
//    difference(){
//    translate([0,0,l/2+knob_c])threaded_rod(d = d, l = l, pitch = 3, bevel = true);
//    translate([0,d/2+3.1,l/2+knob_c])cube([d,d,l+2],center = true);
//    }
//    translate([0,0,knob_c/2])cuboid([knob_a,knob_b,knob_c], fillet = 2,edges=EDGES_BOTTOM + EDGES_Z_ALL);
//    
//}

//
//module mount(screw = false,rod = false,internal = false,l=15){
//   $slop = 0.13;
//   dS = 7;
//   pitch = 2;
//   starts=2;
//   h = 10;
//   module screwrod(l = 20,internal = internal){
//      threaded_rod(l=l, internal = internal,pitch=pitch, d=dS,starts=starts,anchor=BOTTOM);
//   }
//   if(screw){
//      left(40){
//      intersection(){
//         rotate([0,90,0])screwrod(l=l);
//         up(1)cuboid([50,30,6],anchor=LEFT);
//      }
//         up(1)cuboid([10,15,6], chamfer=1, edges=[LEFT,TOP+FRONT,TOP+BACK,BOTTOM+FRONT,BOTTOM+BACK],anchor = RIGHT);}
//       }
//    if(rod){
//      screwrod();
//      }
//
//}
// 
//
//
module rodmount(mount = true,hole=0,l = 30){
      difference(){
      if(!hole)cuboid([15,15,8],rounding=2,except=[TOP,BOTTOM],anchor=BOTTOM);
      if(mount){
         threaded_rod(d=10, pitch = 2, $slop=0.2,internal=true,l = l,orient=TOP,anchor=BOTTOM);
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

module buttoncase(cap = true,case =true){
   x = 14;
   y = 17;
   z = 14;
   xi = 12;
   yi = 14.2;
   tol = 0.8;
   if(case)
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
         down(0.05)cylinder(d=8,h=3.5/2-0.1,anchor = BOTTOM);
         }//end difference
      for(i=[1:90:279]){
         rotate([0,0,i])translate([-4/2-0.75/2,0,0])union(){
            cuboid([0.75,2.7,3.5]);
            translate([-0.75,0,0.25])cuboid([1.5,1,3]);
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
            translate([i*(xa/2-wall),wall/2,0])cuboid([wall,ya-wall,z-wall],chamfer = 2, except = [LEFT,RIGHT],anchor=FRONT);
            }//end for
         translate([0,wall,0])cuboid([xa,widthy/2-2*wall,z-2*wall],chamfer = 2, edges = FRONT,anchor = FRONT);
        
        }//end difference
}//end body        
module Sidepanel(assemble = 0){
   translate(assemble*[(xa/2-wall),wall/2,0])cuboid([wall-clearance*2,ya-wall-clearance*2,z-wall-clearance*2],chamfer = 2, except = [LEFT,RIGHT],anchor=FRONT);

 }

module leftpanel(button=false,rj45=false){
   difference(){
      Sidepanel();
      if(button)translate([-wall/3,15,0])cuboid([wall*2,17,14]);
      if(rj45)translate([0,18.9,0])cuboid([2*wall,20,17]);
   }//end difference
      if(button)translate([-wall/2+0.2,15,0])rotate([0,90,180])buttoncase(false);
      if(rj45){
         translate([-0.8,30.6,0])rotate([90,0,0])import("keystone1.stl");
         }
}//end leftpanel
 
module rightpanel(jst=false,switch=false,usb=false, usbC=false,screw=1){
   difference(){
      Sidepanel();
      if(switch)translate([-0.75,70,0])rotate([90,0,-90])switchcase(true);
      translate([wall,15.5,-1.8])rotate([0,0,90]){
       if(jst)translate([-7.7,-0.1,-0.4])cuboid([7,3,5.5],rounding=1,except=[FRONT,BACK],anchor=FRONT);
       if(usb)translate([-1.2,-0.1,7.5])cuboid([10.5,3,4.5],rounding=1,except=[FRONT,BACK],anchor=FRONT);
       if(usbC)translate([7.3-2.5,-0.1,-2.0])cuboid([10.5,3,4.5],rounding=1,except=[FRONT,BACK],anchor=FRONT);
       }
       if(screw){
      translate([+wall+1,ya/2.5,0])rotate([0,-90,0])rodmount(false);  
      }
      }
      if(screw)translate([+wall/2-0.2,ya/2.5,0])rotate([0,-90,0])rodmount();

      if(switch)translate([-0.74,70,0])rotate([90,0,-90])switchcase(false);
   }
      
      
module Top(oled=1,led=1){
    difference(){
      union(){
        Body();
        //rod shaft
        if(rod){
         translate([0,12,0])cuboid([16,14,z],chamfer=1,except=[TOP,BOTTOM]);
        }
         }
          if(rod){
         //hole for rod
           translate([0,12,0])cyl(d=11,h=z);
           }
        translate([0,-0.01,cutheight])cuboid([xa+1,ya+1,z],anchor = FRONT+TOP);
        if(oled){
         translate([0,17,z/2-wall])cuboid([26.5+2.4,27.5+2.4,wall*2],anchor=BOTTOM);
        }
        //status LED
        if(led)translate([xa/2-widthx/4,ya-20,0])zcyl(h=xi*3+2,d=dLED);
    }//end difference
    if(led)translate([xa/2-widthx/4,ya-20,z/2-wall+0.1])difference(){
    cuboid([10,10,3],anchor=TOP);
    zcyl(h=xi*3+2,d=dLED);
    }
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
   cuboid([7,3.7,wall*2]);
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
         //rod shaft
          if(rod){
         translate([0,12,0])cuboid([16,16,z],chamfer=1,except=[TOP,BOTTOM]);
         translate([0,8,-5])cuboid([16,16,z/2],chamfer=1,except=[TOP,BOTTOM]);
         }//end if
         //led mount (IRR and IRE)
         for(i=[-1,1]){
            difference(){
               translate([i*(xi/2+2*wall+2),ya-10,-z/2])cuboid([9,8,z/2+cutheight],anchor=BOTTOM+BACK);
               translate([0,ya-10-4,0])xcyl(h=xi*3+2,d=dLED);
               }//end difference
            //translate([i*(xi/2+2*wall),ya-6,-z/2])cuboid([5,8,z/2+cutheight],anchor=BOTTOM+BACK);

         }//end for
         }//end union
          if(rod){
         //hole for rod
           translate([0,12,0])cyl(d=11,h=z);
         //screwhole
            translate([0,-17,-3])rotate([-90,0,0])rodmount(hole=true);
            }//end if
        translate([0,-0.01,cutheight])cuboid([xa+1,ya+1,z],anchor = FRONT+BOTTOM);
        
        //hole for LEDs
        translate([0,ya-10-4,0])xcyl(h=xi*3+2,d=2);
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
    translate([-xa/2+25+wall+1,17.5,0])rotate([0,0,90])difference(){
        union(){
          translate([0,-33,-z/2])cuboid([26,8,lolinspace],anchor = BOTTOM+FRONT);
          translate([0,24,-z/2])cuboid([26,5.5,lolinspace],anchor = BOTTOM+BACK);
           }//end union
           #translate([11,-31,-z/2])cyl(d=2,h=lolinspace+1,anchor = BOTTOM);
           #translate([-10.3,-31,-z/2])cyl(d=2,h=lolinspace+1,anchor = BOTTOM);
    }//end difference
           
 }
 
module standscrew(l=10,d = 10, h = 50,cut = true){
   difference(){
      union(){
         threaded_rod(d=d-0.1, pitch = 2, l = l,orient=LEFT,anchor=TOP);
         xcyl(d=10,h=h,anchor=RIGHT);
         }
         if(cut)down(4)cuboid([2*h,10,d],anchor=TOP);
   }
}//end adjscrew

module lockscrew(l=10,d = 10,cut = true){
   difference(){
      union(){
         threaded_rod(d=d-0.1, pitch = 2, l = l,orient=LEFT,anchor=TOP,bevel1=true);
         cuboid([d*1.5,d*2.5,d],chamfer = 2, except=[RIGHT],anchor = RIGHT);
         }
      if(cut)down(4)cuboid([80,80,d],anchor=TOP);
}
}


difference(){
    union(){
   up(15)rotate([0,0,0])Top(oled=false,led=true);
   down(00)Bottom();
   }//end union
   *down(5)fwd(1)cube(40);
  }
mirror([1,0,0])translate([xa/2-1.5*wall+10,wall/2,0])rightpanel(jst=0,usb=1, usbC = 0, switch=1,screw=0);
translate([xa/2-1.5*wall+10,wall/2,0])rightpanel(jst=0,usb=0,screw=1, usbC = 0, switch=0);

down(20)lockscrew(l=10);
*translate([0,-15,-33])rotate([0,0,90])standscrew();



