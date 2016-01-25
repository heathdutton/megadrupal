<?php

/**
 * @file
 * 
 */

class views_handler_field_node_registration_capacity extends views_handler_field_entity {

  function query() {
    // do nothing -- to override the parent query.
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['unlimited_text'] = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['unlimited_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Unlimited text'),
      '#description' => t('Text to use for unlimited capacity. Defaults to "0". Text will be translated otherwise.'),
      '#default_value' => $this->options['unlimited_text'],
    );
  }

  function render($values) {
    if ($node = $this->get_value($values)) {
      $capacity = $node->registration->capacity();
      if (!$capacity && !empty($this->options['unlimited_text'])) {
        return t($this->options['unlimited_text']);
      }

      return $capacity;
    }
  }

}
