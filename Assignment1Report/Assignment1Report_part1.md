1.bresenham algorithm code:
 
void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	// TODO: Implement bresenham algorithm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	
	int x;
	int y;
	int e;
	int max_x; 
	int reflectFlag;
	int deltaX, deltaY;

	if (p1.x > p2.x) deltaX = p1.x - p2.x;
	else deltaX = p2.x - p1.x;

	if (p1.y > p2.y) deltaY = p1.y - p2.y;
	else deltaY = p2.y - p1.y;

	bool switchFlag = false;
	
	if (deltaX > deltaY) 
	{
		if (p2.x > p1.x)
		{
			x = p1.x;
			y = p1.y;
			max_x = p2.x;
			
			if (p2.y > p1.y) reflectFlag = 1;
			else reflectFlag = -1;
		}
		else 
		{ //(p1.x >= p2.x)
			x = p2.x;
			y = p2.y;
			max_x = p1.x;
			
			if (p1.y > p2.y) reflectFlag = 1;
			else reflectFlag = -1;
		}
	}
	else 
	{
		int tmpNum;
		 
		if(p2.y > p1.y)
		{
			x = p1.y;
			y = p1.x;
			max_x = p2.y;
			
			if (p2.x > p1.x) reflectFlag = 1;
			else reflectFlag = -1;
		}
		else 
		{
			x = p2.y;
			y = p2.x;
			max_x = p1.y;
			
			if (p1.x > p2.x) reflectFlag = 1;
			else reflectFlag = -1;
		}
		tmpNum = deltaX;
		deltaX = deltaY;
		deltaY = tmpNum;
		switchFlag = true;
	}
	
	e = 2 * deltaY - deltaX;
	
	while (max_x >= x)
	{
		
		if (switchFlag) PutPixel(y, x, color);
		else PutPixel(x, y, color);

		if (e > 0)
		{
			y += reflectFlag;
			e -= 2 * deltaX;
		}
		e += 2 * deltaY;
		x++;
	}


	
}

2. picture of the sanity check results:

   https://user-images.githubusercontent.com/63167182/105028914-55ebb800-5a5a-11eb-96b0-60e202f1c5db.png

3. drawing of new obj:

   https://user-images.githubusercontent.com/63167182/105031114-7ec17c80-5a5d-11eb-8c87-68d2394cddcb.png
