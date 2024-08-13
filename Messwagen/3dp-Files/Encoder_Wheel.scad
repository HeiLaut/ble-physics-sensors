include<BOSL2/std.scad>
include <BOSL2/threading.scad>
$fn = 60;
$slop = 0.13;
dS = 7;
pitch = 2;
starts=2;
h = 15;


module wheel(){
   a = 360/20;
   d = 24;
difference(){
   union(){
      xcyl(d =d,h=2);
      xcyl(d =6,h=2.5,anchor=LEFT);
      }
   //rotate([0,90,0])rotate_extrude()translate([d/2+2,0,0])scale([1.5,0.8,1])circle(2);
   xcyl(d= 3.2, h = 80);
   for(i=[0:a:360]){
      left(5)rotate([i,0,0])rotate([0,90,0])rotate_extrude(angle=a/5)translate([5,0,0])square([d/2-6,10]);
   //rotate([i,0,0])scale([1,0.3,1])translate([0,0,12])xcyl(d=10,h=20);
   
  }
 }
}

!wheel();
