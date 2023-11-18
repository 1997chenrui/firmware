#coding=utf-8
#!/usr/bin/python
import os
# 切换工作目录
def change_wd(wd):
    os.chdir(wd)
# 打开app
def open_app(app_dir):
  os.startfile(app_dir)
# 打开app和固件生成工具
def exec(wk_dir, proj_name):
  change_wd(wk_dir)
  app_dir = r'.\MDK-ARM\{}.uvprojx'.format(proj_name)
  open_app(app_dir)
  gen_dir = r'.\固件生成'
  # 必须保证根目录在固件生成目录
  change_wd(gen_dir)
  app_dir = r'.\德新普固件生成工具'
  open_app(app_dir)
if __name__ == "__main__":
  wk_dir = r'd:\Software\gitlab\firmware\boards\ihc-route\Project-A\固件生成'
  change_wd(wk_dir)
  while True:
    str = input("板卡范围:1-5,q:quit;Please input idx:")
    if str == 'q':break
    # 1号板
    elif str == '1': exec(r'..\..\..\ihc-route\Project-A',r'ihc_route_a')
    # 2号板
    elif str == '2': exec(r'..\..\..\ihc-slideframe\Project-A',r'ihc_slide_frame_a')
    # 3号板
    elif str == '3': exec(r'..\..\..\ihc-motorr\Project-A',r'ihc-motorr-a')
    # 4号板
    elif str == '4': exec(r'..\..\..\ihc-motorhand\Project-A',r'ihc-motorhand-a')
    # 5号板
    elif str == '5': exec(r'..\..\..\ihc-mixing\Project-A',r'ihc-mixing-a')

#   wk_dir = r'..\..\boards\ihc-route\Project-A'
#   change_wd(wk_dir)
#   app_dir = r'.\MDK-ARM\ihc_route_a.uvprojx'
#   open_app(app_dir)
#   gen_dir = r'.\固件生成'
#   change_wd(gen_dir)
#   app_dir = r'.\德新普固件生成工具'
#   open_app(app_dir)