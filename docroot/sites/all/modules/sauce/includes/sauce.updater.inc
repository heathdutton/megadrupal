<?php

/**
 * @file
 * Subclasses of the Updater class to update Drupal core knows how to update.
 * At this time, only modules and themes are supported.
 */

/**
 * Class for updating modules using FileTransfer classes via authorize.php.
 */
class LibraryUpdater extends Updater implements DrupalUpdaterInterface {

  /**
   * Return the directory where a module should be installed.
   *
   * If the module is already installed, drupal_get_path() will return
   * a valid path and we should install it there (although we need to use an
   * absolute path, so we prepend DRUPAL_ROOT). If we're installing a new
   * module, we always want it to go into sites/all/modules, since that's
   * where all the documentation recommends users install their modules, and
   * there's no way that can conflict on a multi-site installation, since
   * the Update manager won't let you install a new module if it's already
   * found on your system, and if there was a copy in sites/all, we'd see it.
   */
  public function getInstallDirectory() {
    if ($relative_path = libraries_get_path($this->name)) {
      $relative_path = dirname($relative_path);
    }
    else {
      $relative_path = 'sites/all/libraries';
    }
    watchdog("sauce_updater_info","relative_path $relative_path");
    return DRUPAL_ROOT . '/' . $relative_path;
  }

  public function isInstalled() {
    return (bool) libraries_get_path($this->name);
  }

  public static function canUpdateDirectory($directory) {
    if (file_scan_directory($directory, '/.*\.info$/')) {
      watchdog("sauce_updater_info","found info file");
      return FALSE;
    }
    return TRUE;
  }

  public static function canUpdate($project_name) {
    return (bool) libraries_get_path($project_name);
  }

  /**
   * Return available database schema updates one a new version is installed.
   */
  public function getSchemaUpdates() {
    return array();
  }

  /**
   * Returns a list of post install actions.
   */
  public function postInstallTasks() {
    return array(
      l(t('Administration pages'), 'admin'),
    );
  }

  public function postUpdateTasks() {
    // We don't want to check for DB updates here, we do that once for all
    // updated modules on the landing page.
  }

}