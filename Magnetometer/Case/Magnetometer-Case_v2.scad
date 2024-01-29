   include<BOSL2/std.scad>
include<CASE/Case.scad>
include<BOSL2/threading.scad>

x = 30;
y = 52;
z = 25;
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
   difference(){
      case([x,y,z],cutheight=2,part="bottom",snap=2.5);
      translate([0,0,-z/2-3])cuboid([13,13,15],rounding=2,except=[TOP,BOTTOM]);
      }
difference(){
   union(){
      translate([0,-y/2+3,-z/2-5])cuboid([26,8,7.4],anchor = BOTTOM+FRONT);
      translate([0,y/2-2,-z/2-5])cuboid([26,5.5,7.4],anchor = BOTTOM+BACK);
   }//end union
   translate([10.8,-y/2+3+5.4,-z/2-5])cyl(d=2,h=10,anchor = BOTTOM);
   translate([-10.3,y/2+3-8,-z/2-5])cyl(d=2,h=10,anchor = BOTTOM);

   }//end difference
   translate([0,0,-z/2-3])rotate([0,90,0])rodmount();
}//end bottom

module buttoncase(){
   x = 14;
   y = 17;
   z = 14;
   xi = 12;
   yi = 14.2;
   tol = 0.8;
   *difference(){
      cuboid([x,y,z],anchor=TOP);
      up(0.1)cuboid([xi,yi,10],anchor=TOP);
      fwd(1.4)down(6)cuboid([7,y,6],anchor=TOP);
      down(6)cuboid([xi,1.5,6],anchor=TOP);
   }
   // button cap
   down(0.25)difference(){
      cuboid([xi-tol,yi-tol,5],anchor = BOTTOM);
      down(0.05)cylinder(d=7,h=3.5/2-0.1,anchor = BOTTOM);
      }//end difference
   for(i=[1:90:279]){
      rotate([0,0,i])translate([-4/2-0.75/2,0,0])union(){
         cuboid([0.75,2.7,3.5]);
         translate([-0.75/2,0,0.25])cuboid([1,1,3]);
         translate([0.75/2,2.7/2,-3.5/2+0.7])rotate([90,90,0])linear_extrude(2.7)polygon([[0,0],[0.7,0.5],[0.7,0]]);
      }//end union
   }//end for
//   up(20)difference(){
//      cuboid([xi-tol,yi-tol,5],anchor = TOP);
//      down(2)cuboid([3.8,3.8,3],anchor = TOP);
//      }
      if(modules){
         color("green")translate([0,0,5])import("TactSwitch.stl");
      }//end if
}

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
   translate([0,-40,0])difference(){
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
   }//end if "li2mdl"
   
   }//end sensor

 module frontplate(){
 difference(){
   case([x,y,z],cutheight=2,part="plate",snap=2.5,explode=0);
   translate([-7.2,-y/2+0.5,-1])cuboid([7,3,5.5],rounding=1,anchor=BACK);
   translate([7.3,-y/2+0.5,-2.5])cuboid([10.5,3,4.5],rounding=1,anchor=BACK);
   

   }//end difference
   }//end frontplate
 

module rodmount(){
      difference(){
      cuboid([8,15,15],rounding=2,except=[LEFT,RIGHT]);
      threaded_rod(d=10, pitch = 2, $slop=0.2,internal=true,l = 30,orient=LEFT);
      }//end difference
}//end rodmount

module top(){
   difference(){
      down(5)case([x,y,z],cutheight=2,part="top",snap=2.5,explode=0);
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
    translate([-0.8,-y/2+3.5,9])cyl(d=3,h=15.5,anchor=TOP);
   translate([0,0,9.5])buttoncase();
}//end top
translate([0,-40,0])sensor(false,true, "li2mdl");
left(100)frontplate();
down(30)bottom();
top();

buttoncase();
down(80)standscrew(l=9,h=30);