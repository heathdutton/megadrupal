<?php

/**
 * @file
 * 
 */

class views_handler_field_node_registration_available_slots extends views_handler_field_entity {

  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->view->init_style();

    $this->additional_fields['nid'] = 'nid';

    $handler_name = 'registration_used_slots';
    $field_handlers = &$this->view->display_handler->handlers['field'];

    $fields = $this->view->display_handler->get_option('fields');
    if (!isset($fields[$handler_name])) {
      // Create, init and attach handler.
      $handler = views_get_handler($this->table, $handler_name, 'field');
      $handler_options = array(
        'id' => $handler_name,
        'table' => $this->table,
        'field' => $handler_name,
        'label' => 'AUTOMATIC',
        'exclude' => TRUE,
      );
      $handler->main_field = $options['id'];
      $handler->init($view, $handler_options);
      $field_handlers[$handler_name] = $handler;

      // Create and attach fake field.
      $fields[$handler_name] = $handler_options;
      $this->view->display_handler->set_option('fields', $fields);
    }
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
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

      $values = (array) $values;
      $used_slots = (int) $values['registration_used_slots'];

      if (!$capacity && !$used_slots && !empty($this->options['unlimited_text'])) {
        return t($this->options['unlimited_text']);
      }

      $available_slots = max(0, $capacity - $used_slots);
      return $available_slots;
    }
  }

}
