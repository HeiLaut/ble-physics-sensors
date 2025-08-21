d_rad = 52;
h_rad = 5;
n_speichen=20;
a_speiche = 2;
$fn=100;
d_nabe= 20;
d_felge = 4 ;
rille = 2;
d_achse=2.8;


difference(){
    cylinder(d = d_rad, h  = h_rad);
    for(i=[0:360/n_speichen:360]){
        rotate([0,0,i]){
        translate([0,0,-0.1])linear_extrude(h_rad+0.2){
            intersection(){
                polygon([[0,0],[d_rad/2-d_felge,+a_speiche/2],[d_rad/2-d_felge,-a_speiche/2]]);
                difference(){
                    circle(d=d_rad-d_felge*2);
                    circle(d=d_nabe);
                }
            }   
        }
        }
    }
    
    translate([0,0,h_rad]/2)rotate_extrude()translate([d_rad/2-rille+0.1,0,0])polygon([[0,0],[rille,h_rad/2],[rille,-h_rad/2]]);
    //oder:
    //translate([0,0,h_rad]/2)rotate_extrude()translate([d_rad/2-rille+2.5,0,0])circle(d=h_rad*1.2);
    translate([0,0,-0.1])cylinder(d=d_achse,h=h_rad+0.2);
    
}

translate([60,0,0])difference(){
   h=(20.4-5)/2;
   cylinder(d = 6, h=h);
   translate([0,0,-0.5])cylinder(d = 3, h=h+1);
}

