<?php
/**
 * @file
 * Interview sub entity controller class.
 */

abstract class ConsultInterviewSubEntityController extends EntityAPIControllerExportable {
  /**
   * Remove interview related values from sub entities.
   */
  public function export($entity, $prefix = '') {
    $vars = get_object_vars($entity);
    unset($vars[$this->statusKey], $vars[$this->moduleKey], $vars['is_new']);
    if ($this->nameKey != $this->idKey) {
      unset($vars[$this->idKey]);
    }

    // Remove interview data that was added to the sub entity for easy access.
    unset($vars['interview']);
    unset($vars['weight']);
    unset($vars['enabled']);
    unset($vars['shown']);

    return entity_var_json_export($vars, $prefix);
  }
}
