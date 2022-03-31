include <BOSL/constants.scad>
use <BOSL/transforms.scad>
use <BOSL/shapes.scad>
use <BOSL/masks.scad>

abstand = 80;
innenhoehe = abstand;
tiefe = 20;
breite = tiefe;
rand = 1.2;
dLED = 5.5;
dSock = 6;

$fn = 30;

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
      cuboid([abstand+2*breite,tiefe,abstand+breite]);
      up(tiefe/2)cuboid([abstand,tiefe+1,abstand+1]);
      difference(){
      translate([0,rand,0])cuboid([abstand+2*breite-2*rand,tiefe-rand,abstand+breite-2*rand]);
      up(tiefe/2)cuboid([abstand+2*rand,tiefe+1,abstand+1+rand*2]);
      }
   }
}

module hole_x(d = dLED){
   left(rand)rotate([0,90,0])cylinder(d = d, h = 2*rand);
}

module hole_y(d = dLED){
   back(rand)rotate([90,0,0])cylinder(d = d, h = 2*rand);
}
module hole_z(d = dLED){
   down(rand)cylinder(d = d, h = 2*rand);
}
difference(){
   case();
   translate([abstand/2+0.1,0,abstand/2])hole_x();
   translate([-(abstand/2+0.1),0,abstand/2])hole_x();
   translate([abstand/2+tiefe/2,-tiefe/2+1,abstand/3])hole_y();
   for(i=[-20,20,0]){
      translate([i,-tiefe/2+1,-abstand/2])hole_y(dSock);
   }
}
translate([0,-tiefe/2,-abstand/2-tiefe-2*rand-1])rotate([0,-90,-90])mount(0);


