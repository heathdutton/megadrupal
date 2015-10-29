<?php
/**
 * @file
 * Views specific include file for implementing Gamertag handlers.
 */

/**
 * Field handler for all Gamertag platforms.
 */
class views_handler_field_user_gamertags_all extends views_handler_field {
  function construct() {
    parent::construct();
  }

  /**
   * Defines an option field.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['gamertags_display_titles'] = array('default' => TRUE);
    $options['gamertags_as_text'] = array('default' => FALSE);
    $options['gamertags_delimiter'] = array('default' => ', ');
    return $options;
  }

  /**
   * Returns an option field for displaying Gamertags as text only.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['gamertags_display_titles'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display titles'),
      '#description' => t('Display the platform title.'),
      '#default_value' => $this->options['gamertags_display_titles'],
    );
    $form['gamertags_as_text'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show gamertag as text only'),
      '#description' => t('Display the user input for the gamertag, not the actual gamercard.'),
      '#default_value' => $this->options['gamertags_as_text'],
    );
    $form['gamertags_delimiter'] = array(
      '#type' => 'textfield',
      '#title' => t('Delimiter'),
      '#description' => t('What to use for separating the Users Gamertags.'),
      '#default_value' => $this->options['gamertags_delimiter'],
      '#size' => 5,
    );
  }

  /**
   * Handles rendering the Gamertags.
   */
  function render($values) {
    // Fetch the user data containing the Gamertags.
    $data = unserialize($values->users_data);
    // Remove the form_build_id.
    unset($data['form_build_id']);
    // Fast return if there's nothing to view.
    if (empty($data) || !_gamertags_test_any()) {
      return;
    }
    // Build a 'fake' user account for the Gamertags render method.
    $account = new stdClass();
    foreach ($data as $key => $value) {
      $account->$key = $value;
    }

    $gamertags = array();
    foreach (gamertags_return_all() as $class => $platform_info) {
      if (variable_get("{$class}_collect", TRUE) && !empty($data[$class])) {
        // Display as text as per options_form setting above.
        if ($this->options['gamertags_as_text']) {
          $gamertags[] = $this->options['gamertags_display_titles'] ? $platform_info['title'] .': '. check_plain($data[$class]) : check_plain($data[$class]);
        }
        else {
          $gamertags[] = $this->options['gamertags_display_titles'] ? $platform_info['title'] .': '. call_user_func_array(array($class, 'render'), array($account)) : call_user_func_array(array($class, 'render'), array($account));
        }
      }
    }
    return implode(check_plain($this->options['gamertags_delimiter']), $gamertags);
  }
}

/**
 * Field handler for a single Gamertag platform.
 */
class views_handler_field_user_gamertags_single extends views_handler_field {
  function construct() {
    parent::construct();
  }

  /**
   * Defines an option field.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['gamertags_as_text'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Returns an option field for displaying Gamertags as text only.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['gamertags_as_text'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show gamertag as text only'),
      '#description' => t('Display the user input for the gamertag, not the actual gamercard.'),
      '#default_value' => $this->options['gamertags_as_text'],
    );
  }

  /**
   * Handles rendering the Gamertags.
   */
  function render($values) {
    // Fetch the user data containing the Gamertags.
    $data = unserialize($values->users_data);
    // Remove the form_build_id.
    unset($data['form_build_id']);
    // Don't display if there's no Gamertag entered for the account.
    if (empty($data) || empty($data[$this->field])) {
      return;
    }
    // Display as text as per options_form setting above.
    if ($this->options['gamertags_as_text']) {
      return check_plain($data[$this->field]);
    }
    // Build a 'fake' user account for the Gamertags render method.
    $account = new stdClass();
    foreach ($data as $key => $value) {
      $account->$key = $value;
    }
    return call_user_func_array(array($this->field, 'render'), array($account));
  }
}
