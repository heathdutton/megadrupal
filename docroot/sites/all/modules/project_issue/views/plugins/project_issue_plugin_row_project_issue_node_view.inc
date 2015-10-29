<?php

/**
 * Plugin which performs a node_view on the resulting object.
 *
 * Most of the code on this object is in the theme function.
 *
 */
class project_issue_plugin_row_project_issue_node_view extends views_plugin_row {
  function option_definition() {
    $options = parent::option_definition();

    $options['include_assigned'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['include_assigned'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display the user the issue is assigned to'),
      '#default_value' => $this->options['include_assigned'],
    );
  }
}
