<?php

/**
 * @file
 * Handler for dfv_plugin_style.
 */

class dfv_plugin_style extends views_plugin_style {

  /**
   * Define options.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['title_field'] = array('default' => NULL);
    $options['key_field'] = array('default' => NULL);
    return $options;
  }

  /**
   * Provide a form for setting options.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = $form['grouping'][0]['field']['#options'];
    unset($options['']);
    $form['title_field'] = array(
      '#type' => 'select',
      '#title' => t('Title Field'),
      '#options' => $options,
      '#required' => TRUE,
      '#default_value' => $this->options['title_field'],
      '#weight' => -3,
    );
    $form['key_field'] = array(
      '#type' => 'select',
      '#title' => t('Key Field'),
      '#options' => $options,
      '#required' => TRUE,
      '#default_value' => $this->options['key_field'],
      '#weight' => -3,
    );
  }

  /**
   * Render the display in this style.
   */
  public function render() {
    $options = $this->display->handler->get_option('dfv_options');

    // Play nice with View UI 'preview' : if the view is not executed
    // just display the HTML.
    if (empty($options)) {
      return parent::render();
    }

    $title_field = $this->options['title_field'];
    $key_field = $this->options['key_field'];
    if (!$key_field && !$title_field) {
      drupal_set_message(t('Check view settings'), 'error');
    }

    $results = array();
    foreach ($this->view->result as $row_index => $row) {
      $results[$this->view->style_plugin->get_field($row_index, $key_field)] = $this->view->style_plugin->get_field($row_index, $title_field);
    }

    return $results;
  }
}
