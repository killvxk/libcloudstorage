#ifndef FUSE_HIGH_LEVEL_H
#define FUSE_HIGH_LEVEL_H

#ifdef WITH_LEGACY_FUSE

#ifndef FUSE_USE_VERSION
#define FUSE_USE_VERSION 26
#endif

#include <fuse.h>

#include <json/json.h>

namespace cloudstorage {

struct fuse_cmdline_opts {
  int clone_fd;
  int singlethread;
  int foreground;
  int show_help;
  int show_version;
  char *mountpoint;
};

struct FuseHighLevel {
  FuseHighLevel(fuse_args *args, const char *mountpoint, void *userdata);
  ~FuseHighLevel();

  int run(bool singlethread, bool) const;

  fuse *fuse_;
  fuse_chan *channel_;
  fuse_session *session_;
  std::string mountpoint_;
};

int fuse_parse_cmdline(struct fuse_args *, fuse_cmdline_opts *);
void fuse_cmdline_help();
fuse_operations high_level_operations();

}  // namespace cloudstorage

#endif  // WITH_LEGACY_FUSE
#endif  // FUSE_HIGH_LEVEL_H
