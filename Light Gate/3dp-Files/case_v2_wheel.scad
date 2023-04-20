include <BOSL/constants.scad>
use <BOSL/transforms.scad>
use <BOSL/shapes.scad>
use <BOSL/masks.scad>


depth  = 20;
armWidth = 30;
baseHeight = 26.5;
gapWidth = 20;
gapHeight = 60;
wall = 1.2;

//position of usb and mounting holes
usbPos = 16;
mcDist = 12;
//
espMount = [[-8.1,0,-24.2],[-46.5,0,-3.2]];

con = "rj45"; //"din" or "rj45" or "none"
esp = true; //esp holes and mount on or off (Master: on; Slave: off)
screwD = 2;

dLED = 5.2;
dSock = 6;
dDIN = 15.2;
*translate([29,2.2,-2.9-gapHeight/2])rotate([90,0,0])import("lolin32_lite.stl");

module rj45(){
   difference(){
      import("parts/keystone.stl");
   }
}


screwholes = [[-gapWidth/2-wall-screwD,0,gapHeight/2-screwD],
              [+gapWidth/2+wall+screwD,0,gapHeight/2-screwD],
              [-gapWidth/2-wall-armWidth+screwD,0,gapHeight/2-screwD],
              [+gapWidth/2+wall+armWidth-screwD,0,gapHeight/2-screwD],
              [-gapWidth/2-wall-screwD,0,-gapHeight/2+screwD],
              [+gapWidth/2+wall+screwD,0,-gapHeight/2+screwD],
              [-gapWidth/2-wall-armWidth+2,0,-gapHeight/2+screwD],
              [+gapWidth/2+wall+armWidth-2,0,-gapHeight/2+screwD]];
$fn = 25;
module hole_x(d = dLED, h = 10, off = wall){
   left(off)rotate([0,90,0])cylinder(d = d, h = h);
}

module hole_y(d = dLED,h = 10,off = wall){
   back(off)rotate([90,0,0])cylinder(d = d, h = h);
}
module hole_z(d = dLED,h = 10,off = wall){
   down(off)cylinder(d = d, h = h);
}
module mount(){
       dRod = 10.5;
       x=20;
       y=30;
       z=20;
       l = 11;
       rotate([-90,-90,0])translate([x/2,0,x/2])difference(){
         cuboid([x,y,z],fillet=3,edges = EDGES_Z_LF);
          scale([1,1,1.05])translate([2.5,y/2+0.5,0])rotate([90,0,0])cylinder(d = dRod, h = y+1);
          translate([2.5,0,-y/2])rotate([0,0,0])cylinder(d = dRod, h = y+1);

          translate([-x/2+2,0,0])rotate([90,0,90]){
            rotate([0,0,90])cylinder(h = 3.5, d=8, $fn = 6);
            translate([0,l/2,1.7])cube([7,l,3.5],center = true);
           translate([0,0,-2.5])cylinder(d = 5, h =10);
          }
       }
    }


module case(){
    module arm(){
        difference(){
            cuboid([armWidth+2*wall,depth+wall,gapHeight+wall],fillet = 2,edges = EDGES_Y_TOP);
            translate([0,wall/2+0.01,-wall/2])cuboid([armWidth,depth,gapHeight]);

        }
    }
    module base(){
        difference(){
            cuboid([gapWidth+4*wall+2*armWidth,depth+wall,baseHeight+2*wall],fillet = 2,edges = EDGES_Y_BOT);
            translate([0,wall/2+0.01,0])cuboid([gapWidth+2*wall+2*armWidth,depth,baseHeight],fillet = 2,edges = EDGES_Y_BOT);
            translate([-gapWidth/2-armWidth/2-wall,wall/2+0.01,gapHeight/2-baseHeight/2])cuboid([armWidth,depth,gapHeight],fillet = 1,edges = EDGES_Y_BOT);
            translate([gapWidth/2+armWidth/2+wall,wall/2+0.01,gapHeight/2-baseHeight/2])cuboid([armWidth,depth,gapHeight],fillet = 1,edges = EDGES_Y_BOT);

            
        }
    }
    translate([-gapWidth/2-armWidth/2-wall,0,0])arm();
    translate([gapWidth/2+armWidth/2+wall,0,0])arm();
    translate([0,0,-gapHeight/2-baseHeight/2-wall])base();
}
module lid(){
    difference(){
        union(){
            translate([-gapWidth/2-armWidth/2-wall,0,0])cuboid([armWidth+2*wall,wall,gapHeight+wall],fillet = 2,edges = EDGES_Y_TOP);
            translate([gapWidth/2+armWidth/2+wall,0,0])cuboid([armWidth+2*wall,wall,gapHeight+wall],fillet = 2,edges = EDGES_Y_TOP);
            translate([0,0,-gapHeight/2-baseHeight/2-wall])cuboid([gapWidth+4*wall+2*armWidth,wall,baseHeight+2*wall],fillet = 2,edges = EDGES_Y_BOT);
        }
        for(i = screwholes){
            translate(i)rotate([90,0,0])translate([0,0,-depth/2-wall/2])
         down(0.5)cylinder(h=depth+1, d = screwD);
        }
    }



}
module caseassembly(){
    difference(){
        depthLED = 4;
        posLED = [armWidth/2+gapWidth/2+wall,-depth/2,gapHeight/7];
        posIRLED = [gapWidth/2+depthLED/2+wall/2,wall/2,gapHeight/3];
        union(){
            case();
            back(depthLED/2)translate(posLED)cuboid([dLED+2,depthLED,dLED+2],fillet = 2, edges = EDGES_Y_ALL);
           
            translate(posIRLED)cuboid([depthLED,depth, dLED+2]);
            translate([-gapWidth-depthLED-wall,0,0])translate(posIRLED)cuboid([depthLED,depth, dLED+2]);
           
           translate([0,0,-37]){ translate(posIRLED)cuboid([depthLED,depth, dLED+2]);
            translate([-gapWidth-depthLED-wall,0,0])translate(posIRLED)cuboid([depthLED,depth, dLED+2]);
           }

         }
         //signal LED hole
        translate(posLED)hole_y(off=depthLED);
         
         //IR LED top
         translate([0,+1,0]){
        translate([0,0,posIRLED[2]])hole_x(h=(gapWidth+depthLED*2+2*wall), off = (gapWidth+depthLED*2+2*wall)/2);
         }
         //IR LED bottom
         translate([0,-1,-37]){
        translate([0,0,posIRLED[2]])hole_x(h=(gapWidth+depthLED*2+2*wall), off = (gapWidth+depthLED*2+2*wall)/2);
         }
         //achis hole
         translate([-gapWidth+5,0,gapHeight/3-37/2])rotate([0,90,0])cylinder(d = 3.4, h  = gapWidth+10);
         
         
         //esp connector holes
         if(esp){
         translate([gapWidth/2+armWidth+wall,-depth/2+mcDist+1,-gapHeight/2-wall-8-usbPos])cube([4,3.5,8]);
         translate([gapWidth/2+armWidth+wall,-depth/2+mcDist+1,-gapHeight/2-wall+7.5-usbPos])cube([4,7,8]);
         }
          }
          
          for(i = screwholes){
              translate(i)rotate([90,0,0])translate([0,0,-depth/2-wall/2])difference(){
             cylinder(h=depth,d=screwD*2.5);
             down(0.5)cylinder(h=depth+1, d = screwD);
            }
        
        }
        translate([0,-depth/2-wall/2,-gapHeight/2-baseHeight-wall*2-19.5])mount();
}

module wheel(){
   dRod = 10.5;
       x=20;
       y=30;
       z=20;
       l = 11;
   difference(){
      down(35)cuboid([30,22+4*wall,110],fillet=5,edges = EDGES_X_TOP+EDGES_Y_BOT);
      down(70)cuboid([35,23,60]);
      down(0)cuboid([30-8*wall,40,40]);
      left(25)up(15)rotate([0,90,0])cylinder(d=3,h=50);
      translate([0,30,-76])rotate([90,0,0])cylinder(d = dRod, h = 60);
   }
   *up(15)rotate([0,90,0])
      difference(){
         cylinder(d = 51.5,h = 4);
         cylinder(d=2,h=10);
      }
   
}

module wheel_v2(){
   dRod = 10.5;
       x=20;
       y=30;
       z=20;
       l = 11;
   difference(){
      down(35)cuboid([32+2*wall,22+4*wall,110],fillet=5,edges = EDGES_X_TOP+EDGES_Y_BOT);
      fwd(wall)down(70)cuboid([30,27,60]);
      down(0)cuboid([30-8*wall,40,40]);
      left(25)up(15)rotate([0,90,0])cylinder(d=3,h=50);
      translate([0,30,-76])rotate([90,0,0])cylinder(d = dRod, h = 60);
      translate([-20,0,-76])rotate([0,90,0])cylinder(d = dRod, h = 60);
      down(54.5)fwd(3)cuboid([37,30,30]);
   }
   *up(15)rotate([0,90,0])
      difference(){
         cylinder(d = 51.5,h = 4);
         cylinder(d=2,h=10);
      }
   
}


*wheel_v2();


caseassembly();
translate([0,20,0])lid();


if(esp)
translate([gapWidth/2+armWidth+wall,-depth/2+mcDist,-gapHeight/2+wall/2-screwD*2.5/4]){
color("green")for(i = espMount){
             translate(i)rotate([90,0,0])difference(){
             cylinder(h=mcDist,d=screwD*2.5);
             down(0.5)cylinder(h=mcDist, d = screwD-0.5);
          }
      }
  }


