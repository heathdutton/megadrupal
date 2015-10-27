<?php

/**
 * @file
 * SSH key field handler for delete links.
 */

class sshkey_handler_field_sshkey_link extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['key_id'] = 'key_id';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  function query() {
    $base_table = $this->view->base_table;

    // If the current field is under a relationship you can't be sure that the
    // base table of the view is the base table of the current field.
    // For example a field from a node author on a node view does have users as base table.
    if (!empty($this->relationship)) {
      foreach ($this->view->relationship as $relationship) {
        if ($relationship->alias == $this->relationship) {
          $base_table = $relationship->definition['base'];
        }
      }
    }

    // Get the entity type according to the base table of the field.
    $this->entity_type = $this->definition['entity_tables'][$base_table];
    $entity_info = entity_get_info($this->entity_type);

    $this->additional_fields['entity_id'] = array(
      'table' => $base_table,
      'field' => $entity_info['entity keys']['id'],
    );

    $this->ensure_my_table();
    $this->add_additional_fields();
  }
}
