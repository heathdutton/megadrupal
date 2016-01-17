<?php
/**
* @file
* Provide node nid argument handler.
*/
/**
* Argument handler to accept a person id.
*/
class league_handler_filter_league_team_id extends views_handler_filter_equality {
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    $settings = variable_get('league_settings');
    $form['value'] = array( 
      '#type' => 'select',
      '#title' => t('Team'),
      '#options' => league_get_nodes($settings['advanced']['team_bundle']),
      '#default_value' => $this->value,
      '#required' => FALSE,
    );
  }
}
