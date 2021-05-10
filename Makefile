CC=gcc

handler: queue.c driver.c scheduler.c cpu.c 
		$(CC) -o project2 queue.c driver.c scheduler.c cpu.c