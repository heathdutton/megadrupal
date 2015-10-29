<?php

/**
 * @file
 * Views handler for displaying the Skype status.
 */

/**
 * User handler to provide renderer that allows to display the Skype status of a user.
 */
class skype_status_handler_field_user extends views_handler_field_user {
  function option_definition() {
    $options = parent::option_definition();
    $options['format'] = array('default' => 'button');
    $options['show_empty'] = array('default' => TRUE, 'bool' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['format'] = array(
      '#title' => t('Format'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'select',
      '#options' => array(
        'button' => 'Skype Button',
        'id' => 'Skype ID (plain text)',
      ),
      '#default_value' => $this->options['format'],
    );
    unset($form['type']);
    unset($form['link_to_user']);
  }

  function render($values) {
    $id = $values->{$this->field_alias};
    $format = $this->options['format'];

    if ($format == 'id') {
      return $id;
    }
    else {
      $settings = skype_status_get_profile_settings();
      $variables = array('id' => $id, 'type' => 'profile', 'action' => $settings['action']);
      return theme('skype_status', $variables);
    }
  }
}