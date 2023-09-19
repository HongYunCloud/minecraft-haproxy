this is not official version of HAProxy, NEVER submit issue to HAProxy issues
and email list.

这不是 HAProxy 的官方版本，绝对不要提交问题到 HAProxy issues 及邮件列表。

# Minecraft-HAProxy
1. 负载均衡：HAProxy可以根据不同的算法（如轮询，服务器响应时间，会话数量等）自动分发服务器的请求负载。

2. 高可用性：如果其中一个服务器宕机，HAProxy可以自动将请求路由到另一个在线的服务器。

3. 安全性：HAProxy可以防止常见的四层攻击，如Dos攻击。

4. 虚拟主机：通过客户端输入的服务器地址，将连接分到不同的后端进行相应，使得一个端口开多种不同的服务器。

5. 多服务共享端口：HAProxy可以通过数据包猜测连接类型，在同一个端口上运行多个协议（如HTTP, SSL, SMTP, Minecraft）。

6. 透明代理：HAProxy可以设置为透明代理模式，将客户端的IP直接传递到Minecraft服务器。

## 添加的采样项目

- `req.mc_ext` `BOOL` 是否为 minecraft 协议
- `req.mc_ver` `SINT` minecraft 协议版本
- `req.mc_sni` `STR` 用于连接的主机名或者IP地址，例如 localhost 或者 127.0.0.1。
- `req.mc_port` `SINT` 用于连接的端口。
- `req.mc_state` `SINT` 1 为获取 STATUS, 2 为加入服务器。

需要注意的是，SRV记录只是一个简单的重定向。例如，`_minecraft._tcp.example.com` 指向 `mc.example.org`，那么连接到 `example.com` 的用户会同样连接到 `mc.example.org`，并将 `mc.example.org` 作为服务器地址。

## 配置示例
```
global
    log stdout format raw local0 info
    strict-limits

defaults
    log global
    timeout client 60s
    timeout server 60s
    timeout connect 1s

frontend ft_minecraft
    mode tcp
    # option tcplog
    bind :25565

    acl app1 req.mc_sni -i app1.example.com
    acl app2 req.mc_sni -i app2.example.com
    
    tcp-request inspect-delay 5s
    
    use_backend bk_app1 if app1
    use_backend bk_app2 if app2
    default_backend bk_default

backend bk_app1
    mode tcp
    balance roundrobin

    server server1 127.0.0.1:10101 check
    server server2 127.0.0.1:10102 check

backend bk_app2
    mode tcp
    balance roundrobin

    server server1 127.0.0.1:10201 check
    server server2 127.0.0.1:10202 check

backend bk_default
    mode tcp
    balance roundrobin

    server server1 127.0.0.1:10301 check
    server server2 127.0.0.1:10302 check
```

在此配置中，Minecraft-Haproxy 将会绑定 25565 端口。

客户端如使用 `app1.example.com` 来加入服务器，则会根据 `roundrobin` 算法转发到 `127.0.0.1:10101`、 `127.0.0.1:10102` 中的一个。

客户端如使用 `app2.example.com` 来加入服务器，则会根据 `roundrobin` 算法转发到 `127.0.0.1:10201`、 `127.0.0.1:10202` 中的一个。

客户端如使用其他域名来加入服务器，则会根据 `roundrobin` 算法转发到 `127.0.0.1:10301`、 `127.0.0.1:10302` 中的一个。

# HAPROXY README
```
The HAProxy documentation has been split into a number of different files for
ease of use.

Please refer to the following files depending on what you're looking for :

  - INSTALL for instructions on how to build and install HAProxy
  - BRANCHES to understand the project's life cycle and what version to use
  - LICENSE for the project's license
  - CONTRIBUTING for the process to follow to submit contributions

The more detailed documentation is located into the doc/ directory :

  - doc/intro.txt for a quick introduction on HAProxy
  - doc/configuration.txt for the configuration's reference manual
  - doc/lua.txt for the Lua's reference manual
  - doc/SPOE.txt for how to use the SPOE engine
  - doc/network-namespaces.txt for how to use network namespaces under Linux
  - doc/management.txt for the management guide
  - doc/regression-testing.txt for how to use the regression testing suite
  - doc/peers.txt for the peers protocol reference
  - doc/coding-style.txt for how to adopt HAProxy's coding style
  - doc/internals for developer-specific documentation (not all up to date)
```

# License
使用 `HAPROXY` 协议授权，请参见 [LICENSE](LICENSE)。

可导出的 `include` 使用 `LGPLv2` 授权，其余部分使用 `GPLv2` 授权。