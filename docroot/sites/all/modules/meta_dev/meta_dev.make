; Drush make file for meta_dev dependencies.

api = 2
core = "7.x"

projects[] = drupal

projects[coder][version] = "2.5"
projects[coder][subdir] = "dev"

projects[devel][version] = "1.5"
projects[devel][subdir] = "dev"

projects[devel_contrib][version] = "1.0"
projects[devel_contrib][subdir] = "dev"

projects[dev_roles_users][version] = "1.0-beta1"
projects[dev_roles_users][subdir] = "dev"

projects[masquerade][version] = "1.0-rc7"
projects[masquerade][subdir] = "dev"

projects[search_krumo][version] = "1.6"
projects[search_krumo][subdir] = "dev"

