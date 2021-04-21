# STM32
All STM32 Project
instruction of using git:
 ---when uploading a new Keil Project---
     1,go to the path of you git folder(where has ".gt" folder and ".gitignore" file)
     2,right click "Git Bash Here"
     3,type "git add + 'project name' "
     4,type "git commit -m "XXXXXX" "
     5,"git push origin master"
     
 update existing project:
 1, go to git path and right click "Git Bash Here"
 2,XXXX means project name
 $ git add XXXX
 3,XXXX means the comment you want to add on changed files
 $ git commit -m XXXX 
 4,
 $ git push -u origin master 
 update online changed code to local:

1,check remote Repository
$ git remote -v

2,get remote version to local and copy as "temp"
$ git fetch origin master:temp

3,compare the difference between temp and local version
$ git diff temp

4, merge temp to master
$ git merge temp

5, delete temp branch if you don't need it anymore 
$ git branch -d temp
