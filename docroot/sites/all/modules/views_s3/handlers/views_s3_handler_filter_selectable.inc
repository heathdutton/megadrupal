<?php

/**
 * @file views_s3_handler_filter_selectable.inc
 * Basic S3 selectable filter handler.
 */

/**
 * Simple filter to handle equality.
 */
class views_s3_handler_filter_selectable extends views_handler_filter {

  function option_definition() {
    $options = parent::option_definition();
    $options['select_options'] = array('default' => isset($this->definition['select options']) ? $this->definition['select options'] : array('' => t('- Select one -')));
    $options['select_title'] = array('default' => isset($this->definition['select title']) ? $this->definition['select title'] : t('Options'));
    $options['select_value'] = array('default' => isset($this->definition['select value']) ? $this->definition['select value'] : '');
    $options['select_description'] = array('default' => isset($this->definition['select description']) ? $this->definition['select description'] : '');
    
    return $options;
  }
  
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['select_value'] = array(
      '#type' => 'select',
      '#title' => $this->options['select_title'],
      '#options' => $this->options['select_options'],
      '#description' => $this->options['select_description'],
      '#default_value' => isset($this->options['select_value']) ? $this->options['select_value'] : '',
    );
  }
  
  /**
   * Display the filter on the administrative summary
   */
  function admin_summary() {
    return check_plain((string) $this->operator) . ' ' . check_plain((string) $this->options['select_value']);
  }

  /**
   * Add this filter to the request.
   */
  function query() {
    $this->query->request_arguments[$this->real_field] = $this->options['select_value'];
  }

}