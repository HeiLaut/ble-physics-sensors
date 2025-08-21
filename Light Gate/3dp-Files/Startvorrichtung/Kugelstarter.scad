include<BOSL2/std.scad>
$fn =60;
x = 80;
y = 22;
z = 21;
d = 17;//17;//12.65;//25.26;
dH = d+1;
faktor = 1.2;

difference(){
   right(40)cuboid([20,10,5],anchor=LEFT+TOP);
   up(0.1)translate([53.6,-6,0])cyl(d=6,h=9,anchor=TOP);
}
left(40)cuboid([20,10,5],anchor=RIGHT+TOP);

difference(){
   cuboid([x,y,z],anchor=TOP);
   up(0.1)cyl(d=dH,h=z+2,anchor=TOP);
   down(z+0.1)xcyl(d=3,h=x*2,anchor=BOTTOM);
   down(z-2.5)right(faktor*dH)
      sphere(d=dH,anchor=BOTTOM);
   right(faktor*dH)cyl(d=dH,h=z-dH/2-2.5,anchor=TOP);
   rotate([0,0,0])translate([0,0,-z+2.5])xcyl(d=dH,h=faktor*dH,anchor=LEFT+BOTTOM);

}

*translate([0,-107.9,0])import("LightGateCasev3_1.stl");