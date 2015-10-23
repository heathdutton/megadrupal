<?php

/**
 * @file
 * Handler for operations links on the Elearning entities.
 */

/**
 * Handler for operations links on the elearning entities.
 */
class elearning_ui_handler_entity_operations_field extends views_handler_field {

  /**
   * Overwrites views_handler_field::query().
   */
  public function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Overwrites views_handler_field::init().
   */
  public function init(&$view, &$options) {
    parent::init($view, $options);
    $entity_info = entity_get_info($view->base_table);
    $id = $entity_info['entity keys']['id'];
    $this->additional_fields[$id] = $id;
  }

  /**
   * Overwrites views_handler_field::render().
   */
  public function render($values) {
    $entity_type = $this->table;
    $entity_info = entity_get_info($entity_type);
    $id_key = $entity_info['entity keys']['id'];
    $entity_id = ($this->get_value($values, $id_key));
    $entity_type_path_element = str_replace('elearning_', '', $entity_type);
    $links = menu_contextual_links('elearning_ui', 'admin/elearning/' . $entity_type_path_element, array($entity_id));
    if (!empty($links)) {
      return theme('links', array(
        'links' => $links,
        'attributes' => array(
          'class' => array(
            'links',
            'inline',
            'operations',
            ),
          ),
        )
      );
    }
  }
}
