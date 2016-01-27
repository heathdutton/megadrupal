<?php
/**
 * @file
 * views_plugin_row_sgrid_style.inc
 * Views row style plugin for Sortable grid module
 */

/**
 * The basic 'fields' row plugin
 *
 * This displays fields one after another, giving options for inline
 * or not.
 *
 * @ingroup views_row_plugins
 */
class views_plugin_row_sgrid_style extends views_plugin_row {
  function option_definition() {
    $options = parent::option_definition();

    $options['inline'] = array('default' => array());
    $options['separator'] = array('default' => '');
    $options['hide_empty'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Provide a form for setting options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $options = $this->display->handler->get_field_labels();

    if (empty($this->options['inline'])) {
      $this->options['inline'] = array();
    }

    $form['inline'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Inline fields'),
      '#options' => $options,
      '#default_value' => $this->options['inline'],
      '#description' => t('Inline fields will be displayed next to each other rather than one after another.'),
    );

    $form['hide_empty'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide empty fields'),
      '#default_value' => $this->options['hide_empty'],
      '#description' => t('Do not display fields, labels or markup for fields that are empty.'),
    );

  }

  /**
   * Perform any necessary changes to the form values prior to storage.
   * There is no need for this function to actually store the data.
   */
  function options_submit(&$form, &$form_state) {
    $form_state['values']['row_options']['inline'] = array_filter($form_state['values']['row_options']['inline']);
  }

  /**
   * Render a row object. This usually passes through to a theme template
   * of some form, but not always.
   */
  function render($row) {
    return theme($this->theme_functions(),
      array(
        'view' => $this->view,
        'options' => $this->options,
        'row' => $row,
        'field_alias' => isset($this->field_alias) ? $this->field_alias : '',
      ));
  }
}

