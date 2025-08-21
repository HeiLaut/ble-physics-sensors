
module oledCase(PART = "cutout"){
    if(PART == "cutout")cutout();
    if(PART == "snap")snap();
    if(PART == "cover")cover();
    $fn = 60;
    wall = 2;
    PARTS = 0;
    CUT = 0;
    FACE = FRONT;
    tol = 0.2;
    //OLED BOARD
    boardSize = [39,12,3.5];
    //OLED DISPLAY
    disSize = [25,8];

    module board(){
        color("red"){
            cuboid(boardSize,anchor= BOTTOM);
            translate([boardSize[0]/2-3/2-0.2,0,boardSize[2]])cuboid([3,10,8.5],anchor=BOTTOM);
            }
        
        }


    module cutout(){
        union(){
            //cuboid([50,30,wall],chamfer = 4, except=[TOP,BOTTOM],anchor=TOP);
            cuboid([disSize[0],disSize[1],wall+1],rounding = 1, except=[TOP,BOTTOM],anchor=TOP);
            #cuboid([disSize[0],disSize[1],wall],chamfer = -1, edges=[BOTTOM],anchor=TOP);
            }
            }
    
    
    module snap(){       
        for(i=[-1,1]){
            fwd(i*(boardSize[1]/2+1+tol)){
                cuboid([boardSize[0],2,boardSize[2]],anchor=BOTTOM);
                translate([0,-i/2,boardSize[2]-1])xcyl(d = 2,h = boardSize[0]);
                }
            }
    }

    module cover(){
        difference(){
            cuboid([boardSize[0]+2*wall+tol*2,boardSize[1]+2*wall+4,boardSize[2]+tol*2+wall],anchor=BOTTOM,chamfer=1,edges=TOP);
            cuboid([boardSize[0]+tol*3,boardSize[1]+2*wall+3*tol,boardSize[2]+tol*2],anchor=BOTTOM);
            for(i=[-1,1]){
                translate([0,-i*(boardSize[1]/2+1+tol+1/2),boardSize[2]-1-tol/2])xcyl(d = 2+tol,h = boardSize[0]);
                 translate([boardSize[0]/2-3/2-0.2,0,boardSize[2]])cuboid([5,12,8.5],anchor=BOTTOM);
                
            }
    }
    }
//    difference(){
//        union(){
//            baseplate();
//            up(tol/2+20
//            )cover();
//            if(PARTS)board();
//            }
//        if(CUT)cuboid(100,anchor=FACE);
//        }
}    