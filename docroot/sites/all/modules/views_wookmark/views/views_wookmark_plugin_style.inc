<?php

class views_wookmark_plugin_style extends views_plugin_style {

  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['align'] = array('default' => 'center');
    $options['direction'] = array('default' => 'left');
    $options['itemWidth'] = array('default' => 0);
    $options['flexibleWidth'] = array('default' => 0);
    $options['offset'] = array('default' => 2);
    $options['fillEmptySpace'] = array('default' => false);
    $options['autoResize'] = array('default' => false);
    $options['resizeDelay'] = array('default' => 50);
    $options['default_css'] = array('default' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['align'] = array(
      '#type' => 'select',
      '#title' => t('Grid Layout Alignment'),
      '#options' => array(
        'left' => t('Left'),
        'right' => t('Right'),
        'center' => t('Center'),
      ),
      '#default_value' => $this->options['align'],
      '#description' => t('Please specify the grid layout. It defaults to center'),
    );

    $form['direction'] = array(
      '#type' => 'select',
      '#title' => t('Direction'),
      '#options' => array(
        'left' => t('Left'),
        'right' => t('Right'),
      ),
      '#default_value' => $this->options['direction'],
      '#description' => t('Please specify whether to start in top left or top right corner'),
    );

    $form['itemWidth'] = array(
      '#type' => 'textfield',
      '#title' => t('Item Width'),
      '#description' => t('Optional, the maximum width of a grid item. If 0, it defaults to width of first provided item.'),
      '#default_value' => (int) $this->options['itemWidth'],
      '#required' => TRUE,
    );

    $form['flexibleWidth'] = array(
      '#type' => 'textfield',
      '#title' => t('Flexible Width'),
      '#description' => t('Optional, the maximum width of a grid item.'),
      '#default_value' => $this->options['flexibleWidth'],
      '#size' => 30,
    );

    $form['offset'] = array(
      '#type' => 'select',
      '#title' => t('Offset'),
      '#description' => t('Vertical and horizontal distance between items, defaults to 2.'),
      '#default_value' => $this->options['offset'],
      '#required' => TRUE,
      '#options' => drupal_map_assoc(range(0,25)),
    );

    $form['fillEmptySpace'] = array(
      '#type' => 'checkbox',
      '#title' => t('Fill Empty Space.'),
      '#default_value' => $this->options['fillEmptySpace'],
      '#description' => t('Optional, fill the bottom of each column with widths of flexible height.'),
    );

    $form['autoResize'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto Resize.'),
      '#default_value' => $this->options['autoResize'],
      '#description' => t('This will auto-update the layout when the browser window is resized.'),
    );

    $form['resizeDelay'] = array(
      '#type' => 'textfield',
      '#title' => t('Resize Delay'),
      '#description' => t('Optional, time in miliseconds between browser resize and layout update.'),
      '#default_value' => (int) $this->options['resizeDelay'],
      '#dependency' => array('edit-style-options-autoresize' => array('1')),
    );

    $form['default_css'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include css'),
      '#description' => t('Include the default css file that gives some basic styling to Wookworm layouts.'),
      '#default_value' => $this->options['default_css'],
    );
  }

  function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);
    if (!is_numeric($form_state['values']['style_options']['itemWidth'])) {
      form_set_error('options][itemWidth', t('Please enter a numeric item width value.'));
    }
    if (!is_numeric($form_state['values']['style_options']['flexibleWidth'])) {
      form_set_error('options][flexible_width', t('Please enter a numeric offset value.'));
    }
    if (!is_numeric($form_state['values']['style_options']['resizeDelay'])) {
      form_set_error('options][resizeDelay', t('Please enter a numeric offset value.'));
    }
  }
}
