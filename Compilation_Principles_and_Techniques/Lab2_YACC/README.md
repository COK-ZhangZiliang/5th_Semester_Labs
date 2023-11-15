### 语法分析程序的设计与实现（YACC实现）
 
- `code`: 源文件
- `Syntax.exe`: 可执行文件

> 可以通过以下指令编译：
```bash
bison --yacc -d syntax.y
flex lexer.l
gcc lex.yy.c y.tab.c -o Syntax
```
编译前确保已经安装`flex`和`bison`并加入到了系统的环境变量中。