<?php

/**
 * @file
 * Separated administrator forms for canvas field.
 *
 */

/**
 *
 * @param array $field
 * @param array $instance
 * @return array
 */
function canvas_field_widget_settings_form_passthrough($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];
  $settings = canvas_field_defaults($settings, TRUE);
  $form = array();

  $form['style'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Style'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
    'width' => array(
      '#prefix' => '<div class="container-inline">',
      '#title' => t('Canvas Size'),
      '#type' => 'textfield',
      '#field_prefix' => '<br />',
      '#default_value' => $settings['style']['width'],
      '#size' => 5,
      '#field_suffix' => 'x',
      '#element_submit' => array('canvas_field_size_submit'),
    ),
    'height' => array(
      '#type' => 'textfield',
      '#default_value' => $settings['style']['height'],
      '#size' => 5,
      '#field_suffix' => 'pixels',
      '#suffix' => '</div>',
    ),
    'background-color' => array(
      '#type' => 'textfield',
      '#title' => t('Background Color'),
      '#default_value' => $settings['style']['background-color'],
      '#size' => 8,
      '#maxlength' => 6,
      '#field_prefix' => '#',
      '#description' => t('Enter the hexadecimal color code you want as a background color for your canvas. (e.g. FFFFFF)'),
    ),
    'border-color' => array(
      '#type' => 'textfield',
      '#title' => t('Border Color'),
      '#default_value' => $settings['style']['border-color'],
      '#size' => 8,
      '#maxlength' => 6,
      '#field_prefix' => '#',
      '#description' => t('Enter the hexadecimal color code you want as a border color for your canvas. (e.g. FFFFFF)')
    ),
    'border-width' => array(
      '#type' => 'textfield',
      '#title' => t('Border Width'),
      '#default_value' => $settings['style']['border-width'],
      '#size' => 3,
      '#maxlength' => 3,
      '#field_suffix' => 'px',
      '#description' => t('Enter the pixel width of the border you want to use for the canvas.'),
    ),
    'border-style' => array(
      '#type' => 'select',
      '#title' => t('Border Style'),
      '#default_value' => $settings['style']['border-style'],
      '#options' => array(
        'solid' => t('Solid'),
        'dotted' => t('Dotted'),
        'inset' => t('Inset'),
        'outset' => t('Outset'),
      ),
      '#description' => t('Choose the border style of the canvas border.'),
    ),
  );
  $form['color'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow Color Selection'),
    '#default_value' => $settings['color'],
  );
  $form['mode_start'] = array(
    '#type' => 'select',
    '#title' => t('New Image Behavior'),
    '#description' => t('This determines whether the canvas should be shown for <b>new</b> images, and whether a file upload can be shown instead.  <i>Note that using "Never Show File Field" will make it impossible for Internet explorer users to use this field.</i>'),
    '#options' => array(
      CANVAS_FIELD_NEVER => 'Never show Canvas Field',
      CANVAS_FIELD_FILEFIELD => 'Default to File field',
      CANVAS_FIELD_CANVASFIELD => 'Default to Canvas Field',
      CANVAS_FIELD_CANVASFIELD_ONLY => 'Never Show File Field'
    ),
    '#default_value' => $settings['mode_start'],
  );
  $form['mode_edit'] = array(
    '#type' => 'select',
    '#title' => t('Existing Image Behavior'),
    '#description' => t('This determines whether the canvas should be shown to <b>edit</b> an image.'),
    '#options' => array(
      CANVAS_FIELD_NEVER => 'Never Show Canvas Field',
      CANVAS_FIELD_FILEFIELD => 'Default to File Field',
      CANVAS_FIELD_CANVASFIELD => 'Default to Canvas Field'
    ),
    '#default_value' => $settings['mode_edit'],
  );
  return $form;
}
