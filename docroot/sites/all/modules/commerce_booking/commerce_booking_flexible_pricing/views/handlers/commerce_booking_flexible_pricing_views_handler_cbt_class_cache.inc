<?php
/**
 * Views handler to allow filtering on ticket class.
 */

class commerce_booking_flexible_pricing_views_handler_cbt_class_cache extends views_handler_filter_in_operator {

  /**
   * Overrides views_handler_filter_in_operator::get_value_options().
   */
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    if (!empty($this->options['entity_type'])) {
      if (substr($this->options['entity_type'], 0, 1) == '!') {
        $entity_type = @$this->view->args[substr($this->options['entity_type'], 1)];
      }
      else {
        $entity_type = $this->options['entity_type'];
      }
    }

    if (!empty($this->options['entity_id'])) {
      if (substr($this->options['entity_id'], 0, 1) == '!') {
        $entity_id = @$this->view->args[substr($this->options['entity_id'], 1)];
      }
      else {
        $entity_id = $this->options['entity_id'];
      }
    }

    if (!empty($entity_type) && !empty($entity_id)) {
      $wrapper = entity_metadata_wrapper($entity_type, entity_load_single($entity_type, $entity_id));
    }
    else {
      $wrapper = NULL;
    }

    foreach (commerce_booking_flexible_pricing_get_ticket_classes($wrapper) as $class) {
      $this->value_options[$class->id] = $class->label;
    }

    return $this->value_options;
  }

  /**
   * Overrides views_handler_filter_in_operator::option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['entity_type'] = array('default' => '');
    $options['entity_id'] = array('default' => '');

    return $options;
  }

  /**
   * Overrides views_handler_filter_in_operator::expose_form().
   */
  function expose_form(&$form, &$form_state) {
    parent::expose_form($form, $form_state);
    $form['entity_type'] = array(
      '#type' => 'textfield',
      '#title' => t('Entity type'),
      '#description' => t('The type of entity we use for building our list of possible options. This can be retrieved from arguments using !0 etc.'),
      '#default_value' => !empty($this->options['entity_type']) ? $this->options['entity_type']: '',
    );
    $form['entity_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Entity ID'),
      '#description' => t('The id of the entity we use for building our list of possible options. This can be retrieved from arguments using !0 etc.'),
      '#default_value' => !empty($this->options['entity_id']) ? $this->options['entity_id']: '',
    );
  }

}
