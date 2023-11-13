### 语法分析程序的设计与实现（LR 分析方法）

- `.vscode`: 编译参数的配置
- `code`: 源文件 & 可执行文件
- `grammar`: 文法文件

> 可以直接使用 VSCode 打开项目，使用`.vscode`的编译参数编译。
> 
> 亦或在命令行输入以下命令编译：
```bash g++
cd code
g++ main.cpp grammar.cpp table.cpp -o Syntax
```
> 会在code目录下得到可执行文件`Syntax`
> 
> hint: 请勿任意修改文件位置，否则可能会导致程序无法正常运行
