<?php

/**
 * @file
 * Zoundation block grid style plugin.
 *
 * @see http://foundation.zurb.com/docs/grid.php#block-grids
 */

/**
 * Style plugin to render rows in a block-grid.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_zoundation_views_block_grid extends views_plugin_style {
  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['large_block_grid'] = array('default' => 4);
    $options['small_block_grid'] = array('default' => 2);

    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // array keyed by number 1-12
    $sizes = array_combine(range(0,12), range(0,12));

    $form['large_block_grid'] = array(
      '#type' => 'select',
      '#title' => t('Large block grid'),
      '#options' => $sizes,
      '#default_value' => $this->options['large_block_grid'],
      '#description' => t('Choose 0 to omit this grid setting.'),
    );

    $form['small_block_grid'] = array(
      '#type' => 'select',
      '#title' => t('Small block grid'),
      '#options' => $sizes,
      '#default_value' => $this->options['small_block_grid'],
      '#description' => t('Choose 0 to omit this grid setting.'),
    );
  }
}
