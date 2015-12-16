; PP Crop

api = 2
core = 7.x

projects[file_entity][subdir] = "contrib"
projects[file_entity][version] = "2.0-alpha3"

projects[epsacrop][type] = "module"
projects[epsacrop][subdir] = "contrib"
projects[epsacrop][download][type] = "git"
projects[epsacrop][download][url] = "http://git.drupal.org/project/epsacrop.git"
projects[epsacrop][download][branch] = "7.x-2.x"
projects[epsacrop][download][revision] = "0b018a52af689132059ceec4c532bc1b79609634"
projects[epsacrop][patch][] = "https://drupal.org/files/crop_style_from_code-1396500-19.patch"
projects[epsacrop][patch][] = "https://www.drupal.org/files/issues/epsacrop-styles_list_alter-2077809-2.patch"
