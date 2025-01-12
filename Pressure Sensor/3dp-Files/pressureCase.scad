
include<CASE/Case2.scad>

x = 35;
y = 67;
z = 26;
cutheight = 6;

up(z+10)top();
back(y/2+10)up(z/2)frontplate();
backplate();
bottom();

module bottom(){
   dist=25;
   case([x,y,z],part = "bottom",cutheight=cutheight,lolin32lite=1,embosstext = "Druck, Temp",embossheight = 7);
   *back(dist)difference(){
   union(){
      color("green")cuboid([x,1.5,z-0.2],chamfer=2,except=[FRONT,BACK],anchor=BOTTOM);
      translate([8,1.7,18])cuboid([10,2,10],chamfer=2,edges=BOTTOM+BACK);
      }
      for(i=[-1,1]){
         translate([i*(x/2),0,z/2+cutheight+2/8])rotate([0,i*90,0])prismoid(size1=[2*2,y-0.15*2], size2=[2*3,y-0.15*2], h=2,anchor=TOP);
         }//end for
      
      up(3.3)cuboid([22,2,3],anchor=BOTTOM);
      translate([9.6,1.8,20.3])ycyl(d=2,h=10);
      for(i=[-1,1])translate([-i*x/2,0,z/2+8])cuboid([0.3,5,5],anchor=BOTTOM);
      }//end difference
      translate([-10,25,0])difference(){
         cuboid([5,10,16],anchor=BOTTOM);
         #translate([0,-3,14])cyl(d=2,h=5);
      }
   }//end bottom
module top(){
   difference(){
      case([x,y,z],part = "top",cutheight=cutheight,charge_view = 1,power_view = 1,);
      for(i=[0:1:5]){
         translate([0,-z/2+30+i*3,0])rotate([30,0,0])cuboid([x,1.5,8]);
      }
      }
   
}

module backplate(){
   fwd(y/2+10)up(z/2)case([x,y,z],part = "plate",usb=1,jst=1,cutheight=cutheight);
   }
   
module frontplate(){
   difference(){
      case([x,y,z],part = "plate",usbC=0,cutheight=cutheight);
      for(i=[0:1:7]){
         translate([0,0,-z/2+4.5+i*3])rotate([-30,0,0])cuboid([x-5,8,1.5]);
      }
      }
   }

