include <BOSL/constants.scad>
use <BOSL/transforms.scad>
use <BOSL/shapes.scad>
use <BOSL/masks.scad>

$fn = 60;
x= 52;
z = 23;
y = 30+28;



module esp(){
   color("red")translate([1.5,42.5,0])rotate([180,0,-90])import("/parts/lolin32_lite.stl");
}

module connectorboard(){
   xB = 38;
   yB = 30;
   zB = 1.5;
   cuboid([xB,yB,zB]);
   color("blue")translate([0,yB/2-5,zB-0.5]){
   translate([xB/2-8.5,0,0])import("/parts/jst.stl");
   translate([xB/2-26,0,0])import("/parts/jst.stl");
   }


}
module parts(){
   translate([-38/2,30/2+1,0.75])connectorboard();
   translate([0.5,-25,0])rotate([0,1,0])rotate([0,0,90])esp();
}

module case(){
   module screwshaft(){
        difference(){
            cylinder(d=4,h=z-3);
            up(15)cylinder(d=2,h=z);
            up(5)rotate([0,-50,0])cuboid([20,10,10]);
        }
    }
    for(i=[ [-x/2+1.7,-y/2+1.7,-z/2+3.7],
            [-x/2+1.7,y/2-1.7,-z/2+3.7]]){
        translate(i)screwshaft();
    }
    mirror([1,0,0])for(i=[ [-x/2+1.7,-y/2+1.7,-z/2+3.7],
            [-x/2+1.7,y/2-1.7,-z/2+3.7]]){
        translate(i)screwshaft();
    }
   module lolinmount(){
       d = 2;
       h = 6.5;
       translate([-x/2+5,-y/2+13.7,-z/2+h/2])difference(){
            cuboid([5,26,h]);
            translate([-0.75,1-11.2,-2])cylinder(d=d,h=h);
      }
      translate([x/2-8.5,-y/2+13.7,-z/2+h/2])difference(){
            cuboid([5,26,h]);
            translate([-0.75,11,-2])cylinder(d=d,h=h);
      }
   }
   module conmount(){
      h = 6;
      translate([x/2-35/2-2,25/2,-z/2+h/2])difference(){
         cuboid([37,25,h]);
         translate([0,3,-0.2])cuboid([35-6,25,h+1]);
      }
     
   }
       
        
   difference(){
      cuboid([x+3,y+3,z+1.5],chamfer =2,edges = EDGES_Z_ALL);
      translate([0,0,0.75])cuboid([x,y,z+1],chamfer =2,edges = EDGES_Z_ALL);
      //JST
      translate([16.54,y/2+1,-1])cuboid([11,3,8]);
      translate([-1,y/2+1,-1])cuboid([11,3,8]);
      //esp
      translate([x/2+1,-23,-1.5])cuboid([3,10,8.5]);
      translate([x/2+1,-7.5,-2.5])cuboid([3,9,5]);
     }
    translate([0.5,0.5,0])lolinmount();
    conmount();
   }
   
module lid(){
   difference(){
         cuboid([x+3,y+3,1.5],chamfer =2,edges = EDGES_Z_ALL);
       for(i=[ [-x/2+1.7,-y/2+1.7,-z/2+3.7],
            [-x/2+1.7,y/2-1.7,-z/2+3.7]]){
        translate(i)cylinder(d=2,h=z);
    }
    mirror([1,0,0])for(i=[ [-x/2+1.7,-y/2+1.7,-z/2+3.7],
            [-x/2+1.7,y/2-1.7,-z/2+3.7]]){
        translate(i)cylinder(d=2,h=z);
    }
}
}
difference(){
   union(){
      case();
      translate([0,0,z/2+10])lid();
      *translate([x/2-1,-2.5,-5])parts();
   }
   //translate([0,-50,-50])cube([100,100,100]);
}