<?php
namespace Drupal\go\Drush;

/**
 * Override some default behaviors of parent class.
 * At current, happening error when enable go module
 * Error: Class 'DrushMakeProject_Library' not found in ...
 * Require class 'DrushMakeProject_Library' at sites/all/modules/drush/commands/make/make.project.inc, line 453
 *
 * @see http://api.drupalize.me/api/drupal/class/DrushMakeProject_Library/7
 */
class GoLibrary extends \DrushMakeProject_Library {
  /**
   * Override default value of parent.
   */
  protected function generatePath() {
   return parent::generatePath($base = FALSE);
  }
}
