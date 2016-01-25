define drush_vagrant::user ($home_dir = '') {
  # We can't retrieve the resource name in the paramaters themselves to define
  # a default parameter. That is, the following doesn't work, despite indica-
  # tions that it should in the Puppet docs:
  # define drush_vagrant::user ($home_dir = "/home/$name")
  # define drush_vagrant::user ($home_dir = "/home/$title")
  # ... So, we use a wrapper definition, and then pass along the $name in the home_dir, if required.
  if ($home_dir == '') {
    $home = "/home/${name}"
  }
  else {
    $home = $home_dir
  }
  drush_vagrant::user_account {$name:
    home_dir => $home,
  }
}
