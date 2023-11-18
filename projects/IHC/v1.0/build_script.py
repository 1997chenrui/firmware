#coding=utf-8
#!/usr/bin/python
import sys
import json
# 配置文件对应的对象
class ItemCfg:
    def __init__(self):
        self.itemName = "IHC"
        self.hardVersion = "1"
        self.projectPathMap = {}

    def list_name(self):
        return list(self.projectPathMap)

    def to_json(self):
        d = {}
        d["itemName"] = self.itemName
        d["hardVersion"] = self.hardVersion
        d["projectPathMap"] = self.projectPathMap
        return json.dumps(d)

    def from_json(self, content):
        d = json.loads(content)
        self.hardVersion = d["hardVersion"]
        self.itemName = d["itemName"]
        self.projectPathMap = d["projectPathMap"]
        
    def read_json(self):
        with open('d:\Software\gitlab\firmware\projects\IHC\v1.0\a.txt', 'r') as f:
            print(f.read())
            # return self.from_json(r.read())


    def print_string(self):
        print(self.itemName)
        print(self.hardVersion)
        print(self.projectPathMap)

class UpdateMsg:
    def __init__(self):
        self.msg = []

    def to_json(self):
        d = {}
        d["msg"] = self.msg
        return json.dumps(d)

    def from_json(self, content):
        d = json.loads(content)
        self.msg = d

    def string_print(self):
        print(self.msg)

class UpdateRecordItem:
    def __init__(self):
        self.version = ""
        self.binmap = {}
        self.msg = []

    def to_json(self):
        d = {}
        d["version"] = self.version
        d["binmap"] = self.binmap
        d["msg"] = self.msg
        return json.dumps(d)

    def from_json(self, content):
        d = json.loads(content)
        self.version = d["version"]
        self.binmap = d["binmap"]
        self.msg = d["msg"]

    def string_print(self):
        print(self.version)
        print(self.binmap)
        print(self.msg)

def main():
    try:      
        with open('a.txt', 'r') as f:
            print(f.readlines())
        # cfg = ItemCfg()  
        # cfg.from_json('{"itemName":"IHC1","hardVersion":"2","projectPathMap":{"a":"b"}}')
        # cfg.read_json()
        # cfg.print_string()
        # print(cfg.to_json())

        # msg = UpdateMsg()
        # msg.from_json('["[功能新增] 电机控制","[功能新增] 温度控制","[功能新增] 传感器读取","[功能新增] LED控制","[功能新增] 混匀功能控制","[功能新增] 通讯实现"]')
        # msg.string_print()

        # item = UpdateRecordItem()
        # item.from_json('{"version":"1.0.1","binmap":{"ihc.1-1-1.0.1":"Desinpro-ihc-route-project-a-v1.0.1"},"msg":["[功能新增] 电机控制"]}')
        # item.string_print()

        input("输入任意字符退出......")
    except Exception as err:
        print(err)

if __name__ == "__main__":
    try:
        main()
    finally:
        sys.exit()