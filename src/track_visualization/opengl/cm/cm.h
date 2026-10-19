#include "color_mapper.tab.c"
extern int debug;
extern FILE* yyin;

int compile_color_config(FILE* fp) {
  // 1. COMPILE PHASE: Parse ONCE from stdin
  yyin=fp;
  if (yyparse() == 0 && rootAST) {
      std::cout << "\n--- Generated Abstract Syntax Tree (AST) ---\n";
      rootAST->print();
      std::cout << "---------------------------------------------\n\n";
      return SUCCESS;
  }
  else {
      std::cerr << "Fehler beim Parsen des Ausdrucks.\n";
      return FAILURE;
  }
}

void map_color(Track* t) {
  // Traverse linked list starting from rootAST (index 0)
  MappingNode* current = dynamic_cast<MappingNode*>(rootAST.get());
  bool matched = false;
  double result = 0;

  while (current != nullptr) {
      // evaluate() checks the bool expression and mutates t->color if true, returning > 0
      result = current->evaluate(t);
      if (result != 0) {
          matched = true;
          break; // Stop at the first successfully mapped rule
      }
      current = current->next.get();
  }

  if(debug) {
    std::cout << "Input Track=\n"
    "t_id=" << t->id << "\n" <<
    "t_start=(" << t->start_x << ", " << t->start_y << ", " << t->start_z << ")\n" <<
    "t_end=(" << t->end_x << ", " << t->end_y << ", " << t->end_z << ")\n" <<
    "t_b=" << t->b << " t_c=" << t->c << "\n" <<
    "machine_speed=" << t->machine_speed << "\n" <<
    "laser_power=" << t->laser_power << " laser_status=" << t->laser_status << "\n" <<
    "g_code=" << t->g_code << "\n" <<
    "color=(" << (int)t->color.r << ", " << (int)t->color.g << ", " << (int)t->color.b << ")\n" <<
    "t_hrad=" << t->hradius << " t_vrad=" << t->vradius << "\n" <<
    " ---> Result = " << result << "\n" <<
    " ---> Matched successfully? = " << (matched ? "YES" : "NO") << "\n" <<
    "---------------------------\n\n";
  }
}


/*
int main() {
  Track t1 = (Track){1,         //id
             2.3, 11.2, -7.9,   //start
             5.12, 17.10, -0.9, //end
             20,350.9,          //b,c
             200, 1750,         //machine_speed, laser_power
             1,                 //laser_status
             0,                 //g_code
             (Color){66,77,88},
             0.5,0.5
            };
  Track t2 = (Track){2,         //id
             2.3, 11.2, -7.9,   //start
             5.12, 17.10, -0.9, //end
             40,350.9,          //b,c
             200, 1750,         //machine_speed, laser_power
             1,                 //laser_status
             0,                 //g_code
             (Color){66,77,88},
             0.5,0.5
            };
  Track t3 = (Track){3,         //id
             2.3, 11.2, -7.9,   //start
             5.12, 17.10, -0.9, //end
             60,350.9,          //b,c
             200, 1750,         //machine_speed, laser_power
             0,                 //laser_status
             0,                 //g_code
             (Color){66,77,88},
             0.5,0.5
            };
  Track t4 = (Track){4,         //id
             2.3, 11.2, -7.9,   //start
             5.12, 17.10, -0.9, //end
             0,0,          //b,c
             200, 1750,         //machine_speed, laser_power
             1,                 //laser_status
             0,                 //g_code
             (Color){66,77,88},
             0.5,0.5
            };

        const char* fname = "color_config.ctxt";
        FILE* colorf = fopen(fname, "r");
        if(compile(colorf) == SUCCESS) {

          // 2. RUNTIME PHASE: Execute the AST multiple times
          //TODO Read a track in, return mapped Color

          eval(t1);
          eval(t2);
          eval(t3);
          eval(t4);
        }

    return 0;
}*/
