<?php

/**
 * @file
 * A relationship handler for Views that allows fetching only statistics
 * relating to the active user. A lot of the code is copied from the Flag
 * module – cred to the Flag maintainers for making this work at all.
 */

class tutor_statistics_handler_relationship_user extends views_handler_relationship {
  function option_definition() {
    $options = parent::option_definition();
    $options['user_scope'] = array('default' => 'current');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['user_scope'] = array(
      '#type' => 'radios',
      '#title' => t('Statistics for'),
      '#options' => array('current' => t('Current user'), 'any' => t('Any user')),
      '#default_value' => $this->options['user_scope'],
    );
  }

  function query() {
    if ($this->options['user_scope'] == 'current') {
      $this->definition['extra'][] = array(
        'field' => 'uid',
        'value' => '***CURRENT_USER***',
        'numeric' => TRUE,
      );
    }
    parent::query();
  }
}
