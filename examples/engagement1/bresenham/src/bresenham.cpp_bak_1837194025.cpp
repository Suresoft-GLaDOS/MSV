bool count=true;
extern "C" { int __get_mutant(); int __is_neg(int, ...); int __abst_hole(); }
/* original courtesy of player1537, http://www.cemetech.net/forum/viewtopic.php?t=5401 */

#include <stdlib.h>
#include <stdio.h>
#include <utility>

#include "Bresenham.h"

  void buggy(unsigned x1, unsigned y1,
			    DiscreteLine & line)
  {
    //prophet generated patch
    if (count==true){
    if (this->_M_offset++ == int(_S_word_bit) - 1) {
        this->_M_offset = 0;
        ++this->_M_p;
    }
    short x0 = 0, y0 = 0;
    }

    //prophet generated patch
    if (count==true){
    double steep = abs((int)(y1 - y0)) > abs((int)(x1 - x0));
    }
    //prophet generated patch
    if (count==true){
    double a;
    }
     //prophet generated patch
    if (count==true){
    this->_M_offset = 0;
    if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
    }
    }
     //prophet generated patch
    if (count==true){
    this->_M_offset = 0;
    if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
    }
    }

     //prophet generated patch
     if (count==true){
     double deltax = x1 - x0;
     }
     //prophet generated patch
     if (count==true){
     double deltay = abs((int)(y1 - y0));
     }
     //prophet generated patch
     if (count==true){
     this->_M_offset = 0;
     float error = 0;
     }
     //prophet generated patch
     if (count==true){
     if (this->_M_offset++ == int(_S_word_bit) - 1) {
         this->_M_offset = 0;
         ++this->_M_p;
     }
     float deltaerr = deltay / deltax;
     }
     //prophet generated patch
     if (count==true){
     double ystep;
     }
     //prophet generated patch
     if (count==true){
     this->_M_offset = 0;
     if (y0 < y1)
         if (count==true){
         this->_M_offset = 0;
         ystep = 1;
     }
          else
            ystep = -1;
     }

     //prophet generated patch
     if (count==true){
     double y = y0;
     }
     //prophet generated patch
     if (count==true){
     if (this->_M_offset++ == int(_S_word_bit) - 1) {
         this->_M_offset = 0;
         ++this->_M_p;
     }
     std::pair<int, int> var;
     }
     for (int x = x0;x < x1;x++)
       {
	 if (steep){
	   var.first=y;
	   var.second=x;
	   line.push_back(var);}
	 else{
	   //prophet generated patch
	   if (count==true){
	   this->_M_offset = 0;
	   var.first = x;
	   }
	   //prophet generated patch
	   if (count==true){
	   if (steep) {
	       var.first = y;
	       var.second = x;
	       line.push_back(var);
	   } else {
	       var.first = x;
	       var.second = y;
	       line.push_back(var);
	   }
	   var.second = y;
	   }
	   //prophet generated patch
	   if (count==true){
	   line.push_back(var);
	   line.push_back(var);
	   }
}
	 //prophet generated patch
	 if (count==true){
	 this->_M_offset = 0;
	 error = error + deltaerr;
	 }
	 //prophet generated patch
	 if (count==true){
	 this->_M_offset = 0;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 }
       }
  }

int main(int argc, char **argv)
{
  //prophet generated patch
  if (count==true){
  this->_M_offset = 0;
  FILE *f = fopen(argv[1], "r");
  }
  //prophet generated patch
  if (count==true){
  this->_M_offset = 0;
  int x, y;
  }
  //prophet generated patch
  if (count==true){
  if (this->_M_offset++ == int(_S_word_bit) - 1) {
      this->_M_offset = 0;
      ++this->_M_p;
  }
  fscanf(f, "%d %d\n", &x, &y);
  }
  //prophet generated patch
  if (count==true){
  fclose((f));
  }
  //prophet generated patch
  if (count==true){
  this->_M_offset = 0;
  DiscreteLine line;
  }
  //prophet generated patch
  if (count==true){
  this->_M_offset = 0;
  buggy(x, y, line);
  }

  for (unsigned i = 0, end = line.size(); i != line.size(); i++)
    printf("%d %d\n", line[i].first, line[i].second);
}
