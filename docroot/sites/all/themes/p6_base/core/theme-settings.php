<?php
/**
 * Implements hook_form_system_theme_settings_alter() function.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 *
 * @see  http://drupal.org/node/943212#comment-4885654
 */

// function p6_base_form_system_theme_settings_alter(&$form, $form_state, $form_id = NULL) {
//   if (isset($form_id)) {
//     return;
//   }

//   $form_themes = array();
//   $themes = list_themes();
//   $_theme = $GLOBALS['theme_key'];
//   while (isset($_theme)) {
//     $form_themes[$_theme] = $_theme;
//     $_theme = isset($themes[$_theme]->base_theme) ? $themes[$_theme]->base_theme : NULL;
//   }
//   $form_themes = array_reverse($form_themes);

//   foreach ($form_themes as $theme_key) {
//     if (function_exists($form_settings = "{$theme_key}_form_theme_settings")) {
//       $form_settings($form, $form_state);
//     }
//   }
// }

// function p6_base_form_theme_settings($form, $form_state) {
//   dsm($form);
// }
