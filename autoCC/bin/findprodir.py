#coding=utf-8
import json
import os


def find_file(path, file_path=[]):
    file_dir = os.listdir(path)
    file_dir_abspath = [os.path.join(path, i) for i in file_dir]
    for j in file_dir_abspath:
        if os.path.isfile(j):
            file_path.append(j)
        else:
            find_file(j)
    return file_path


def findbuildir(dir,dst):
    filelist=find_file(dir)

    for l in filelist:
        if l.find(dst)!=-1:
            return l[0:l.find(dst)-1]
    return None
        

def findprodir():
    cwdPath = os.getcwd()
    proPath = cwdPath+"\\projects"
    dirls = os.listdir(proPath)
    print("\033[34m\r\ncurrent project the following:\r\n")
    for d in dirls:
        print(d)
    
    print("\r\nPlease choose one project,Direct input the name\r\n\033[0m")

    
    inputFlag=True
    
    while(inputFlag):
        choosePro = input()
        for d in dirls:
            if(d==choosePro):
                inputFlag=False

        if(inputFlag==True): 
            print("\033[31mPlease Enter again,Direct input is Error\r\n\033[0m")
              
    #choosePro="TCT_V2"
    with open(proPath+"\\"+choosePro+"\\proCfg.json","r",encoding="utf-8") as result:
        data =json.load(result)
        return data


def find_file_c(path, file_path=[]):
    file_dir = os.listdir(path)
    file_dir_abspath = [os.path.join(path, i) for i in file_dir]
    for j in file_dir_abspath:
        if os.path.isfile(j):
            if(j[len(j)-2:len(j)]=='.c' or j[len(j)-2:len(j)]=='.h'):

                file_path.append(j)
        else:
            find_file_c(j)
    return file_path
'''
fileList = find_file_c(os.getcwd())

nonelist=[]
errlist=[]
notlist=[]
print("all file num is "+str(len(fileList)))

num =0 
for f in fileList:
    print("["+str(num)+"/"+str(len(fileList))+"]")
    with open(f,'rb+') as fp:
        content = fp.read()
        encode = chardet.detect(content)['encoding']
        if(encode is None):
            nonelist.append(f)
        elif encode != 'utf-8':
            try:
                gbk_Content = content.decode(encode)
                utf_byte = bytes(gbk_Content, encoding = 'utf-8')
                fp.seek(0)
                fp.write(utf_byte)
            except IOError:
                errlist.append(f)
            except UnicodeDecodeError:
                errlist.append(f)
        else:
            notlist.append(f)
    num+=1

print("none:")
for i in nonelist:
    print(i)
print("\r\nerr:")
for i in errlist:
    print(i)
print("\r\nnot:")
for i in notlist:
    print(i)
'''      


