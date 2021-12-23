#include <stdio.h>
#include <string.h>

char *File_base(char name[100]) {
    char n_name[100];
    int k = 0;
    int z = 0;
    while(name[k] != '\0') {
        n_name[k] = '\0';
        if(name[k] == '/')
            z = k;
        k++;
    }
    
    z++;
    int r = z;
    for(int i = 0; 0 < k - z; i++) {
        n_name[i] = name[z];
        z++;
        r = i;
    }
    n_name[r + 1] = '\0';
    
    return n_name;
}

void List(char arch_name[100]) {
    FILE *archive = fopen(arch_name, "r");
    
    fseek(archive, 9, 0);
    int amount_files = fgetc(archive);
    fseek(archive, 12, 0);
    
    for(int i = 0; i < amount_files; i++) {
        char c;
        char name[100];
        int z = 0;
        
        while( (c = fgetc(archive)) != '\0' ) {
            name[z] = c;
            z++;
        }
        
        printf("%s\n", name);
        fgetc(archive);
    }
    
    fclose(archive);
}

void Extract(char arch_name[100]) {
    FILE *archive = fopen(arch_name, "r");
    char file_list[100][100];
    
    char way[100];
    int k = 0;
    int z = 0;
    
    while(arch_name[k] != '\0') {
        if(arch_name[k] == '/')
            z = k + 1;
        way[k] = arch_name[k];
        k++;
    }
    
    while(z < k) {
        way[z] = '\0';
        z++;
    }
    
    fseek(archive, 9, 0);
    int amount_files = fgetc(archive);
    fseek(archive, 12, 0);
    
    for(int i = 0; i < amount_files; i++) {
        char c;
        char name[100];
        int z = 0;
        
        while( (c = fgetc(archive)) != '\0' ) {
            name[z] = c;
            z++;
        }
        name[z] = '\0';
        
        int d = 0;
        while(way[d] != '\0') {
            file_list[i][d] = way[d];
            d++;
        }
        
        for(int v = d; v < 100; v++)
            file_list[i][v] = '\0';
        
        for(int k = 0; name[k] != '\0'; k++) {
            file_list[i][k + d] = name[k];
        }
        
        fgetc(archive);
    }
    
    for(int i = 0; i < amount_files; i++) {
        char pr_c_2 = '\0';
        char pr_c_1 = '\0';
        char c = '\0';
        FILE *file = fopen(file_list[i], "w");
        
        while(c != 'D' || pr_c_1 != 'n' || pr_c_2 != 'E') {
            pr_c_2 = pr_c_1;
            pr_c_1 = c;
            c = fgetc(archive);
            if(c != 'D' || pr_c_1 != 'n' || pr_c_2 != 'E')
                fprintf(file, "%c", pr_c_2);
        }
        
        fclose(file);
        fgetc(archive);
    }
    
    fclose(archive);
}

void Create(char file_list[100][100], char name[100], int amount_files) {
    FILE *archive = fopen(name,"w");
    
    fprintf(archive, "%s%c%c", "ARCH", '\0', '\0');
    fprintf(archive, "%s%c%c%c", "A_F", amount_files, '\0', '\0');
    
    for(int i = 0; i < amount_files; i++) {
        fprintf(archive, "%s%c%c", File_base(file_list[i]), '\0', '\0');
    }
    
    for(int i = 0; i < amount_files; i++) {
        FILE *file = fopen(file_list[i],"r");
        char c = fgetc(file);
        
        while(feof(file) == 0) {
            fprintf(archive, "%c", c);
            c = fgetc(file);
        }
        
        fclose(file);
        fprintf(archive, "%c%s%c", '\0', "EnD", '\0');
    }
    
    fclose(archive);

}

int main(int argc, const char * argv[]) {
    char name[100];
    for(int i = 0; i < 100; i++)
        name[i] = '\0';
    strcpy(name, argv[2]);
    printf("%s\n",argv[2]);
    
    if(strcmp(argv[3], "--create") == 0) {
        char file_list[100][100];
        
        for(int i = 0; i < argc - 4; i++)
            strcpy(file_list[i], argv[i + 4]);
        
        Create(file_list, name, argc - 4);
    }
    
    if(strcmp(argv[3], "--list") == 0) {
        List(name);
    }
    
    if(strcmp(argv[3], "--extract") == 0) {
        Extract(name);
    }
    
    return 0;
}
