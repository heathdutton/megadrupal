<?php
/**
 * @file
 * Hooks and callbacks for field widget.
 */

/**
 * Implements hook_field_widget_info().
 */
function cmp_field_widget_info() {
  return array(
    'cmp_field_widget' => array(
      'label'       => t('Default'),
      'field types' => array('cmp'),
      'behaviors'   => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value'   => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function cmp_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $required = $element['#required'];
  $item = &$items[$delta];

  $settings_defaults = $instance['settings'];
  $settings_saved = isset($instance['settings']['wrapper']) ? $instance['settings']['wrapper'] : array();
  $settings = array_merge($settings_defaults, $settings_saved);

  $element['fid'] = array(
    '#type'  => 'hidden',
    '#value' => isset($item['fid']) ? $item['fid'] : 0,
  );

  $element['width'] = array(
    '#type'  => 'hidden',
    '#value' => isset($item['width']) ? $item['width'] : 0,
  );

  $element['height'] = array(
    '#type'  => 'hidden',
    '#value' => isset($item['height']) ? $item['height'] : 0,
  );

  $element['url'] = array(
    '#title'            => t('Page URL'),
    '#type'             => 'textfield',
    '#required'         => $required,
    '#default_value'    => isset($item['url']) ? $item['url'] : '',
    '#description'      => t('URL of target page. Do not need to be encode.'),
    '#element_validate' => array('_cmp_url_validate'),
    '#weight'           => 1,
  );

  // Add the additional alt and title fields.
  $element['title'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Title'),
    '#default_value' => isset($item['title']) ? $item['title'] : '',
    '#description'   => t('The title is used as a tool tip when the user hovers the mouse over the image.'),
    '#maxlength'     => 1024,
    '#weight'        => 1,
    '#access'        => $settings['title_field'],
  );

  $element['alt'] = array(
    '#title'         => t('Alternate text'),
    '#type'          => 'textfield',
    '#default_value' => isset($item['alt']) ? $item['alt'] : '',
    '#description'   => t('This text will be used by screen readers, search engines, or when the image cannot be loaded.'),
    '#maxlength'     => 512,
    '#weight'        => 2,
    '#access'        => $settings['alt_field'],
  );

  return $element;
}

/**
 * Implements hook_field_widget_error().
 */
function cmp_field_widget_error($element, $error, $form, &$form_state) {
  if ($error['error_element']['url']) {
    form_error($element['url'], $error['message']);
  }
}
