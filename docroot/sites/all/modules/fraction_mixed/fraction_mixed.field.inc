<?php

/**
 * @file
 *  Mixed Fraction Field API functions for widgets and formatters.
 *
 *  © 2014-2015 RedBottle Design, LLC and Five Star Tool Company.
 *  All rights reserved. This code is licensed under the GNU GPL v2 or later.
 *
 * @author Guy Paddock (guy.paddock@redbottledesign.com)
 */

/***************************************************************
 * Hook implementations
 ***************************************************************/
/**
 * Implements of hook_field_info().
 */
function fraction_mixed_field_widget_info() {
  return array(
    FRMI_WIDGET_TYPE_MIXED_FRACTION => array(
      'label'       => t('Mixed fraction'),
      'field types' => array(FRMI_FIELD_TYPE_FRACTION),
      'settings'    => _fraction_mixed_separator_settings_defaults(),
      'behaviors'   => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value'   => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_settings_form().
 */
function fraction_mixed_field_widget_settings_form($field, $instance) {
  $form     = array();
  $widget   = $instance['widget'];
  $defaults = field_info_widget_settings($widget['type']);
  $settings = array_merge($defaults, $widget['settings']);

  if ($widget['type'] == FRMI_WIDGET_TYPE_MIXED_FRACTION) {
    $form = _fraction_mixed_separator_settings_form($settings);
  }

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function fraction_mixed_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $numerator    = isset($items[$delta]['numerator'])   ? $items[$delta]['numerator']    : '';
  $denominator  = isset($items[$delta]['denominator']) ? $items[$delta]['denominator']  : '';

  $settings = $instance['widget']['settings'];

  if ($instance['widget']['type'] == FRMI_WIDGET_TYPE_MIXED_FRACTION) {
    if (!empty($numerator)) {
      $default_value = _fraction_mixed_format_fraction($numerator, $denominator, $settings);
    }

    else {
      $default_value = '';
    }

    $element += array(
      '#type'             => 'textfield',
      '#default_value'    => $default_value,
      '#size'             => 15,
      '#element_validate' => array('fraction_mixed_element_validate'),
    );
  }

  return $element;
}

/**
 * Implements of hook_field_info().
 */
function fraction_mixed_field_formatter_info() {
  return array(
    FRMI_FORMATTER_TYPE_MIXED_FRACTION => array(
      'label'       => t('Mixed fraction'),
      'field types' => array(FRMI_FIELD_TYPE_FRACTION),
      'settings'    => _fraction_mixed_separator_settings_defaults(),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function fraction_mixed_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display  = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = '';

  if ($display['type'] == FRMI_FORMATTER_TYPE_MIXED_FRACTION) {
    $summary = _fraction_mixed_separator_settings_summary($settings);
  }

  return $summary;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function fraction_mixed_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display  = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element = array();

  if ($display['type'] == FRMI_FORMATTER_TYPE_MIXED_FRACTION) {
    $element = _fraction_mixed_separator_settings_form($settings);
  }

  return $element;
}

/**
 * Implements hook_field_formatter_view().
 */
function fraction_mixed_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $settings = $display['settings'];
  $element = array();

  // Iterate through the items.
  foreach ($items as $delta => $item) {
    // Merge the item with default values, in case it's empty.
    if (($display['type'] == FRMI_FORMATTER_TYPE_MIXED_FRACTION) &&
        !fraction_field_is_empty($item, $field)) {
      $value = _fraction_mixed_format_fraction($item['numerator'], $item['denominator'], $settings);

      $element[$delta] = array(
        '#type'   => 'markup',
        '#markup' => $value,
      );
    }
  }

  return $element;
}

/***************************************************************
 * Form callbacks
 ***************************************************************/
/**
 * Form element validation handler for mixed number fraction elements.
 */
function fraction_mixed_element_validate($element, &$form_state) {
  $field    = field_widget_field($element, $form_state);
  $instance = field_widget_instance($element, $form_state);

  $value    = trim($element['#value']);
  $settings = $instance['widget']['settings'];

  $parsed_value = _fraction_mixed_parse_fraction($value, $settings);

  if (empty($parsed_value)) {
    $setting_whole_number_separator = $settings[FRMI_SETTING_WHOLE_NUMBER_SEPARATOR];
    $setting_fraction_separator     = $settings[FRMI_SETTING_FRACTION_SEPARATOR];

    $format_examples = array(
      "1 2{$setting_fraction_separator}3",
      "1-2{$setting_fraction_separator}3",
      "1{$setting_whole_number_separator}2{$setting_fraction_separator}3"
    );

    form_error(
      $element,
      t('%name must be a valid mixed number fraction in any of the following formats: %example.',
        array(
          '%name'    => $instance['label'],

          /* array_unique() filters out redundant examples if the configured
           * separator lines up with our predefined separator example.
           */
          '%example' => implode(', ', array_unique($format_examples)),
        )));
  }
  else {
    $values['numerator']    = $parsed_value->getNumerator();
    $values['denominator']  = $parsed_value->getDenominator();

    // Set the numerator and denominator values for the form.
    form_set_value($element, $values, $form_state);
  }
}

/***************************************************************
 * Internal functions
 ***************************************************************/
/**
 * Internal convenience function for building the settings form used by
 * the widget, formatter, and feeds target forms to specify the mixed number
 * separators.
 *
 * @param array $settings
 *   The widget, formatter, or feed target settings array.
 *
 * @return array
 *   The form elements for configuring the separators.
 */
function _fraction_mixed_separator_settings_form(array $settings) {
  $form = array();

  $settings = $settings + _fraction_mixed_separator_settings_defaults();

  $form[FRMI_SETTING_WHOLE_NUMBER_SEPARATOR] = array(
    '#type'          => 'textfield',
    '#title'         => t('Whole number separator'),
    '#description'   => t('Specify the separator to accept between the whole number and fraction.'),
    '#default_value' => $settings[FRMI_SETTING_WHOLE_NUMBER_SEPARATOR],
  );

  $form[FRMI_SETTING_FRACTION_SEPARATOR] = array(
    '#type'          => 'textfield',
    '#title'         => t('Fraction separator'),
    '#description'   => t('Specify the separator to accept between the numerator and denominator in the fraction.'),
    '#default_value' => $settings[FRMI_SETTING_FRACTION_SEPARATOR],
  );

  return $form;
}

/**
 * Internal convenience function for converting a mixed number value
 * into a <code>MixedFraction</code> object, honoring the provided widget,
 * formatter, or feeds target settings.
 *
 * NOTE: The whole number separator setting is honored only if the value
 *       does not include a space or dash separating the input value.
 *       In other words, this can interpret both "1 1/2" and "1-1/2" without
 *       needing to respect the whole number separator setting. If neither
 *       of those formats is encountered, then this falls back to the
 *       separator setting (not sure what else it would be, but you never
 *       know -- en-dash maybe?).
 *
 * @param string $value
 *   The mixed number as a string.
 *
 * @param array $settings
 *   The widget, formatter, or feeds target settings.
 *
 * @return MixedFraction
 *   Either the mixed fraction resulting from parsing the value; or, NULL if
 *   the value could not be parsed.
 */
function _fraction_mixed_parse_fraction($value, array $settings) {
  $settings = $settings + _fraction_mixed_separator_settings_defaults();

  $setting_whole_number_separator = $settings[FRMI_SETTING_WHOLE_NUMBER_SEPARATOR];
  $setting_fraction_separator     = $settings[FRMI_SETTING_FRACTION_SEPARATOR];

  /* Determine whole number separator dynamically from input string then
   * fall-back to configured separator, to address DDO-2393599.
   */
  $whole_number_separators = array_unique(array($setting_whole_number_separator, '-', ' '));
  $fraction_separators     = array_unique(array($setting_fraction_separator,     '/'));

  $parser       = MixedFractionParser::getInstance();
  $parseResult  = $parser->parseFraction($value, $whole_number_separators, $fraction_separators);

  if (!empty($parseResult)) {
    // Convert the parse result to a fraction.
    $result = new MixedFraction($parseResult, NULL, NULL, $whole_number_separators, $fraction_separators);
  }
  else {
    $result = NULL;
  }

  return $result;
}

/**
 * Internal convenience function for converting a numerator and denominator
 * into a formatted mixed number string value, honoring the provided widget,
 * formatter, or feeds target settings.
 *
 * @param string $numerator
 *   The numerator of the fraction.
 *
 * @param string $denominator
 *   The denominator of the fraction.
 *
 * @param array $settings
 *  The widget, formatter, or feeds target settings.
 *
 * @return string
 *   The resulting mixed number.
 */
function _fraction_mixed_format_fraction($numerator, $denominator, array $settings) {
  $settings = $settings + _fraction_mixed_separator_settings_defaults();

  $whole_number_separator = $settings[FRMI_SETTING_WHOLE_NUMBER_SEPARATOR];
  $fraction_separator     = $settings[FRMI_SETTING_FRACTION_SEPARATOR];

  $fraction = mixed_fraction($numerator, $denominator);
  $value    = $fraction->toMixedNumber($whole_number_separator, $fraction_separator);

  return $value;
}

/**
 * Internal convenience function for getting the default mixed number separator
 * settings.
 *
 * @return array
 *   The default settings for the mixed number widget, formatter, or feed
 *   target.
 */
function _fraction_mixed_separator_settings_defaults() {
  return array(
    FRMI_SETTING_WHOLE_NUMBER_SEPARATOR => '-',
    FRMI_SETTING_FRACTION_SEPARATOR     => '/',
  );
}

/**
 * Internal convenience function for building the summary of display settings,
 * used by both the formatter and feeds target settings forms.
 *
 * @param array $settings
 *   The formatter or feed target settings array.
 *
 * @return  array
 *   The summary of the settings for the formatter or feed target.
 */
function _fraction_mixed_separator_settings_summary(array $settings) {
  $settings = $settings + _fraction_mixed_separator_settings_defaults();

  $whole_number_separator = $settings[FRMI_SETTING_WHOLE_NUMBER_SEPARATOR];
  $fraction_separator     = $settings[FRMI_SETTING_FRACTION_SEPARATOR];

  $summary =
    t('Format: 1@whole_separator2@fraction_separator3',
      array('@whole_separator' => $whole_number_separator, '@fraction_separator' => $fraction_separator));

  return $summary;
}