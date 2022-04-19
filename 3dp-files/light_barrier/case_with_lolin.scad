include <BOSL/constants.scad>
use <BOSL/transforms.scad>
use <BOSL/shapes.scad>
use <BOSL/masks.scad>



abstand = 80;
innenhoehe = abstand;
tiefe = 20;
breite = 20;
rand = 1.2;
dLED = 5.2;
dSock = 6;
dDIN = 15.2;
lolinB = 26;
lolinL = 58;

screwholes= [[abstand/2+rand+1.5,tiefe/2,abstand/2+breite/2-rand-1.5],
             [abstand/2-rand-1.5+breite,tiefe/2,abstand/2+breite/2-rand-1.5],
             [-(abstand/2+breite-rand-1.5),tiefe/2,abstand/2+breite/2-rand-1.5],
             [-(abstand/2+rand+1.5),tiefe/2,abstand/2+breite/2-rand-1.5],
             [-(abstand/2+rand+1.5),tiefe/2,-abstand/2+breite/2],
             [+(abstand/2+rand+1.5),tiefe/2,-abstand/2+breite/2],
             [abstand/2-rand-1.5+breite,tiefe/2,-6-5],
             [abstand/2-rand-1.5+breite,tiefe/2,16+5]];
             
$fn = 30;

module lolin(){
   import("lolin.stl");
}

module mount(x_d){
       dRod = 10.5;
       x=20;
       y=30;
       z=20;
       l = 11;
       translate([-x_d+4.2,0,x/2])difference(){
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
   difference(){
      cuboid([abstand+2*breite,tiefe,abstand+breite],chamfer = 1.5, edges = EDGES_Y_ALL);
      up(tiefe/2+25)cuboid([abstand,tiefe+1,abstand+50]);
      difference(){
      translate([0,rand-1/2*rand,0])cuboid([abstand+2*breite-2*rand,tiefe-rand+0.1,abstand+breite-2*rand],chamfer = 1.5, edges = EDGES_Y_ALL);
      up(tiefe/2)cuboid([abstand+2*rand,tiefe+1,abstand+1+rand*2]);
      }
      //holes for DIN 
      translate([(abstand)/2+breite-1,0,5]){
         hole_x(d = dDIN, h = 5);
         down(11)hole_x(d = 2, h = 5);
         up(11)hole_x(d = 2, h = 5);
      }
      //hole for usb
      *translate([+abstand/2+breite,8.5,-abstand/2-2.5])cuboid([4,3.5,8]);
   }
   //esp holder lolin
   module espholder(){
   h = tiefe/2-rand+7;
   for(i=[[abstand/2 +breite-rand-lolinL+2,4.5,-abstand/2+lolinB/2-rand-1.5],
      [abstand/2 +breite-rand-lolinL+2,4.5,-abstand/2-lolinB/2+rand+0.5],
      [abstand/2 +breite-rand-3,4.5,-abstand/2+lolinB/2-rand-1.5],
      [abstand/2 +breite-rand-3,4.5,-abstand/2-lolinB/2+rand+0.5]]){
   translate(i) 
   difference(){
      hole_y(d = 7, h = h);
      back(0.5)hole_y(d = 2.5, h =h+1) ;
   }
     }
  }
  *espholder();
}

module lid(){
   difference(){
      cuboid([abstand+2*breite,rand,abstand+breite],chamfer = 1.5, edges = EDGES_Y_ALL);
      up(tiefe/2)cuboid([abstand,rand+1,abstand+1]);
       for(i=screwholes){
         translate(i)
      rotate([90,0,0])
         down(0.5)cylinder(h=tiefe+1, d = 2);
      }
   }
   translate([0,-2.5,-abstand/2-breite/2+rand+1.5])cuboid([abstand+2*tiefe-4,5,2]);
   
   translate([-(abstand+2*breite-2*rand)/2+2,-3,-abstand/2-breite/2+1+rand])rotate([0,90,0])cylinder(d=2, h = abstand+2*breite-2*rand-4);
}

module hole_x(d = dLED, h = 10){
   left(rand)rotate([0,90,0])cylinder(d = d, h = h);
}

module hole_y(d = dLED,h = 10){
   back(rand)rotate([90,0,0])cylinder(d = d, h = h);
}
module hole_z(d = dLED,h = 10){
   down(rand)cylinder(d = d, h = h);
}
difference(){
   union(){
      case();
      translate([-abstand/2-rand-5/2,0,abstand/2])cuboid([5,tiefe,10]);
      translate([+abstand/2+rand+5/2,0,abstand/2])cuboid([5,tiefe,10]);
      
      translate([abstand/2+tiefe/2,-tiefe/2+2,abstand/3])cuboid([8,3,8],fillet = 3, edges= EDGES_Y_ALL);
      
      translate([+abstand/2+rand+5/2,0,abstand/2])cuboid([5,tiefe,10]);

      translate([0,-tiefe/2,-abstand/2-breite-2*rand])rotate([0,-90,-90])mount(1);
      
      for(i=screwholes){
         translate(i)
      rotate([90,0,0])difference(){
         cylinder(h=tiefe,d=5);
         down(0.5)cylinder(h=tiefe+1, d = 2);
      }
      }

   }
   //led holes
   translate([abstand/2+0.1,0,abstand/2])hole_x();
   translate([-(abstand/2+0.1)-8,0,abstand/2])hole_x();
   translate([abstand/2+tiefe/2,-tiefe/2+3,abstand/3])hole_y(h=5);
   *for(i=[-20,20,0]){
      translate([i,-tiefe/2+1,-abstand/2])hole_y(dSock);
   }
   translate([-(abstand+2*breite-2*rand)/2+1.5,tiefe/2-3,-abstand/2-breite/2+rand+0.4])rotate([0,90,0])cylinder(d=2.3, h = abstand+2*breite-2*rand-3);
   
}
*color("green")translate([breite+9.5,tiefe/2-9,-abstand/2-0.5])rotate([-90,90,0])lolin();
translate([0,tiefe+5,0])lid();