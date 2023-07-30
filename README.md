# 2048-zos

This is a 2048 migration for Zeal-8-bit-OS

## Install

You need to install [sdcc](https://sourceforge.net/projects/sdcc/) and [Zeal-8-bit-OS](https://github.com/Zeal8bit/Zeal-8-bit-OS) to use it.

```bash
# This will output 2048.bin, please put it into ZOS to run
cd /path/to/2048-zos/
export ZOS_PATH="/path/to/Zeal-8-bit-OS"
make

export EXTRA_ROMDISK_FILES="/path/to/2048-zos/bin/2048.bin"
# Build Zeal-8-bit-OS
```

After you build it, open your Zeal-8-bit-OS with your Zeal-8-bit Computer or Zeal-WebEmulator.

## Play

Run following commands in **Zeal-8-bit-OS:**

```bash
exec 2048.bin
```

Something you need to know:

```bash
 w
asd = Move
```

```bash
Q = Quit
```

```bash
S = Save
```

Sit back and relax!

## Thanks

[Zeal 8-bit](https://zeal8bit.com)

[程序媛阿薇](https://www.zhihu.com/people/cheng-xu-yuan-a-wei)
