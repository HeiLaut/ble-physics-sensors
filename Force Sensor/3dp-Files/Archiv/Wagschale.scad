$fn = 60;
d1 = 50;
d2 = 60;
h = 10;
difference(){
   cylinder(d1 = d1, d2 = d2, h = h);
   #translate([0,0,2])cylinder(d1 = d1, d2 = d2, h = h);
   cylinder(d=4,h=5);
   translate([0,0,1])cylinder(d=7.6, h = 4);
}