<?php
/**
 * @file
 * Contains the list style plugin.
 */

/**
 * Style plugin to render each item of an ordered or unordered list.
 *
 * @ingroup views_style_plugins
 */
class views_jqm_plugin_style_basic_grid extends views_plugin_style {
  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['columns'] = array('default' => 2);
    $options['data_theme'] = array('default' => 'b');

    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['columns'] = array(
      '#type' => 'select',
      '#title' => t('Number of columns'),
      '#default_value' => $this->options['columns'],
      '#options' => array(
        2 => 2, 
        3 => 3, 
        4 => 4, 
        5 => 5),
      '#required' => TRUE,
      '#element_validate' => array('views_element_validate_integer'),
    );    
    $form['data_theme'] = array(
      '#title' => t('Theme Swatch'),
      '#description' => t('The theme swatch for the grid. (a, b, c, d, e, custom)'),
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['data_theme'],
    );

  }
}
