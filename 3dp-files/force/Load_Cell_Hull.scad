a1 = 5;
a2 = 20;
d_h = 5.5;  
d_h_load = 4+2;
x_lc = 13;
z_lc = 13;
y_lc = 80.5;
    d_d = 10.7;
    $fn = 40;


module load_cell(){
    x = x_lc+0.2;
    z = z_lc+0.2;
    y = 80.5;
    d_h = d_h -0.5;
    
    translate([-x/2,0,-z/2])difference(){
    cube([x,y,z]);
    translate([x/2,a1,-0.5])cylinder(d = d_h, h = z+1);
    translate([x/2,a2,-0.5])cylinder(d = d_h, h = z+1);
    translate([x/2,y-a1,-0.5])cylinder(d = d_h_load, h = z+1);
    translate([x/2,y-a2,-0.5])cylinder(d = d_h_load, h = z+1);
    }
}

module cover_mount(){
    x = 38;
    z = x;
    y = 26;
    wall = 0;
    translate([-x/2,-wall,-z/2])
    difference(){
    cube([x,y+ wall,z]);
    translate([x/2,a1+wall,-0.5])cylinder(d = d_h, h = z+1);
    translate([x/2,a2+wall,-0.5])cylinder(d = d_h, h = z+1);
    translate([-0.5,y/2+wall-1/2,z/2-z_lc+1])rotate([0,90,0])cylinder(d = d_d, h = x+1);
        translate([x/2,y/2+wall-1/2,]){
            cylinder( h = 5, d = d_h_load);
        }
    }
    
}

module cover_load(){
    z_1 = 53;
    wall = 2;
    y_1 = 26 + 2*wall;
    x_1 = 38+ 2*wall+2;
    x = 38;
    z = x;
    y = 26;
    deckel = 10;
    difference(){
    hull(){
    translate([-x_1/2, -wall, -z_1/2]){
    translate([0,-deckel,0])cube([x_1, y_1+deckel, z_1]);
    //*translate([2.5,-1,z_1-z-(z_1-z)/2]);
    }
    translate([0,82,0])cube(20, center = true);}
    translate([0,y/2-0.5-deckel,0])cube([x+1,y+2*wall+10+deckel,z+1], center = true);
    translate([0,y_lc/2,0])cube([x_lc+6, y_lc+6, z_lc+6], center = true);
    translate([0,y_lc-a1,-z/2])cylinder(d = d_h_load, h = z+1);
    translate([0,y_lc-a2,-z/2])cylinder(d = d_h_load, h = z+1);
    translate([-x/2,-wall,-z/2]){
    translate([x/2,a1+wall,-10]){
        cylinder(d = d_h, h = z+20);
       translate([0,0,2.5]) cylinder(d = 9, h = 4.5, $fn = 6);
                translate([0,0,1.35*z]) cylinder(d = 10.2, h = 4.5);

    }
    translate([x/2,a2+wall,-10]){
        cylinder(d = d_h, h = z+20);
        translate([0,0,2.5]) cylinder(d = 9, h = 4.5, $fn = 6);
        translate([0,0,1.35*z]) cylinder(d = 10.2, h = 4.5);

    }

    translate([-5,y/2+wall-1/2,z/2-z_lc+1])rotate([0,90,0])cylinder(d = d_d, h = x+10);
        translate([x/2,y/2+wall-1/2,-15]){
            translate([0,0,10.5])cylinder(h= 5, d = 8.5, $fn = 6);
            cylinder( h = 15, d = d_h_load);
        }
    }
        

}
}

module deckel(){
    z_1 = 53;
    wall = 1.5;
    y_1 = wall+10;
    x_1 = 38+ 2*wall+2;
    x = 38.5;
    z = x;
    y = 9;
    deckel = 10;
   x_e = 31;
   y_e = 10;
   z_e = 39;
     b = 3;
    translate([0,0,0]){
        difference(){
           union(){
        translate([0,-y/2-5,0])cube([x_1,y_1,z_1], center = true);
        cube([x+0.6,y,z], center = true);
        translate([0,-y,0])cube([x_e+b,y_e,z_e+b], center = true);

           }
        translate([0,-y/2+1,0])color("red")cube([x_e,y_e+y+wall,z_e], center = true);
        translate([0,-7.5,z_e/2])cube([10,12,b+25],center = true);
        //translate([0,-1,0])cube([x-2*wall,y+5,z-2*wall], center = true);
        }

    }
}

*color("green")cover_load();

*difference(){
    cover_mount();
    load_cell();
    translate([-1.5*x_lc,0,-z_lc/2])difference(){
    cube([x_lc,50,z_lc]);}
}


*translate([137.3,-50,-43.5])import("esp32.stl");
deckel();

    