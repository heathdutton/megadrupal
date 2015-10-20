<?php

/**
 * @file
 * Process and preprocess function extentions.
 */

/**
 * Implements hook_preprocess().
 */
function pure_preprocess(&$variables, $hook) {
  // If a theme function is not a core theme function, run it through
  // template_preprocess() to get the default data.
  if (!isset($variables['classes_array'])) {
    template_preprocess($variables, $hook);
    $variables['pure_not_core'] = TRUE;
  }
  else {
    $variables['pure_not_core'] = FALSE;
  }

  // We are running preprocess functions from Pure, so theme hook suggestions
  // get lost if they are placed in the derivative functions as normal. Because
  // of this, we will need to look for an extra series of functions for
  // declaring the extra suggestions.
  _pure_suggestion_collector($variables, $hook);

  // Run all possible preprocess functions so new theme hooks do not have to be
  // entered into a hook_theme() implementation.
  _pure_hook_engine('preprocess', $variables, $hook);
}

/**
 * Implements hook_process().
 */
function pure_process(&$variables, $hook) {
  // If a hook is not core, run it through template_process() for final
  // processing.
  if ($variables['pure_not_core']) {
    template_process($variables, $hook);
  }

  // Run all possible process functions so new theme hooks do not have to be
  // entered into a hook_theme() implementation.
  _pure_hook_engine('process', $variables, $hook);
}

/**
 * Implements hook_preprocess_html().
 */
function pure_preprocess_html(&$variables, $hook) {
  // Process the extra stylesheets from this and other subthemes.
  _pure_process_stylesheets();
}

/**
 * Implements hook_suggestions_form().
 */
function pure_suggestions_form($variables, $hook, $suggestions) {
  // Create some default theme hooks to get things started.
  if (!empty($variables['element']['#form_id']) && !in_array($variables['element']['#form_id'] . '_form', $variables['theme_hook_suggestions'])) {
    $suggestions[] =  'form__' . $variables['element']['#form_id'];
  }

  return $suggestions;
}

/**
 * Implements hook_preprocess_form().
 *
 * Since Pure overrides the default theme_form() function, we will need to
 * perform some processing on attributes to make it work in a template.
 */
function pure_preprocess_form(&$variables, $hook) {
  $element = &$variables['element'];
  if (isset($element['#action'])) {
    $element['#attributes']['action'] = drupal_strip_dangerous_protocols($element['#action']);
  }
  element_set_attributes($element, array('method', 'id'));
  if (empty($element['#attributes']['accept-charset'])) {
    $element['#attributes']['accept-charset'] = "UTF-8";
  }
  $variables['attributes_array'] = $element['#attributes'];
}

/**
 * Implements hook_suggestions_form_content().
 */
function pure_suggestions_form_content($variables, $hook, $suggestions) {
  // Provide some default suggestions for theme_form_content(). The order in
  // #theme is backwards in specificity, so we reverse it before we get started.
  foreach (array_reverse($variables['form']['#theme']) as $suggestion) {
    if ($suggestion != $hook) {
      $suggestions[] = 'form_content__' . $suggestion;
    }
  }

  return $suggestions;
}

