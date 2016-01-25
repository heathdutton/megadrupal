<?php

/**
 * @file
 * Field API implementation of sclib_enkoder.
 */

define("SCLIB_ENKODER_NO_ENKODE", 0);
define("SCLIB_ENKODER_FULL_ENKODE", 1);
define("SCLIB_ENKODER_EMAIL_ENKODE", 2);

/**
 * Implements hook_sclib_enkoder_compatible_formats().
 *
 * @see sclib_enkoder.api.php
 */
function sclib_enkoder_sclib_enkoder_compatible_formats() {
  $formats = array();

  // Built-in support for email module
  if (module_exists("email")) {
    $formats["email"] = array();
    foreach (array("default", "contact", "plain") as $type) {
      $formats["email"][] = array(
        "type" => "email_$type",
        "custom callback" => null,
      );
    }
  }

  return $formats;
}


/**
 * Implements hook_field_formatter_info_alter().
 *
 * Modify existing text formatters to be able to be enkoded.
 */
function sclib_enkoder_field_formatter_info_alter(&$info) {
  foreach (array('default', 'plain', 'trimmed', 'summary_or_trimmed') as $type) {
    $info["text_$type"]['settings'] += array(
      'sclib_enkoder_enkode' => SCLIB_ENKODER_NO_ENKODE,
      'sclib_enkoder_overridden_module' => $info["text_$type"]['module'],
    );
    $info["text_$type"]['module'] = 'sclib_enkoder';
  }

  // Get additional overrides.
  $otherOverrides = module_invoke_all("sclib_enkoder_compatible_formats");
  foreach ($otherOverrides as $module => $typeInfos) {
    foreach ($typeInfos as $typeInfo) {
      if ($info[$typeInfo["type"]]['module'] != 'sclib_enkoder') {
        $info[$typeInfo["type"]]['settings'] += array(
          'sclib_enkoder_enkode' => SCLIB_ENKODER_NO_ENKODE,
          'sclib_enkoder_overridden_module' => $module,
        );
        $info[$typeInfo["type"]]['module'] = 'sclib_enkoder';
      }
    }
  }
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function sclib_enkoder_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = '';

  // Be sure to invoke the older summary generator, if possible.
  if (isset($settings['sclib_enkoder_overridden_module'])
      && !preg_match('/^\s*$/', $settings['sclib_enkoder_overridden_module'])) {
    $summary = module_invoke($settings['sclib_enkoder_overridden_module'],
      'field_formatter_settings_summary',
      $field, $instance, $view_mode);
    if ($summary != '') {
      $summary .= "<br/>";
    }
  }

  switch ($settings['sclib_enkoder_enkode']) {
    case SCLIB_ENKODER_FULL_ENKODE:
      $summary .= t('Enkode? Yes');
      break;

    case SCLIB_ENKODER_EMAIL_ENKODE:
      $summary .= t('Enkode? Email patterns only');
      break;

    // Intentional fall through of case SCLIB_ENKODER_NO_ENKODE.
    default:
      $summary .= t('Enkode? No');
      break;
  }

  return $summary;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function sclib_enkoder_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element = array();

  // Be sure the invoke the older form generator, if available.
  if (isset($settings['sclib_enkoder_overridden_module'])
    && !preg_match('/^\s*$/', $settings['sclib_enkoder_overridden_module'])) {
    $element = module_invoke($settings['sclib_enkoder_overridden_module'],
      'field_formatter_settings_form',
      $field, $instance, $view_mode, $form, $form_state);
  }

  $element['sclib_enkoder_enkode'] = array(
    '#title' => t('Enkode?'),
    '#type' => 'select',
    '#options' => array(
      SCLIB_ENKODER_NO_ENKODE => t('No'),
      SCLIB_ENKODER_FULL_ENKODE => t('Yes'),
      SCLIB_ENKODER_EMAIL_ENKODE => t('Email patterns only'),
    ),
    '#default_value' => $settings['sclib_enkoder_enkode'],
  );

  return $element;
}

/**
 * Implements hook_field_formatter_view().
 */
function sclib_enkoder_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $elements = array();
  $settings = $display['settings'];

  // Invoke the previous formatter; do it direct to make this call fast.
  // See field.default.inc field_default_view().
  if (isset($settings['sclib_enkoder_overridden_module'])
    && !preg_match('/^\s*$/', $settings['sclib_enkoder_overridden_module'])) {
    $function = $settings['sclib_enkoder_overridden_module'] . '_field_formatter_view';
    if (function_exists($function)) {
      $elements = $function($entity_type, $entity, $field, $instance, $langcode, $items, $display);
    }
  }

  if ($settings['sclib_enkoder_enkode'] != SCLIB_ENKODER_NO_ENKODE) {
    foreach ($items as $delta => $item) {
      $elements[$delta]['#markup'] = _sclib_enkoder_format($elements[$delta]['#markup'], $settings['sclib_enkoder_enkode']);
    }
  }

  return $elements;
}

/**
 * Helper to translate markup and text.
 */
function _sclib_enkoder_format($text, $setting) {
  $result = $text;
  switch ($setting) {
    case SCLIB_ENKODER_FULL_ENKODE:
      $result = sclib_enkoder($result);
      break;

    case SCLIB_ENKODER_EMAIL_ENKODE:
      // Catch any mailto: segments.
      $result = preg_replace_callback('/<A[^<]+HREF\s*=\s*[\'"]?MAILTO:[A-Z\p{L}0-9._%+-]+@[A-Z\p{L}0-9.-]+\.[A-Z\p{L}]{2,6}[^>]+>.+?<\/A>/ui',
        create_function(
          '$matches',
          'return sclib_enkoder($matches[0]);'
        ),
        $result);

      // Catch any remaining email address patterns.
      $result = preg_replace_callback('/\b[A-Z\p{L}0-9._%+-]+@[A-Z\p{L}0-9.-]+\.[A-Z\p{L}]{2,6}\b/ui',
        create_function(
          '$matches',
          'return sclib_enkoder($matches[0]);'
        ),
        $result);
      break;
  }

  return $result;
}
