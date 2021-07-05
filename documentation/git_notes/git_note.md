# git cmd

## 用户名设置

项目级别仓库用户名设置：

* git config user.name Ericyinwork

* git config user.email ericyinwork@163.com

查看仓库用户名设置

* cat .git/config

系统级别仓库用户名设置：
* git config --global user.name Ericyinwork
* git config --global user.email ericyinwork@163.com

查看系统仓库用户名设置

* cat ~/.gitconfig

## 基本操作

* git status									查看工作区、暂存区状态

* git add ./git add [文件名]		提交到暂存区，并且转换换行符
* git rm -cached [文件名]		     从暂存区撤回
* git commit -m"commit message" [file name]	将暂存区的内容提交到本地库
* git restore [文件名]			  	撤销某个文件修改的操作
* git reset HEAD [file]			  可以吧暂存区的修改撤销掉（unstage），重新放回工作区
### 查看历史记录
* git log 					   查看版本历史记录
* git log --pretty=oneline		 每个历史只显示一行（hash值和日志）
* git log --oneline			   每个历史只显示一行且显示hash的部分值
* git reflog				  现实历史只显示一行，并且显示指针（要移动到版本多少步）
### 版本前进后退
* git reset --hard [局部索引值]	
* git reset --hard HEAD^^		注：一个^表示后退一步，n 个^表示后退 n 步
* git reset --hard HEAD~n		注：表示后退 n 步
### 比较文件差异
git diff [文件名]				  将工作区中的文件和暂存区进行比较
git diff [本地库中的文件和本地库历史记录比较]
## 分支管理



## Vim

创建文本		vim 文件名.txt  

 保存并退出	Esc  :wq  回车  



## 设置签名

用户名：Ericyinwork

Email地址：ericyinwork@163.com

作用:区分不同开发人员的身份

辨析：与登录远程库（代码托管中心）账号密码没有任何关系

命令：

项目级别仅在本地仓库有效

系统级别：登录当前操作系统用户范围

## 异常报错解决

![image-20210617142606742](F:\rt-thread\documentation\git_notes\git_note.assets\image-20210617142606742.png)

重新登录

$ git config --global http.sslVerify "false"

