/* !* Color Reduction
 *
 * \description a color reduction which reduces the colors of an image.
 *
 * \author Sebastian Dichler <el16b032@technikum-wien.at>
 *
 * \version Rev.: 01, 10.01.2017 - Created
 *          Rev.: 02, 10.01.2017 - Imported code from color invert program
 *          Rev.: 03, 10.01.2017 - Changing code for color reduction
 *          Rev.: 04, 12.01.2017 - Moved functions and struct into headerfile
 *          Rev.: 05, 12.01.2017 - Filestructure for handin
 *          Rev.: 06, 12.01.2017 - Fixing some bugs
 *
 * \information Works only with ASCII P3 format.
 *              Tested on MacOS Sierra
 *
 */

#include "myHeader.h"

extern void helpdesk_1(void);
extern int removecomment(void);

int main (int argc, char *argv[])
{
    char id[80];
    unsigned int width;
    unsigned int height;
    unsigned int maxColor;
    struct ppmPixel *ppmPixelPointer = NULL;
    int i = 0;
#if DEBUG
    int p = 0;
#endif
    int redChannel = 0;
    int blueChannel = 0;
    int greenChannel = 0;
    
    unsigned long colorreduce = 0;
    char *pEnd;
    
/*------------------------------------------------------------------*/
/* S T A R T   O F   P R O G R A M                                  */
/*------------------------------------------------------------------*/
#if TIME
    time_t start = time(NULL);
#endif
    clear();
    
    if (argc < 4)
    {
        printf(BOLD"\nERROR: Parameter Input ./colinvert INFILE OUTFILE FACTOR [CHANNELS]\n"RESET);
        return -1;
    }
    
    if (strcmp(argv[1], argv[2]) == 0)
    {
        printf(BOLD"\nERROR: Use another output file name, can't overwrite input file\n"RESET);
        return -1;
    }
    
    for (i = 0; i < strlen(argv[3]); i++)
    {
        if (argv[3][i] == '.' || argv[3][i] == ',')
        {
            printf(BOLD"\nERROR: Floating point number not allowed\n"RESET);
            return -1;
        }
    }
    
    if ((argv[3][0] == 'R' || argv[3][1] == 'R' || argv[3][2] == 'R' ||
         argv[3][0] == 'r' || argv[3][1] == 'r' || argv[3][2] == 'r') ||
        (argv[3][0] == 'B' || argv[3][1] == 'B' || argv[3][2] == 'B' ||
         argv[3][0] == 'b' || argv[3][1] == 'b' || argv[3][2] == 'b') ||
        (argv[3][0] == 'G' || argv[3][1] == 'G' || argv[3][2] == 'G' ||
         argv[3][0] == 'g' || argv[3][1] == 'g' || argv[3][2] == 'g'))
    {
        printf(BOLD"\nERROR: Missing FACTOR Number\n"RESET);
        return -1;
    }
    
/* ---- Opening Input file and output file ---- */
    
    pFin = fopen(argv[1], "rb");
    if (pFin == NULL)
    {
        printf(BOLD"\nERROR: Could not open file \"%s\" for reading.\n"RESET, argv[1]);
        return -1;
    }
    
    pFout = fopen(argv[2], "wb");
    if (pFout == NULL)
    {
        printf(BOLD"\nERROR: Could not open file \"%s\" for writing.\n"RESET, argv[2]);
        return -1;
    }
    
/*------------------------------------------------------------------*/
/* S T A R T   O F   P3                                             */
/*------------------------------------------------------------------*/
    
/* ---- Read datas ---- */
    
    fgets(id,80, pFin);
    if (id[0] == 'P' && id[1] == '3' && id[2] == '\n')
    {
        
/* ---- Removing comment between P3 and main informaion ---- */
        
        removecomment();
        
/* ---- Read main information ---- */
        
        fscanf(pFin, "%u", &width);
        fscanf(pFin, "%u", &height);

        removecomment();
        
        fscanf(pFin, "%u", &maxColor);
        
/* ---- Colorreduce above maxColor is not allowed ---- */
        
        colorreduce = strtoul(argv[3],&pEnd,0);
        
        if (colorreduce > maxColor)
        {
            printf(BOLD"\nERROR: Factor not allowed (choose between 0 and %d)\n"RESET, maxColor);
            return -1;
        }
        
/* ---- Removing comment ---- */
        
        removecomment();
        
#if DEBUG
        printf(RED"Width=%u, Hight=%u, MaxColor=%u\n\n"RESET, width, height, maxColor);
#endif
        
/* ---- Allocating memory for picture ---- */
        
        ppmPixelPointer = (struct ppmPixel *)malloc(width*height*sizeof(struct ppmPixel));
        if (ppmPixelPointer == NULL)
        {
            printf(BOLD"\nERROR: Could not get memory.\n"RESET);
            return -1;
        }
        
/* ---- Read Pixels ---- */
        
        printf("\n");
        printf(BOLD"Importing file...\n"RESET);
        
        for (i = 0; i < width*height; i++)
        {
            removecomment();
            
            fscanf(pFin, "%u", &(ppmPixelPointer+i)->r);
            fscanf(pFin, "%u", &(ppmPixelPointer+i)->g);
            fscanf(pFin, "%u", &(ppmPixelPointer+i)->b);
            
#if DEBUG
            p++;
            printf(RED"Pixel [%d] %u\t %u\t %u\n"RESET, p,
                   (ppmPixelPointer+i)->r, (ppmPixelPointer+i)->g, (ppmPixelPointer+i)->b);
#endif
        }
        
        printf(BOLD"Done file import!\n\n"RESET);
        
/* ---- Check user input for inversion ---- */
        
        if (argc == 4)
        {
            redChannel = 1;
            blueChannel = 1;
            greenChannel = 1;
        }
        
        if (argc == 5)
        {
            if (argv[4][0] == 'R' || argv[4][1] == 'R' || argv[4][2] == 'R' ||
                argv[4][0] == 'r' || argv[4][1] == 'r' || argv[4][2] == 'r')
            {
                redChannel = 1;
            }
            if (argv[4][0] == 'B' || argv[4][1] == 'B' || argv[4][2] == 'B' ||
                argv[4][0] == 'b' || argv[4][1] == 'b' || argv[4][2] == 'b')
            {
                blueChannel = 1;
            }
            if (argv[4][0] == 'G' || argv[4][1] == 'G' || argv[4][2] == 'G' ||
                argv[4][0] == 'g' || argv[4][1] == 'g' || argv[4][2] == 'g')
            {
                greenChannel = 1;
            }
        }
        
/* ---- Color reduction ---- */
        
        printf(BOLD"Reducing colors...\n"RESET);
        
        for (i = 0; i < width*height; i++)
        {
            
/* ---- If there is no colorreducement set ---- */
            
            if (colorreduce == 0)
            {
                if (redChannel == 1)
                {
                    (ppmPixelPointer+i)->r = (ppmPixelPointer+i)->r;
                }
                if (greenChannel == 1)
                {
                    (ppmPixelPointer+i)->g = (ppmPixelPointer+i)->g;
                }
                if (blueChannel == 1)
                {
                    (ppmPixelPointer+i)->b = (ppmPixelPointer+i)->b;
                }
            }
            
/* ---- Else check if color = maxColor and colorreduce = maxColor ---- */
/* ---- Else write new color with old color - modulo ---- */
            
            else
            {
                if (redChannel == 1)
                {
                    if ((ppmPixelPointer+i)->r == maxColor && colorreduce == maxColor)
                    {
                        (ppmPixelPointer+i)->r = 0;
                    }
                    else
                    {
                        (ppmPixelPointer+i)->r = (ppmPixelPointer+i)->r - ((ppmPixelPointer+i)->r)%colorreduce;
                    }
                }
                if (greenChannel == 1)
                {
                    if ((ppmPixelPointer+i)->g == maxColor && colorreduce == maxColor)
                    {
                        (ppmPixelPointer+i)->g = 0;
                    }
                    else
                    {
                        (ppmPixelPointer+i)->g = (ppmPixelPointer+i)->g - ((ppmPixelPointer+i)->g)%colorreduce;
                    }
                }
                if (blueChannel == 1)
                {
                    if ((ppmPixelPointer+i)->b == maxColor && colorreduce == maxColor)
                    {
                        (ppmPixelPointer+i)->b = 0;
                    }
                    else
                    {
                        (ppmPixelPointer+i)->b = (ppmPixelPointer+i)->b - ((ppmPixelPointer+i)->b)%colorreduce;
                    }
                }
            }
        }
        
        printf(BOLD"Done reducing colors!\n\n"RESET);
        
/* ---- Write header into file output ---- */
        
        printf(BOLD"Writing file...\n"RESET);
        
        fprintf(pFout, "P3\n");
        fprintf(pFout, "#Color Reduction by Sebastian Dichler\n");
        fprintf(pFout, "%u %u\n", width, height);
        fprintf(pFout, "%u\n", maxColor);
        
/* ---- Write pixels into output file ---- */
        
        for (i = 0; i < width*height; i++)
        {
            fprintf(pFout, "%u %u %u\n",
                    (ppmPixelPointer+i)->r, (ppmPixelPointer+i)->g, (ppmPixelPointer+i)->b);
        }
        
        printf(BOLD"Done writing output file!\n"RESET);
    }
    
/* ---- Unknown Dataformat (not P3) ---- */
    
    else
    {
        printf(BOLD"\nERROR: Wrong Dataformat...\n"RESET);
        return -1;
    }
    
    free(ppmPixelPointer);
    fclose(pFin);
    fclose(pFout);
#if TIME
    printf("Ellapsed time: "BOLD"%.2f"RESET" sec\n", (double)(time(NULL) - start));
#endif
    
    return 0;
}
