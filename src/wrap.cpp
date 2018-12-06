//
//  wrap.cpp
//  myfs
//
//  Created by Oliver Waldhorst on 02.08.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#include "wrap.h"
#include "myfs.h"

int wrap_getattr(const char *path, struct stat *statbuf) {
    return MyFS::Instance()->fuseGetattr(path, statbuf);
}

int wrap_readlink(const char *path, char *link, size_t size) {
    return MyFS::Instance()->fuseReadlink(path, link, size);
}

int wrap_mknod(const char *path, mode_t mode, dev_t dev) {
    return MyFS::Instance()->fuseMknod(path, mode, dev);
}
int wrap_mkdir(const char *path, mode_t mode) {
    return MyFS::Instance()->fuseMkdir(path, mode);
}
int wrap_unlink(const char *path) {
    return MyFS::Instance()->fuseUnlink(path);
}
int wrap_rmdir(const char *path) {
    return MyFS::Instance()->fuseRmdir(path);
}
int wrap_symlink(const char *path, const char *link) {
    return MyFS::Instance()->fuseSymlink(path, link);
}
int wrap_rename(const char *path, const char *newpath) {
    return MyFS::Instance()->fuseRename(path, newpath);
}
int wrap_link(const char *path, const char *newpath) {
    return MyFS::Instance()->fuseLink(path, newpath);
}
int wrap_chmod(const char *path, mode_t mode) {
    return MyFS::Instance()->fuseChmod(path, mode);
}
int wrap_chown(const char *path, uid_t uid, gid_t gid) {
    return MyFS::Instance()->fuseChown(path, uid, gid);
}
int wrap_truncate(const char *path, off_t newSize) {
    return MyFS::Instance()->fuseTruncate(path, newSize);
}
int wrap_utime(const char *path, struct utimbuf *ubuf) {
    return MyFS::Instance()->fuseUtime(path, ubuf);
}
int wrap_open(const char *path, struct fuse_file_info *fileInfo) {
    return MyFS::Instance()->fuseOpen(path, fileInfo);
}
int wrap_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    return MyFS::Instance()->fuseRead(path, buf, size, offset, fileInfo);
}
int wrap_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    return MyFS::Instance()->fuseWrite(path, buf, size, offset, fileInfo);
}
int wrap_statfs(const char *path, struct statvfs *statInfo) {
    return MyFS::Instance()->fuseStatfs(path, statInfo);
}
int wrap_flush(const char *path, struct fuse_file_info *fileInfo) {
    return MyFS::Instance()->fuseFlush(path, fileInfo);
}
int wrap_release(const char *path, struct fuse_file_info *fileInfo) {
    return MyFS::Instance()->fuseRelease(path, fileInfo);
}
int wrap_fsync(const char *path, int datasync, struct fuse_file_info *fi) {
    return MyFS::Instance()->fuseFsync(path, datasync, fi);
}
#ifdef __APPLE__
int wrap_setxattr(const char *path, const char *name, const char *value, size_t size, int flags, uint32_t x) {
    return MyFS::Instance()->fuseSetxattr(path, name, value, size, flags, x);
}
int wrap_getxattr(const char *path, const char *name, char *value, size_t size, uint x) {
    return MyFS::Instance()->fuseGetxattr(path, name, value, size, x);
}
#else
int wrap_setxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
    return MyFS::Instance()->fuseSetxattr(path, name, value, size, flags);
}
int wrap_getxattr(const char *path, const char *name, char *value, size_t size) {
    return MyFS::Instance()->fuseGetxattr(path, name, value, size);
}
#endif
void* wrap_init(struct fuse_conn_info *conn) {
    return MyFS::Instance()->fuseInit(conn);
}
int wrap_listxattr(const char *path, char *list, size_t size) {
    return MyFS::Instance()->fuseListxattr(path, list, size);
}
int wrap_removexattr(const char *path, const char *name) {
    return MyFS::Instance()->fuseRemovexattr(path, name);
}
int wrap_opendir(const char *path, struct fuse_file_info *fileInfo) {
    return MyFS::Instance()->fuseOpendir(path, fileInfo);
}
int wrap_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    return MyFS::Instance()->fuseReaddir(path, buf, filler, offset, fileInfo);
}
int wrap_releasedir(const char *path, struct fuse_file_info *fileInfo) {
    return MyFS::Instance()->fuseReleasedir(path, fileInfo);
}
int wrap_fsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo) {
    return MyFS::Instance()->fuseFsyncdir(path, datasync, fileInfo);
}
int wrap_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    return MyFS::Instance()->fuseCreate(path, mode, fi);
}
void wrap_destroy(void *userdata) {
    MyFS::Instance()->fuseDestroy();
}
