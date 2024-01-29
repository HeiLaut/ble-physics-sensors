   include<BOSL2/std.scad>
include<CASE/Case.scad>

x = 30;
y = 52;
z = 17;
module arrow(l = 5, a = 1, b=3 ,h=1){
   linear_extrude(h)union(){
      translate([0,-a/2,0])square([l-2,a]);
      translate([l-2,0,0])polygon([[0,-b/2],[2,0],[0,b/2]]);
      }
}
module bottom(){
translate([-11.3,16.6,-10])color("grey")rotate([0,180,90])import("lolin32_lite.stl");
case([x,y,z],cutheight=2,part="bottom",snap=2.5);
difference(){
   union(){
      translate([0,-y/2+3,-z/2-5])cuboid([26,8,3.4],anchor = BOTTOM+FRONT);
      translate([0,y/2-2,-z/2-5])cuboid([26,5.5,3.4],anchor = BOTTOM+BACK);
   }//end union
   translate([10.8,-y/2+3+5.4,-z/2-5])cyl(d=2,h=10,anchor = BOTTOM);
   translate([-10.3,y/2+3-8,-z/2-5])cyl(d=2,h=10,anchor = BOTTOM);

   }//end difference
}//end bottom

module sensor(base = true, cover = true,sensor = "gy271"){
   if(sensor == "gy271"){
   xs = 12.3;
   ys = 14.5;
   zs = 5;
   wall = 2;
   l = 20;
   
   if(base){
   difference(){
   case([x,y,z],cutheight=2,part="plate",snap=2.5,explode=-y/2);
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
   translate([0,-40,0])difference(){
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
   
   if(sensor == "li2mdl"){
    xs = 17.8;
   ys = 26;
   zs = 5;
   wall = 2;
   l = 40;
   
   if(base){
   difference(){
   case([x,y,z],cutheight=2,part="plate",snap=2.5,explode=-y/2);
   cuboid([xs,3+2*wall,4]);
   }//end difference
   fwd(l)difference(){
   union(){
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
   translate([0,-40,0])difference(){
      translate([0,-10.5,0])color("red")cuboid([xs+3*wall,ys,zs+3*wall],chamfer = 1.5, except=[FRONT,BACK]);
      #translate([0,4,0])cuboid([xs+2*wall+.2,ys,zs+2*wall+0.2],chamfer = 1.5, except=[FRONT,BACK],anchor=BACK);
      translate([0,-10.5,(zs+3*wall)/2-0.4]){
      rotate([0,0,180])arrow();
      rotate([0,0,-90])arrow();
      cyl(d=2,h=1,anchor=BOTTOM);
         translate([-4,-2,0])rotate([0,0,180])linear_extrude(1)text("y",size=3,font="Arial");
         translate([2,-6,0])rotate([0,0,0])linear_extrude(1)text("x",size=3,font="Arial");
         translate([-0,2,0])rotate([0,0,0])linear_extrude(1)text("z",size=3,font="Arial");
      }//end translate
   }// end difference
   }//end if
   }//end if "li2mdl"
   
   }//end sensor

 module frontplate(){
 difference(){
   case([x,y,z],cutheight=2,part="plate",snap=2.5,explode=0);
   translate([-7.2,-y/2+0.5,-1])cuboid([7,3,5.5],rounding=1,anchor=BACK);
   translate([7.3,-y/2+0.5,-2.5])cuboid([10.5,3,4.5],rounding=1,anchor=BACK);
   

   }//end difference
   }//end frontplate
 

 module frontplate(){
 difference(){
   case([x,y,z],cutheight=2,part="plate",snap=2.5,explode=0);
   translate([-7.2,-y/2+0.5,-1])cuboid([7,3,5.5],rounding=1,anchor=BACK);
   translate([7.3,-y/2+0.5,-2.5])cuboid([10.5,3,4.5],rounding=1,anchor=BACK);
   

   }//end difference

  
}//end frontplate 


module top(){
   difference(){
      down(5)case([x,y,z],cutheight=2,part="top",snap=2.5,explode=0);
      translate([-0.8,-y/2+5,5])difference(){
         union(){
            cyl(d=10,h=4);
            cuboid([10,10,4],anchor=FRONT);
            }//end union
            cyl(d=8.5,h=4);
            cuboid([8.5,10,4],anchor=FRONT);
      }//end difference
       translate([-0.8,-y/2+5,2.3])union(){
            cyl(d=10,h=4);
            cuboid([10,10,4],anchor=FRONT);
            }//end union
}//end difference
    translate([-0.8,-y/2+3.5,5])cyl(d=3,h=11.5,anchor=TOP);

}//end top
!sensor(false,true, "li2mdl");
down(5)frontplate();
translate([-11.3,16.6,-10])color("grey")rotate([0,180,90])import("lolin32_lite.stl");
bottom();
top();
