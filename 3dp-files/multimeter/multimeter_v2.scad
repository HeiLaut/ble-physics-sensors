include <BOSL2/std.scad>

$fn=30;
boards = false;
boardx = 48;
boardy = 55;
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
   cylinder(h=h,d=5);
   if(stick){
      translate([0,0,h])cylinder(h=5,d=2);
   }
}

module mountingcap(){
   difference(){
      mountingstick(2.5,false);
      cylinder(d=2.2,h=2.5);
      left(1.8)cylinder(d=5,h=3,$fn=3);
   }
}
module boardplate(){
   h = 1;
   if(boards){
   translate([0,0,h+3]){
   esp();
   translate([30,55,0])ads(-90);
   translate([0,30,0])ina(180);
   }
   }
   color("green")cube([boardx,boardy,h]);
   translate([0,boardy,0])cuboid([10,6,h],anchor=LEFT+BOTTOM+FWD,rounding=3,edges=[BACK+LEFT,BACK+RIGHT]);
   translate([boardx-10,-6,0])cuboid([10,6,h],anchor=LEFT+BOTTOM+FWD,rounding=3,edges=[FRONT+LEFT,FRONT+RIGHT]);
   translate([0,0,h])for(i=[[2.5,2.5,0],[41,23.5,0],
      [44.5,29.5,0],[44.5,29.5+22.9,0],
      [2.8,32.7,0],[2.8+20,32.7,0],[2.8,32.7+17,0],[2.8+20,32.7+17,0]]){
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
   y = 75;
   h = 15;
   rounding = 2;
   difference(){
      cuboid([x,y,h],anchor=BOTTOM,rounding=rounding, edges = [FRONT+LEFT,FRONT+RIGHT,BACK+LEFT, BACK + RIGHT]);
      up(wall)cuboid([x-2*wall,y-2*wall,h-wall+0.01],anchor=BOTTOM,rounding=rounding, edges = [FRONT+LEFT,FRONT+RIGHT,BACK+LEFT, BACK + RIGHT]);
   }
   
   for(i=[  [-x/2+wall+5/2-1,-y/2+wall+5/2-1,wall],
            [-(-x/2+wall+5/2-1),-y/2+wall+5/2-1,wall],
            [-(-x/2+wall+5/2-1),-(-y/2+wall+5/2-1),wall],
            [-x/2+wall+5/2-1,-(-y/2+wall+5/2-1),wall]]){
      translate(i)screwmount(h-wall);
   }
}
case();
translate([-boardx/2,-boardy/2,1])boardplate();
!translate([-30,0,0])mountingcap();