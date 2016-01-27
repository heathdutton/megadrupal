<?php

/**
 * Implements hook_form_system_theme_settings_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */

function tundra_form_system_theme_settings_alter(&$form, &$form_state) {
  /**
   * @ code
   * a bug in D7 causes the theme to load twice, if this file is loaded a
   * second time we return immediately to prevent further complications.
   */
  global $tundra_altered, $base_path, $base_url, $theme_info, $base_theme_info;
  if ($tundra_altered) return;
  $tundra_altered = TRUE;

  if (!isset($base_theme_info[0])) {
    $base_theme_info[0] = $theme_info;
  }

  $subject_theme = arg(count(arg()) - 1);
  $tundra_theme_path = drupal_get_path('theme', 'tundra');
  $abs_tundra_theme_path = $base_path . $tundra_theme_path;
  $theme_path = drupal_get_path('theme', $subject_theme);

  // Decoy function to fix erros resulting from missing preview.js
  drupal_add_js('Drupal.color = { callback: function() {} }', 'inline');

  // Variable that contains easing options. Chose not to use function because $tundra_theme_path would be unavailable
  if (file_exists($tundra_theme_path . '/scripts/jquery.easing-sooper.js')) {
    $easing_options = array(
    'linear' => 'linear',
    'swing' => 'swing',
    'easeInTurbo' => 'easeInTurbo',
    'easeOutTurbo' => 'easeOutTurbo',
    'easeInTurbo2' => 'easeInTurbo2',
    'easeOutTurbo2' => 'easeOutTurbo2',
    'easeInTurbo3' => 'easeInTurbo3',
    'easeOutTurbo3' => 'easeOutTurbo3',
    'easeInTurbo4' => 'easeInTurbo4',
    'easeOutTurbo4' => 'easeOutTurbo4',
    'easeInSine' => 'easeInSine',
    'easeOutSine' => 'easeOutSine',
    'easeInExpo' => 'easeInExpo',
    'easeOutExpo' => 'easeOutExpo',
    'easeInCirc' => 'easeInCirc',
    'easeOutCirc' => 'easeOutCirc',
    'easeInElastic' => 'easeInElastic',
    'easeOutElastic' => 'easeOutElastic',
    'easeInOvershoot' => 'easeInOvershoot',
    'easeOutOvershoot' => 'easeOutOvershoot',
    'easeInOvershootTurbo' => 'easeInOvershootTurbo',
    'easeOutOvershootTurbo' => 'easeOutOvershootTurbo',
    'easeInBounce' => 'easeInBounce',
    'easeOutBounce' => 'easeOutBounce',
    );
  }
  else {
    $easing_options = array(
    'linear' => 'linear',
    'swing' => 'swing',
    );
  }

// Return the additional form widgets
return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 * We hijack the function that is reserved for the user module in order
 * to get the full monty of $form stuff. The module cache is cleared to make sure
 * our hook implementation is known before this point. Don't tell Dries about this!
 */
if (module_exists('color')) {
  registry_rebuild();
  function user_form_system_theme_settings_alter(&$form, &$form_state) {
    if (isset($form['color'])) {
      $form['arctica_settings']['color'] = $form['color'];
      unset($form['color']);
      $form['arctica_settings']['color']['#title'] = 'Color module';
      $form['arctica_settings']['color']['#weight'] = 1;
    }
  }
}

/**
 * Validation Function to enforce hexadecimal values
 * @todo make the regex for the hex color recognition
 */
function _tundra_is_hex_color($formelement, &$form_state) {
  return TRUE;
}
