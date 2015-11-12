<?php

/**
 * @file
 * Switch between different build methods.
 */

/**
 * Interfaces, abstract objects and exceptions.
 */

interface DrushInstanceBuildInterface {
  public function build();
}

class DrushInstanceBuildException extends Exception {
  public function __construct($type, $message) {
    $this->type = $type;
    $this->_message = $message;
    parent::__construct($message);
  }
}

abstract class DrushInstanceBuildAbstract {

  // Directory of old build, if rebuilt.
  public $old_dir = NULL;

  /**
   * Inject sites default into a temporary build directory.
   */
  protected function injectSitesDefault($instance, $site_tmp) {
    // Replace sites/default with source specified, defaulting to
    // the makefile source (see validation hook)
    $sites_default = "$site_tmp/sites/default";
    drush_delete_dir($sites_default);
    // Assume deployed codebase should always be a working copy, so hot-swap a
    // TRUE value into that drush_get_option before downloading from source
    $old_wc = drush_get_option('working-copy');
    drush_set_option('working-copy', TRUE);
    _drush_instance_make_download_factory(
      'Sites-default',
      $instance['sources']['sites_default'],
      $sites_default
    );
    // Swap the old option for working-copy back in
    drush_set_option('working-copy', $old_wc);
    drush_log("Replaced sites/default with relevant source", "ok");
  }

  /**
   * Move an old site out of the way to make way for a new one.
   */
  protected function roundRobin($root) {
    // Move it to one side and report success or failure
    $this->old_dir = "$root-old-" . rand(1, 99999);
    $ret = drush_move_dir($root, $this->old_dir);
    if (!$ret) {
      throw new DrushInstanceBuildException("DRUSH_INSTANCE_MOVE", "Cannot move existing instance aside in order to rebuild");
    }
    drush_log("Moved existing instance aside to {$this->old_dir}", "ok");
  }

  /**
   * Find a build file in a remote repository.
   */
  protected function findBuildFile($repository, $filename) {
    $master_tmp = make_tmp();
    $makefile_tmp = "$master_tmp/makefile";
    _drush_instance_make_download_factory(
      'Instance-makefile',
      $repository,
      $makefile_tmp
    );
    $makefile_location = "$makefile_tmp/" . $filename;
    if(file_exists($makefile_location)) {
      return $makefile_location;
    }
  }

}

/**
 * Return a build method object.
 */
function instance_build_switcher_get_builder($which, $data) {
  drush_include_engine("build", $which);
  $class = "DrushInstanceBuild$which";
  return new $class($data);
}
