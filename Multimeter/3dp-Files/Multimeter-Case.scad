include<CASE/Case2.scad>
include<BOSL2/std.scad>
modules=0;

if(modules){
   translate([0,12,20])rotate([90,0,180])import("ina219_h.stl");
   translate([-11.5+8,-7.2,20.2])rotate([180,0,-90])import("lolin32_lite.stl");
   }

module top(){
   difference(){
      case([50,100,30],part="top",clearance=0.25,explode = 80,rd=2,lolinspace = 20,button = false,reset = 0, thread = false);
      //holes for LED
      for(i=[[7.3+8,-8,30],[-5.7+8,-30.6,30]]){
         translate(i)cuboid([2,2,40],rounding=0.5,except=[TOP,BOTTOM]);
      }
      *for(i=[-40:25:50]){
         #translate([0,i,41.6])cuboid([30,5,1],rounding=2,except=[TOP,BOTTOM]);
      }
      }
}
module bottom(){
   case([50,100,30],part="bottom",clearance=0.25,explode = 0,rd=2,lolinspace = 20,button = true,reset = 1, thread = false,lolinx=8);
   right(8)difference(){
      translate([0,5,10])cuboid([26,10,20]);
      translate([10,3.5,12])cyl(d=2,h=20);
      translate([-10,3.5,12])cyl(d=2,h=20);
   }
}
module backplate(){
   difference(){
      translate([0,-60,12])case([50,100,30],part="plate",explode = 0,rd=2,lolinspace = 20,button = false,reset = 0, thread = false,jst = false,usb=true,lolinx=8);
      translate([-15,-59,15])rotate([0,90,90])switchcase(hole = true);
      }
      translate([-15,-58.4,15])rotate([0,90,90])switchcase(hole = 0);
  
}
 module switchcase(hole = false){
   a = [4.5,4.4,3];//top cut
   b=[5,6,2];
   c= [3.5,4.4,0.5];
   if(hole){
   cuboid([7,3.7,3*2]);
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

module frontplate(){
d_sock = 6;
   difference(){
        case([50,100,30],part="plate",explode = 0,rd=2,lolinspace = 20,button = false,reset = 0, thread = false);
        for(i=[-1,0,1]){
         down(4)left(i*15){
            ycyl(d=6,h=5);
            translate([0,1.5,7])rotate([90,0,180])linear_extrude(0.5){
            #if(i==-1)text("A+",halign="center", valign="baseline", font = "Arial", size = 6);
            if(i==-0)text("A-",halign="center", valign="baseline", font = "Arial", size = 6);
            if(i==1)text("V-",halign="center", valign="baseline", font = "Arial", size = 6);}
            }
         }
         
         
        }
}
top();
bottom();
translate([0,60,12.5])frontplate();
color("red")backplate();