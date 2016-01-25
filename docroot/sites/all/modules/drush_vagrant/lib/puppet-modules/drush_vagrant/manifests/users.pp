class drush_vagrant::users ($users = '') {

  if $users != '' {
    drush_vagrant::user { [$users]: }
  }
  drush_vagrant::user {'root': home_dir => '/root', }

}
