<?php

/**
 * @file
 * Provide views area handler for whos.amung.us module.
 *
 * @author Francisco J. Cruz Romanos <grisendo@gmail.com>
 */

class views_handler_area_whos_amung_us extends views_handler_area {

  /**
   * Override init function to allow show it when empty.
   */
  public function init(&$view, &$options) {
    parent::init($view, $options);
    if ($this->handler_type == 'empty') {
      $this->options['empty'] = TRUE;
    }
  }

  /**
   * Set custom label in Views UI.
   */
  public function label() {
    if (!isset($this->options['label'])) {
      return $this->ui_name();
    }
    return $this->options['label'];
  }

  /**
   * Defines default values for custom options.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $this->definition['field'] = !empty($this->definition['field']) ? $this->definition['field'] : '';
    $label = !empty($this->definition['label']) ? $this->definition['label'] : $this->definition['field'];
    $default_options = whos_amung_us_default_form_values();
    foreach ($default_options as $key => $value) {
      $options[$key] = array('default' => $value, 'translatable' => FALSE);
    }
    return $options;
  }

  /**
   * Provide text for the administrative summary.
   */
  public function admin_summary() {
    $label = $this->label();
    if ($label) {
      return $label . ': ' . $this->options['widget'];
    }
    else {
      return $this->options['widget'];
    }
  }

  /**
   * Build the options form.
   */
  public function options_form(&$form, &$form_state) {
    module_load_include('inc', 'whos_amung_us', 'includes/whos_amung_us.admin');
    parent::options_form($form, $form_state);
    $form['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#default_value' => isset($this->options['label']) ? $this->options['label'] : '',
      '#description' => t('The label for this area that will be displayed only administratively.'),
    );
    $options = array(
      'values' => $this->options,
    );
    $options['values'] += whos_amung_us_default_form_values();
    $form += whos_amung_us_configuration_form(array(), $options, 'views', 'options');
  }

  /**
   * Called to add the field to a query.
   */
  public function query() {
    return NULL;
  }

  /**
   * Render the field.
   *
   * @param stdClass $empty
   *   The values retrieved from the database.
   */
  public function render($empty = FALSE) {
    if (($this->options['empty']) || (!$empty)) {
      if (isset($this->view->live_preview) && $this->view->live_preview) {
        return t('Whos.amung.us widget is not compatible with live preview.');
      }
      $this->options += whos_amung_us_default_form_values();
      return theme(
        'whos_amung_us',
        array(
          'widget' => $this->options['widget'],
          'options' => $this->options,
          'id' => 'vws' . rand(1, 999),
        )
      );
    }
    else {
      return '';
    }
  }

  /**
   * Provides the handler some groupby.
   */
  public function use_group_by() {
    return FALSE;
  }

}
