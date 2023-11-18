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
    elif str == 'b1': exec_app_open_dir(r'..\..\..\boot\Route-2',r'route-2')
    # b2 other boot号板
    elif str == 'b2': exec_app_open_dir(r'..\..\..\boot\other-2',r'other-2')
    # 1号板 电源板
    elif str == '1': exec_app_gen_open_dir(r'..\..\..\PowerCtl\PowerCtl0901',r'PowerCtl0901')
    # 2号板 信号采集板
    elif str == '2': exec_app_gen_open_dir(r'..\..\..\SignalGet\SignalGet1102',r'SignalGet1102')
    # 2-1号板 新改进版本信号采集板
    elif str == '2-1': exec_app_gen_open_dir(r'..\..\..\SignalGet\SignalGet1103',r'SignalGet1103')
    # 3号板 光源板
    elif str == '3': exec_app_gen_open_dir(r'..\..\..\LampCtl\LampCtl0602',r'LampCtl0602')
    # 4号板 温控板
    elif str == '4': exec_app_gen_open_dir(r'..\..\..\MotorCtl\TempCtl1101',r'TempCtl1101')
    # 5号板 液路板
    elif str == '5': exec_app_gen_open_dir(r'..\..\..\MotorCtl\FluidCtl1202',r'FluidCtl1202')
    # 6号板 试剂样本臂板
    elif str == '6': exec_app_gen_open_dir(r'..\..\..\MotorCtl\MotorCtl4902',r'MotorCtl4902')
    # 7号板 抓手板
    elif str == '7': exec_app_gen_open_dir(r'..\..\..\MotorCtl\MotorCtl5002',r'MotorCtl5002')


#   wk_dir = r'..\..\boards\ihc-route\Project-A'
#   change_wd(wk_dir)
#   app_dir = r'.\MDK-ARM\ihc_route_a.uvprojx'
#   open_app(app_dir)
#   gen_dir = r'.\固件生成'
#   change_wd(gen_dir)
#   app_dir = r'.\德新普固件生成工具'
#   open_app(app_dir)