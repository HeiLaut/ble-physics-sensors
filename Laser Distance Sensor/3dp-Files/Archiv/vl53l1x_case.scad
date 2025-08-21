include<BOSL2/std.scad>
include<CASE/Case.scad>

x = 30;
y = 52;
z = 25;
cutheight=5;
modules = 0;
module bottom(){
if(modules){
    translate([-11.3,16.6,-8.5+8.5])color("grey")rotate([0,180,90])import("lolin32_lite.stl");
    }
    case([x,y,z],cutheight=cutheight,part="bottom",snap=2.5,explode=0);
    up(5)difference(){
   union(){
      translate([0,-y/2+3,-z/2-5])cuboid([26,8,3.4],anchor = BOTTOM+FRONT);
      translate([0,y/2-2,-z/2-5])cuboid([26,5.5,3.4],anchor = BOTTOM+BACK);
   }//end union
   translate([10.8,-y/2+3+5.4,-z/2-5])cyl(d=2,h=10,anchor = BOTTOM);
   translate([-10.3,y/2+3-8,-z/2-5])cyl(d=2,h=10,anchor = BOTTOM);

   }//end difference
}//end bottom
*bottom();
module sensor(){
    up(3.7)difference(){
        d=6.6;
        s=4.4;
        case([x,y,z],part = "plate",explode=-28);
        back(1)union(){
         translate([-s/2,0,4])ycyl(d=d,h=4);
         up(4)cuboid([s,4,d]);
         translate([s/2,0,4])ycyl(d=d,h=4);
         }//end union
        for(i=[1,-1]){
         translate([i*8,0,4])ycyl(d=2,h=3,anchor=FRONT);
        }//end difference
    }

        if(modules){color("green")translate([8,-10,-5])rotate([90,0,180])import("vl53l1x.stl");}//end if
}//end front     
module frontplate(){
 difference(){
   case([x,y,z],cutheight=2,part="plate",snap=2.5,explode=0);
   translate([-7.2,-y/2+0.5,-1-4])cuboid([7,3,5.5],rounding=1,anchor=BACK);
   translate([7.3,-y/2+0.5,-2.5-4])cuboid([10.5,3,4.5],rounding=1,anchor=BACK);
   translate([0,-y/2-1,5])cuboid([14.2,4,12]);

   }//end difference
   translate([0,-28,5])rotate([90,90,0])buttoncase();

}//end frontplate 

module top(){
   difference(){
      down(8)case([x,y,z],cutheight=cutheight,part="top",snap=2.5,explode=0);
      *translate([-0.8,-y/2+5,5])difference(){
         union(){
            cyl(d=10,h=4);
            cuboid([10,10,4],anchor=FRONT);
            }//end union
            cyl(d=8.5,h=4);
            cuboid([8.5,10,4],anchor=FRONT);
      }//end difference
       *translate([-0.8,-y/2+5,3])union(){
            cyl(d=10,h=4);
            cuboid([10,10,4],anchor=FRONT);
            }//end union
}//end difference
    *translate([-0.8,-y/2+3.5,5])cyl(d=3,h=18,anchor=TOP);

}//end top


module buttoncase(){
   x = 14;
   y = 17;
   z = 14;
   xi = 12;
   yi = 14.2;
   tol = 0.8;
   difference(){
      cuboid([x,y,z],anchor=TOP);
      up(0.1)cuboid([xi,yi,10],anchor=TOP);
      fwd(1.4)down(6)cuboid([7,y,6],anchor=TOP);
      down(6)cuboid([xi,1.5,6],anchor=TOP);
   }
   up(20)difference(){
      cuboid([xi-tol,yi-tol,5],anchor = TOP);
      down(2)cuboid([3.8,3.8,3],anchor = TOP);
      }
      if(modules){
         color("green")translate([0,0,5])import("TactSwitch.stl");
      }//end if
}

bottom();
fwd(10)frontplate(); 
up(20)top();  
back(40)sensor();
