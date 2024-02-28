import sys
import re

intsize: int = 1;

def macro(_in, _out):
    return { "in": _in, "out": _out };

def THROW(string: str):
    print(string);
    exit(1);

def evaluate(string: str):
    ret: str = "";
    global intsize
    if string[0] == '\"':
        for idx, i in enumerate(string[1:]):
            if i == '\"':
                if idx != len(string[1:])-1:
                    THROW("extraneous characters after string token");
                break;
            ret += i;
        return ret;
    elif string[0] == 'x':
        return int(string[1:], 16).to_bytes(intsize).decode();
    elif string[0] == 'd':
        return int(string[1:], 10).to_bytes(intsize).decode();
    elif string[0] == 'o':
        return int(string[1:], 8).to_bytes(intsize).decode();
    elif string[0] == 'b':
        return int(string[1:], 2).to_bytes(intsize).decode();
    else:
        try:
            return int(string, 16);
        except ValueError:
            THROW("expected evaluateable token");

def parse_file(string: str):
    global intsize
    macros: list[dict] = [];
    li: list[str] = [];
    buf: str = "";
    scont: bool = False;
    for i in string:
        if i == '"':
            scont = not scont;
        if scont:
            buf += i;
            continue;
        if i == ' ' or i == '\n' or i == '\t':
            if buf == "": continue;
            li.append(buf);
            buf = "";
            continue;
        buf += i;
    for idx, i in enumerate(li):
        if i == '=>':
            if idx == 0: THROW("expected token before =>");
            if idx == len(li)-1: THROW("expected token after =>");
            s1 = evaluate(li[idx-1]);
            s2 = evaluate(li[idx+1]);
            macros.append(macro(s1, s2));
        if i == '!intsize':
            if idx == len(li)-1: THROW("expected token after !intsize");
            intsize = int.from_bytes(evaluate(li[idx+1]).encode());
    print(macros);
    return macros;

def apply_macros(string: str, macros: list):
    ret: str = string;
    for i in macros:
        print(i["in"] + ": " + i["out"]);
        ret = ret.replace(i["in"], i["out"]);
    return ret;


fp = open(sys.argv[1], "r");
macro_string = fp.read();
fp.close();
fp = open(sys.argv[2], "r");
file_string = fp.read();
fp.close();

ret = apply_macros(file_string, parse_file(macro_string));

fp = open(sys.argv[3], "w");
fp.write(ret);
fp.close();


