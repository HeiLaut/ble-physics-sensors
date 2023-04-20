include <BOSL/constants.scad>
use <BOSL/transforms.scad>
use <BOSL/shapes.scad>


module esp(){;
translate([122,63,-41])rotate([90,0,0])import("esp32.stl");
}

module ver2(){
   
   dMount = 5;
   dLoad = 5;
   distHoles = 15;
   distFirstHole = 20;
   xLoad = 80;
   yLoad = 12.7;
   zLoad = yLoad;
   xEsp = 31;
   yEsp = 39;
   dRod = 10.5;
   $fn = 40;
   versatz = -4.5;
   
   module holes(){
      //holes that fit into load cell
      translate([versatz,0,0]){
      for(i=[[20,0,-50],[35,0,-50]]){
         translate(i)cylinder(h = 60, d = dMount);
      }
      
      translate([-35,0,-20])cylinder(h = 60, d = dLoad);
      //stand mount
      translate([15,0,16])rotate([0,90,0])cylinder(d = dRod, h = 50);
      //screw to secure load cell on stand
      translate([xLoad/2+versatz-8,-yEsp/2+yEsp/2,16])rotate([90,0,0])
      {
         cylinder(h = yEsp, d = dMount);
         translate([0,0,yEsp/2-5.5]){
            rotate([0,0,90])cylinder(h = 3.5, d=8, $fn = 6);
            translate([0,3,1.7])cube([7,9,3.5],center = true);
      }
         cube();
   }
   }
      
}
   
   module loadcell(){
      difference(){
         translate([versatz,0,0])cube([xLoad,yLoad,zLoad],center = true);
      holes();}
   }
   module cell_holder(){
      z = 35;
      x = 26;
      y = yEsp+1;
      translate([xLoad/2-x/2+versatz,0,6])
         difference(){
         cube([x,y,z], center = true);
         translate([0,8,-6])cube([x+1,yLoad+1+16,zLoad],center=true);
      }
      difference(){
         translate([x+x/2+versatz,0,z/2-1.5])cuboid([20,y,15], center = true, chamfer = 8, edges=EDGES_Z_RT);
         *translate([x+9/2+1+x/2+versatz+4.5-1,yEsp/2+1,-30])cylinder(d = 10, h = 60,$fn=4);
         *translate([x+9/2+1+x/2+versatz+4.5-1,-yEsp/2-1,-30])cylinder(d = 10, h = 60);
      }
      //esp mount part
      translate([-4+versatz,0,z/2-1.5]){
         difference(){
         h = 15;
         cube([xEsp+6,yEsp+1,h],center = true);
         translate([0,yEsp/2-4.5,-h/2])cube([xEsp-1,10,h],center = true);
         ver = 12;
         translate([0,0,h/2-ver/2]){
            cube([xEsp-1,yEsp+2,ver+0.1],center = true);
            translate([0,0,3])cube([xEsp,yEsp+2,1.2],center = true);
         }
         
      }
      //end stop for esp
      translate([xEsp/2,yEsp/2-0.8,5]){
         hull(){
         cube([5,2.5,5], center = true);
         translate([0.5,-2.5,-2.5])rotate([0,0,180])cylinder(h = 5, d = 6, $fn = 3);
         }
      }
      *color("blue")rotate([0,180,0])translate([xEsp/2,-yEsp/2,-5])esp();
      }
     
   }
   module case(){
      wall = 2;
      wall2 = 5;
      x = xLoad + 10;
      y = yEsp + 5;
      z = 35+1;
      module body(){
         difference(){
            cuboid([x+2*wall,y+2*wall,z+wall2], center = true,fillet = 2, edges=EDGES_Z_ALL);
            translate([0,0,wall2/2])cuboid([x,y,z+0.1], center = true, fillet = 2, edges=EDGES_Z_ALL);
            //bottom holes for screws
            for(i=[[20+versatz,0,-z/2-wall2/2-0.1],[35+versatz,0,-z/2-wall2/2-0.1]]){
               translate(i)cylinder(d = 10,h=4);
               }
               //USB- hole:
            translate([-8.5,-y/2-wall/2,14.5])cube([11,wall+1,5],center=true);
               //switch-hole:
            translate([-8.5,+y/2+wall/2,14.5])cube([6.5,wall+1,6.5],center=true);

            }
            for(i=[[x/2-2,y/2-2,-z/2+wall],
                   [x/2-2,-y/2+2,-z/2+wall],
                   [-x/2+2,y/2-2,-z/2+wall],
                   [-x/2+2,-y/2+2,-z/2+wall]]){
            translate(i)difference(){
               cylinder(d = 5, h = z+0.5);
               translate([0,0,z/2])cylinder(d = 2, h = z+0.6);
            }
         }
         }
      module lid(){
         difference(){
         cuboid([x+2*wall,y+2*wall,1.5*wall],center = true,fillet = 2, edges=EDGES_Z_ALL);
            for(i=[[x/2-2,y/2-2,-z/2+wall],
                   [x/2-2,-y/2+2,-z/2+wall],
                   [-x/2+2,y/2-2,-z/2+wall],
                   [-x/2+2,-y/2+2,-z/2+wall]]){
            translate(i){
               cylinder(d = 3, h = z+0.6);
               translate([0,0,16])cylinder(d = 4.5, h = 2.1);
            }
                      
                   }
           
         }
      }
      body();
      translate([0,0,30])lid();
      }
 
   *loadcell();
   difference(){
      union(){
          cell_holder();
          translate([0,0,4])case();
      }
      holes();
     
   }
}
difference(){   
   ver2();
   *translate([0,60,0])cube(120, center = true);
}
