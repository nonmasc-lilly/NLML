#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DARR_INIT(size_ref) malloc(1); size_ref = 0
#define DARR_APPEND(darr, item, darr_size)\
    (darr) = realloc((darr), ((darr_size)+1)*sizeof((item)));\
    (darr)[(darr_size)] = (item);\
    darr_size+=1

typedef unsigned int COUNT;
typedef unsigned int BOOL;
typedef enum {
    T_STRING,
    T_NUMBER
} TYPE;
struct status { int intsize; };
struct macro { char *in, *out; };

void THROW(const char *message);
char *evaluate(char *str, struct status *status);
struct macro *parse_string(char *str, struct status *status,
    COUNT *rlen);
char *apply_macros(char *str, struct macro *macros, COUNT macro_len);
char *str_replace(char *input, char *curtok, char *newtok);


int main(int argc, char **argv) {
    FILE *fp;
    char *ruleset, *content, *ret;
    struct macro *macros;
    struct status status = {1};
    COUNT fsz, macro_len, i;

    if(argc < 4) {
        fprintf(stderr, "USAGE: %s <rule file> "
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

    macros = parse_string(ruleset, &status, &macro_len);
    ret = apply_macros(content, macros, macro_len);

    /* output new string to file */
    fp = fopen(argv[3], "w");
    fwrite(ret, 1, strlen(ret), fp);
    fclose(fp);
    for(i=0; i<macro_len; i++) {
        free(macros[i].in);
        free(macros[i].out);
    }
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
        tmp = strtol(str+1, &eptr, 16);
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
        tmp = strtol(str+1, &eptr, 10);
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
        tmp = strtol(str+1, &eptr, 8);
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
        tmp = strtol(str+1, &eptr, 2);
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
        tmp = strtol(str+1, &eptr, 16);
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
struct macro *parse_string(char *str, struct status *status,
        COUNT *rlen) {
    struct macro *ret, tmp2;
    char **li, *buf, *tmp, *s1, *s2, *eptr, tmp4, tmp5[3];
    BOOL scont, nscont;
    COUNT i, j, lisz, bsz, retsz;
    long tmp3, tmp6;
    li = DARR_INIT(lisz);
    buf = DARR_INIT(bsz);
    ret = DARR_INIT(retsz);
    nscont = scont = 0;
    for(i=0; str[i]; i++) {
        if(str[i] == '\"') {
            scont = !scont;
            nscont = 1;
        }
        if(scont) {
            if(str[i] == '\\') {
                i++;
                switch(str[i]) {
                case '\\': tmp4 = '\\'; break; 
                case 'n':  tmp4 = 0xA;  break;
                case 'r':  tmp4 = 0xD;  break;
                case 'a':  tmp4 = 0x7;  break;
                case 'f':  tmp4 = 0xC;  break;
                case 't':  tmp4 = 0x9;  break;
                case 'v':  tmp4 = 0xB;  break;
                case 'b':  tmp4 = 0x8; break;
                case '\'': tmp4 = '\''; break;
                case '\"': tmp4 = '\"'; break;
                case 'x':
                    tmp5[0] = str[i+1];
                    tmp5[1] = str[i+2];
                    tmp5[2] = 0x0;
                    tmp6 = strtol(tmp5, &eptr, 16);
                    if(!(*tmp5) || *eptr)
                        THROW("expected '\\x' to contain valid "
                              "two character hex argument (such "
                              "as \\x0C or \\x33)\n");
                    i+=2;
                    tmp4 = (char)tmp6;
                    break;
                }
            } else tmp4 = str[i];
            DARR_APPEND(buf, tmp4, bsz);
            continue;
        }
        if(str[i] == ' ' || str[i] == '\n' || str[i] == '\t' || nscont) {
            if(bsz == 0) continue;
            DARR_APPEND(buf, 0, bsz);
            DARR_APPEND(li, buf, lisz);
            buf = DARR_INIT(bsz);
            nscont = 0;
            continue;
        }
        DARR_APPEND(buf, str[i], bsz);
    }
    for(i=0; i<lisz; i++) {
        if(!strcmp(li[i], "=>")) {
            if(i==0) THROW("expected token before '=>'\n");
            if(i==lisz-1) THROW("expected token after '=>'\n");
            s1 = evaluate(li[i-1], status);
            s2 = evaluate(li[i+1], status);
            tmp2.in = s1;
            tmp2.out = s2;
            DARR_APPEND(ret, tmp2, retsz);
        }
        if(!strcmp(li[i], "!intsize")) {
            if(i == lisz-1) THROW("expected token after '!intsize'");
            tmp3 = 0;
            for(j=0; j<status->intsize; j++) {
                ((char*)(&tmp3))[j] = li[i][j];
            }
            status->intsize = tmp3;
        }
    }
    for(i=0; i<lisz; i++) free(li[i]);
    free(li);
    free(buf);
    *rlen = retsz;
    return ret;
}
char *apply_macros(char *str, struct macro *macros, COUNT macro_len) {
    char *ret, *tmp;
    COUNT i;
    ret = malloc(strlen(str)+1);
    strcpy(ret, str);
    for(i=0; i<macro_len; i++) {
        tmp = str_replace(ret, macros[i].in, macros[i].out);
        free(ret);
        ret = tmp;
    }
    return ret;
    
}
char *str_replace(char *input, char *curtok, char *newtok) {
    char *ret, *ins, *tmp;
    COUNT front, numrep;
    if(!input||!curtok) return NULL;
    if(!strlen(curtok)) return NULL;
    if(!newtok) newtok = "";
    ins = input;
    for(numrep=0; tmp = strstr(ins, curtok); numrep++)
        ins = tmp+strlen(curtok);
    tmp = ret = malloc(strlen(input) +
        (strlen(newtok)-strlen(curtok))*numrep + 1);
    if(!ret) return NULL;
    while(numrep--) {
        ins = strstr(input, curtok);
        front = ins-input;
        tmp = strncpy(tmp, input, front) + front;
        tmp = strcpy(tmp, newtok) + strlen(newtok);
        input += front + strlen(curtok);
    }
    strcpy(tmp, input);
    return ret;
}


