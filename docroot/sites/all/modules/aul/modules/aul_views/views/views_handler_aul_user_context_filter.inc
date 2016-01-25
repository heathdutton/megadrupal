<?php

/**
 * @file AUL grants exposed filter.
 */

/**
 * Class for AUL grants exposed filter.
 */
class views_handler_aul_user_context_filter extends views_handler_filter {

  /**
   * Clear query alter
   */
  function query() {
    
  }
  
  /**
   * Display the filter on the administrative summary
   */
  function admin_summary() {
    
    $default_value = '';
    if (!empty($this->value['entity_id'])) {
      $aul_user = $this->value;
      $default_value = $aul_user['entity_label'] . ' (' . $aul_user['entity_id'] . ')';
    }
    $exposed = ($this->options['exposed']) ? ' (' . t('exposed') . ')' : '';
    
    return check_plain((string) $this->operator) . ' ' . check_plain((string) $default_value . $exposed);
  }
  
  /**
   * Subtasks checkbox. Provides show/hide choise
   */
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    $form['value'] = array(
      '#type' => 'entityreference',
      '#title' => t('User'),
      '#description' => t('Choose user for access context. Default value - current logged in user.'),
      '#era_entity_type' => 'user',
      '#era_cardinality' => 1,
      '#default_value' => (int) $this->value['entity_id'],
    );
    return $form;
  }

}
