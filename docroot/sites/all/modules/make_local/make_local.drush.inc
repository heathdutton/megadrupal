<?php

/**
 * Custom drush make download mechanism for sourcing local directories.
 */
function make_download_local($name, $download, $location) {
  $path = $download['source'];

  if (is_dir($path) || is_file($path)) {
    drush_log(dt('@project found in @url.', array('@project' => $name, '@url' => $path)), 'ok');
    return drush_copy_dir($path, $location, TRUE);
  }

  return drush_set_error('DRUSH_BAD_PATH', dt('Could not evaluate source path !path.', array('!path' => $path)));
}


/**
 * Custom project type handling.
 */
class DrushMakeProject_Local extends DrushMakeProject_Library {

  /**
   * Override contructor so that projects can decide where they belong
   * using 'subdir', relative to the Drupal root.
   */
   protected function __construct(&$project) {
     parent::__construct($project);
     $this->contrib_destination = '';
   }
}
