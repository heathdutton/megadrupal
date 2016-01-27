<?php

/**
 * @file
 * Provide views handlers for amazon.module
 */


/**
 * Plugin which loads and themes an amazon item.
 *
 * @ingroup views_plugin_rows
 */
class amazon_views_plugin_row_amazon_view extends views_plugin_row {
  function option_definition() {
    $options = parent::option_definition();

    $options['display_format'] = array('default' => 'default');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['display_format'] = array(
      '#type' => 'select',
      '#title' => t('Display format'),
      '#options' => array(
        'default' => t('Default'),
        'details' => t('Details'),
        'thumbnail' => t('Thumbnail'),
        'inline' => t('Inline'),
      ),
      '#default_value' => $this->options['display_format'],
    );
  }
}

