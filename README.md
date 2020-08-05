# tfb - Trim Files from Beginning

## Introduction

`tfb` is a simple utility program to trim file size, starting at the beginning of the file. It was originally inspired by the `truncate` utility, but unlike truncate it removes blocks of data starting at the beginning of a file.

`tfb` operates on Linux-based operating system and will successfully trim files on **ext4** or **XFS** file systems. It uses Linux-specific **fallocate()** syscall, similar to **fallocate** utility. Due to filesystem limitations data will be trimmed in multiples of filesystem's logical block size (usually 4096 bytes on ext4).

`tfb` was developed to reduce administrative efforts when dealing with large log files, when historical data is of minimal significance.

## Options

`tfb` supports the following options:
* `-h`: Prints help message.
* `-s SIZE`: **SIZE** of the data to be removed from the beginning of the file. **SIZE** must be an non-zero positive integer followed by a **units** suffix. **Units** are B,K,M,G.
* `-f FILE`: **FILE** to be trimmed from the beginning. The **FILE** must be located on an **ext4** or **XFS** filesystem and the current user must have write access to it.

## Usage

`tfb` usage is fairly straight-forward.

To trim 1MB (1024KB / 1048576 bytes) from the beginnig of a file use:

```
tfb -s 1M -f /var/log/myapp.log
```

## Build Instructions

`tfb` is intentionally minimalistic. It can be build using GCC on Linux 3.x and onwards distributions. Only the standard library is used.

`.deb` package building is currently supported.

`.rpm` will be added later.

## License

[BSD 2-Clause License](https://tldrlegal.com/license/bsd-2-clause-license-(freebsd))
