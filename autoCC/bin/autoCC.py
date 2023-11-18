#coding=utf-8
from asyncio.windows_events import NULL
from distutils.command.build import build
import os
from xml.etree.ElementTree import tostring
import configtools
import subprocess
import findprodir
import time
import sys
import shutil
import json
import zip

'''
print("\033[31m这是红色字体\033[0m")
print("\033[32m这是绿色字体\033[0m")
print("\033[33m这是黄色字体\033[0m")
print("\033[34m这是蓝色字体\033[0m")
print("\033[38m这是默认字体\033[0m")  # 大于37将显示默认字体

'''


if __name__ == '__main__':
    cwdPath = os.getcwd()+"\\autoCC"
    toolsPath = cwdPath + "\\tools"
    logPath = cwdPath+"\\build_log"


    if(os.path.exists(cwdPath)==False):
        print("\033[31work path is not found!\033[0m")
        exit()

    if(os.path.exists(logPath)==False):
         os.mkdir(logPath)

    rt=configtools.configtools()
    if rt == 0 :
        exit(1)
    elif rt ==1:#本地工具模式
        keilPath = "%KEILPATH%"
        UV4Path =keilPath+"\\UV4\\UV4.exe"
        #寻找该项目的工程
        procfg=findprodir.findprodir()
        print("\033[34mIs about to begin building of "+procfg["proPath"]+"\r\n\033[0m")
        print("\033[34mThis project contains the following boards\r\n\033[0m")
        for k in procfg["boards"]:  
            print("id:"+k[0]+"  name:"+k[2]+"   path:"+k[1])

        rt=input("\033[34m Are you sure?y/n\033[0m")
        if(rt!='y'):
            exit()
                
        
        #删除一些缓存文件
        proPath = os.getcwd()+"\\projects\\"+procfg["proPath"]

        if(os.path.exists(proPath+"\\Firmware_out")==False): 
            os.mkdir(proPath+"\\Firmware_out")

        if(os.path.exists(proPath+"\\Firmware_out\\TEMP")==False): 
            os.mkdir(proPath+"\\Firmware_out\\TEMP")  

        filelist =os.listdir(proPath+"\\Firmware_out\\TEMP")
        for f in filelist:
            os.remove(proPath+"\\Firmware_out\\TEMP"+"\\"+f)

        #编译主流程开始
        allbuildingTime=0;
        alreadyBuildList=[]
        for k in procfg["boards"]:            

            uv_pro_path = os.getcwd()+"\\boards"+k[1]+"\\MDK-ARM"
            uv_pro_boards=k[0]
            uv_pro_name = k[2]
            uv_pro_name_pro =k[2]+".uvprojx"
            #,encoding="utf",errors='ignore'
            #开始编译之前 清除上一次的log文件
            if(os.path.exists(logPath+"\\"+uv_pro_name+"_build_log")==True):
                os.remove(logPath+"\\"+uv_pro_name+"_build_log")

            #每次编译一个项目前 先看看是否跟编译过的项目重复 防止浪费生命
            repeatFlag=False
            timesaved=0
            for a in alreadyBuildList:
                if(k[2]==a[2] and k[1]==a[1] and k[0]!=a[0]):
                    repeatFlag=True
                    timesaved=buildingTime
                    break
            if(repeatFlag==False):

                uv4Cmd = UV4Path+" -j4 -r "+uv_pro_path+"\\"+uv_pro_name_pro+" -o "+logPath+"\\"+uv_pro_name+"_build_log"

                #编译前先清理build目录
                if(os.path.exists(uv_pro_path+"\\MDK-ARM\\Build")==True): 
                    filelist =os.listdir(uv_pro_path+"\\MDK-ARM\\Build")
                    for f in filelist:
                        os.remove(uv_pro_path+"\\MDK-ARM\\Build"+"\\"+f)
                #启动一个编译进程
                UV4 =subprocess.Popen(uv4Cmd, shell=True)
                print("\033[34mNow is start building of "+uv_pro_name_pro+"\r\n\033[0m")
                
                buildingTime=0
                #等待编译进程结束
                while(UV4.poll() is None):
                    print('.',end='')
                    sys.stdout.flush()
                    time.sleep(1)
                    buildingTime+=1
            
                allbuildingTime+=buildingTime
                print("\033[32m\r\nbuilding the "+ str(uv_pro_name_pro)+" In "+str(buildingTime)+" seconds \r\n\033[0m")

                if(os.path.exists(logPath+"\\"+uv_pro_name+"_build_log")==False):
                    print("\033[31mbuild Error\033[0m")
                    exit()
                #编译后根据log文件 判断结果
                with open(logPath+"\\"+uv_pro_name+"_build_log", 'r',encoding="gb2312",errors='ignore') as file:
                    for line in file:
                        if(line.find("Error(s),")!=-1):
                            errbegen =line.find(" - ")+3
                            errend  = line.find("Error(s),")-1
                            if(line[errbegen:errend]!="0"):

                                print("\033[31m\r\nbuilding the "+ str(uv_pro_name_pro)+" is Error,Please see the log file\033[0m")
                                print("\033[31mthe has "+line[errbegen:errend]+" Error!\033[0m")
                                exit()
                        if(line.find("Warning(s).")!=-1):
                            errbegen =line.find("Error(s),")+10
                            errend  = line.find("Warning(s).")-1
                            if(line[errbegen:errend]!="0"):
                                print("\033[33m\r\nbuilding the "+ str(uv_pro_name_pro)+" is successful,But has "+line[errbegen:errend]+" Warning.\r\n\033[0m")
                            else:
                                print("\033[32m\r\ngod job!!! 0 Error and 0 Waring\033[0m") 
                                print("\033[32mbuilding the "+ str(uv_pro_name_pro)+" is successful\r\n\033[0m") 
            else:
                print("\033[32m\r\nrepeat build,help you skip!\033[0m") 
                print("\033[32malready saved you "+ str(timesaved) +" seconds of your life\033[0m") 

            FromelfPath =keilPath+"\\ARM\\ARMCC\\bin\\fromelf.exe"
            FromelfPath_input = uv_pro_path+"\\Build\\"+uv_pro_name +".axf"
            FromelfPath_out = uv_pro_path+"\\Build\\"+uv_pro_name+'-'+uv_pro_boards+".bin"

            #判断下 目录是否正确 防止一些工程的编译文件夹和其他的工程不同
            #如果不同 就搜索文件 
            if(os.path.exists(FromelfPath_input)==False):
                rt=findprodir.findbuildir(uv_pro_path,uv_pro_name +".axf")
                if(rt==None):
                     print("\033[31mNot find axf file, maybe build  fail\033[0m") 
                     exit()
                else:
                   FromelfPath_input=rt+"\\" +uv_pro_name +".axf"
                   FromelfPath_out=rt +"\\"+uv_pro_name+'-'+uv_pro_boards+".bin"
                
            fromelfCmd= FromelfPath+" --bin -o " +FromelfPath_out+" "+FromelfPath_input

            if(os.path.exists(FromelfPath_out)==True):
                os.remove(FromelfPath_out)

            Fromel =subprocess.Popen(fromelfCmd, shell=True)
            print("\033[34mStart generate bin file\033[0m")
            #等待bin转换进程结束
            while(Fromel.poll() is None):
                time.sleep(1)
                allbuildingTime+=1

            if(os.path.exists(FromelfPath_out)==False):
                print("\033[31mGenerate bin file Error\033[0m")
                exit()

            #先暂时的把Bin文件放到TEMP备用 待全部工程生成后 在统一进行版本名称修改    
            proPath = os.getcwd()+"\\projects\\"+procfg["proPath"]

            if(os.path.exists(proPath+"\\Firmware_out")==False): 
                os.mkdir(proPath+"\\Firmware_out")

            if(os.path.exists(proPath+"\\Firmware_out\\TEMP")==False): 
                os.mkdir(proPath+"\\Firmware_out\\TEMP")  

            shutil.copyfile(FromelfPath_out,proPath+"\\Firmware_out\\TEMP\\"+uv_pro_name+'-'+uv_pro_boards+".bin")
            if(os.path.exists(proPath+"\\Firmware_out\\TEMP\\"+uv_pro_name+'-'+uv_pro_boards+".bin")==False):
                print("\033[31mCopy bin file to TEMP Error\033[0m")
                exit()
            print("\033[32mgenerate bin file successful\033[0m")
            alreadyBuildList.append(k)

        #打印编译总体信息
        
        print("\033[32mbuild "+ str(len(procfg["boards"]))+"tager,All in "+str(allbuildingTime)+" seconds\033[0m")
    
        #版本控制         
        firmwarePath=os.getcwd()+"\\projects\\"+procfg["proPath"]+"\\Firmware_out"
        print("\033[34mcuurent project is "+procfg["proName"]+"\033[0m")
        print("\033[34mcuurent version is "+procfg["version"]+"\033[0m")
        print("\033[34mChoose your upgrade mode"+"\033[0m")
        print("\033[34m[b]:bigVersion  [m]:mediumVersion  [s]:smallVersion  [n]: not upgrade\033[0m")

        #等待用户输入 版本更新方式
        inputFlag=True
        while(inputFlag):
            upMode=input()
            if(upMode!='b' and upMode!='m'and upMode!='s'and upMode!='n'):
                print("\033[31mupgrade mode input err,please Enter again\033[0m")
                print("\033[34m[b]:bigVersion  [m]:mediumVersion  [s]:smallVersion  [n]: not upgrade\033[0m")
            else:
                inputFlag=False
        i=0
        bigEnd=0
        mediumEnd=0
        smallEnd=0
        #对版本号进行分割
        for c in procfg["version"]:
            if(c=='.'):
                if(bigEnd==0):
                    bigEnd=i
                elif(mediumEnd==0):
                    mediumEnd=i 
                    break    
            i+=1
        smallEnd=len(procfg["version"])

        bigStr=procfg["version"][0:bigEnd]
        mediumStr=procfg["version"][bigEnd+1:mediumEnd]
        smallStr=procfg["version"][mediumEnd+1:smallEnd]
        #按照所选更新方式 进行版本迭代
        if(upMode=='b'):
            bigStr=str(int(bigStr)+1)
            mediumStr='0'
            smallStr='0'
        elif(upMode=='m'):
            mediumStr=str(int(mediumStr)+1)
            smallStr='0'
        elif(upMode=='s'):
            smallStr=str(int(smallStr)+1)
        #elif(upMode=='n'):
            
        #设置迭代后的新版本号
        procfg["version"]=bigStr+'.'+mediumStr+'.'+smallStr


        #等待用户确认版本号命名规则
        proname=procfg["proName"]
        boardadder="#boardadder#"
        hardwareVersion=procfg["hardwareVersion"]
        firmwareVersion=procfg["version"]
        namerule =proname+'.'+boardadder+'-'+hardwareVersion+'-'+firmwareVersion+'.bin'
        #print("\033[34mname rule is: "+namerule+"\033[0m")
        inputFlag=True
        while(inputFlag):
            print("\033[34mname rule is: "+proname+'.'+boardadder+'-'+hardwareVersion+'-'+firmwareVersion+'.bin'+"\033[0m")
            print("\033[34mEnter an option to continue\033[0m")
            print("\033[34m[ok]:complete  [pname]:proName    [hv]:hardwareVersion  [fv]:firmwareVersion\033[0m")
            upMode=input()
            if(upMode!='ok' and upMode!='pname'and upMode!='adder' and upMode!='hv' and upMode!='fv'):
                print("\033[31moption input err,please Enter again\033[0m")
                print("\033[34m[ok]:complete  [pname]:proName   [hv]:hardwareVersion  [fv]:firmwareVersion\033[0m")
            elif(upMode=='ok'):
                print("\033[34mname rule is: "+proname+'.'+boardadder+'-'+hardwareVersion+'-'+firmwareVersion+'.bin'+"\033[0m")
                rt=input("\033[34m Are you sure?y/n\033[0m")
                if(rt=='y'):
                        namerule =proname+'.'+boardadder+'-'+hardwareVersion+'-'+firmwareVersion+'.bin'
                        inputFlag=False
            elif(upMode=='pname'):
                print("\033[34mEnter proName :\033[0m")
                proname=input()
            elif(upMode=='hv'):
                print("\033[34mEnter hardwareVersion :\033[0m")
                hardwareVersion=input()
            elif(upMode=='fv'):
                print("\033[34mEnter firmwareVersion :\033[0m")
                firmwareVersion=input()

        #把固件按新的版本号进行重命名

        tempPath =proPath+"\\Firmware_out\\TEMP"
        for b in procfg["boards"]:
             boardadder=b[0]
             namerule =proname+'.'+boardadder+'-'+hardwareVersion+'-'+firmwareVersion+'.bin'
             os.rename(tempPath+"\\"+b[2]+'-'+b[0]+".bin",tempPath+"\\"+namerule)

        #等待用户输入更新信息
        versionupdatePath=tempPath+"\\update.json"#本次固件更新的信息文件路径
        versionhistoryPath=os.getcwd()+"\\projects\\"+procfg["proPath"]+"\\update_history.json"#所有的更新信息文件路径

        versionupdate=[]#本次更新的信息
        versionhistory={}#历史更新的信息

        inputFlag=True
        while(inputFlag):
            print("\033[34mEnter an option to perform the appropriate action\033[0m")
            print("\033[34m[ok]:complete  [del]:delete One  [ls]:show msg  [add]:add func  [update]:update func [bug]:Fix bug \033[0m")
            upMode=input()
            if(upMode!='ok' and upMode!='del'and upMode!='add' and upMode!='update' and upMode!='ls' and upMode!='bug'):
                print("\033[31moption input err,please Enter again\033[0m")
                print("\033[34m[ok]:complete  [del]:delete One  [ls]:show msg  [add]:add func  [update]:update func [bug]:Fix bug \033[0m")
            elif(upMode=='ok'):
                if(len(versionupdate)==0):
                    rt=input("\033[34mYou didn't enter anything. Are you sure?y/n  \033[0m")
                    if(rt=='y'):
                        inputFlag=False
                else:
                    inputFlag=False
            elif(upMode=='ls'):
                print("\033[34mcurrent update msg:\033[0m")
                for l in versionupdate:
                    print(l)
            elif(upMode=='del'):
                print("\033[34mcurrent update msg:\033[0m")
                for l in versionupdate:
                    print(l)
                print("\033[34mEnter want del id:\033[0m")
                idx=input()
                idx=int(idx)
                if(idx>=0 and idx<=100):
                    del versionupdate[idx]
            elif(upMode=='add'):
                print("\033[34mEnter msg :\033[0m")
                msg=input()
                msg="[新增] "+msg
                versionupdate.append(msg)
            elif(upMode=='update'):
                print("\033[34mEnter msg :\033[0m")
                msg=input()
                msg="[优化] "+msg
                versionupdate.append(msg)
            elif(upMode=='bug'):
                print("\033[34mEnter msg :\033[0m")
                msg=input()
                msg="[bug修复] "+msg
                versionupdate.append(msg)

        print("\033[34mcurrent update msg:\033[0m")
        for l in versionupdate:
            print(l)
            #将更新信息写入文件

        jsonPath = versionupdatePath
        with open(jsonPath,"w",encoding="utf-8") as f:
            json.dump(versionupdate,f,indent=4,ensure_ascii=False)

        #追加更新历史并写入文件
        versionhistory["originVersion"]=procfg["version"]#记录由来版本
        versionhistory["Version"]=firmwareVersion#当前版本
        versionhistory["msg"]=versionupdate
        versionhistory["buildTime"]=time.strftime("%Y%m%d", time.localtime())
        gitLabCfgPath=os.getcwd()+"\\.git"

        #查询git 分支md5值
        if(os.path.exists(gitLabCfgPath+"\\HEAD")==False):
            #尝试手动输入输入md5
            print("\033[31git_cfg path is not found!\033[0m")
            versionhistory["buildMd5"]=input("Try entering or typing manually or enter \'exit\' to exit")
            if(versionhistory["buildMd5"]=="exit"):
                exit()
        else:#正常读取文件输入md5
            with open(gitLabCfgPath+"\\HEAD","r",encoding="utf-8") as f:
                fp=f.read()
                filelist =os.listdir(gitLabCfgPath+"\\refs\\heads")
                for f in filelist:
                    if(fp.find(f)!=0):
                        with open(gitLabCfgPath+"\\refs\\heads\\"+f,"r",encoding="utf-8") as md:
                                versionhistory["buildMd5"]=md.read()
                                #去掉回车结束符
                                versionhistory["buildMd5"]=versionhistory["buildMd5"][0:len(versionhistory["buildMd5"])-1]
                                break
            if("buildMd5" not in versionhistory):
                #尝试手动输入输入md5
                print("\033[31git_head file is not found!\033[0m")
                versionhistory["buildMd5"]=input("Try entering or typing manually or enter \'exit\' to exit")
                if(versionhistory["buildMd5"]=="exit"):
                    exit()

        jsonPath=versionhistoryPath
        with open(jsonPath,"r",encoding="utf-8") as f:
            data=[]#data是一个列表
            try:
                data =json.load(f)
            except json.decoder.JSONDecodeError:#如果json文件为空
                data=[]
        with open(jsonPath,"w",encoding="utf-8") as f:
            data.append(versionhistory)
            json.dump(data,f,indent=4,ensure_ascii=False)

            #所有操作完成后
            #将新版本写入配置文件
        jsonPath = os.getcwd()+"\\projects\\"+procfg["proPath"]+"\\proCfg.json"
        with open(jsonPath,"w",encoding="utf-8") as f:
            json.dump(procfg,f,indent=4,ensure_ascii=False)

        #进行压缩包处理
        zipoutname=firmwarePath+"\\"+procfg["proName"]+'-'+hardwareVersion+'-'+firmwareVersion+'-'+time.strftime("%Y%m%d", time.localtime())
        zip.batch_zip(tempPath,zipoutname)

        if(os.path.exists(zipoutname+'.zip')==False):
            print("\033[31mGenerate zip file fail!\033[0m")
            exit()
        else:
            print("\033[32mAll jobs is complete!")
            print("new firmware adder is "+zipoutname)
            print("bye!\033[0m")

        #收尾工作 清楚缓存 和 log
        proPath = os.getcwd()+"\\projects\\"+procfg["proPath"]

        if(os.path.exists(proPath+"\\Firmware_out")==False): 
            os.mkdir(proPath+"\\Firmware_out")

        if(os.path.exists(proPath+"\\Firmware_out\\TEMP")==False): 
            os.mkdir(proPath+"\\Firmware_out\\TEMP")  

        filelist =os.listdir(proPath+"\\Firmware_out\\TEMP")
        for f in filelist:
            os.remove(proPath+"\\Firmware_out\\TEMP"+"\\"+f)

        filelist =os.listdir(logPath)
        for f in filelist:
            os.remove( logPath+"\\"+f)

           


     
    elif rt ==2:

        keilPath = toolsPath +"\\keil"
        UV4Path =keilPath+"\\UV4\\UV4.exe"
        uv_pro_path = ""
        uv_pro_name = ""

        FormelfPath =keilPath+"\\ARM\\ARMCC\\bin\\fromelf.exe"
        FormelfPath_input = uv_pro_path+"\\Build\\uv_pro_name"+".axf"
        FormelfPath_out = uv_pro_path+"\\Build\\uv_pro_name"+".bin"
        #fromelf.exe --bin -o ../Output/test.bin ./Objects/test.axf

        fromelfCmd= FormelfPath+" --bin -o " +FormelfPath_out+" "+FormelfPath_input
        uv4Cmd = UV4Path+" -j0 -r "+uv_pro_path+ "-o "+logPath+"build_log.txt"

        UV4 =subprocess.Popen(uv4Cmd, shell=False)


