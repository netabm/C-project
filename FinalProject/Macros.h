#ifndef MACROS_H_
#define MACROS_H_

#define CLOSE_RETURN_ONE(fp){fclose(fp); return 1;}
#define CLOSE_RETURN_ZERO(fp){fclose(fp); return 0;}
#define OPEN_FILE(fp, fileName, type, msg){fp = fopen(fileName, type); if(!fp){printf("%s\n", msg); return 0;}}

#endif