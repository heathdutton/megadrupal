<?php

/**
 * @file
 * Handler code.
 */

/**
 * Handler for field locale_info.
 */
class SmartlingHandlerFieldLocaleInfo extends views_handler_field {

  /**
   * Render.
   *
   * @param object $values
   *   Entity.
   *
   * @return string
   *   Return locale value.
   */
  public function render($values) {
    $info = entity_get_info();
    $entity_type = '';
    foreach ($info as $ent_type => $val) {
      if ($this->table === $val['base table']) {
        $entity_type = $ent_type;
        break;
      }
    }
    if (empty($entity_type)) {
      return '';
    }

    $rid = $values->{$this->view->base_field};
    $result = smartling_entity_load_all_by_conditions(array(
      'rid' => $rid,
      'entity_type' => $entity_type
    ));

    if ($result) {
      foreach ($result as $value) {
        $langs[] = smartling_get_full_language_name($value->target_language);
      }
      return implode(', ', $langs);
    }
    else {
      return '';
    }
  }

}
