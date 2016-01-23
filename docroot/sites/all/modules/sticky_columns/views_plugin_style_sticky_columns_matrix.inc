<?php

/**
 * @file
 * Contains the matrix style plugin.
 */

/**
 * Style plugin to render results in a matrix.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_sticky_columns_matrix extends views_matrix_plugin_style_matrix {

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
      '#description' => t('Makes the first column, containing the row header, sticky.'),
      '#default_value' => $this->options['sticky_column'],
    );
    // Move x and y field config below our checkbox.
    $form['xconfig']['#weight'] = 5;
    $form['yconfig']['#weight'] = 5;
  }

}
