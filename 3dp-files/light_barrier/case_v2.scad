include <BOSL/constants.scad>
use <BOSL/transforms.scad>
use <BOSL/shapes.scad>
use <BOSL/masks.scad>


depth  = 20;
armWidth = 30;
baseHeight = 26.5;
gapWidth = 80;
gapHeight = 80;
wall = 1.2;

//position of usb and mounting holes
usbPos = 16.5;
mcDist = 12;
//
espMount = [[-8.2,0,-24],[-46.5,0,-3]];



screwD = 2;

dLED = 5.2;
dSock = 6;
dDIN = 15.2;
*translate([29,2.2,-3-gapHeight/2])rotate([90,0,0])import("lolin32_lite.stl");

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
            cuboid([armWidth+2*wall,depth+wall,gapHeight+wall]);
            translate([0,wall/2+0.01,-wall/2])cuboid([armWidth,depth,gapHeight]);

        }
    }
    module base(){
        difference(){
            cuboid([gapWidth+4*wall+2*armWidth,depth+wall,baseHeight+2*wall]);
            translate([0,wall/2+0.01,0])cuboid([gapWidth+2*wall+2*armWidth,depth,baseHeight]);
            translate([-gapWidth/2-armWidth/2-wall,wall/2+0.01,gapHeight/2-baseHeight/2])cuboid([armWidth,depth,gapHeight]);
            translate([gapWidth/2+armWidth/2+wall,wall/2+0.01,gapHeight/2-baseHeight/2])cuboid([armWidth,depth,gapHeight]);

            
        }
    }
    translate([-gapWidth/2-armWidth/2-wall,0,0])arm();
    translate([gapWidth/2+armWidth/2+wall,0,0])arm();
    translate([0,0,-gapHeight/2-baseHeight/2-wall])base();  
}
module lid(){
    difference(){
        union(){
            translate([-gapWidth/2-armWidth/2-wall,0,0])cuboid([armWidth+2*wall,wall,gapHeight+wall]);
            translate([gapWidth/2+armWidth/2+wall,0,0])cuboid([armWidth+2*wall,wall,gapHeight+wall]);
            translate([0,0,-gapHeight/2-baseHeight/2-wall])cuboid([gapWidth+4*wall+2*armWidth,wall,baseHeight+2*wall]);
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

         }
        translate(posLED)hole_y(off=depthLED);
        translate([0,0,posIRLED[2]])hole_x(h=(gapWidth+depthLED*2+2*wall), off = (gapWidth+depthLED*2+2*wall)/2);
        translate([gapWidth/2+armWidth+wall,0,0]){
             hole_x(d = dDIN, h = 5);
             down(11)hole_x(d = 2, h = 5);
             up(11)hole_x(d = 2, h = 5);
          }
         translate([gapWidth/2+armWidth+wall,-depth/2+mcDist+1,-gapHeight/2-wall-8-usbPos])cube([4,3.5,8]);;
          }
          for(i = screwholes){
              translate(i)rotate([90,0,0])translate([0,0,-depth/2-wall/2])difference(){
             cylinder(h=depth,d=screwD*2.5);
             down(0.5)cylinder(h=depth+1, d = screwD);
            }
        
        }
        translate([0,-depth/2-wall/2,-gapHeight/2-baseHeight-wall*2-20])mount();
}
caseassembly();
*translate([0,20,0])lid();


translate([gapWidth/2+armWidth+wall,-depth/2+mcDist,-gapHeight/2+wall/2-screwD*2.5/4]){
for(i = espMount){
             translate(i)rotate([90,0,0])difference(){
             cylinder(h=mcDist,d=screwD*2.5);
             down(0.5)cylinder(h=mcDist, d = screwD);
          }
      }
  }
