#include "fillit.h"
#include <fcntl.h>

/*
    Prototype: int parseTetri(t_block *b, int y, char* str)
    Description: partseTetri, pareses a string for '.' && '#'.
                If the '#' are found, they are recorded in t_block.x
                && t_block.y position. A 0 is returned if the line is empty.
                A -1 is returned if the format is bad. A 1 is returned if success.
*/
/* This is how the blocks ar gathered and made in the beginning of main */
int parseTetri(t_block *blocks, int *i, int *y, char* str)
{
    int x;

    x = 0;
    if (str == NULL)
    {
        printf("There is nothing here!\n");
        return (0);
    }
    if (ft_strlen(str) % 4 != 0)
        return (-1);
    while (str[x])
    {
        /* Check if '#' point is reached,
            Record x position 0 - 3
            Record y position from outside func.
            i iterates to next block if multiple '#' points are found.
        */
        if (str[x] == '#')
        {
            blocks->point[*i].x = x;
            blocks->point[*i].y = *y;
            *i += 1;
        }
        else if (str[x] != '.')
            return (-1);
        x++;
    }
    return (1);
}

/*
    prototype int   checkDim(t_block block)
    Description: checkDim, checks the dimensionality of a block.
                If a block is 1x4, 2x2, or 2x3 and the reverse of those.
                If a block is valid or not, i.e. no 4x2, 3x5, etc.
                Will also set the dimensions of a block. 
                returns 1 if good, 0 if bad.
*/
/* IM PRETTY SURE THIS IS NOT NECESSARY */
int     checkDim(t_block *block)
{
    int i;
    int k;
    int x_min;
    int x_max;
    int y_min;
    int y_max;

    i = 0;
    k = 0;
    x_min = block->point[0].x;
    y_min = block->point[0].y;
    x_max = block->point[0].x;
    y_max = block->point[0].y;
    while (i < 4)
    {
        if (block->point[i].x > x_max)
            x_max = block->point[i].x;
        if (block->point[i].x < x_min)
            x_min = block->point[i].x;
        if (block->point[i].y > y_max)
            y_max = block->point[i].y;
        if (block->point[i].y < y_min)
            y_min = block->point[i].y;
        i++;
        if (x_min < 0 || x_max > 3)
            return (0);
    }
   // printf("XMax Dimemsion: %d XMIN Dimension: %d, YMax Dimension: %d, YMin Dimension: %d\n", x_max, x_min, y_max, y_min);
    /* normalize the points such that at least 1 is min 0, for simpler placing */
    if (x_min > 0)
    {
        i = 0;
        while (i < 4)
            block->point[i++].x -= x_min;
    }
    if (y_min > 0)
    {
        i = 0;
        while (i < 4)
            block->point[i++].y -= y_min;
    }
        block->x_dim = (x_max - x_min) + 1;
        block->y_dim = (y_max - y_min) + 1;
    return(1);
}

/*
    prototype void printAll(t_point finalDim, int mapDim)
    Description: printAll takes the maximum dimension of the map.,
                and the worst case scenario 
*/
/* IM PRETTY SURE I DONT USE THIS */
void    printAll(char map[17][17], int mapDim)
{
    t_point dim;
    int x;
    int y;
    x = 0;
    y = 0;
    dim.x = 0;
    dim.y = 0;
    while ( y < mapDim)
    {
        while (x < mapDim)
        {
            if (map[y][x] != '.' && (y >= dim.y))
                dim.y = y;
            if (map[y][x] != '.' && (x >= dim.x))
                dim.x = x;
            
            x++;
        }
        x = 0;
        y++;
    }
    x = 0;
    y = 0;
    if ( dim.x >= dim.y )
        dim.y = dim.x;
    else
        dim.x = dim.y;
    while (y < dim.y + 1)
    {
        while (x < dim.x + 1)
        {
            ft_putchar(map[y][x]);
            x++;
        }
        ft_putchar('\n');
        x = 0;
        y++;
    }
}
/*   
    prototype void    printMapPls(map, sizeofmap)
    Description:    Will print the map for the given size.
*/
void    printMapPls(char map[17][17], int mapSize)
{
    int k = 0;
    for(int i = 0; i < mapSize; i++)
    {
        for (k = 0; k < mapSize; k++)
        {
            ft_putchar(map[i][k]);
        }
        ft_putchar('\n');
       k = 0;
    }
	printf("---------\n");
}

/*
    prototype void  initMap(char **map, int dim)
        initMap takes a pointer to a map, initializes the map to a series of '.''s
*/
void  initMap(char map[17][17], int dim)
{
    printf("Map size: %d\n", dim);
    int x;
    int y;

    x = 0;
    y = 0;
    while(y < dim)
    {
        while(x < dim)
        {
            map[y][x] = '.';
            ft_putchar(map[y][x]);
            x++;
        }
        ft_putchar('\n');
        map[y][x] = '\0';
        x = 0;
        y++;
    }
}
/*
    protoype  int   placePoint(char **map, t_block *blocks, t_point mapPoints, int index)
            Description:  Tries to recursively place a point based on the mapPoints location in the map.
                        Points are placed till the either the block is finished or a spot is not available.
                        If a spot is not free the function is called again with a change in first the x position, then the y position.
                        Should not happen but if a spot just cannot befound a critical error is thrown.
*/
int         checkMap(char map[17][17], t_block block, int mapSize, int l, t_point *lastPlace, int v)
{
    int k = 0;
    int ox = block.ox;
    int oy = block.oy;
    int x;
    x = 0;
    int y; 
    y = 0;
    int i = 0;
    char *lett = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    //I subbed one and added one places this could be fixed.
    mapSize -= 1;
    //checks if map is too small returns -1
    if (block.point[0].y > mapSize || \
    block.point[0].x > mapSize || \
    block.point[1].y > mapSize || \
    block.point[1].x > mapSize || \
	block.point[2].y > mapSize || \
    block.point[2].x > mapSize || \
    block.point[3].y > mapSize || \
    block.point[3].x > mapSize)
		return (-1);
    
    while (y < mapSize + 1)
    {
        while (x < mapSize + 1)
        {  
            /* MUCH LIKE THE UNDO THIS IS THE PLACEMENT */
            //bascially iterates through map addes x + offset + inherent value of block checks if map is free and places 
            //does this for all four points, if all four are free it places it at once else keeps going and returns 0 if nothing could be placed.
            if ((map[y + block.point[0].y + oy][x + block.point[0].x + ox] == '.')&& \
                (map[y + block.point[1].y + oy][x + block.point[1].x + ox] == '.')&& \
			    (map[y + block.point[2].y + oy][x + block.point[2].x + ox] == '.')&& \
                (map[y + block.point[3].y + oy][x + block.point[3].x + ox] == '.')&& 
                y + block.point[0].y + oy <= mapSize && y + block.point[1].y + oy <= mapSize \
                && y + block.point[2].y + oy <= mapSize && y + block.point[3].y + oy <= mapSize \
                && x + block.point[0].x + ox <= mapSize && x + block.point[1].x + ox <= mapSize \
                && x + block.point[2].x + ox <= mapSize && x + block.point[3].x + ox <= mapSize)
            {
                lastPlace->x = x + block.point[0].x + ox;
                lastPlace->y = y + block.point[0].y + oy;
                if (!v){
                map[y + block.point[3].y + oy][x + block.point[3].x + ox] = lett[l];
                map[y + block.point[2].y + oy][x + block.point[2].x + ox] = lett[l];
                map[y + block.point[1].y + oy][x + block.point[1].x + ox] = lett[l];
                map[y + block.point[0].y + oy][x + block.point[0].x + ox] = lett[l];
                }

                l++;

                k = 0;
                while ( i <= mapSize)
                {
                    while (k <= mapSize)
                    {
                        ft_putchar(map[i][k]);
                        k++;
                    }
                    ft_putchar('\n');
                    k = 0;
                    i++;
                }   
				printf("------------------------\n");
                return (1);
            }
           x++;
        }
        x = 0;
        y++;
    }
	
     if (block.point[3].y >= y || block.point[3].x >= y)
         return (-1);
    return (0);
}


int         checker(char map[17][17], t_block block, int mapSize, t_point *lastPlace, int i)
{

    int x;
    x = 0;
    int y; 
    y = 0;
    char *lett = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    mapSize -= 1;
    printf("we are currently checking fabulous block #: %d, of letter: %c\n", i, lett[i]);
    while (y < mapSize + 1)
    {
        while (x < mapSize + 1)
        {  
           //checks if all points are free to be placed and actaully does the undoing.     
            if ((map[y + block.point[0].y][x + block.point[0].x] == lett[i])&& \
                (map[y + block.point[1].y][x + block.point[1].x] == lett[i])&& \
			    (map[y + block.point[2].y][x + block.point[2].x] == lett[i])&& \
                (map[y + block.point[3].y][x + block.point[3].x] == lett[i])&& 
                y + block.point[0].y <= mapSize && y + block.point[1].y <= mapSize \
                && y + block.point[2].y <= mapSize && y + block.point[3].y <= mapSize \
                && x + block.point[0].x <= mapSize && x + block.point[1].x <= mapSize \
                && x + block.point[2].x <= mapSize && x + block.point[3].x <= mapSize)
            {
                lastPlace->x = x + block.point[0].x;
                lastPlace->y = y + block.point[0].y;
/* THIS IS THE UNDOING METHOD LOL */
                map[y + block.point[0].y][x + block.point[0].x] = '.';
                map[y + block.point[1].y][x + block.point[1].x] = '.';
			    map[y + block.point[2].y][x + block.point[2].x] = '.';
                map[y + block.point[3].y][x + block.point[3].x] = '.';
                return (1);
            }
           x++;
        }
        x = 0;
        y++;
    }
    return (0);
}
/*
    prototype void redo(t_block block, int x, int y)
    Description: increases the x or y value of a block given an undo transformation.
*/
void     redo(t_block *block, int x, int y)
{  
    if( x > y)
        block->ox += 1; //each block has a ox and oy value for offseting.
    else
    {
        block->ox = 0;
        block->oy += 1;
    }
}   

int     undo(t_block *block, char map[17][17], int x, int y, int mapMax)
{

            x = (x - block->point[0].x) + block->point[3].x; //This is the farthest right position 
            y = (y - block->point[0].y) + block->point[3].y;//basically checks if the block can be undo, 
            //because it will either move to the right or down, if the map is too small
            //or if theere is another block in the way it should not undo.
            if (x < mapMax - 1 && map[y][x + 1] == '.')
                redo(block, 1, 0); //redo is the new method of shifting a block right or down
            else if (y < mapMax - 1 && map[y + 1][x] == '.')
                redo(block, 0, 1);   
            else
                return (0);
        
            return (1);
}
/*
    prototype void  tetriMap(char **map, t_block *blocks, int mapMax)
    Description: for the dimensions of the map, each block is paced in the left upper
        most position. If a position is unavailable then it is tansformed in the x direction 
        checks spacing and repeats until it can be placed.
*/
void    tetriMap(char map[17][17], t_block *blocks, int index, int mapMax, int i, t_point lastPlace)
{    
	int ret = 0; 
    int v = 0;
    int x = lastPlace.x;
    int y = lastPlace.y;

    /* Iterate through the blocks until we reach the last block */
    while ( i != index) 
    {
        /* v basically allows us to check the last place of a map without placing */
        v = 0;
        if ((ret = (checkMap(map, blocks[i], mapMax, i, &lastPlace, v))) > 0)
            i++;
		else if (ret == -1 )
            mapMax++; //checkMap will return -1 if the block its trying to place is to big for the map.
        else //This else triggers if we could not place the point easily in checkMap
        {
            v = 1; //We set V to 1 as we are now evaulating where can place the next block.
            i--; //We move down with the i value as to undo all points up to the last point.
            //Once the last point is reach we move forward by pushing the fist point to the right or down if we can.
            //This is not a perfect method because we need to keep the first blocks in our stack i.e. ABC further to the left
            //And up as possible with blocks DEF etc. should following more towards the bottom right. 
            //To get this result a edit must be made in this algorthim.
            //Instead of moving the first block to the right first, we should move the last placed block first. 
            //Try to place after that.
            checker(map, blocks[i], mapMax, &lastPlace, i); //Checker checks if we can place a block and assigns those coordinates to lastplace.
            x = lastPlace.x;
			y = lastPlace.y;
            while(!undo(&blocks[i], map, x, y, mapMax)) //While undo is returning a negative result, it undos each placement.
            {
                //changing this algorthim could fix the code if instead of undoing while negative it just undid until something moved.
                v = 1;
                i--;
                checker(map, blocks[i], mapMax, &lastPlace, i);
                x = lastPlace.x; //undo removes points based on lastplace.
                y = lastPlace.y;
                if (i < 0) //i will eventually reach a - number this is how we know we got to the endo of the blocks
                {
                    mapMax++; //mapmax increases because we kepting undoing and were never able to place.
                    i = 0;
                    break;
                }
            }
            printMapPls(map,mapMax); //this prints the map.
        }
    }
}


/*
    prototype void  printMap(int index)
	3

    Description: printMap, takes the index of the counted blocks and
                creates the maximum map size for those blocks.
    SOON
        will places pieces onto map if space is free.
*/
void    printMap(int index, t_block *blocks)
{
    int i; //this iterates through the while loop of the index
    int dimMap; //saves maximum dimension of map.
    char map[17][17]; //This is the map we will be printint out. living on the stack.
	t_point lastpoint; //saves the last point we placed something.
	lastpoint.x = 0; //init last point, you have to init these a function would be useful.
	lastpoint.y = 0; //init last point
    t_point dimPrint; //another point saving the dimensions to print, but is a bit redudant based on how we solve the map now.
    dimMap = 2; //initial dim of map should always be 2.
    dimPrint.x = 0; //init dimprint
    dimPrint.y = 0; //init dimprint
    i = 0; //init i
    
    /*  This whole while loop basically checks for the maximum dimensions by doing some arguably incorrect matrix math */
    /*  Have not tried yet, but it is probably not necessary */
    while (i < index)
    {
        /* This set of if statements finds space taken of tetris blocks */
        if (dimPrint.x <= dimPrint.y)
        {
            dimPrint.x += blocks[i].x_dim;
            if (blocks[i].y_dim > dimPrint.y || dimPrint.y== 0)
                dimPrint.y = blocks[i].y_dim;
        }
        else
        {
            dimPrint.y += blocks[i].y_dim;
            if (blocks[i].x_dim > dimPrint.x || dimPrint.x == 0)
                dimPrint.x = blocks[i].x_dim;

        }
        i++;
    }
    if (dimPrint.x > dimPrint.y)
        dimMap = dimPrint.x;
    else
        dimMap = dimPrint.y;
    /* This marks the end of the uncessary thing above */

    i = 0;
    initMap(map, 17); //init the map, with a bunch of '.'s 
    tetriMap(map,blocks, index, 2, i, lastpoint); //This is the thing we have been working for the past couple weeks.

}
/*
    Main: The beginning of the program.
    0. 
*/

int main()
{
    /* Variables */ 
    int fileDesc;   // The fil descriptor.
    char *temp;     // String for reading lines with Get Next Line.
    int y;          // y keeps track of the y value per each block read.
    int index;      // index is the number of blocks read.
    int i = 0;      // i is a temporary number for tracking count of points in a block.
    int ret;        // ret is the returned valued saved as an integer. used for checking errors.
    int d_n = 0;    // d_n was a number for quickly checking if a double \n character was encountered.
    ret = 1;        // ret is initialized to a positive to start.
    y = 0;
    index = 0;
    
    t_block blocks[26];   //Blocks is the storage location for every block in this program. it is initialized on the stack.
    /* Open file for testing */
   fileDesc = open("test", O_RDONLY);   
    /* Check file if file is valid. */
    if (fileDesc <= 0) 
    {
            printf("The file provided is invalid\n");
            return (0);
    } 
    /* Print the file iteratively */
    while (get_next_line(fileDesc,&temp))
    {
        /* Checks if the the string is of line size 4 for a block */
        if (ft_strlen(temp) == 4)
        {
            /*  essentially if y == 4, increase index to next block*/
            if (y % 4 == 0 && y != 0)
            {
                i = 0;
                y = 0;
                index++;
            }
            /*  parseTetri reads the line addes the points found and returns -1 if it finds an error in format. */
            ret = parseTetri(&blocks[index], &i, &y, temp);
            /* Basically checking the result of our parsed read */
            if (ret < 0)
            {
                printf("Bad news bears this is busted! \n");
                return (0);
            }
            /*  This only occurs if its a new line or at End Of File */
            if (ret == 0)
                break;
            printf("Done %d\n",y);
            /* since each line represents a movment in the y axis increment y */
            y++;
            /* we saw no double new lines so this is zero */
            d_n = 0;
        }
        else if(ft_strlen(temp) == 0 && d_n == 0) //this is a really dumb way to check for a new line.
        {
            printf("New Line\n");
            d_n = 1;
        }
        else //This only occurs if d_n = 1, this happens with 
            return (0);

    }
    if (index == 0 && d_n == 1) //so this is only one block and the file ended with a \n. this is bad format.
    {
        printf("Bad format\n");
        return (0);
    }
    else if (index == 0 && d_n == 0) //this would be one block with no \n. if block count > 1 a \n at the end is required
        printf("Good format\n");
   
    printf("index: %d\n", index);
    index++;
    for (int k = 0; k < index; k++) //This can be changed to a while loop.
    {
        if (!checkDim(&blocks[k])) //This checks the dimensions of each block, which may be redundant now.
        {
            printf("Error Bad formatting! Shame on you!\n");
            return (0);
        }
    }
    close(fileDesc); //closes the file read.
    printMap(index, blocks); //This moves into placing things on the map, we send the index and blocks to it.
    return (0);
}