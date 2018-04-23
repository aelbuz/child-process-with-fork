#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int squareEdge, rectangleEdgeX, rectangleEdgeY;
	int areaSquare, areaRectangle;
	int myPipe[2];

	pipe(myPipe);
	pid_t pid1 = fork();

	if (pid1 > 0) // Parent process
	{
		printf("Enter the square edge: ");
		scanf("%d", &squareEdge);
		
		printf("Enter the rectangle's first edge: ");
		scanf("%d", &rectangleEdgeX);
		
		printf("Enter the rectangle's second edge: ");
		scanf("%d", &rectangleEdgeY);
		
		write(myPipe[1], &squareEdge, sizeof(squareEdge));
		write(myPipe[1], &rectangleEdgeX, sizeof(rectangleEdgeX));
		write(myPipe[1], &rectangleEdgeY, sizeof(rectangleEdgeY));
		
		wait(NULL);
		
		read(myPipe[0], &areaSquare, sizeof(areaSquare));
		read(myPipe[0], &areaRectangle, sizeof(areaRectangle));
		
		if (areaSquare > areaRectangle)
			printf("Difference of areas(Square - Rectangle): %d\n", areaSquare - areaRectangle);
		else
			printf("Difference of areas(Rectangle - Square): %d\n", areaRectangle - areaSquare);
	}

	else // Child process 1
	{
		read(myPipe[0], &squareEdge, sizeof(squareEdge));
		
		areaSquare = squareEdge * squareEdge;
		
		write(myPipe[1], &areaSquare, sizeof(areaSquare));

		if (pid1 == 0) // Child process 2
		{
			read(myPipe[0], &rectangleEdgeX, sizeof(rectangleEdgeX));
			read(myPipe[0], &rectangleEdgeY, sizeof(rectangleEdgeY));
			
			areaRectangle = rectangleEdgeX * rectangleEdgeY;
			
			write(myPipe[1], &areaRectangle, sizeof(areaRectangle));
			
			exit(0);
		}
	}

	return 0;
}