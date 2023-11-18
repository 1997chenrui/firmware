#coding=utf-8
#!/usr/bin/python
import os
# 切换工作目录
def change_wd(wd):
    os.chdir(wd)
# 打开app
def open_app(app_dir):
  os.startfile(app_dir)
# 打开文件夹
def open_dir(path):
  os.system("explorer.exe %s" % path)
# 打开app和固件生成工具
def exec_app_gen_open_dir(wk_dir, proj_name):
  change_wd(wk_dir)
  app_dir = r'.\MDK-ARM\{}.uvprojx'.format(proj_name)
  open_app(app_dir)
  gen_dir = r'.\固件生成'
  open_dir(gen_dir)

# 打开app和文件资源管理器
def exec_app_open_dir(wk_dir, proj_name):
  change_wd(wk_dir)
  app_path = r'.\MDK-ARM'
  open_dir(app_path)
  app_dir = r'.\MDK-ARM\{}.uvprojx'.format(proj_name)
  open_app(app_dir)

if __name__ == "__main__":
  wk_dir = r'd:\Software\gitlab\firmware\boards\PowerCtl\PowerCtl0901\固件生成'
  while True:
    change_wd(wk_dir)
    str = input("id:1,2,3,4,5,6,7,q;Please input id:")
    if str == 'q':break
    # b1 boot号板
    elif str == 'b1': exec_app_open_dir(r'..\..\..\boot\Route',r'route')
    # b2 other boot号板
    elif str == 'b2': exec_app_open_dir(r'..\..\..\boot\other',r'other')
    # 1号板 电源板
    elif str == '1': exec_app_gen_open_dir(r'..\..\..\PowerCtl\MB0201A01',r'MB0201A01')
    # 2号板 样本瓶抓手板
    elif str == '2': exec_app_gen_open_dir(r'..\..\..\MotorCtl\MB0201A02',r'MB0201A02')
    # 3号板 吸附臂组板
    elif str == '3': exec_app_gen_open_dir(r'..\..\..\MotorCtl\MB0201A03',r'MB0201A03')
    # 4号板 玻片抓手板
    elif str == '4': exec_app_gen_open_dir(r'..\..\..\MotorCtl\MB0201A04',r'MB0201A04')
    # 5号板 吸附板
    elif str == '5': exec_app_gen_open_dir(r'..\..\..\MotorCtl\MB0201A05',r'MB0201A05')
    # 6号板 染色液路板
    elif str == '6': exec_app_gen_open_dir(r'..\..\..\MotorCtl\MB0201A06',r'MB0201A06')
    # 7号板 信号板
    elif str == '7': exec_app_gen_open_dir(r'..\..\..\MotorCtl\SignalGet1201',r'SignalGet1201')


#   wk_dir = r'..\..\boards\ihc-route\Project-A'
#   change_wd(wk_dir)
#   app_dir = r'.\MDK-ARM\ihc_route_a.uvprojx'
#   open_app(app_dir)
#   gen_dir = r'.\固件生成'
#   change_wd(gen_dir)
#   app_dir = r'.\德新普固件生成工具'
#   open_app(app_dir)