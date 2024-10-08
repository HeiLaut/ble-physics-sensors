include<CASE/Case2.scad>

module ens160Aht2x(){
   difference(){
      color("black")cuboid([24.4,19.6,1.8]);
      translate([-24.4/2+1.5,19.6/2-1.5,0])cyl(d=2.8,h=2,anchor=LEFT+BACK);
      }
   translate([0,-19.6/2+0.4,-9])cuboid([21.5,2,8.35],anchor=BOTTOM+FRONT);
}
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
   case([x,y,z],part = "bottom",cutheight=cutheight,lolin32lite=1,embosstext = "Luftqualität",embossheight = 7);
   back(dist)difference(){
   union(){
      color("green")cuboid([x,1.5,z-0.2],chamfer=2,except=[FRONT,BACK],anchor=BOTTOM);
      translate([8,1.7,18])cuboid([10,2,10],chamfer=2,edges=BOTTOM+BACK);
      }
      for(i=[-1,1]){
         translate([i*(x/2),0,z/2+cutheight+2/8])rotate([0,i*90,0])prismoid(size1=[2*2,y-0.15*2], size2=[2*3,y-0.15*2], h=2,anchor=TOP);
         }//end for
      
      up(3.3)cuboid([22,2,2.6],anchor=BOTTOM);
      translate([9.6,1.8,20.3])ycyl(d=2,h=10);
      for(i=[-1,1])translate([-i*x/2,0,z/2+8])cuboid([0.3,5,5],anchor=BOTTOM);
      }//end difference
     translate([0,dist+4,z/2])rotate([90,0,180])if($preview)ens160Aht2x();
     
   }//end bottom
module top(){
case([x,y,z],part = "top",cutheight=cutheight,charge_view = 1,power_view = 1,);
}

module backplate(){
   fwd(y/2+10)up(z/2)case([x,y,z],part = "plate",usbC=1,jst=1,cutheight=cutheight);
   }
   
module frontplate(){
   difference(){
      case([x,y,z],part = "plate",usbC=0,cutheight=cutheight);
      for(i=[0:1:7]){
         translate([0,0,-z/2+4.5+i*3])rotate([-30,0,0])cuboid([x-5,8,1.5]);
      }
      }
   }

