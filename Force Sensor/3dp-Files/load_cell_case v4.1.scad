include <BOSL/constants.scad>
use <BOSL/transforms.scad>
use <BOSL/shapes.scad>

//Load cell properties
lcX = 80;
lcY = 12.7;
lcZ = lcY;

x = lcX+5;
y = 20;
z = 65;
wall = 1.2;

$fn = 30;
connector = "rj45"; //"rj45" or "none"
y2 = 20;
master = true;
d_rod = 10.5;


module hook(){
   difference(){
      linear_extrude(7)import("haken.svg");
      translate([15,40,3.5])ycyl(d=5,h=20);
      translate([15,38.5,3.5])ycyl(d=9,h=4,$fn=6);
      rotate([5,-4,0])translate([20,25,5])cuboid([40,50,5]);
      }
}




module rj45(){
   difference(){
      import("parts/keystone.stl");
   }
}


 module screwshaft(d1=4,d2=2,h=5){
        translate([0,0,0])rotate([90,0,0])difference(){
            cylinder(d=d1,h=h);
            cylinder(d=d2,h=h);
        }
    }
    
module holes(dMount = 5,dLoad = 5){
  
   rotate([90,0,0]){
   //holes that fit into the load cell
        for(i=[[20,0,-30],[35,0,-30]]){
            translate(i)cylinder(h = 60, d = dMount);
        }
            //hole for mount
        for(i=[[-20,0,-30],[-35,0,-30]]){

        translate(i)cylinder(h = 60, d = dLoad);   
        }
     }
    }
    
module trapez(x1 = 30, x2 = 20, y = 5){
   polygon(points=[[-x1/2,0],[-x2/2,y],[x2/2,y],[x1/2,0]]);
}

module rod_cross(d= d_rod, h = 100){
   translate([0,0,-h/2])cylinder(d = d_rod, h = h);
   translate([-h/2,0,0])rotate([0,90,0])cylinder(d = d_rod, h = h);
}


module connectorA(){
   x = 20;
   y = 20;
   z = 20;
   height = 7;
   
   difference(){
      cuboid([x,y,z]);
      translate([0,9,0])rotate([90,0,0]){
            rotate([0,0,90])cylinder(h = 3.5, d=8, $fn = 6);
            translate([0,10,1.7])cube([7,20,3.5],center = true);
           translate([0,0,-2.5])cylinder(d = 5, h =10);
          }
          translate([0,-2,0])rod_cross();

   }
   translate([0,-y/2-height,-z/2])linear_extrude(z)trapez(30,20,height);
   
   //screw holder
  
}
module loadcell(){
    difference(){
    cube([lcX,lcY,lcZ],center = true);
    holes();}
}


module connectorB(){
   xCh = lcX;
   z = 20;
   y = 12;
   height = 7;

   difference(){
      cuboid([xCh,y,z]);
      translate([0,y/2-height-0.2,-z/2])linear_extrude(z)trapez(30.8,20.8,height+0.2);
      holes();
      back(30)holes(10,0);
   }

}

module load_connector(){
    difference(){
       left(18)fwd(21)cuboid([x-35,9,y-5]);
        holes();
        fwd(10)rotate([90,0,0])cylinder(h=20,d=5);
       translate([0,-19.5,0])rotate([90,0,0]){
            rotate([0,0,90])cylinder(h = 4, d=9.5, $fn = 6);
            translate([0,10,2])cube([8,20,4],center = true);
           translate([0,0,-2.5])cylinder(d = 5, h =10);
          }
    }
 }
 
 
 
module case(x=x,y=y,z=z,a=1){
    difference(){
       cuboid([x+wall*2,y+wall*2,z+wall*2],fillet = 3, edges=EDGES_Y_ALL);
        translate([0,-a*wall,0])cuboid([x,y+wall,z],fillet = 3, edges=EDGES_Y_ALL);
           

    }
}

module lolin_lid(){
   h_lol =5.5;
   module lolinmount(){
    
    lolinypos =20;
    d1 = 4;
    d2 = 2;
    x_lol=26;
    y_lol=49.5;
    lolinpos = 0;
    color("blue"){
    translate([-4,-1,0]){
    translate([-lolinpos-9,22.5,0])difference(){
        up(h_lol/2)right(10)cube([x_lol,d1,h_lol],center = true);//cylinder(h = h_lol,d =d1);
        up(1)cylinder(h = h_lol,d =d2);
    }
    translate([-lolinpos+12,-16,0])difference(){
        up(h_lol/2)left(11)fwd(2)cube([x_lol,d1*2,h_lol],center = true);
        //cylinder(h = h_lol,d =d1);
        up(1)cylinder(h = h_lol,d =d2);
    }
}
}
             
}
      //rj45 jack
      if(connector == "rj45"){
         translate([28.7,-0.5,66])rj45();
      }
      y = y2;
      difference(){
         union(){
            case(x,y,z,-1);
             for(i=[[-x/2+wall+1,y/2+wall,-z/2+wall+01],
        [x/2-wall-1,y/2+wall,z/2-wall-1],
        [x/2-wall-1,y/2+wall,-z/2+wall+1],
        [-x/2+wall+1,y/2+wall,z/2-wall-1] ]){
        translate(i)screwshaft(6,0,y+1);
        }
       }
         if(master){
         //BATTERY connector
         translate([x/2+1,-y/2+h_lol+3.7,7.2-6-5])cuboid([2*wall,6,7]);
         //USB connector
         translate([x/2+1,-y/2+h_lol+2.5,-8.5-6-5])cuboid([2*wall,4,8]);
         
         //tara button hole
         translate([x/2+1,0,17])cuboid([2*wall,6,6]);
         }
         //rod hole
       translate([0,50,20.5])rotate([90,0,0])cylinder(d = d_rod, h = 100);
         //rj45 hole
         if(connector == "rj45"){
         translate([-x/2,-1,10])cuboid([3,16,20]);
            }
       for(i=[[-x/2+wall+0.5,y/2-wall,-z/2+wall+0.5],
        [x/2-wall-0.5,y/2-wall,z/2-wall-0.5],
        [x/2-wall-0.5,y/2-wall,-z/2+wall+0.5],
        [-x/2+wall+0.5,y/2-wall,z/2-wall-0.5] ]){
        translate(i){
            screwshaft(4,0,y);
            back(5)screwshaft(2,0,y);
        }
    }

    }
    *translate([-0,-y/2+h_lol,10-6-5])rotate([90,0,0])import("parts/lolin32_lite.stl");
    
    //mounting brackets for lolin lite
    if(master){
    translate([17,-y/2,-15])rotate([-90,90,0])lolinmount();
    }
    
}
module load_cell_case(){
   difference(){ 
   union(){
       color("green")case();
       for(i=[[-x/2+wall+0.5,y/2-wall,-z/2+wall+0.5],
           [x/2-wall-0.5,y/2-wall,z/2-wall-0.5],
           [x/2-wall-0.5,y/2-wall,-z/2+wall+0.5],
           [-x/2+wall+0.5,y/2-wall,z/2-wall-0.5] ]){
           translate(i)screwshaft(4,2,y);
       }}
    fwd(wall/2)up(20.5)rotate([90,0,0])rod_cross();
    // hole for screw 
    fwd(wall/2)down(-z/2+10)cylinder(d = 5,h = 20);
    // hole for load hook
    fwd(wall/2)down(z/2+10)cylinder(d = 10,h = 20);   
   
       }
}

up(40)rotate([-90,0,0])lolin_lid();


rotate([-90,0,0])load_cell_case();
up(wall/2)back(22.5)connectorA();
*fwd(60)up(wall/2)back(22.5)translate([0,-16,0])connectorB();

!fwd(60)up(wall/2)load_connector();

*fwd(8)loadcell();
*hook();
