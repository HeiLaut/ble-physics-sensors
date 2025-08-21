include <BOSL/constants.scad>
use <BOSL/transforms.scad>
use <BOSL/shapes.scad>

//constants
dMount = 5;
dLoad = 5;
xLoad = 80;
yLoad = 12.7;
zLoad = yLoad;
versatz = -4.5;
x = 120;
y = 55;
z = 35;
w = 1.5; //wall
x_lol=26;
y_lol=49.5;
$fn = 20;

lolinpos = x/2-x_lol/2-w-3;
h_lol = 14;
lolinypos =1;

module blende(){
    color("yellow")import("RJ45-blende.STL");
}
module hx711(){
    import("hx711.stl");
}
module lolin(){
    color("red")translate([-10,18,0])rotate([180,0,-90])import("lolin32_lite.stl");
}

module rj45(){
    import("rj45.stl");
}


module lolinmount(){
    d1 = 4;
    d2 = 2;
    fwd(lolinypos){
    translate([-lolinpos-9,22.5,-z/2+w])difference(){
        up(h_lol/2)right(10)cube([x_lol,d1,h_lol],center = true);//cylinder(h = h_lol,d =d1);
        up(1)cylinder(h = h_lol,d =d2);
    }
    translate([-lolinpos+12,-16,-z/2+w])difference(){
        up(h_lol/2)left(11)fwd(2)cube([x_lol,d1*2,h_lol],center = true);
        //cylinder(h = h_lol,d =d1);
        up(1)cylinder(h = h_lol,d =d2);
    }
}
        
        
}

module lid(){
   difference(){
      cuboid([x,y,w],chamfer =2,edges = EDGES_Z_ALL);
      #for(i=[ [-x/2+w+2,-y/2+w*2,-w/2],
            [x/2-2*w,y/2-2*w,-w/2],
            [-x/2+2*w,y/2-2*w,-w/2],
            [x/2-2*w,-y/2+2*w,-w/2]]){
               translate(i)cylinder(d = 2,w);
            }
      right(x/2-xLoad/2-2)translate([-35.5,0,-20])cylinder(h = 60, d = dLoad+2);   

         }
         

}

module holes(){
    //holes that fit into the load cell
        translate([versatz,0,0]){
           for(i=[[20,0,-50],[35,0,-50]]){
            translate(i)cylinder(h = 60, d = dMount);
        }      
        translate([-35,0,-20])cylinder(h = 60, d = dLoad);   
        }
    }
   
module loadcell(){
    difference(){
    translate([versatz,0,0])cube([xLoad,yLoad,zLoad],center = true);
    holes();}
}

module case(){
    difference(){
        cuboid([x,y,z-w],chamfer =2,edges = EDGES_Z_ALL);
        up(w)cuboid([x-2*w,y-2*w,z-2*w],chamfer = 2, edges = EDGES_Z_ALL);
        translate([(x/2-xLoad/2+2)+versatz+20,0,-z/2+w/2-0.1])cylinder(d1=10,d2=5,h=2);
        translate([(x/2-xLoad/2+2)+versatz+35,0,-z/2+w/2-0.1])cylinder(d1=10,d2=5,h=2);
        //cut for rj45 mouting plate
        translate([-40,-y/2+2,1])cuboid([29,w*3,19]);
        //rod hole
        up(z/2-7)rotate([0,90,0])cylinder(d=10.5,h=150);
       
         translate([x/2.5,0,z/2-7])rotate([90,0,0]){
                cylinder(h = y/2+5, d = 5);
               }
      

    }
    up(1.1)difference(){
                right(x/2-xLoad/2-19)cuboid([xLoad-xLoad/3,yLoad+5,z-w*2.5]);
                right(x/2-xLoad/2-3)cuboid([xLoad+4,yLoad+2,z-2*w+1]);
            }
    module screwshaft(){
        difference(){
            cylinder(d=4,h=z-w);
            cylinder(d=2,h=z-w);
        }
    }
    for(i=[ [-x/2+w+2,-y/2+w*2,-z/2+w/2],
            [x/2-2*w,y/2-2*w,-z/2+w/2],
            [-x/2+2*w,y/2-2*w,-z/2+w/2],
            [x/2-2*w,-y/2+2*w,-z/2+w/2]]){
        translate(i)screwshaft();
    }
   
    
}

module externalparts(){
        up(-z/2+h_lol+w)fwd(lolinypos)left(lolinpos)lolin();
        down(3)color("green")right(x/2-xLoad/2+2)loadcell();
        for(i = [-15,30]){
        translate([i,y/2-w-5,-z/2+w+2])hx711();
        *translate([-26,-y/2-9,0])rj45();
             
            
         *translate([-x/2+29,-y/2+6,z/2-1])rotate([180,0,0])blende();

        }
    }

difference(){
    union(){
        case();
        *externalparts();
        }
        right(x/2-xLoad/2+2)holes();
    
    *fwd(50)cuboid([x+10,100,z+10]);
    }


module rodandcellholder(){
//load cell base
    difference(){
        union(){
           translate([x/2-xLoad/5-0.3-0.1,0,-(z/2-zLoad/2-2*w+3)/2-zLoad/2+1])cuboid([xLoad/2.7,yLoad+5,z/2-zLoad/2-2*w-3]);
           translate([x/2-xLoad/5-0.3-0.1,-11.25,-4.85+1])cuboid([xLoad/2.7,5,z-10.2-2.5-2*w]);
             //border for load cell
            
            //stand mount block
            difference(){
                
          color("green")translate([x/2-xLoad/5-0.4,-(yLoad+5+(y-(yLoad+5))/2-2*w)/2+yLoad-5,(z-w*2)/2-(10.2+2.5)/2+1.3])cuboid([xLoad/2.7-0.0,yLoad+5+(y-(yLoad+5))/2-2*w,10.2+1.5],chamfer=7, edges =EDGE_FR_RT);
           //hole for screw and nut to hold the rod
           translate([x/2.5,0,z/2-7])rotate([90,0,0]){
                cylinder(h = y/2+5, d = 5);
                translate([0,0,20]){
                    rotate([0,0,90])cylinder(h = 3.5, d=8.4, $fn = 6);
                    translate([0,3,1.7])cube([7.4,9,3.5],center = true);
                }
           }
      }
            
        }
        down(z/2)right(x/2-xLoad/2+2)holes();
        //rod hole
        up(z/2-7)right(30)rotate([0,90,0])cylinder(d=10.5,h=50);

    }

}
right()rodandcellholder();
up(z/2+5)lid();


