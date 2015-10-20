; Drush Make file for standard Drupal 7 site
api = 2
core = 7.x
projects[] = drupal

projects[skinr][subdir] = "contrib"
projects[jquery_ui][subdir] = "contrib"
projects[jquery_update][subdir] = "contrib"
projects[jquery_update][version] = "2.x-dev"
projects[browserclass][subdir] = "contrib"
;projects[plinko][subdir] = "contrib"
projects[mobile_tools][subdir] = "contrib"
projects[dialog][subdir] = "contrib"

projects[plinko][subdir] = "contrib"
projects[plinko][download][type] = "git"
projects[plinko][download][url] = "http://git.drupal.org/project/plinko.git"
projects[plinko][directory_name] = "plinko"

projects[plink][type] = theme
projects[plink][download][type] = "git"
projects[plink][download][url] = "http://git.drupal.org/project/plink.git"
projects[plink][directory_name] = "plink"
