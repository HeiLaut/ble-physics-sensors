include <BOSL/constants.scad>
use <BOSL/transforms.scad>
use <BOSL/shapes.scad>
use <BOSL/masks.scad>

$fn = 30;
espX = 50;
espY = 39;
espZ = 1.5;
wall = 1.5;
usbX = 8;
usbZ = 4;
z = 20;
dist = 5;
dDIN = 15.2;
dMag = 20;


module esp(){;
translate([121.5,63,-41])rotate([90,0,0])import("esp32.stl");
}

module ToF(){
    color("grey")rotate([0,0,90])translate([-19.8,-13.3,wall+dist])import("VL53L0X.stl");
}

module hull(){
    up(z/2+wall/2)difference(){
       cuboid([espX+2*wall,espY+2*wall,z+wall+1], fillet = 2, edges=EDGES_Z_ALL);
       up(wall/2)cuboid([espX,espY,z+wall+0.1], fillet = 1, edges=EDGES_Z_ALL);
     
       //Socket holes
       dSoc = 6; //diameter socket
       distZ = 15;
       distY = 10;
       
        rotate([90,0,0])translate([espX/2,0,0]){
             hole_x(d = dDIN, h = 5);
             down(11)hole_x(d = 2, h = 5);
             up(11)hole_x(d = 2, h = 5);
          }
       //switchcable hole
       #translate([0,+espY/2,z/2-wall/2+1.5])cuboid([3,wall*2,3]);
       
       //magnet holes
       translate([-5,0,-z/2-wall])cylinder(d = dMag, h = wall*2);
       
       
        //lid holder
        translate([-espX/2+1.5,-(espY)/2,z/2-1.5])rotate([0,90,0])cylinder(h=espX-3,d=2);
    }
    
    //screw holes
    for(i=[-1,1]){
    translate([i*(espX/2-1.5),espY/2-2,wall/2])difference(){
               cylinder(d = 5, h = z+0.5);
               translate([0,0,z/2])cylinder(d = 2, h = z+0.6);
            }
        }
   
    //lid
    module lid(){
     up(z+2.2+10){
         difference(){
       cuboid([espX+2*wall,espY+2*wall,wall], fillet = 2, edges=EDGES_Z_ALL);for(i=[-1,1]){
    translate([i*(espX/2-1.5),espY/2-2,-2])cylinder(d=2.5,h=5);
       }
        left(5)down(wall)cylinder(d = 3, h = wall*2);
     }
     poslid=0.5;
     translate([0,-espY/2+wall+poslid,-2.5-wall/2])cuboid([espX-3,wall*2,5]);
     translate([-espX/2+1.5,-(espY)/2+0.3+poslid,-3.5])rotate([0,90,0])cylinder(h=espX-3,d=1.5);
   }
}
   lid();
    
}
module hole_x(d = 5, h = 10, off = wall){
   left(off)rotate([0,90,0])cylinder(d = d, h = h);
}

module mount(x_d=espX/2){
       dRod = 10.5;
       x=20;
       y=30;
       z=20;
       l = 11;
       translate([-x_d-z/2-1,0,z/2-wall/2+0.25])difference(){
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
    
difference(){
    hull();
    *down(2)translate([-25 ,0,-1])cube(50);
   
}
    mount();
    *down(5)ToF();
    //color("green")translate([espX/2,-espY/2,z-espZ-7])esp();

module startbutton(){
   difference(){
   cylinder(d=12+2*wall,h = 20);
   up(wall)cylinder(d=12,h = 20);
     down(0.5)cylinder(d = 7, h  = 2*wall);
   }
}
translate([50,0,0])startbutton();