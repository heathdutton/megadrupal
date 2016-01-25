<?php

/**
 * @file
 * Views handler to display the content of a Form Maker.
 */

/**
 * Field handler to present the Form Maker to the user.
 */
class form_maker_handler_field_form_view extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
  }
  function option_definition() {
    $options = parent::option_definition();
    $options['label'] = array('default' => 'Form', 'translatable' => TRUE);
    return $options;
  }
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }
  function render($values) {
    $node = node_load($values->{$this->aliases['nid']});
    if (node_access('view', $node)) {
      // Populate $node->content['form_maker'] by reference.
      form_maker_node_view($node, 'full');
      $form = isset($node->content['form_maker']) ? $node->content['form_maker'] : NULL;
    }
    else {
      return;
    }
    if (strpos(current_path(), 'view')) {
      return NULL;
    }
    else {
      return $form;
    }
  }
}
