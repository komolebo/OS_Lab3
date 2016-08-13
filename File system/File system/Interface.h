
struct File
{
	char * name;
	unsigned short attr;
	unsigned short reserve_field;
	int time_created;
	int date_created;
	int time_last_access;
	int date_last_access;
	int reserved;
	unsigned short time_modified;
	unsigned short date_modified;
	unsigned int first_cluster;
	unsigned int size;
};

struct Settings
{
	unsigned int clusters_number = 0xffff;
};

void previous_load();
void add_file(char * name, unsigned int n);
void red_file(char * name, unsigned int n);

void mem_dump(unsigned int from, unsigned int to);
void output_fat();