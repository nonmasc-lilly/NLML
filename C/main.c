#include <stdio.h>
#include <stdlib.h>

#define DARR_INIT(size_ref) malloc(1); size_ref = 0
#define DARR_APPEND(darr, item, darr_size)\
    (darr) = realloc((darr), ((darr_size)+1)*sizeof((item)));\
    (darr)[(darr_size)] = (item);

typedef unsigned int COUNT;
typedef enum {
    T_STRING,
    T_NUMBER
} TYPE;

struct status {
    int intsize;
};

struct macro {
    TYPE types[2];
    void *in;
    void *out;
};

void THROW(const char *message);
char *evaluate(char *str, struct status *status);
struct macro *parse_string(char *str, struct status *status);
char *apply_macros(char *str, struct macro *macros);

int main(int argc, char **argv) {
    FILE *fp;
    char *ruleset, *content, *ret;
    struct macro *macros;
    struct status status = {1};
    COUNT fsz;

    if(argc < 4) {
        fprintf(2, "USAGE: %s <rule file>"
            "<input file> <output file>\n", argv[0]);
        exit(1);
    }
    /* read rule file */
    fp = fopen(argv[1], "r");
    ruleset = malloc(fsz = (fseek(fp, 0L, SEEK_END), ftell(fp)));
    rewind(fp);
    fread(ruleset, 1, fsz, fp);
    fclose(fp);
    /* read input file */
    fp = fopen(argv[2], "r");
    content = malloc(fsz = (fseek(fp, 0L, SEEK_END), ftell(fp)));
    rewind(fp);
    fread(content, 1, fsz, fp);
    fclose(fp);

    macros = parse_string(ruleset, &status);
    ret = apply_macros(content, macros);

    /* output new string to file */
    fp = fopen(argv[3], "w");
    fwrite(fp, 1, strlen(ret), ret);
    fclose(fp);
    free(macros);
    free(ret);
    return 0;
}

void THROW(const char *message) {
    fprintf(stderr, message);
    exit(1);
}

char *evaluate(char *str, struct status *status) {
    char *ret, *eptr;
    COUNT i, retsz;
    long tmp;
    ret = DARR_INIT(retsz);
    switch(str[0]) {
    case '\"':
        for(i=1; str[i]; i++) {
            if(str[i] == '\"') {
                if(i != strlen(str)-1)
                    THROW("error: unexpected characters after string");
                break;
            }
            DARR_APPEND(ret, str[i], retsz);
        }
        DARR_APPEND(ret, 0, retsz);
        return ret;
    case 'x':
        tmp = strtol(str, eptr, 16);
        if(!(*str) || *eptr)
            THROW("explicit hex conversion failed");
        for(i=0; i<status->intsize; i++) {
            if(i > sizeof(tmp))
                THROW("intsize is greater than maximum int size");
            DARR_APPEND(ret, ((char*)(&tmp))[i], retsz);
        }
        DARR_APPEND(ret, 0, retsz);
        return ret;
    case 'd':
        tmp = strtol(str, eptr, 10);
        if(!(*str) || *eptr)
            THROW("decimal conversion failed");
        for(i=0; i<status->intsize; i++) {
            if(i > sizeof(tmp))
                THROW("intsize is greater than maximum int size");
            DARR_APPEND(ret, ((char*)(&tmp))[i], retsz);
        }
        DARR_APPEND(ret, 0, retsz);
        return ret;
    case 'o':
        tmp = strtol(str, eptr, 8);
        if(!(*str) || *eptr)
            THROW("octal conversion failed");
        for(i=0; i<status->intsize; i++) {
            if(i > sizeof(tmp))
                THROW("intsize is greater than maximum int size");
            DARR_APPEND(ret, ((char*)(&tmp))[i], retsz);
        }
        DARR_APPEND(ret, 0, retsz);
        return ret;
    case 'b':
        tmp = strtol(str, eptr, 1);
        if(!(*str) || *eptr)
            THROW("binary conversion failed");
        for(i=0; i<status->intsize; i++) {
            if(i > sizeof(tmp))
                THROW("intsize is greater than maximum int size");
            DARR_APPEND(ret, ((char*)(&tmp))[i], retsz);
        }
        DARR_APPEND(ret, 0, retsz);
        return ret;
    default:
        tmp = strtol(str, eptr, 16);
        if(!(*str) || *eptr)
            THROW("expected valid token");
        for(i=0; i<status->intsize; i++) {
            if(i > sizeof(tmp))
                THROW("intsize is greater than maximum int size");
            DARR_APPEND(ret, ((char*)(&tmp))[i], retsz);
        }
        DARR_APPEND(ret, 0, retsz);
        return ret;
    }
}
