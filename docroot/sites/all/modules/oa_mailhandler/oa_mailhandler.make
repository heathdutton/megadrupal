; Open Atrium Mailhandler Makefile

api = 2
core = 7.x

projects[mailhandler][version] = 2.9
projects[mailhandler][subdir] = contrib

projects[mailcomment][version] = 2.x-dev
projects[mailcomment][subdir] = contrib
projects[mailcomment][download][type] = git
projects[mailcomment][download][branch] = 7.x-2.x
projects[mailcomment][download][revision] = 2a7b52

projects[feeds_comment_processor][version] = 1.0-beta1
projects[feeds_comment_processor][subdir] = contrib
projects[feeds_comment_processor][patch][2288317] = https://www.drupal.org/files/issues/2288317-3.patch
