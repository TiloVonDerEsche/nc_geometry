#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main() {
    // filename und Zeichenpuffer definieren
    char line[1000];
    char filePath[] = "test.mpf";

    // file zum Lesen öffnen
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("An error occured, while trying to open a file");
        return 1;
    }  


    int laser_on_off = 0;
    float machine_speed = 0.0;

    size_t mpf_lines = 205000;
    double* cords = malloc(mpf_lines * 4 * sizeof(double));

    int i = 0;
    for(int i = 0; i < mpf_lines; i++)
    {
        cords[i * 4] = 0;
        cords[i * 4 + 1] = 0;
        cords[i * 4 + 2] = 0;
        cords[i * 4 + 3] = 0;
    }


    if (cords == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    


    char format[] = " %*[^-0123456789]%lf"; 

    double number = 0;

    double new_x = 0;
    double new_y = 0;
    double new_z = 0;

    int x_has_changed = 0;
    int y_has_changed = 0;
    int z_has_changed = 0;

    //int i = 0;

    printf("Reading file with path: %s\n",filePath);
    //////////////////////////////////////////////////////////////////////////////////
    //--------------------------------Read-Loop-------------------------------------//
    //////////////////////////////////////////////////////////////////////////////////

    while (fgets(line, sizeof(line), file) != NULL) {
        
        x_has_changed = 0;
        y_has_changed = 0;
        z_has_changed = 0;

        //read the on/off state of the laser
        if (strstr(line, "G00") != NULL)
        {
            laser_on_off = 0;
        }
        else if (strstr(line, "G01") != NULL)
        {
            laser_on_off = 1;
        }

        //read the set machine speed
        if (strstr(line, "VIT_TIR=") != NULL)
        {

            //sscanf returns 1 when it successfully parses one item based on the format specifier provided.
            if (sscanf(strstr(line, "VIT_TIR="), format, &number) == 1)
            {
                machine_speed = number;
            }
        }

        //ignore comments
        if (strstr(line, ";") != NULL)
        {
            continue;
        }

        if (strstr(line, " X") != NULL)
        {

            //sscanf returns 1 when it successfully parses one item based on the format specifier provided.
            if (sscanf(strstr(line, " X"), format, &number) == 1)
            {
                new_x = number;
                x_has_changed = 1;
            }
        }

        if (strstr(line, " Y") != NULL)
        {

            //sscanf returns 1 when it successfully parses one item based on the format specifier provided.
            if (sscanf(strstr(line, " Y"), format, &number) == 1)
            {
                new_y = number;
                y_has_changed = 1;
            }
        }

        if (strstr(line, " Z") != NULL)
        {
            
            //sscanf returns 1 when it successfully parses one item based on the format specifier provided.
            if (sscanf(strstr(line, " Z"), format, &number) == 1)
            {
                new_z = number;
                z_has_changed = 1;
            }
        }
            
        if (x_has_changed == 1 || y_has_changed == 1 || z_has_changed == 1)
        {
            //copy the last point
            for (int j = 0; j < 4; j++) 
            {
                cords[i * 4 + j] = cords[(i-1) * 4 + j];
            }


            //only change the parameters that have changed
            if (x_has_changed == 1)
            {
                cords[i * 4] = new_x;
            }
            if (y_has_changed == 1)
            {
                cords[i * 4 + 1] = new_y;
            }
            if (z_has_changed == 1)
            {
                cords[i * 4 + 2] = new_z;
            }

            cords[i * 4 + 3] = laser_on_off;

            i += 1;
        }

    }

    //close the mpf file
    fclose(file);



    //negitive values don't make sense for these counting variables.
    unsigned int zero_counter = 0;
    unsigned int cords_length = 0;
    ///////////////////////////////////////////////////////
    //----------Get the actual length of cords-----------//
    ///////////////////////////////////////////////////////

    unsigned int j;
    for(j = 0; j < mpf_lines; j++) 
    {

        if (cords[j * 4] == 0 && cords[j * 4 + 1] == 0 && cords[j * 4 + 2] == 0 && cords[j * 4 + 3] == 0)
        {
            zero_counter++;
        }
        else
        {
            zero_counter = 0; 
        }

        if (zero_counter > 4)
        {
            break;
        }

        // printf("P%d(%f, %f, %f)\n",j, cords[j * 4], cords[j * 4 + 1], cords[j * 4 + 2]);
    }
    cords_length = j - 4;
    ///////////////////////////////////////////////////////

    //printf("%d",cords_length);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //----------Simulate the machine and note the position at every timestep of 0.002 seconds and write to the csv------------//
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Point -1
    double xn1 = 0;
    double yn1 = 0;
    double zn1 = 0;

    //Point 0
    double x0 = 0;
    double y0 = 0;
    double z0 = 0;

    //Point 1
    double x1 = 0;
    double y1 = 0;
    double z1 = 0;

    

    //last vector (Vector from P-1 to P0)
    double lvx = 0;
    double lvy = 0;
    double lvz = 0;

    //current vector (Vector from P0 to P1)
    double vx = 0;
    double vy = 0;
    double vz = 0;

    double length_of_last_vector = 1;

    double length_of_curr_vector = 1;

    //dot_product between last and current vector is used to determine their respective angle
    double dot_product = 0;

    double angle_last_curr = 0;


    double time = 0;
    double temp_time = 0;

    //for some reason does the precision dictate the number of lines in the csv
    double precision = 100000;

    //determine when the machine slows down
    double angle_threshold = 75.0;

    //~boolean variable, that is 1 when the angle_last_curr exceeds the angle_threshold and otherwise 0
    unsigned int critical_area = 0;


    //Prepare the output csv
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    FILE* csvFile = fopen("trace_output.csv", "w"); // or "a" to append

    if (csvFile == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    // Write the header row of the csv
    fprintf(csvFile, "Time in seconds, Laser On/Off, Angle Between Last and Current Vector, P-1, P0, P1, last vector(P-1 to P0), length last vector, current vector(P0 to P1), length current vector, dot_product, (length_of_last * length_of_curr), angle between last and current vector \n"); 
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    for(int i = 0; i < cords_length - 1; i ++)
    {
       if (i > 0) 
       {
            //point-1
            xn1 = cords[(i-1) * 4];
            yn1 = cords[(i-1) * 4 + 1];
            zn1 = cords[(i-1) * 4 + 2];
       } //xn1, yn1, zn1 = 0; for i = 0 


        //point0
        x0 = cords[i * 4];
        y0 = cords[i * 4 + 1];
        z0 = cords[i * 4 + 2];

        //point1
        x1 = cords[(i+1) * 4];
        y1 = cords[(i+1) * 4 + 1];
        z1 = cords[(i+1) * 4 + 2];


        //last vector from point-1 to point0
        lvx = x0 - xn1;
        lvy = y0 - yn1;
        lvz = z0 - zn1;

        //current vector from point0 to point1
        vx = x1 - x0;
        vy = y1 - y0;
        vz = z1 - z0;

        length_of_last_vector = sqrt(((lvx) * (lvx)) + ((lvy) * (lvy)) + ((lvz) * (lvz)));

        length_of_curr_vector = sqrt(((vx) * (vx)) + ((vy) * (vy)) + ((vz) * (vz))); // 1/1000 of the length of the vector

        //last_vector * current_vector
        dot_product = lvx * vx + lvy * vy + lvz * vz;
      
        //the angle between the last and current vector
        angle_last_curr = acos(dot_product / (length_of_curr_vector * length_of_last_vector))* (180 / M_PI); //convert the radiant result to degrees

        if (angle_last_curr > angle_threshold)
        {
            critical_area = 1;
        }
        else
        {
            critical_area = 0;
        }

        laser_on_off = cords[i * 4 + 3];
        

        // printf("%d.: lv(%f, %f, %f), v(%f, %f, %f)\n",i, lvx, lvy, lvz, vx, vy, vz);

        //While point_1 isn't reached go towards it.
        //TODO round the cords

        while((round(x0 * precision) / precision) != (round(x1 * precision) / precision) || (round(y0 * precision) / precision) != (round(y1 * precision) / precision) || (round(z0 * precision) / precision) != (round(z1 * precision) / precision))
        {
            
            

            //Go 1/1000 step on the vector to point1
            //TODO uncomment "/ precision"
            x0 += vx; //precision;
            y0 += vy; //precision;
            z0 += vz; //precision;

            // printf("vector(%f,%f,%f))\n",vx/1000,vy/1000,vz/1000);
            // printf("p0(%f,%f,%f),\np1(%f,%f,%f)\n",x0,y0,z0, x1,y1,z1);

            
            //time counts the overall passed time.
            time += length_of_curr_vector / machine_speed * 60; //in seconds (t = s/v)

            //temp_time counts to 0.002 and then gets reset to 0, so that an entry is being made every 0.002 seconds.
            temp_time += length_of_curr_vector / machine_speed * 60;

            if (temp_time > 0.002)
            {
                // printf("%f",temp_time);
                //write a row to the csv
                fprintf(csvFile, "time= %f, %d, P-1(%f,%f,%f), P0(%f,%f,%f), P1(%f,%f,%f), lv(%f,%f,%f), %f, v(%f,%f,%f), %f, dotP= %f, lenP= %f, angle= %f, critical?=%d\n",time,laser_on_off,xn1,yn1,zn1,x0,y0,z0,x1,y1,z1,lvx,lvy,lvz,length_of_last_vector,vx,vy,vz,length_of_curr_vector, dot_product, (length_of_curr_vector*length_of_last_vector),angle_last_curr,critical_area);

                //reset temp_time so it can count to 0.002 again
                temp_time = 0;
            }


            //printf("%s\n",(round(x0 * precision) / precision) != (round(x1 * precision) / precision) && (round(y0 * precision) / precision) != (round(y1 * precision) / precision) && (round(z0 * precision) / precision) != (round(z1 * precision) / precision) ? "true" : "false");
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    fclose(csvFile);
    free(cords);


    return 0;
}