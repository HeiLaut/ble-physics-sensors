include<BOSL2/std.scad>
include<BOSL2/threading.scad>

$fn = 60;
dM = 22;
module thread(orient = BACK){
   threaded_rod(d=10, pitch = 2, $slop=0.2,internal=true,l = dM/2-2,anchor=BOTTOM,orient=orient);
}


module mount(){
difference(){
    cuboid([40,15,30],anchor = TOP,chamfer = 2);
    cuboid([21,5,22],anchor=TOP);
    down(10)back(-1.5)thread();
    for(i=[-1,1])left(i*(20-9))down(25)xcyl(d=1.1, h  = 18);
    }
    }
    
    
module cablesupport(){
    difference(){
      cuboid([40,15,30],anchor = TOP,chamfer = 2);
      down(10)ycyl(d= 10.5,h=15);
      down(0)thread(BOTTOM);
      for(i=[1,-1])left(i*10)down(30)cyl(d=4,h=10,anchor=BOTTOM);
    }

}


cablesupport();