# 创建
git init

# 保存
git add .
git commit -m "description"
git push

# 查看，回退
git status
git log --oneline
git log
git checkout <commit-hash>
git checkout master
(或者 git checkout main，取决于主分支叫啥)
git reset --hard <commit-hash>

# 对比代码差异
- 如果想知道从 01114ea 到 d6b4810 这两次提交之间，到底改了哪些文件的哪几行代码
 （老版本写在前面，新版本写在后面）：
  git diff 01114ea d6b4810
- 只看“概览”（改了哪些文件）：
  git diff 01114ea d6b4810 --stat
- 只对比“某一个文件”的差异（命令格式：git diff 老版本 新版本 文件的具体路径）：
  git diff 01114ea d6b4810 payment.js

# 在 GitHub 生成 Token
1. 登录你的 GitHub 网页。
2. 点击右上角的个人头像，在下拉菜单中选择 Settings（设置）。
3. 在左侧菜单栏一直往下滑到底，点击 Developer settings（开发者设置）。
4. 在左侧点击 Personal access tokens，然后选择 Tokens (classic)。
5. 点击页面右上角的 Generate new token，选择 Generate new token (classic)
  （此时可能会让你输入一次网页登录密码验证身份）。
6. 填写配置：Note（备注）： 随便写，比如 Linux-Git-Token。
   Expiration（有效期）： 可以选 30 days 或 No expiration（永不过期）。
7. Select scopes（权限） 非常重要： 找到并勾选 repo 这一项（勾选主选项即可，它会自动包含底下的子项）。
   这代表你允许这个 Token 操作你的代码库。
8. 滑到页面最底部，点击绿色的 Generate token。
9. 复制

# 首次推送到远程网站仓库，建立关联
git remote add origin https://github.com/YYWD2007/mario-pro2.git
git branch -M main
git config pull.rebase false
git pull origin main --allow-unrelated-histories
git push -u origin main

# 合作
GitHub 仓库的 Settings -> Collaborators（协作者）里，把他的 GitHub 账号加进来。
这样，他就可以用他自己的 Token，直接 git push 到你的仓库里了。