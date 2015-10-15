<?php
/**
 * @file
 * Subclasses of the ModuleUpdater class to update modules.
 */

module_load_include('inc', 'module_install', 'module_install.api');
module_load_include('inc', 'system', 'system.updater');

/**
 * Extends the default ModuleUpdater to change the destination install path.
 */
class ModuleInstallUpdater extends ModuleUpdater {

  /**
   * Returns the install directory.
   *
   * @return string
   *   The path to the install directory.
   */
  public function getInstallDirectory() {
    if ($relative_path = drupal_get_path('module', $this->name)) {
      $relative_path = dirname($relative_path);
    }
    else {
      $relative_path = module_install_get_destination_choice('modules');
    }
    return DRUPAL_ROOT . '/' . $relative_path;
  }

  public function postInstall() {
    module_install_del_destination_choice();
    return parent::postInstall();
  }

}
