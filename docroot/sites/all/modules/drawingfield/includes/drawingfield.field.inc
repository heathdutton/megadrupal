<?php
/**
 * @file
 * Content module integration.
 */

/**
 * Implements hook_field_info().
 */
function drawingfield_field_info() {
  return array(
    // The machine name of the field,
    'drawingfield' => array(
      // The human-readable label of the field that will be
      // seen in the Manage fields screen.
      'label' => t('Drawing field'),
      // A description of what type of data the field stores.
      'description' => t('Store a canvas based drawing in the database.'),
      'default_widget' => 'drawingfield_default',
      'default_formatter' => 'drawingfield_formatter',
    ),
  );
}

/**
 * Implements hook_field_validate().
 *
 * This hook gives us a chance to validate content that's in our
 * field. We're really only interested in the $items parameter, since
 * it holds arrays representing content in the field we've defined.
 * We want to verify that the items only contain RGB hex values like
 * this: #RRGGBB. If the item validates, we do nothing. If it doesn't
 * validate, we add our own error notification to the $errors parameter.
 *
 * @see drawingfield_field_widget_error()
 */
function drawingfield_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  if (preg_match('~[^\w\d\#]~', $instance['widget']['settings']['background_color'])) {
    form_set_error('width', t("Only hexa values and color name are allowed."));
  }
  if (!is_numeric($instance['widget']['settings']['width'])) {
    form_set_error('width', t("Only numeric values are allowed in width field."));
  }
  if (!is_numeric($instance['widget']['settings']['height'])) {
    form_set_error('height', t("Only numeric values are allowed in height field."));
  }
  foreach ($items as $item) {
    if (!empty($item['value'])) {
      if (!empty($field['max_length']) && drupal_strlen($item['value']) > $field['max_length']) {
        form_set_error($errors, t('%name: the value may not be longer than %max characters.',
                array('%name' => $field['widget']['label'], '%max' => $field['max_length'])));
      }
    }
  }
}

/**
 * Implements hook_field_is_empty().
 *
 * Hook_field_is_emtpy() is where Drupal asks us if this field is empty.
 * Return TRUE if it does not contain data, FALSE if it does. This lets
 * the form API flag an error when required fields are empty.
 */
function drawingfield_field_is_empty($item, $field) {
  return empty($item['data']);
}

/**
 * Implements hook_field_formatter_info().
 *
 * We need to tell Drupal that we a types of formatters
 * that will change the background color.
 *
 * @see drawingfield_field_formatter_view()
 */
function drawingfield_field_formatter_info() {
  return array(
    // The machine name of the formatter.
    'drawingfield_formatter' => array(
      // The human-readable label shown on the Display
      // fields screen.
      'label' => t('Drawing Image'),
      // An array of the field types this formatter
      // can be used on.
      'field types' => array('drawingfield'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 *
 * Two formatters are implemented.
 * - field_example_simple_text just outputs markup indicating the color that
 *   was entered and uses an inline style to set the background
 *   color to that value.
 *
 * @see drawingfield_field_formatter_info()
 */
function drawingfield_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  switch ($display['type']) {
    // This formatter simply outputs the field as text and with a color.
    case 'drawingfield_formatter':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#markup' => theme('drawingfield_display', array(
            'data' => $item['data'],
            'settings' => $instance['widget']['settings'],
          )),
        );
      }
      break;
  }
  return $element;
}

/**
 * Implements hook_field_widget_info().
 *
 * Three widgets are provided.
 * These widget types will eventually show up in hook_field_widget_form,
 * where we will have to flesh them out.
 *
 * @see drawingfield_field_widget_form()
 */
function drawingfield_field_widget_info() {
  return array(
    // The machine name of the widget.
    'drawingfield_default' => array(
      // The human-readable label of the field that will be
      // seen in the Manage fields screen.
      'label' => t('Drawing field widget'),
      // An array of the field types this widget can be
      // used with.
      'field types' => array('drawingfield'),
    ),
  );
}

/**
 * Implements hook_field_widget_settings_form().
 */
function drawingfield_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];
  if (empty($settings)) {
    $settings['background_color'] = '';
    $settings['width'] = DEFAULT_DRAWING_WIDTH;
    $settings['height'] = DEFAULT_DRAWING_HEIGHT;
  }
  $form = array();
  $form['background_color'] = array(
    '#type' => 'textfield',
    '#maxlength' => 7,
    '#size' => 7,
    '#title' => t('Background Color'),
    '#default_value' => $settings['background_color'],
    '#required' => FALSE,
    '#description' => t('Set default color code or color name for background.If not set, default color will be transparent.'),
  );
  $form['width'] = array(
    '#type' => 'textfield',
    '#maxlength' => 4,
    '#size' => 3,
    '#title' => t('Width'),
    '#default_value' => $settings['width'],
    '#required' => FALSE,
    '#description' => t('Set default width in px.'),
  );
  $form['height'] = array(
    '#type' => 'textfield',
    '#maxlength' => 3,
    '#size' => 2,
    '#title' => t('Height'),
    '#default_value' => $settings['height'],
    '#required' => FALSE,
    '#description' => t('Set default height in px.'),
  );
  return $form;
}

/**
 * Implements hook_field_widget_form().
 *
 * Hook_widget_form() is where Drupal tells us to create form elements for
 * our field's widget.
 *
 * We provide one of three different forms, depending on the widget type of
 * the Form API item provided.
 */
function drawingfield_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $widget = $element;
  $widget['#delta'] = $delta;
  $widget += array(
    '#type' => 'drawingfield',
    '#default_value' => isset($items[$delta]['data']) ? $items[$delta]['data'] : NULL,
    '#background_color' => isset($instance['widget']['settings']['background_color']) ? check_plain($instance['widget']['settings']['background_color']) : 'transparent',
    '#width' => isset($instance['widget']['settings']['width']) ? check_plain($instance['widget']['settings']['width']) : DEFAULT_DRAWING_WIDTH,
    '#height' => isset($instance['widget']['settings']['height']) ? check_plain($instance['widget']['settings']['height']) : DEFAULT_DRAWING_HEIGHT,
  );
  $element['data'] = $widget;
  return $element;
}

/**
 * Implements hook_field_widget_error().
 *
 * Hook_field_widget_error() lets us figure out what to do with errors
 * we might have generated in hook_field_validate(). Generally, we'll just
 * call form_error().
 *
 * @see drawingfield_field_validate()
 * @see form_error()
 */
function drawingfield_field_widget_error($element, $error, $form, &$form_state) {
  switch ($error['error']) {
    case 'drawingfield_invalid':
      form_error($element, $error['message']);
      break;
  }
}
