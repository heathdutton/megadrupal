<?php

/**
 * @file
 * Provides the views handler for location fields.
 */

/**
 * Handler class for a Views location filter.
 */
class SearchApiViewsHandlerFilterLocation extends SearchApiViewsHandlerFilter {

  public function option_definition() {
    $options = parent::option_definition();

    $options['plugin']['default'] = '';
    foreach (search_api_location_get_input_plugins() as $id => $plugin) {
      $options["plugin-$id"]['default'] = array();
    }

    $options['radius_type']['default'] = 'select';
    $options['radius_options']['default'] = "- -\n5 5 km\n10 10 km\n16.09 10 mi";
    $options['radius_units']['default'] = '1';

    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['plugin'] = array(
      '#type' => 'select',
      '#title' => t('Input method'),
      '#description' => t('Select the method to use for parsing locations entered by the user.'),
      '#options' => search_api_location_get_input_plugin_options(),
      '#default_value' => $this->options['plugin'],
      '#required' => TRUE,
    );
    foreach (search_api_location_get_input_plugins() as $id => $plugin) {
      $form["plugin-$id"] = array(
        '#type' => 'fieldset',
        '#title' => t('Input method settings'),
        '#description' => $plugin['description'],
        '#tree' => TRUE,
        '#dependency' => array(
          'edit-options-plugin' => array($id)
        ),
      );
      if (!empty($plugin['form callback'])) {
        $plugin_form = $plugin['form callback']($form_state, $this->options["plugin-$id"]);
        if ($plugin_form) {
          $form["plugin-$id"] += $plugin_form;
        }
      }
    }

    $form['radius_type'] = array(
      '#type' => 'select',
      '#title' => t('Type of distance input'),
      '#description' => t('Select the type of input element for the distance option.'),
      '#options' => array(
        'select' => t('Select'),
        'textfield' => t('Text field'),
      ),
      '#default_value' => $this->options['radius_type'],
      '#dependency' => array(
        'edit-options-expose-use-operator' => array(1),
      ),
    );
    $form['radius_options'] = array(
      '#type' => 'textarea',
      '#title' => t('Distance options'),
      '#description' => t('Add one line per option for “Range” you want to provide. The first part of each line is the distance in kilometres, everything after the first space is the label. "-" as the distance ignores the location for filtering, but will still use it for facets, sorts and distance calculation. Skipping the distance altogether (i.e., starting the line with a space) will provide an option for ignoring the entered location completely.'),
      '#default_value' => $this->options['radius_options'],
      '#dependency' => array(
        'edit-options-radius-type' => array('select'),
      ),
    );
    $form['radius_units'] = array(
      '#type' => 'textfield',
      '#title' => t('Distance conversion factor'),
      '#description' => t('Enter the conversion factor from the expected unit of the user input to kilometers. E.g., miles would have a factor of 1.60935.'),
      '#default_value' => $this->options['radius_units'],
      '#dependency' => array(
        'edit-options-radius-type' => array('textfield'),
      ),
    );
  }

  public function operator_form(&$form, &$form_state) {
    if ($this->options['radius_type'] == 'select') {
      $form['operator'] = array(
        '#type' => 'select',
        '#options' => $this->operator_options(),
      );
      $form['operator']['#default_value'] = $this->operator;
    }
    else {
      $form['operator'] = array(
        '#type' => 'textfield',
        '#title' => t('Distance'),
        '#size' => 10,
        '#default_value' => $this->operator,
      );
    }
  }

  public function operator_options() {
    $options = array();
    $lines = array_filter(array_map('rtrim', explode("\n", $this->options['radius_options'])));
    foreach ($lines as $line) {
      $pos = strpos($line, ' ');
      $range = substr($line, 0, $pos);
      $options[$range] = trim(substr($line, $pos + 1));
    }
    return $options;
  }

  public function query() {
    while (is_array($this->value)) {
      $this->value = reset($this->value);
    }
    $this->value = trim($this->value);
    if (!$this->value || !($this->operator || is_numeric($this->operator))) {
      return;
    }
    if (empty($this->options['plugin'])) {
      $vars = array(
        '%filter' => $this->ui_name(TRUE),
        '%view' => $this->view->get_human_name(),
      );
      watchdog('search_api_location', 'Filter %filter in view %view has no location input plugin selected. Ignoring location filter.', $vars, WATCHDOG_WARNING);
      return;
    }
    $plugin = search_api_location_get_input_plugins($this->options['plugin']);
    if (!$plugin) {
      $vars = array(
        '%filter' => $this->ui_name(TRUE),
        '%view' => $this->view->get_human_name(),
        '%plugin' => $this->options['plugin'],
      );
      watchdog('search_api_location', 'Filter %filter in view %view uses unknown location input plugin %plugin. Ignoring location filter.', $vars, WATCHDOG_WARNING);
      return;
    }
    $location = $plugin['input callback']($this->value, $this->options['plugin-' . $this->options['plugin']]);
    if (!$location) {
      drupal_set_message(t('The location %address could not be resolved and was ignored.', array('%address' => $this->value)), 'warning');
      return;
    }
    $location = explode(',', $location, 2);
    $location_options = (array) $this->query->getOption('search_api_location', array());
    // If the radius isn't numeric omit it. Necessary since "no radius" is "-".
    $radius = (!is_numeric($this->operator)) ? NULL : $this->operator;
    if ($this->options['radius_type'] == 'textfield' && is_numeric($this->options['radius_units'])) {
      $radius *= $this->options['radius_units'];
    }
    $location_options[] = array(
      'field' => $this->real_field,
      'lat' => $location[0],
      'lon' => $location[1],
      'radius' => $radius,
    );
    $this->query->setOption('search_api_location', $location_options);
  }

}
