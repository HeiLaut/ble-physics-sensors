difference(){
   translate([0,0,10.5])rotate([-90,0,0])import("RJ45-blende.STL");
   cube([50,50,2]);
   cube([4,50,10]);
   translate([33.5,0,0])cube([6,50,10]);

}