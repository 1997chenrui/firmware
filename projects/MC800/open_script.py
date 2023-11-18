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
  wk_dir = r'd:\Software\gitlab\firmware\boards\SignalGet_Disk\SignalGet0802\固件生成'
  while True:
    change_wd(wk_dir)
    str = input("板卡范围:b1 b2 1-13,q:quit;Please input idx:")
    if str == 'q':break
    # b1 boot号板
    elif str == 'b1': exec_app_open_dir(r'..\..\..\boot\Route-2',r'route-2')
    # b2 other boot号板
    elif str == 'b2': exec_app_open_dir(r'..\..\..\boot\other-2',r'other-2')
    # 1号板 路由板
    elif str == '1': exec_app_gen_open_dir(r'..\..\..\PowerCtl\PowerCtl0901',r'PowerCtl0901')
    # 2号板 信号采集板
    elif str == '2': exec_app_gen_open_dir(r'..\..\..\SignalGet_Disk\SignalGet0802',r'SignalGet0802')
    # 3号板 信号采集板 左
    elif str == '3': exec_app_gen_open_dir(r'..\..\..\SignalGet_Disk\SignalGet0802',r'SignalGet0802')
    # 4号板 温度控制板
    elif str == '4': exec_app_gen_open_dir(r'..\..\..\MotorCtl\TempCtl4002',r'TempCtl4002')
    # 5号板 液路板
    elif str == '5': exec_app_gen_open_dir(r'..\..\..\MotorCtl\FluidCtl1002',r'FluidCtl1002')
    # 6号板 样本针控制板
    elif str == '6': exec_app_gen_open_dir(r'..\..\..\MotorCtl\MotorCtl3402',r'MotorCtl3402')
    # 7号板 试剂针控制板
    elif str == '7': exec_app_gen_open_dir(r'..\..\..\MotorCtl\MotorCtl3502',r'MotorCtl3502')
    # 8号板 抓手控制板
    elif str == '8': exec_app_gen_open_dir(r'..\..\..\MotorCtl\MotorCtl3602',r'MotorCtl3602')
    # 9号板 伺服控制板（右）
    elif str == '9': exec_app_gen_open_dir(r'..\..\..\MotorCtl\MotorCtl3702',r'MotorCtl3702')
    # 10号板 伺服控制板(左)
    elif str == '10': exec_app_gen_open_dir(r'..\..\..\MotorCtl\MotorCtl3702-2',r'MotorCtl3702-2')
    # 11号板 盘片仓控制板
    elif str == '11': exec_app_gen_open_dir(r'..\..\..\MotorCtl\MotorCtl3902',r'MotorCtl3902')
    # 12号板 样本架A20控制板
    elif str == '12': exec_app_gen_open_dir(r'..\..\..\MotorCtl\MotorCtl4002',r'MotorCtl4002')
    # 13号板 样本架A21控制板
    elif str == '13': exec_app_gen_open_dir(r'..\..\..\MotorCtl\MotorCtl4002A21',r'MotorCtl4002A21')

#   wk_dir = r'..\..\boards\ihc-route\Project-A'
#   change_wd(wk_dir)
#   app_dir = r'.\MDK-ARM\ihc_route_a.uvprojx'
#   open_app(app_dir)
#   gen_dir = r'.\固件生成'
#   change_wd(gen_dir)
#   app_dir = r'.\德新普固件生成工具'
#   open_app(app_dir)