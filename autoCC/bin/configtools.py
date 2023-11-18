from genericpath import isdir
import os
import configparser
from pathlib import Path
import sys, ctypes



def configtools():
    cwdPath = os.getcwd()+"\\autoCC"
    setcfgPath = cwdPath+"\\setcfg.INI"
    cfp=configparser.ConfigParser()

    cfp.read(setcfgPath,encoding='utf-8')
    value= cfp.get("tools", "userLocalTool")

    if(value=='1'):
        #使用开发者本地的keil
        env_dist = os.environ
        for key in env_dist:
            #print(key+"="+env_dist[key]+"\r\n") 
            if(key=="KEILPATH"):
                return 1
        #print("No set Keil evn,please input your keil root directory\r\n")  
        #inputPath = input()
        #keilPath = inputPath
        #os.system("setx /m keilPath "+keilPath)
        print("No set Keil evn,please Run tools directory \"setkeilenv.ps\"\r\n") 
        return 0

    else:
        #使用工具自带的keil
        toolsPath = cwdPath + "\\tools"
        keilPath = toolsPath +"\\keil"
        if Path(keilPath).is_dir():
            cfp=configparser.ConfigParser()

            tooliniPath =keilPath+"\\TOOLS.INI"
            print(tooliniPath+"\r\n")
            

            cfp.read(tooliniPath,encoding='utf-8')

            cfp.set("UV2","RTEPATH",'"'+keilPath+"\\Packs"+'"')
            cfp.set("ARM","PATH",'"'+keilPath+"\\ARM"+'"')
            cfp.set("ARMADS",'"'+"PATH",keilPath+"\\ARM"+'"')

            with open(tooliniPath, "w",encoding='utf-8') as f:
                cfp.write(f)
            return 2
        else:
            return 0




