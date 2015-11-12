<?php

/**
 * @file
 * Build engine for composer-powered sites.
 */

/**
 * @class
 */
class DrushInstanceBuildComposer extends DrushInstanceBuildAbstract implements DrushInstanceBuildInterface {


  /**
   * Implements ::__construct().
   */
  public function __construct($alias_info) {
    $this->alias_info = $alias_info;
  }
  
  /**
   * Find composer.json in remote repository.
   */
  private function findComposerJson($instance) {
    drush_log("Looking for composer.json", "ok");
    return $this->findBuildFile(
      $instance['sources']['composer'],
      "composer.json"
    );
  }

  /**
   * Run composer in its own checkout, then extract to a temp dir.
   */
  private function composerAndExtract($json_file, $extract_to_dir) {
    // Attempt to build in situ.
    $composer_dir = dirname($json_file);
    $result = drush_shell_cd_and_exec($composer_dir, 'composer update 2>&1');
    if (!$result) {
      throw new DrushInstanceBuildException("DRUSH_INSTANCE_COMPOSER", "Composer error: message follows.\n\n" . join("\n", drush_shell_exec_output()));
    }

    // If all goes well, transfer everything to the new directory.
    // Drupal core comes straight out of vendor and into the root.
    if (file_exists($extract_to_dir)) {
      drush_delete_dir($extract_to_dir);
    }
    drush_move_dir("$composer_dir/vendor/drupal/drupal", $extract_to_dir);
    // Modules, themes and libraries all end up in sites/all.
    foreach (array("modules", "themes", "libraries") as $project_subdir) {
      $from = "$composer_dir/$project_subdir";
      $to = "$extract_to_dir/sites/all/$project_subdir";
      if (!file_exists($from)) {
        continue;
      }
      if (file_exists($to)) {
        drush_delete_dir($to);
      }
      drush_move_dir($from, $to);
    }
  }

  /**
   * Implements DrushInstanceBuildInterface::build().
   */
  public function build() {
    $alias_info = $this->alias_info;
    $instance = $alias_info['instance'];
    $rebuild = (bool)$alias_info['_is_rebuild'];

    if (!$composer_location = $this->findComposerJson($instance)) {
      throw new  DrushInstanceBuildException("DRUSH_INSTANCE_MAKEFILE_NONE", "Composer JSON not present in source.");
    }

    // Make site
    drush_log("Making site", "ok");

    // Avoid collisions - see makefile::build().
    $site_tmp = _drush_instance_tmp_unique();
    $this->composerAndExtract($composer_location, $site_tmp);

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
