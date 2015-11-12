<?php

/**
 * @file
 * Contains the RSS style plugin.
 */

/**
 * Default style plugin to render an RSS feed.
 *
 * @ingroup views_style_plugins
 */
class xcri_cap_plugin_style_xcri_cap extends views_plugin_style {

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['contributor'] = array(
      '#type' => 'textfield',
      '#title' => t('XCRI-CAP contributor'),
      '#default_value' => $this->options['contributor'],
      '#description' => t('This will appear in the XCRI-CAP feed as the main catalog contributor.'),
    );

    $form['description'] = array(
      '#type' => 'textarea',
      '#title' => t('XCRI-CAP description'),
      '#default_value' => $this->options['description'],
      '#description' => t('This will appear in the XCRI-CAP feed as the main catalog description.'),
    );
  }

  function render() {

    foreach ($this->view->result as $row) {
      $rows[] = node_load($row->nid);
    }

    $output = theme($this->theme_functions(),
      array(
        'view' => $this->view,
        'options' => $this->options,
        'rows' => $rows
      ));
    return $output;
  }

  function query() {
    parent::query();
    // Make sure we have nid field as we are gonna need it to load the node on
    // the render function.
    $this->view->query->add_field('node', 'nid');
  }
}
