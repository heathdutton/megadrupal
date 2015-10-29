<?php

/**
 * @file
 * Views handler for Tournament module
 */

class tournament_handler_field_tournament_status extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => 'name');

    return $options;
  }  
  
  function options_form(&$form, &$form_state) {
    $form['type'] = array(
      '#type' => 'select',
      '#options' => array(
        'key' => t('Status value (numeric)'),
        'name' => t('Status label'),
      ),
      '#default_value' => $this->options['type'],
    );
    parent::options_form($form, $form_state);
  }
  
  function render($values) {
    if (!empty($this->options['type']) && $this->options['type'] == 'key') {
      return $values->{$this->field_alias};  
    }
    
    return _tournament_get_status($values->{$this->field_alias});
  }  
}
