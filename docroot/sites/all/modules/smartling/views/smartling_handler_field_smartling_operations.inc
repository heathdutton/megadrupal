<?php

/**
 * @file
 * Handler code.
 */

/**
 * Handler for field locale_info.
 */
class SmartlingHandlerFieldSmartlingOperations extends views_handler_field {

  /**
   * Render.
   *
   * @param object $values
   *   Smartling entity.
   *
   * @return string
   *   Return operations.
   */
  public function render($values) {
    $output = '';

    $entity_type = $values->smartling_entity_data_entity_type;
    $rid = $values->smartling_entity_data_rid;
    $eid = $values->eid;
    $d_locale = $values->smartling_entity_data_target_language;
    $destination = 'admin/content/smartling-submissions';

    $options = array('query' => array('destination' => $destination));

    switch ($entity_type) {
      case 'node':
        $output = l(t('Edit'), 'node/' . $rid . '/edit/' . $d_locale);
        break;

      case 'taxonomy_term':
        $output = l(t('Edit'), 'taxonomy/term/' . $rid . '/edit/' . $d_locale);
        break;

      case 'comment':
        $output = l(t('Edit'), 'comment/' . $rid . '/edit/' . $d_locale);
        break;

      case 'user':
        $output = l(t('Edit'), 'user/' . $rid . '/edit/' . $d_locale);
        break;

      case 'fieldable_panels_pane':
        $output = l(t('Edit'), 'admin/structure/fieldable-panels-panes/view/' . $rid . '/edit');
        break;
    }
    return $output;
  }

}
