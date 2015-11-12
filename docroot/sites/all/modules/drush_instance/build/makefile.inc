<?php

/**
 * @file
 * Build engine for makefile-powered sites.
 */

/**
 * @class
 */
class DrushInstanceBuildMakefile extends DrushInstanceBuildAbstract implements DrushInstanceBuildInterface {

  /**
   * Implements ::__construct().
   */
  public function __construct($alias_info) {
    $this->alias_info = $alias_info;
  }

  /**
   * Find the makefile, based on parent method for finding remote files.
   */
  private function findMakefile($instance) {
    drush_log("Looking for makefile", "ok");
    return $this->findBuildFile(
      $instance['sources']['makefile'],
      $instance['makefile_path']
    );
  }

  /**
   * Implements DrushInstanceBuildInterface::build().
   */
  public function build() {
    $alias_info = $this->alias_info;
    $instance = $alias_info['instance'];
    $rebuild = (bool)$alias_info['_is_rebuild'];

    if (!$makefile_location = $this->findMakefile($instance)) {
      throw new DrushInstanceBuildException("DRUSH_INSTANCE_MAKEFILE_NONE", "Makefile not present in source.");
    }

    // Make site
    drush_log("Making site", "ok");
    // Drush make uses make_tmp() and doesn't play nicely (directory name
    // collisions, and it seems it deletes everything when it's done)
    // So generate our own temporary location
    $site_tmp = _drush_instance_tmp_unique();

    // Run drush make
    drush_invoke('make', array($makefile_location, $site_tmp));

    // Inject a separate sites/default repository into the temporary codebase.
    $this->injectSitesDefault($instance, $site_tmp);

    // Finally, move it to the site alias root
    // If this is a rebuild, then there will be an existing file root
    if($rebuild) {
      $this->roundRobin($alias_info['root']);
    }

    // Move new build into place
    $ret = drush_move_dir($site_tmp, $alias_info['root'], FALSE);
    if (!$ret) {
      throw new DrushInstanceBuildException("DRUSH_INSTANCE_MOVE", "Cannot move instance into site alias location");
    }
    drush_log("Moved new instance in place at " . $alias_info['root'], "ok");
  }
}
