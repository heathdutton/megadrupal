<?php

/**
 * @file
 * Contains the flipped table style plugin.
 */

/**
 * Style plugin to render each item as a column in a table.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_sticky_columns_flipped extends views_flipped_table_plugin_style_flipped_table {

  /**
   * {@inheritdoc}
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['sticky_column'] = array('default' => FALSE);

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['sticky_column'] = array(
      '#type' => 'checkbox',
      '#title' => t('Sticky first column'),
      '#description' => t('Makes the first column, containing the field labels, sticky.'),
      '#default_value' => $this->options['sticky_column'],
    );
  }

}
