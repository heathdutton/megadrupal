<?php

/**
 * @file
 * Simplyscroll views plugin sytle
 */

/**
  * Implementation of views_plugin_style().
  */
class simplyscroll_style_plugin extends views_plugin_style {

 /**
   * Set default options
   */
  function options(&$options) {
    $options = parent::option_definition();
    unset($options['grouping']);
    $options['frameRate'] = array('default' => 24);
    $options['speed'] = array('default' => 5);
    $options['orientation'] = array('default' => 'horizontal');
    $options['direction'] = array('default' => 'forwards');
    $options['auto'] = array('default' => TRUE);
    $options['autoMode'] = array('default' => 'loop');
    $options['manualMode'] = array('default' => 'end');
    $options['pauseOnHover'] = array('default' => TRUE);
    $options['pauseOnTouch'] = array('default' => TRUE);
    $options['pauseButton'] = array('default' => FALSE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['grouping']);
    $form['frameRate'] = array(
      '#type' => 'select',
      '#title' => t('Frame Rate'),
      '#options' => drupal_map_assoc(range(5, 50, 5)),
      '#default_value' => $this->options['frameRate'],
      '#description' => t('Number of movements/frames per second.'),
    );

    $form['speed'] = array(
      '#type' => 'select',
      '#title' => t('Speed'),
      '#options' => drupal_map_assoc(range(1, 15)),
      '#default_value' => $this->options['speed'],
      '#description' => t('Speed of the scroll.'),
    );

    $form['orientation'] = array(
      '#type' => 'select',
      '#title' => t('Orientation'),
      '#default_value' => $this->options['orientation'],
      '#options' => array('horizontal' => 'Horizontal', 'vertical' => 'Vertical'),
      '#description' => t('Horizontal or vertical scroll.'),
    );

    $form['direction'] = array(
      '#type' => 'select',
      '#title' => t('Direction'),
      '#default_value' => $this->options['direction'],
      '#options' => array('forwards' => 'Forward', 'backwards' => 'Backward'),
      '#description' => t('Forward or backward scroll.'),
    );

    $form['auto'] = array(
      '#type' => 'select',
      '#title' => t('Auto Mode'),
      '#default_value' => $this->options['auto'],
      '#options' => array('true' => 'TRUE', 'false' => 'FALSE'),
      '#description' => t('Automatic scrolling, use false for button controls.'),
    );

    $form['autoMode'] = array(
      '#type' => 'select',
      '#title' => t('Auto Mode Format'),
      '#default_value' => $this->options['autoMode'],
      '#options' => array('loop' => 'Loop', 'bounce' => 'Bounce'),
      '#description' => t('auto = true, loop or bounce (disables buttons).'),
    );

    $form['manualMode'] = array(
      '#type' => 'select',
      '#title' => t('Manual Mode Format'),
      '#default_value' => $this->options['manualMode'],
      '#options' => array('loop' => 'Loop', 'end' => 'End'),
      '#description' => t('auto = false, loop or end (end-to-end).'),
    );

    $form['pauseOnHover'] = array(
      '#type' => 'select',
      '#title' => t('Pause scroll on hover (auto only)'),
      '#options' => array('true' => 'TRUE', 'false' => 'FALSE'),
      '#default_value' => $this->options['pauseOnHover'],
    );

    $form['pauseOnTouch'] = array(
      '#type' => 'select',
      '#title' => t('Touch enabled devices only (auto only)'),
      '#options' => array('true' => 'TRUE', 'false' => 'FALSE'),
      '#default_value' => $this->options['pauseOnTouch'],
    );

    $form['pauseButton'] = array(
      '#type' => 'select',
      '#title' => t('Creates a pause button (auto only)'),
      '#options' => array('true' => 'TRUE', 'false' => 'FALSE'),
      '#default_value' => $this->options['pauseButton'],
    );
  }
}
