include<BOSL2/std.scad>
include <BOSL2/threading.scad>
$fn = 60;
$slop = 0.13;
dS = 7;
pitch = 2;
starts=2;
h = 15;


module rodmount(mount = true,hole=0,l = 30){
      difference(){
      if(!hole)cuboid([15,15,8],rounding=2,except=[TOP,BOTTOM],anchor=BOTTOM);
      if(mount){
         threaded_rod(d=10, pitch = 2, $slop=0.2,internal=true,l = l,orient=TOP,anchor=BOTTOM);
      }
      }//end difference
}//end rodmount

module base(){
difference(){
   cuboid([28,120,h],chamfer=2,except=[TOP,BOTTOM],anchor = FRONT);
   cuboid([13,60,h+.2],chamfer=2,except=[TOP,BOTTOM,FRONT],anchor = FRONT);
   translate([0,15,0])xcyl(d= 3.1, h = 80);
   translate([0,110,0])cyl(d=10.5,h = 30);
   translate([0,110,0])rotate([-90,0,0])rodmount(hole=1);
   }
}

//module screw (){
//left(40){
//intersection(){
//   rotate([0,90,0])rod(false,l=15);
//   up(1)cuboid([50,30,6],anchor=LEFT);
//}
//    up(1)cuboid([10,15,6], chamfer=1, edges=[LEFT,TOP+FRONT,TOP+BACK,BOTTOM+FRONT,BOTTOM+BACK],anchor = RIGHT);}
// }
// 
 
module rod(internal=false,l=20){
         threaded_rod(l=l, internal = internal,pitch=pitch, d=dS,starts=starts,anchor=BOTTOM);

}
module wheel(){
   a = 360/20;
   d = 55;
difference(){
   union(){
      xcyl(d =d,h=4);
      xcyl(d =10,h=5,anchor=LEFT);
      }
   rotate([0,90,0])rotate_extrude()translate([d/2+2,0,0])scale([1.5,0.8,1])circle(2);
   xcyl(d= 3.5, h = 80);
   for(i=[0:a:360]){
      left(5)rotate([i,0,0])rotate([0,90,0])rotate_extrude(angle=a/2)translate([12,0,0])square([d/2-14,10]);
   //rotate([i,0,0])scale([1,0.3,1])translate([0,0,12])xcyl(d=10,h=20);
   
  }
 }
}

module nut(){
   difference(){
      xcyl(d = 10,h=4);
      xcyl(d = 3.5,h=4);
      }
}
base();
right(1)wheel();
left(3.5)nut();