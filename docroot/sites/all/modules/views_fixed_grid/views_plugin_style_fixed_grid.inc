<?php
/**
* @file views_plugin_style_fixed_grid.inc
* Implementation of views_plugin_style
*
* In this case we only need the configuration form. All the other
* logic will be handled by the preprocess function
*/

class views_plugin_style_fixed_grid extends views_plugin_style_grid {
 
  /**
   * Set default options
   */
  function options(&$options) {
    $options['alignment'] = 'random';
    $options['rows'] = 6;
    $options['columns'] = 7;
    $options['maxitems'] = NULL;
  }

  /**
   * Provide a form for setting options.
   *
   * @param array $form
   * @param array $form_state
   */ 
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['fill_single_line']);
    $form['rows'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of rows'),
      '#description' => t('Select the number of rows.'),
      '#default_value' => $this->options['rows'],
      '#required' => TRUE,
      '#weight' => -1,
    );
    $form['columns']['#required'] = TRUE;
    $form['columns']['#weight'] = -1;
    $form['maxitems'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum numbers of items'),
      '#description' => t('If you want to always have some empty space select the maximum number of items. Leave it empty to not to have a maximum.'),
      '#default_value' => $this->options['maxitems'],
    );
    $form['alignment']['#options']['random'] = t('Random');
  }
}
