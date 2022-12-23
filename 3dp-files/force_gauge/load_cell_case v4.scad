include <BOSL/constants.scad>
use <BOSL/transforms.scad>
use <BOSL/shapes.scad>

//Load cell properties
lcX = 80;
lcY = 12.7;
lcZ = lcY;

//load cell connector properties
lccX = 20;
lccY = lcY;
lccZ = 2;
//mount and load holes
mX = 7;
mX2 = 12;
dMount = 5;
dLoad = 5;
lX = -7;
lX2 = -12;
versatz = [0,0,0];
$fn = 60;
//dimensions of the case
x = lcX+5;
y = lcY+10;
z = 55;
wall = 1.2;

//dimensions load cell holder
xCh = lcX;
yCh = y-1;
zCh1 = 10;
xCh2 = 20;
zCh2 = 15;

//dimensions load holder
xLh = lcX/1.5;
yLh = y-1;
zLh = 5;

 module screwshaft(d1=4,d2=2,h=5){
        translate([0,0,0])rotate([90,0,0])difference(){
            cylinder(d=d1,h=h);
            cylinder(d=d2,h=h);
        }
    }
module holes(){
    //holes that fit into the load cell
        translate(versatz){
           for(i=[[20,0,-30],[35,0,-30]]){
            translate(i)cylinder(h = 60, d = dMount);
        }
            //hole for mount
        for(i=[[-20,0,-30],[-35,0,-30]]){

        translate(i)cylinder(h = 60, d = dLoad);   
        }
    }
    }
   
module loadcell(){
    difference(){
    translate(versatz)cube([lcX,lcY,lcZ],center = true);
    holes();}
}
module case(x=x,y=y,z=z,a=1){
    difference(){
        cuboid([x+wall*2,y+wall*2,z+wall*2],fillet = 3, edges=EDGES_Y_ALL);
        translate([0,-a*wall,0])cuboid([x,y+wall,z],fillet = 3, edges=EDGES_Y_ALL);
    }
}
module load_cell_mount(){

    up(lcZ/2+zCh1/2+2)
    difference(){
    union(){
        cuboid([xCh,yCh,zCh1]);
        translate([0,0,zCh1/2+zCh2/2])cuboid([xCh2, yCh, zCh2]);
    }
    holes();
    }
    difference(){
    left(lcX/2-xLh/2)down(lcZ/2+zLh/2+2)cuboid([xLh,yLh,zLh]);
        holes();
        down(20)cylinder(h=20,d=5);
    }
  
}


module lolinmount(){
    x_lol=26;
    y_lol=49.5;
    lolinpos = 10;
    h_lol = 5;
    lolinypos =1;
    d1 = 4;
    d2 = 2;
    fwd(lolinypos){
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
    


module lolin_lid(){
    y=11;
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
    
         //BATTERY connector
         translate([x/2+1,3.5,7.2])cuboid([2*wall,6,7]);
         //USB connector
         translate([x/2+1,2.1,-8.5])cuboid([2*wall,3,7]);
       
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
    *translate([-0,00,10])rotate([90,0,0])import("parts/lolin32_lite.stl");
    translate([17,-5.4,-10])rotate([-90,90,0])lolinmount();
    
}
module load_cell_case(){
    case();
    for(i=[[-x/2+wall+0.5,y/2-wall,-z/2+wall+0.5],
        [x/2-wall-0.5,y/2-wall,z/2-wall-0.5],
        [x/2-wall-0.5,y/2-wall,-z/2+wall+0.5],
        [-x/2+wall+0.5,y/2-wall,z/2-wall-0.5] ]){
        translate(i)screwshaft(4,2,y);
    }
}

back(5)load_cell_case();
fwd(15)lolin_lid();
//loadcell();
//down(6)back(20)load_cell_mount();