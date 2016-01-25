<?php

/**
 * @file
 * Git Controller Exportable class.
 *
 * Derived from https://github.com/skwashd/tec/blob/master/includes/tec.controller.exportable.inc
 */

/**
 * Git Entity Controller class with exportable support.
 */
class GitControllerExportable extends EntityAPIControllerExportable {
  /**
   * Implements EntityAPIControllerInterface.
   */
  public function export($entity, $prefix = '') {
    $vars = get_object_vars($entity);
    unset(
      $vars[$this->statusKey],
      $vars[$this->moduleKey],
      $vars['is_new'],
      $vars['created'],
      $vars['modified'],
      $vars['status']
    );
    return entity_var_json_export($vars, $prefix);
  }
}
