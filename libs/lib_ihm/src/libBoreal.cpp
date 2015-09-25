#include <libBoreal.hh>

void print(char* str, char ec)
{
    int i=0;
    while(str[i] != ec)
    {
	printf("%c", str[i]);
	if(str[i] == '\0')break;
	i++;
    }
    printf("%d", i);
}

char* epure(char* strs, char ec)
{
    int i=0;
    char* strd;
    while(strs[i] != ec)
    {
	//printf("%c", str[i]);
	if(strs[i] == '\0')break;
	i++;
    }
    strd = (char*) malloc(i*sizeof(char)+1);
    strncpy(strd, strs, i);
    strd[i] = '\0';
    
    return strd;
}
