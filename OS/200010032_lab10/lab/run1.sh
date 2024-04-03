# copying include files
cp const.h /usr/src/minix/include/minix/const.h
# copying mfs files
cp ./mfs/link.c /usr/src/minix/fs/mfs/link.c
cp ./mfs/misc.c /usr/src/minix/fs/mfs/misc.c
cp ./mfs/read.c /usr/src/minix/fs/mfs/read.c
cp ./mfs/write.c /usr/src/minix/fs/mfs/write.c
cp ./mfs/table.c /usr/src/minix/fs/mfs/table.c
cp ./mfs/proto.h /usr/src/minix/fs/mfs/proto.h
# copying vfs files
cp ./vfs/link.c /usr/src/minix/servers/vfs/link.c
cp ./vfs/open.c /usr/src/minix/servers/vfs/open.c
# copying lib files
cp fslib.c /usr/src/minix/lib/libc/gen/fslib.c

cd /usr/src/
make build MKUPDATE=yes
