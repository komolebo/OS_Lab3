#include "Interface.h"
#include <vector>

using namespace std;

Settings settings;

vector<int> Memory;
vector<File> FAT;

void previous_load()
{
	for (unsigned int i = 0; i <= settings.clusters_number; ++i)
		Memory.push_back(0x0000); 

	// First and second can't be used
	Memory[0] = 0x4449;
	Memory[1] = 0x00FF;
}

void add_file(char * name, unsigned int n)
{
	// Find out whether we have free space
	unsigned int free_clusters = settings.clusters_number - 2;
	for (auto & f : FAT)
		free_clusters -= f.size;

	if (free_clusters < n)
	{
		printf("No space left\n");
		return;
	}

	File f;

	f.size = n;
	f.name = name;

	// Search first free cluster
	for (unsigned int i = 2; i <= settings.clusters_number - 1; ++i)
	{
		if (Memory[i] == 0x0000)
		{
			f.first_cluster = i;
			break;
		}
	}

	unsigned int tmp = f.first_cluster;
	unsigned int cur_cluster = tmp + 1;
	while (n - 1)  // Last cluster for 0xFFFF
	{
		if (Memory[cur_cluster] == 0x0000)
		{
			Memory[tmp] = cur_cluster;
			tmp = cur_cluster;
			n--;
		}
		cur_cluster++;
	}
	Memory[tmp] = 0xFFFF;
	FAT.push_back(f);
}

void red_file(char * name, unsigned int n)
{
	unsigned long int cluster_number = 0x10000;
	File * file = &FAT[0];

	// Find file in FAT
	for (auto & f : FAT)
		if (strcmp(name, f.name) == 0)
		{
			cluster_number = f.first_cluster;
			file = &f;
		}

	if (cluster_number == 0x10000 || n >= file->size)
	{
		printf("File doesn't exist or wrong size \n", name);
		return;
	}

	// Go to new terminate cluster of this file
	int steps = n - 1;
	while (steps-- > 0)
		cluster_number = Memory[cluster_number];

	// Final cluster of the file
	int terminate_cluster = cluster_number; 

	// Clear redundant clusters
	unsigned int next;
	cluster_number = Memory[cluster_number];
	steps = file->size - n;
	while (steps--)
	{
		next = Memory[cluster_number];
		Memory[cluster_number] = 0x0000;
		cluster_number = next;
	}

	Memory[terminate_cluster] = (n == 0) ? 0x0000 : 0xFFFF;  // If n == 0 then clear memory

	// Remake file info in FAT
	if (n)
		file->size = n;
	else // remove file
		for (auto f = FAT.begin(); f != FAT.end(); ++f)
			if (strcmp(name, f->name) == 0)
			{
				FAT.erase(f);
				return;
			}
}

void output_fat()
{
	if (!FAT.size()) printf("No file in FAT \n");

	for (auto & f : FAT)
		printf(" %s  size=%d from %d\n", f.name, f.size, f.first_cluster);
}

void mem_dump(unsigned int from, unsigned int to)
{
	if (to > settings.clusters_number - 0xf || from > settings.clusters_number - 0xf || to % 0x10 != 0 || from % 0x10 != 0)
	{
		printf("Wrong bounds \n");
		return;
	}
	
	for (unsigned int i = from; i <= to + 0x000f; ++i)
	{
		if (i % (0x000F + 1) == 0) // new line
			printf("\n%4x:  ", i);
		printf("%4x ", Memory[i]);
	}
	printf("\n");
}