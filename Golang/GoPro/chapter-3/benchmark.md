# 如何进行benchmark测试

1. 创建module
    go mod init <module name>

    然后添加go文件到这个module当中

2. 编写*_test.go文件
    eg：loop_benchmark_test.go

3. 运行go -bench=.
    -count=? 表示运行几次



    待补充