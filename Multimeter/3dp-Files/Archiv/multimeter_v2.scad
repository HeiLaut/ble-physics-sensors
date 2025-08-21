include <BOSL2/std.scad>

$fn=30;
boards = true;

module ina(rot=0){
   color("yellow")translate([12.8,11.2,0])rotate([90,0,rot])import("ina219_h.stl");
}

module esp(rot=0){
   color("yellow")translate([6.8,1.5,0])rotate([180,0,rot])import("lolin32_lite.stl");
}

module ads(rot=0){
   color("yellow")rotate([0,0,rot])translate([14,8.6,0])import("ads1115.stl");
}

module mountingstick(h=2.5,stick=true){
   difference(){cylinder(h=h,d=6);
      cylinder(h=5,d=2);
   }
}


module boardplate(){
   h = 0;
   if(boards){
   translate([0,0,h+3]){
   esp();
   translate([30,55,0])ads(-90);
   translate([0,30,0])ina(90);
   }
   }
   
   translate([0,0,h])for(i=[[2.5,2.5,0],[41,23.5,0]]){
      translate(i)mountingstick();
   }
   translate([0,0,h])for(i=[[2.5,23.5,0],[41,2.5,0]]){
       translate(i)mountingstick(stick=false);
   }
}
module screwmount(h=12){
   difference(){
      cylinder(d = 5,h=h);
      cylinder(d=2,h=h);
   }
}

module case(){
   wall=1.2;
   x = 53;
   y = 96;
   h = 27;
   rounding = 2;
   dSoc=6;
   screwCoord =[[-x/2+wall+5/2-1,-y/2+wall+5/2-1,wall],
            [-(-x/2+wall+5/2-1),-y/2+wall+5/2-1,wall],
            [-(-x/2+wall+5/2-1),-(-y/2+wall+5/2-1),wall],
            [-x/2+wall+5/2-1,-(-y/2+wall+5/2-1),wall]];
   difference(){
      cuboid([x,y,h],anchor=BOTTOM,rounding=rounding, edges = [FRONT+LEFT,FRONT+RIGHT,BACK+LEFT, BACK + RIGHT]);
      up(wall)cuboid([x-2*wall,y-2*wall,h-wall+0.01],anchor=BOTTOM,rounding=rounding, edges = [FRONT+LEFT,FRONT+RIGHT,BACK+LEFT, BACK + RIGHT]);
      //socket holes
      for(i=[
         [-x/2+wall*1.5,-y/3,h/1.5],
         [-x/2+wall*1.5,-y/3+18,h/1.5],
         [-x/2+wall*1.5,y/3,h/1.5],
         [-x/2+wall*1.5,y/3-18,h/1.5]]){
      translate(i)rotate([0,-90,0])cylinder(d=dSoc,h=wall*2);
         }
      //ina holes
         translate([x/2-wall+1,-21,2.8])cuboid([2*wall,9,6],anchor=BOTTOM,chamfer=1,edges=[FRONT+TOP,FRONT+BOTTOM,BACK+TOP,BACK+BOTTOM]);
         translate([x/2-wall+1,-36.7,3.2])cuboid([2*wall,8.5,8.5],anchor=BOTTOM,chamfer=1,edges=[FRONT+TOP,FRONT+BOTTOM,BACK+TOP,BACK+BOTTOM]);
   }
   //screw mount
   for(i=screwCoord){
      translate(i)screwmount(h-wall*2);
   }
  translate([-x/2+2,-41,wall]) boardplate();
  module lid(){
     module switch(){
         d=10;
         gap=0.6;
         difference(){
            union(){
            cylinder(d=d,h=4);
            cuboid([d,d,4],anchor=BOTTOM+RIGHT);
               
         }
            cylinder(d=d-gap*2,h=5);
            cuboid([d,d-gap*2,5],anchor=BOTTOM+RIGHT);
           }
     }
     difference(){
        union(){
         cuboid([x,y,wall],anchor=BOTTOM,rounding=rounding, edges = [FRONT+LEFT,FRONT+RIGHT,BACK+LEFT, BACK + RIGHT]);
         translate([0,0,-wall+0.2])rect_tube(size=[x-wall-1.5,y-wall-2],h=wall-0.2, wall=3,anchor=BOTTOM,rounding=rounding);
           }
           
           for(i=screwCoord){
              down(wall*2)translate(i)cylinder(d=2.3,h=5);
           }
           //connectors text
           for(i=[["V+",[-x/2+wall*1.5,-y/3,wall/2]],
              ["V-",[-x/2+wall*1.5,-y/3+18,wall/2]],
               ["A-",[-x/2+wall*1.5,y/3,wall/2]],
            ["A+",[-x/2+wall*1.5,y/3-18,wall/2]]]){
            right(2)translate(i[1])linear_extrude(wall/2+0.5)text(i[0],size = 10,valign="center");
               }
               translate([x/2-7,-29,-wall])switch();
        }
        translate([x/2-5,-29,])cyl(h=19.5,d=3, anchor=TOP);
   }
   
   !translate([0,0,h+0.2])lid();
}
difference(){
   case();
  translate([-0,-0,-50])cube(100);
}

