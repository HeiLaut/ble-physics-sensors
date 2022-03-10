include <BOSL/constants.scad>
use <BOSL/transforms.scad>
use <BOSL/shapes.scad>

us_hole_dia = 16.5; //Lochdurchmesser Ultraschallmodul
us_hole_dis = 25;
us_h_speaker = 12.5;
button_x = 6.4;
wall = 1.5;
inner_y = 46;
inner_x = 32;
esp_x = 31.5;
esp_y = 39;
esp_z = 1.5;
inner_z = 30;
outer_r = 3;
outer_x = inner_x+2*wall;
outer_y = inner_y+2*wall;
outer_z = inner_z+2*wall;
us_d_x = 15.5; //Abstand in der Ultrashallsensorlöcher von oben

$fn = 40;

module us_s(){
   translate([5.5,2,11])
   rotate([180,0,90])
   import("HC-SR04.stl");
}
*color("blue")us_s();
    
    
    
module case(){
    module corner(){
        circle(r = outer_r);
    }
    module floor(){
        hull(){
            corner();
            translate([outer_x-2*outer_r,0,0])corner();
            translate([outer_x-2*outer_r,outer_y-2*outer_r,0])corner();
            translate([0,outer_y-2*outer_r,0])corner();
        }
    }
    module walls(){
        linear_extrude(outer_z)
            difference(){
            floor();
            offset(delta = -wall)floor();  
           
        }
        linear_extrude(wall)floor();
    }
    module us_holes(){
        translate([0,us_hole_dis/2,0])cylinder(d = us_hole_dia, h  = us_h_speaker);
        translate([0,-us_hole_dis/2,0])cylinder(d = us_hole_dia, h  = us_h_speaker);
    }
    //Abstandshalter für Module
    module puffer(){
        module tria(a = 10,b = 10){
            p1 = [0,0];
            p2 = [0,a];
            p3 = [b,a];
            polygon(points = [p1,p2,p3]);
        }
        u_z = 8;
        u_y = 1.5;
        u_x = 15;
        
        e_z = 21;
        e_y  =25;
        e_x = 1.5;
        
        e_s_y = 36.6; //Abstand
        
        
           for(i=[[3.5,0,0],[3.5+u_x+3,0,0],[3.5,43,0],[3.5+u_x+3,43,0]]){
              translate(i)difference(){
               cube([3,3,u_z]);
               translate([1.5,1.5,3.5])cylinder(d=1,h=6);
           }
        }
        translate([us_d_x-u_x/2-1.5,0,0])cube([u_x,u_y,u_z]);
        translate([us_d_x-u_x/2-1.5,inner_y-u_y,0])cube([u_x,u_y,u_z]);
        cube([e_x,e_y,e_z]);
        translate([inner_x-e_x,0,0])cube([e_x,e_y,e_z]);
        linear_extrude(inner_z-3*wall)translate([0,e_s_y,0]){
            tria(a=2,b=2);
            translate([0,2])square([2,4]);
        }
        

    }
    translate([wall,wall,wall])puffer();
    difference(){
        translate([outer_r,outer_r,0])walls();
        translate([us_d_x,outer_y/2,-2])us_holes();
        translate([outer_x/2,-2,21+wall+3])cube([8,10,4], center = true);//hole for usb
        translate([outer_x/2,outer_y-4,21+wall+2])cube([button_x,20,button_x], center = true);
        
    }
    
    module deckel(){
        linear_extrude(wall)translate([outer_r,outer_r,0])floor();
        mirror([0,0,1])linear_extrude(5)difference(){
            offset(delta = -wall-0.2)translate([outer_r,outer_r,-wall])floor();
            offset(delta = -2*wall-0.2)translate([outer_r,outer_r,-wall])floor();
        }

        
    }
    module mount(){
       dRod = 10.5;
       x=20;
       y=30;
       z=20;
       l = 11;
       translate([-outer_x/2+x/2-1.5*wall,outer_y/2,x/2])difference(){
         cuboid([x,y,z],fillet=3,edges = EDGES_Z_LF);
          scale([1,1,1.05])translate([2.5,y/2+0.5,0])rotate([90,0,0])cylinder(d = dRod, h = y+1);
          translate([-x/2+2,0,0])rotate([90,0,90]){
            rotate([0,0,90])cylinder(h = 3.5, d=8, $fn = 6);
            translate([0,l/2,1.7])cube([7,l,3.5],center = true);
           translate([0,0,-2.5])cylinder(d = 5, h =10);
          }
       }
    }
    mount();
    *translate([0,0,outer_z+wall])deckel();

}
difference(){
    union(){
    case();
    }
   *cube([outer_x,outer_y/2,outer_z+10]);
}