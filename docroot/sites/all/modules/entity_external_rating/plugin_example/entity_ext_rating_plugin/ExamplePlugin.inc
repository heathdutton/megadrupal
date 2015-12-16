<?php

/**
 * @file
 *  An example of implementing a plugin.
 */

// If a parent plugin is specified in the plugin definition, in module, then
// that plugin class should be extended.
class ExamplePlugin extends EntityExtRatingSource {

  /**
   * The source name of this plugin.
   *
   * @return string
   *   The source name of this plugin.
   */
  public function sourceName() {
    return 'ExamplePlugin';
  }

  /**
   * Add a configuration form. Here we will set how many rating points will
   * return this dummy plugin.
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    // Add the Twitter API URL setting.
    $form['example_ext_rating_plugin_points'] = array(
      '#type' => 'textfield',
      '#title' => t('Rating points'),
      '#description' => t('Please enter the maximum number of rating points this plugin will always return.'),
      '#default_value' => isset($this->config['example_ext_rating_plugin_points'])?$this->config['example_ext_rating_plugin_points']:'',
      '#required' => TRUE,
    );
    return $form;
  }

  /**
   * Validation handler for the configuration form.
   *
   * @param array $values
   *   The values the were submitted (The data from $form_state['values']).
   */
  public function configFormValidate(&$values) {
    parent::configFormValidate($values);
    // Validate that the number of point is not smaller than 0.
    if ($values['example_ext_rating_plugin_points'] < 0 || !is_numeric($values['example_ext_rating_plugin_points'])) {
      form_set_error('example_ext_rating_plugin_points', t('Please insert a valid and positive number'));
    }
  }

  /**
   * Makes the call to the Twitter API.
   *
   * @param $params
   *   It has the same meaning as in the parent class.
   *
   * @see EntityExtRatingSource::saveExternalRating()
   *
   * @return bool
   *   TRUE if the request was succesful, FALSE otherwise.
   */
  public function getExternalRating(&$params) {
    foreach ($params as $key => $value) {
      $params[$key]['rating_value'] = rand(1, $this->config['example_ext_rating_plugin_points']);
    }
    return TRUE;
  }

  /**
   * Returns the html widget for the plugin.
   *
   * @param array $params
   *   An array with parameters that will be used to create the widget.
   *
   * @return string
   *   The html output of the widget.
   */
  public function getHtml($params) {
    return "<div>Example plugin</div>";
  }
}