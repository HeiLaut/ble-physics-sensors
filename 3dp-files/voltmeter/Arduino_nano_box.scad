ar_x = 18.5;
ar_y = 44;
ar_z = 30;
pin_abstand_aussen = 15.9;
pin_abstand_innen = 14.4;
pin_z = 5.7;
pin_base_z = 2.5;
stecker_d = 6;
stecker_h = 18;
wall = 1.5;
x_usb = 10;
z_usb = 4.7;
y_usb = 10;
module ard(){
    include<Arduino_nano.scad>
}
$fn = 30;
module box(){
    difference(){
    cube([ar_x+2*wall,ar_y+2*wall,ar_z+wall], center = true);
    translate([0,0,wall])cube([ar_x,ar_y,ar_z], center = true);
    translate([0,(-ar_y+2*wall)/2,(ar_z)/2])cube([x_usb,y_usb,z_usb*2.5], center = true);  
       
     translate([2,10,-(ar_z+wall)/2-1])cylinder(d = stecker_d, h = stecker_h);
     translate([2,-10,-(ar_z+wall)/2-1])cylinder(d = stecker_d, h = stecker_h);
        translate([-9.5,5.5,-ar_z/2-1])linear_extrude(wall)text("+", font = "Liberation Sans:style=Bold" );
    translate([-6,-12,-ar_z/2-2*wall-1])rotate([90,0,90])linear_extrude(wall)text("-", font = "Liberation Sans:style=Bold" );
        translate([-ar_x/2+0.5,ar_y/2,ar_z/2-2])rotate([90,0,0])cylinder(h = ar_y, d = 2);
        translate([+ar_x/2-0.5,ar_y/2,ar_z/2-2])rotate([90,0,0])cylinder(h = ar_y, d = 2);

    }
    translate([0,0,5])ard();
    x_spacer = (ar_x-pin_abstand_aussen)/2;
    translate([-(ar_x/2-x_spacer/2),0,-(ar_z/2-9)/2-1])cube([x_spacer,ar_y,ar_z/2+5],center = true);
    translate([+(ar_x/2-x_spacer/2),-ar_y/4.6,-(ar_z/2-9)/2-1])cube([x_spacer,ar_y/1.6,ar_z/2+5],center = true);
}
module deckel(){
    
    translate([0,0,ar_z/2+wall+10]){
        difference(){
        union(){
            cube([ar_x+2*wall,ar_y+2*wall,wall], center = true);
            translate([0,0,-1.75*wall])cube([ar_x-0.4,ar_y-0.4,3.5*wall], center = true);
            translate([-ar_x/2+0.7,ar_y/2-wall/2,-2-wall-0.2])rotate([90,0,0])cylinder(h = ar_y-wall, d = 2);
            translate([+ar_x/2-0.7,ar_y/2-wall/2,-2-wall-0.2])rotate([90,0,0])cylinder(h = ar_y-wall, d = 2);}
        translate([0,0,-1.75*wall])cube([ar_x-0.4-2*wall,ar_y-0.4-2*wall,3.5*wall+0.5], center = true);
        }
    }
    
}

        
   
difference(){
    union(){
        box();
        deckel();
    }
    *translate([-25,0,-25])cube(50); 
}