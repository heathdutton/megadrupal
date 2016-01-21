<?php

/**
 * @file
 * Export UI overwrites file.
 */

class CToolsAccessTokenExportUI extends ctools_export_ui {

  /**
   * Gets the UI form
   */
  public static function form(&$form, &$form_state) {
    $token = $form_state['item'];
    $form['random'] = array(
      '#type' => 'checkbox',
      '#title' => t('Random value'),
      '#description' => t('Check this if you want to generate a random value automatically.'),
      '#default_value' => empty($token->value),
    );
    $checkbox_state = array(   // action to take.
      ':input[name=random]' => array('checked' => FALSE),
    );
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Variable value'),
      '#description' => t('This is the secret token that you will need to distribute.'),
      '#default_value' => $token->value,
      '#states' => array(
        'visible' => $checkbox_state,
        'required' => $checkbox_state,
      ),
    );
  }

  /**
   * UI form submission.
   */
  public static function submit(&$form, &$form_state) {
    $values = &$form_state['values'];
    if (empty($values['value']) || !empty($values['random'])) {
      $values['value'] = static::generateToken();
    }
    // Set the updated timestamp.
    $values['updated'] = REQUEST_TIME;
  }

  /**
   * UI form validation.
   */
  public static function validate(&$form, &$form_state) {
    $values = &$form_state['values'];
    if (empty($values['value']) && empty($values['random'])) {
      form_set_error('value', t('This field is required.'));
    }
  }

  /**
   * Generate a random access token.
   *
   * @return string
   *   A random access token.
   */
  public static function generateToken() {
    return drupal_random_key();
  }

  /**
   * Overrides ctools_export_ui::list_table_header().
   */
  public function list_table_header() {
    $header = parent::list_table_header();
    $columns = array(
      array('data' => t('Token value'), 'class' => array('ctools-export-ui-token-value')),
      array('data' => t('Updated'), 'class' => array('ctools-export-ui-token-updated')),
    );
    // Insert the new column in the header array.
    array_splice($header, 1, 0, $columns);

    return $header;
  }

  /**
   * Overrides ctools_export_ui::list_build_row().
   */
  public function list_build_row($item, &$form_state, $operations) {
    parent::list_build_row($item, $form_state, $operations);
    $name = $item->{$this->plugin['export']['key']};
    $date = new \DateTime();
    $date->setTimestamp($item->updated);
    $columns = array(
      array('data' => check_plain($item->value), 'class' => array('ctools-export-ui-token-value')),
      array('data' => $date->format('M d Y, h:i'), 'class' => array('ctools-export-ui-token-updated')),
    );

    if ($form_state['values']['order'] == 'updated') {
      $this->sorts[$name] = 'updated';
    }

      // Insert the new column in the rows array.
    array_splice($this->rows[$name]['data'], 1, 0, $columns);
  }

  /**
   * Overrides ctools_export_ui::list_sort_options().
   */
  public function list_sort_options() {
    $options = parent::list_sort_options();
    $options['updated'] = t('Updated');
    return $options;
  }

}
