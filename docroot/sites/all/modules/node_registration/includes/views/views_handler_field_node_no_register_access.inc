<?php

/**
 * @file
 * 
 */

class views_handler_field_node_no_register_access extends views_handler_field_entity {

  function query() {
    // do nothing -- to override the parent query.
  }

  function option_definition() {
    $options = parent::option_definition();

    // Override the alter text option to always alter the text.
    $options['alter']['contains']['alter_text'] = array('default' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Remove the checkbox
    $form['alter']['#collapsed'] = FALSE;
    unset($form['alter']['alter_text']);
    unset($form['alter']['text']['#dependency']);
    unset($form['alter']['text']['#process']);
    unset($form['alter']['help']['#dependency']);
    unset($form['alter']['help']['#process']);
    $form['#pre_render'][] = 'views_handler_field_custom_pre_render_move_text';
  }

  function render($values) {
    if ($node = $this->get_value($values)) {
      $registration = entity_get_controller('node_registration')->create(array(
        'nid' => $node->nid,
      ));
      if (!node_registration_access($registration, 'add')) {
        return $this->options['alter']['text'];
      }
    }
  }

}
