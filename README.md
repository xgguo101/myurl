# myurl 说明

简单的 HTTP 客户端。不支持 HTTPS。  
命令选项和 curl 差不多。

## 安装

支持 MacOS 和 Linux。

```
$ cd myurl

$ make

$ make install
```

在当前文件夹会生成 myurl 可执行文件。

## 参数说明

1. -I

-I 参数向服务器发出 HEAD 请求，然会将服务器返回的 HTTP 标头打印出来。

```
$ myurl  -I  httpbin.org
```

2. -X

-X 参数指定 HTTP 请求的方法。

```
$ myurl -X PUT httpbin.org

$ myurl -X POST httpbin.org
```

3. -H

-H 参数添加 HTTP 请求的标头。

```
$ myurl -H "X-Forwarded-For: 10.10.10.1"  POST httpbin.org

添加多个字段:
$ myurl -H "X-Forwarded-For: 10.10.10.1"  -H "User-Agent: test" httpbin.org
```

4. -d

-d 参数用于发送 POST 请求的数据体。

```
$ myurl -d id=1 httpbin.org

发送多个字段：
$ myurl -d id=1 -d name=dd -d "test=ad" httpbin.org
```

使用 -d 参数以后，HTTP 请求会自动加上标头 Content-Type : application/x-www-form-urlencoded。并且会自动将请求转为 POST 方法，因此可以省略 -X POST。

5. -v

-v 参数输出通信的整个过程，用于调试。

```
$ myurl -v httpbin.org
```

6. -h

-h 显示帮助信息。

```
$ myurl -h
```

7. -V

-V 显示当前 myurl 版本.

```
$ myurl -V
```
