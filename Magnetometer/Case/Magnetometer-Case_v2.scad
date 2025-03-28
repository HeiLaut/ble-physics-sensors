/*
  Case for phyphox magnetometer
  
  Created by: Heinrich Lauterbach
  License: CC-BY-SA
  This file is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
  To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/
  
  needs BOSL2 library
  download BOSL2 to your libraries folder via:
  
  https://github.com/BelfrySCAD/BOSL2
  
  needs Case2.scad
  https://github.com/HeiLaut/ble-physics-sensors/blob/main/Case2.scad
*/

include<BOSL2/std.scad>
include<CASE/Case2.scad>


top = true;
bottom = true;
sensorplate = true;
cap = true;
backplate = true;
sensortype = "lis2mdl"; 
// "gy271" or "lis2mdl" (adafruit) or "mlx90393" (non-adafruit)



if(sensorplate)rotate([0,0,180])translate([0,-40,0])sensor(true,true, sensortype);
if(backplate)fwd(40)frontplate();
if(bottom)bottom();
if(top)up(50)top();


*down(80)standscrew(l=9,h=40);


x = 40;
y = 52;
z = 30;
modules = 0;

module standscrew(l=10,d = 10, h = 50){
   difference(){
      union(){
         threaded_rod(d=d-0.1, pitch = 2, l = l,orient=LEFT,anchor=TOP);
         xcyl(d=10,h=h,anchor=RIGHT);
      }//end union
      translate([-(h+l)/2+l,0,-d+1.5])cuboid([h+l,d,d]);
      }
}//end adjscrew

module arrow(l = 5, a = 1, b=3 ,h=1){
   linear_extrude(h)union(){
      translate([0,-a/2,0])square([l-2,a]);
      translate([l-2,0,0])polygon([[0,-b/2],[2,0],[0,b/2]]);
      }
}
module bottom(){
if(modules){
   translate([-11.3,16.6,-10])color("grey")rotate([0,180,90])import("lolin32_lite.stl");
   }
      case([x,y,z],cutheight=2,part="bottom",snap=2.5,lolinx=-5,thread = true);
}//end bottom


module sensor(base = true, cover = true,sensor = sensortype){
   if(sensor == "gy271"){
   xs = 12.3;
   ys = 14.5;
   zs = 5;
   wall = 2;
   l = 20;
   
   if(base){
   difference(){
   case([x,y,z],cutheight=2,part="plate",snap=2.5,explode=2.1);
   scale([4,1,1])ycyl(d=3,h=20);
   }//end difference
   fwd(20)difference(){
   union(){
   fwd(2)rotate([-90,0,0])cuboid([xs+2*wall,3+2*wall,l],edges=TOP,rounding=-1,anchor=BOTTOM);
   hull(){
      cuboid([xs+2*wall,ys,zs+2*wall],chamfer = 1.5, except=[FRONT,BACK],anchor=BACK);
      fwd(2)cuboid([xs+2*wall,l-2,3+2*wall],anchor=FRONT);
      }//end hull
      }//end union

      cuboid([xs,ys+1,zs],anchor=BACK);
      down(1.4)cuboid([13.5,ys,1.4],anchor=BACK);
      cuboid([xs,l,3],anchor=FRONT);

      }//end difference
  }//end if
  
   if(cover){
   translate([0,-80,0])difference(){
      translate([0,-10.5,0])color("red")cuboid([xs+3*wall,10,zs+3*wall],chamfer = 1.5, except=[FRONT,BACK]);
      cuboid([xs+2*wall+.2,ys,zs+2*wall+0.2],chamfer = 1.5, except=[FRONT,BACK],anchor=BACK);
      translate([0,-9,(zs+3*wall)/2-0.4]){
      rotate([0,0,180])arrow();
      rotate([0,0,-90])arrow();
      cyl(d=2,h=1,anchor=BOTTOM);
      translate([-6,-3.5,0])linear_extrude(1)text("x",size=3,font="Arial");
      translate([3.4,-3.3,0])rotate([0,0,180])linear_extrude(1)text("y",size=3,font="Arial");
      translate([2.8,2.8,0])rotate([0,0,180])linear_extrude(1)text("z",size=3,font="Arial");
      }//end translate
   }// end difference
   }//end if
   }//end if gy271
   
   if(sensor == "mlx90393"){
   xs = 20.5;
   ys = 20.5;
   zs = 5;
   wall = 2;
   l = 50;

   
   if(base){
   difference(){
   case([x,y,z],cutheight=2,part="plate",snap=2.5,explode=2.1);
   cuboid([xs,3+2*wall,4]);
   }//end difference
   fwd(l)difference(){
   back(0.1)union(){
   fwd(2)rotate([-90,0,0])cuboid([xs+2*wall,3+2*wall,l],edges=TOP,rounding=-1,anchor=BOTTOM);
   hull(){
      cuboid([xs+2*wall,ys,zs+2*wall],chamfer = 1.5, except=[FRONT,BACK],anchor=BACK);
      fwd(2)cuboid([xs+2*wall,l-2,3+2*wall],anchor=FRONT);
      }//end hull
      }//end union

      cuboid([xs,ys+1,zs],anchor=BACK);
      down(1)cuboid([xs+1,ys,1.4],anchor=BACK);
      cuboid([xs,l,4],anchor=FRONT);

      }//end difference
  }//end if
  
   if(cover){
   translate([0,-80,0])difference(){
      translate([0,-10.5,0])color("red")cuboid([xs+3.5*wall,ys,zs+3.5*wall],chamfer = 1.5, except=[FRONT,BACK]);
      for(i=[0,180]){
      rotate([0,i,0])
      union(){
      translate([0,1.8,0])cuboid([xs+2*wall+.2,ys,zs+2*wall+0.2],chamfer = 1.5, except=[FRONT,BACK],anchor=BACK);
      translate([0,-10.5,(zs+3.5*wall)/2-0.4]){
      rotate([0,0,-0+i])arrow();
      rotate([0,0,-90])arrow();
      cyl(d=2,h=1,anchor=BOTTOM);
      rotate([0,0,180])translate([1.5,4,0])linear_extrude(1)text("x",size=3,font="Arial");

      if(i==0){
         rotate([0,0,180])translate([-5.5,-3.8,0])linear_extrude(1)text("y",size=3,font="Arial");
         translate([-3,1.6,0])rotate([0,0,0])linear_extrude(1)text("z",size=3,font="Arial");
         }
      if(i==180){
         rotate([0,0,180])translate([5,-3.8,0])linear_extrude(1)text("y",size=3,font="Arial");
         translate([1,1.6,0])rotate([0,0,0])linear_extrude(1)text("z",size=3,font="Arial");
      }
         }//end union
         }//end for
      }//end translate
   }// end difference
   }//end if
   }//end if "lis2mdl"
   
   
   if(sensor == "lis2mdl"){
   xs = 17.8;
   ys = 26;
   zs = 5;
   wall = 2;
   l = 50;

   
   if(base){
   difference(){
   case([x,y,z],cutheight=2,part="plate",snap=2.5,explode=2.1);
   cuboid([xs,3+2*wall,4]);
   }//end difference
   fwd(l)difference(){
   back(0.1)union(){
   fwd(2)rotate([-90,0,0])cuboid([xs+2*wall,3+2*wall,l],edges=TOP,rounding=-1,anchor=BOTTOM);
   hull(){
      cuboid([xs+2*wall,ys,zs+2*wall],chamfer = 1.5, except=[FRONT,BACK],anchor=BACK);
      fwd(2)cuboid([xs+2*wall,l-2,3+2*wall],anchor=FRONT);
      }//end hull
      }//end union

      cuboid([xs,ys+1,zs],anchor=BACK);
      down(1.4)cuboid([xs+1,ys,1.4],anchor=BACK);
      cuboid([xs,l,4],anchor=FRONT);

      }//end difference
  }//end if
  
   if(cover){
   translate([0,-80,0])difference(){
      translate([0,-10.5,0])color("red")cuboid([xs+3.5*wall,ys,zs+3.5*wall],chamfer = 1.5, except=[FRONT,BACK]);
      for(i=[0,180]){
      rotate([0,i,0])
      union(){
      translate([0,4,0])cuboid([xs+2*wall+.2,ys,zs+2*wall+0.2],chamfer = 1.5, except=[FRONT,BACK],anchor=BACK);
      translate([0,-10.5,(zs+3.5*wall)/2-0.4]){
      rotate([0,0,-0+i])arrow();
      rotate([0,0,-90])arrow();
      cyl(d=2,h=1,anchor=BOTTOM);
      rotate([0,0,180])translate([1.5,4,0])linear_extrude(1)text("y",size=3,font="Arial");

      if(i==0){
         rotate([0,0,180])translate([-5.5,-3.8,0])linear_extrude(1)text("x",size=3,font="Arial");
         translate([-3,1.6,0])rotate([0,0,0])linear_extrude(1)text("z",size=3,font="Arial");
         }
      if(i==180){
         rotate([0,0,180])translate([5,-3.8,0])linear_extrude(1)text("x",size=3,font="Arial");
         translate([1,1.6,0])rotate([0,0,0])linear_extrude(1)text("z",size=3,font="Arial");
      }
         }//end union
         }//end for
      }//end translate
   }// end difference
   }//end if
   }//end if "lis2mdl"
   
   }//end sensor

 module frontplate(){
 difference(){
   case([x,y,z],cutheight=2,part="plate",snap=2.5,explode=0,usbC = 1, switch = 1,lolinx=-5);
}//end difference
   }//end frontplate

module top(){
   difference(){
      down(5)case([x,y,z],cutheight=2,part="top",snap=2.5,button = true, reset = 0,explode=0);
      translate([-0.8,-y/2+5,9])difference(){
         union(){
            cyl(d=10,h=4);
            cuboid([10,10,4],anchor=FRONT);
            }//end union
            cyl(d=8.5,h=4);
            
            cuboid([8.5,10,4],anchor=FRONT);
         }//end difference
       translate([-0.8,-y/2+5,6.3])union(){
            cyl(d=10,h=4);
            cuboid([10,10,4],anchor=FRONT);
            }//end union
       translate([0,0,9.5])cuboid([12.2,14.2,5]);
      }//end difference
   
   
}//end top
