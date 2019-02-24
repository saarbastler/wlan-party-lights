$fn=100;

module glas()
{
  difference()
  {
    union()
    {
      cylinder(d1=35, d2=38,h=24,$fn=8);

      translate([0,0,24]) cylinder(d=38, h=10);

      translate([0,0,34]) cylinder(d=41, h=3);

      translate([0,0,37]) cylinder(d=38, h=9);
    }
   
    translate([0,0,29]) cylinder(d1=31,d2=33,h=18);
    translate([0,0,3]) cylinder(d=31,h=26.01,$fn=8);
  }
}

module deckelTeil(a, b)
{
  difference()
  {
    cylinder(d=41,h=12.49);
    
    translate([0,0,-1])
    {
      rotate([0,0,a]) translate([0,-25,0]) cube([25,50,15]);
      
      rotate([0,0,90+b]) translate([-25,0,0]) cube([50,25,15]);
      
      cylinder(d=39,h=15);
    }
  }
}
module deckel()
{
  translate([0,0,37]) 
  {
    difference()
    {
      color("lightsalmon") cylinder(d=41.01,h=11);
     
      translate([0,0,-1]) cylinder(d=38,h=10);
    }
  }
  
  difference()
  {
    translate([-15,-15,48])
    {
      for(y=[0:20:40])
        for(x=[0:20:40])
          translate([x,y,0])
          {
            color("red") cube([10,10,.1]);
            color("lightsalmon") translate([-10,0,0]) cube([10,10,.1]);
            color("lightsalmon") translate([0,-10,0]) cube([10,10,.1]);
            color("white") translate([-10,-10,0]) cube([10,10,.1]);
          }
    }
    translate([0,0,45]) difference()
    {
      cylinder(d=100, h=10);
      
      translate([0,0,-1]) cylinder(d=41,h=12);
    }
  }

  *translate([0,0,36.5])
  {
    color("white") deckelTeil(-14,14.5);
    color("lightsalmon") deckelTeil(14.5,43);
    color("red") deckelTeil(43,47);
  }
}

module platine()
{
  translate([0,0,30]) cylinder(d=31.8,h=1.5);
}

module ring()
{
  d= 31.5-2*1.5;
  alpha= 15;
  
  translate([0,0,31.5])
  {
    difference()
    {
      cylinder(d=31.5, h=15);
      
      translate([0,0,-1]) cylinder(d= d, 20);
      
      y=d*sin(15);
      *translate([0,-y/2,-1]) cube([20,y,20]);
      rotate([0,0,-alpha]) translate([0,0,-1]) cube([20,5,20]);
      rotate([0,0, alpha]) translate([0,-5,-1]) cube([20,5,20]);
    }
    
    rotate([0,0,-alpha]) translate([d/2+1.5/2,0,0]) cylinder(d=1.5,h=15);
    rotate([0,0,alpha]) translate([d/2+1.5/2,0,0]) cylinder(d=1.5,h=15);
  }
}

module schutz(d=7.5)
{
  translate([0,0,44]) difference()
  {
    union()
    {
      cylinder(d=d+7,h=2);
      cylinder(d=d+2.3, h=10);
    }
    translate([0,0,-1]) cylinder(d=d,h=15);
    translate([0,0,-1]) cylinder(d=d+.5,h=1.3);
  }
}

module gd(col)
{
  color(col) glas();
  deckel();
}

/*translate([0,0,0]) rotate([0,0,0]) gd("aqua");
translate([60,0,0]) rotate([0,-5,0]) gd("red");
translate([120,0,0]) rotate([0,5,0]) gd("yellow");
translate([180,0,0]) rotate([0,-5,0]) gd("green");
translate([240,0,0]) rotate([0,5,0]) gd("fuchsia");
translate([300,0,0]) rotate([0,-5,0]) gd("blue");
*/


*rotate([0,0,0]) gd();

*ring();
*schutz(5.5);
*schutz();

%glas();
deckel();
platine();
ring();
schutz();
