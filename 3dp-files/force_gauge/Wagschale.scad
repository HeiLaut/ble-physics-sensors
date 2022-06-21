$fn = 50;
difference(){
   cylinder(d1 = 50, d2 = 55, h = 10);
   translate([0,0,4])cylinder(d1 = 49, d2 = 54, h = 8.1);
   cylinder(d=4,h=5);
   translate([0,0,1])cylinder(d=7.6, h = 4);
}