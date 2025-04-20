# expctl

Expand control chars.


# To install

```sh
make clobber all
sudo make install clobber
```


# Examples

Print the message of the day with newlines and tabs expanded too:

```sh
$ /usr/local/bin//expctl -n -t < /etc/motd
```

Print the `expctl` binary in expanded control characters:

```sh
$ /usr/local/bin//expctl < /usr/lcoal/bin//expctl
```

Print the `expctl` binary as \xXX hex characters, including newlines and tabs:

```sh
$ /usr/local/bin//expctl -x -n -t < /usr/lcoal/bin//expctl
```


# To use

```
/usr/lcoal/bin//expctl [-h] [-V] [-o | -x] [-n] [-t]

    -h        print help message and exit
    -V        print version string and exit

    -o        print non-printable bytes as \0XXXX octal
    -x        print non-printable bytes as \xXX hex
    -n        print ASCII newline as if it is a control character (def: print ASCII newline)
    -t        print ASCII tab as if it is a control character (def: print ASCII tab)

NOTE: -o and -x are cannot be used as the same time

Exit codes:
    0         all OK
    2         -h and help string printed or -V and version string printed
    3         command line error
 >= 10        internal error

expctl version: 1.4.0 2025-03-30
```


# Reporting Security Issues

To report a security issue, please visit "[Reporting Security Issues](https://github.com/lcn2/expctl/security/policy)".
