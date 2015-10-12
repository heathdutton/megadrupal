<?php

/**
 * @file
 * Contains the base row style plugin.
 */

/**
 * The basic 'fields' row plugin
 * @ingroup views_row_plugins
 */
class timelinr_plugin_row_timelinrfields extends views_plugin_row {
  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['dates_field'] = array('default' => '');
    $options['title_field'] = array('default' => '');
    $options['image_field'] = array('default' => '');
    $options['body_field'] = array('default' => '');
    return $options;
  }
  /**
   * Show a form to edit the style options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['grouping']);

    // Pre-build all of our option lists for the dials and switches that follow.
    $fields = array('' => t('<none>'));
    foreach ($this->display->handler->get_handlers('field') as $field => $handler) {
      if ($label = $handler->label()) {
        $fields[$field] = $label;
      }
      else {
        $fields[$field] = $handler->ui_name();
      }
    }

    $form['dates_field'] = array(
      '#type' => 'select',
      '#required' => TRUE,
      '#title' => t('Date field'),
      '#options' => $fields,
      '#default_value' => $this->options['dates_field'],
      '#description' => t('Select the field that will be used as the date field - navigation of timelinr.'),
    );

    $form['title_field'] = array(
      '#type' => 'select',
      '#required' => TRUE,
      '#title' => t('Title field'),
      '#options' => $fields,
      '#default_value' => $this->options['title_field'],
      '#description' => t('Select the field that will be used as the title field, if one is required.'),
    );

    $form['image_field'] = array(
      '#type' => 'select',
      '#title' => t('Image field'),
      '#options' => $fields,
      '#default_value' => $this->options['image_field'],
      '#description' => t('Select the field that will be used as the image field.'),
    );

    $form['body_field'] = array(
      '#type' => 'select',
      '#title' => t('Body field'),
      '#options' => $fields,
      '#default_value' => $this->options['body_field'],
      '#description' => t('Select the field that will be used as the body field, if one is required.'),
    );
  }
}
