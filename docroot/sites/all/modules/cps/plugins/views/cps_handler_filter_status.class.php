<?php

/**
 * @file
 * Custom filter handler to filter by state and list of states by state type.
 */

/**
 * Class cps_handler_filter_status
 */
class cps_handler_filter_status extends views_handler_filter_in_operator {

  /**
   * @{inheritdoc}
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['expose']['contains']['type'] = array('default' => '');
    return $options;
  }

  function expose_form(&$form, &$form_state) {
    parent::expose_form($form, $form_state);
    $form['expose']['type'] = array(
      '#type' => 'radios',
      '#title' => t('Limit to states of type'),
      '#options' => array_merge(array('' => t('Do not limit')), cps_changeset_get_state_types()),
      '#default_value' => $this->options['expose']['type'],
    );
  }

  /**
   * @{inheritdoc}
   */
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    if (empty($this->options['expose']['type'])) {
      $this->value_options = cps_changeset_get_state_labels();
    }
    else {
      $states = cps_changeset_get_states();
      $this->value_options = array();
      foreach ($states as $key => $state) {
        if ($state['type'] && $state['type'] == $this->options['expose']['type']) {
          $this->value_options[$key] = $state['label'];
        }
      }
    }

    return $this->value_options;
  }
}
