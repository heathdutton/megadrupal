<?php
/**
 * @file
 * Theme functions for fieldset helper module
 */


/**
 * Output toggle all fieldsets link.
 */
function fieldset_helper_toggle_all_output() {
  global $theme;

  if (strpos($_GET['q'], 'admin/modules') === 0) {
    $toggle_all_selectors = array(
      'rubik' => '#system-modules div.column-wrapper > fieldset.collapsible',
      'default' => '#system-modules > div > fieldset.collapsible',
    );
    $toggle_all_selector = ( isset($toggle_all_selectors[$theme]) ) ? $toggle_all_selectors[$theme] : $toggle_all_selectors['default'];
  }
  else {
    $toggle_all_selector = '';
  }

  return theme('fieldset_helper_toggle_all', array('selector' => $toggle_all_selector));
}


////////////////////////////////////////////////////////////////////////////////
// Theme functions
////////////////////////////////////////////////////////////////////////////////

/**
 * Implements hook_theme().
 */
function fieldset_helper_theme() {
  return array(
    'fieldset_helper_toggle_all' => array(
      'variables' => array('selector' => NULL),
      'template' => 'templates/fieldset-helper-toggle-all',
    ),
  );
}

/**
 * Preprocess variables for the fieldset-helper-toggle-all.tpl.php file.
 */
function template_preprocess_fieldset_helper_toggle_all(&$variables) {
  // Get id
  $id = 'fieldset-helper-toggle-all';
  $id .= (!empty($variables['selector'])) ? preg_replace('/[^-a-z0-9]+/', '-', $variables['selector']) : '';
  $id = drupal_html_id($id);

  // Add id to template variables
  $variables['id'] = $id;

  // Add toggle all id and selector to js settings.
  $settings['fieldset_helper_toggle_all'][$id] = $variables['selector'];
  drupal_add_js($settings, 'setting');
}


////////////////////////////////////////////////////////////////////////////////
// Theme preprocess functions
////////////////////////////////////////////////////////////////////////////////

function fieldset_helper_preprocess_html(&$variables) {
  if (!user_access('save fieldset state')) {
    return;
  }

  // Only use regular expression fieldset counting if no fieldsets are found
  if (fieldset_helper_number_of_collapsible_fieldset() !== 0) {
    return;
  }

  // Reset fieldset counter.
  fieldset_helper_number_of_collapsible_fieldset(0, TRUE);
  $regions = array('page_top', 'page', 'page_bottom');
  foreach ($regions as $region) {
    if (!isset($variables[$region]) || !isset($variables[$region]['#children'])) {
      continue;
    }
    $html = $variables[$region]['#children'];
    if ( stripos($html, '<fieldset') !== FALSE && $count = preg_match_all('/<fieldset[^>]+class="[^"]*collapsible/', $html, $mat)) {
      fieldset_helper_number_of_collapsible_fieldset($count);
      return;
    }
  }
}

/**
 * Implements hook_preprocess_page().
 */
function fieldset_helper_preprocess_page(&$variables) {
  if (!user_access('save fieldset state')) {
    return;
  }

  // Count collapsible fieldsets in admin/modules
  if (isset($variables['page']['content']['system_main']['modules'])) {
    foreach (array_keys($variables['page']['content']['system_main']['modules']) as $key) {
      if (strpos($key, '#') !== 0) {
        fieldset_helper_number_of_collapsible_fieldset(1);
      }
    }
  }

  // Add toggle all.
  $number_of_collapsible_fieldsets = fieldset_helper_number_of_collapsible_fieldset();
  $minimum = variable_get('fieldset_helper_toggle_all_minimum', 2);
  if ($number_of_collapsible_fieldsets >= $minimum) {
    if ($output = fieldset_helper_toggle_all_output()) {
      $variables['page']['content']['fieldset_helper_toggle_all'] = array(
        '#children' => fieldset_helper_toggle_all_output(),
        '#weight' => -1000,
      );
      $variables['page']['content']['#sorted'] = FALSE;
    }
  }
}

/**
 * Implements hook_preprocess_fieldset().
 */
function fieldset_helper_preprocess_fieldset(&$variables) {
  _fieldset_helper_alter_fieldset($variables['element']);
}


////////////////////////////////////////////////////////////////////////////////
// Alter fieldset functions.
////////////////////////////////////////////////////////////////////////////////

/**
 * Alter fieldset element and set default collapsible and collapsed attributes.
 */
function _fieldset_helper_alter_fieldset(&$form_element) {
  // Make sure each fieldset element is only altered once.
  if (isset($form_element['#fieldset_helper_processed'])) {
    return;
  }

  // Don't set collapsible or collapsed classes for fieldsets that are missing
  // a <legend> (aka #title).
  if (empty($form_element['#title'])) {
    return;
  }

  // Set id for fieldsets without them.
  if ( empty($form_element['#id']) ) {
    $id = drupal_html_id(preg_replace('/[^a-z0-9]+/', '-', drupal_strtolower('fieldset-' . $form_element['#title'])));
    $form_element['#id'] = $id;
  }

  // Set attribute id.
  $form_element['#attributes']['id'] = $form_element['#id'];

  // Set children. Prevents 'Undefined index: #children in theme_fieldset()'.
  if (!isset($form_element['#children'])) {
    $form_element['#children'] = '';
  }

  // Set all fieldsets to be collapsible.
  if ($default_collapsible = fieldset_helper_default_collapsible()) {
    $form_element['#collapsible'] = $default_collapsible;
  }

  // Collapse all collapsible fieldsets by default.
  if (isset($form_element['#collapsible']) && $form_element['#collapsible'] === TRUE) {
    if ($default_collapsed = fieldset_helper_default_collapsed()) {
      $form_element['#collapsed'] = $default_collapsed;
    }
  }

  // Set fieldset state
  if (user_access('save fieldset state')) {
    $collapsed = isset($form_element['#collapsed']) ? $form_element['#collapsed'] : FALSE;
    $form_element['#collapsed'] = fieldset_helper_state_manager_get_state($form_element['#id'], $collapsed);
  }

  // Alter fieldset element.
  drupal_alter('fieldset', $form_element);

  // Unset collapsible and collapsed classes and collapse.js
  if (isset($form_element['#attributes']['class'])) {
    $form_element['#attributes']['class'] = array_diff($form_element['#attributes']['class'], array('collapsible', 'collapsed'));
  }

  // Reset collapsible fieldsets. From: form_process_fieldset(&$form_element, &$form_state).
  if (!empty($form_element['#collapsible'])) {
    $form_element['#attributes']['class'][] = 'collapsible';
    if (!empty($form_element['#collapsed'])) {
      $form_element['#attributes']['class'][] = 'collapsed';
    }

    // Set has collapsible fieldset.
    $is_vertical_tab = (isset($form_element['#group']) && $form_element['#group'] == TRUE);
    if (!$is_vertical_tab) {
      fieldset_helper_number_of_collapsible_fieldset(1);
    }
  }

  $form_element['#fieldset_helper_processed'] = TRUE;
}
