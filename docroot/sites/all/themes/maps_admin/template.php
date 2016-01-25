<?php

/**
 * @file
 * Theme functions for MaPS Admin.
 */

/**
 * Implements hook_theme_registry_alter().
 */
function maps_admin_theme_registry_alter(&$theme_registry) {
  if (!isset($theme_registry['ctools_export_ui_edit_item_wizard_form'])) {
    $theme_registry['ctools_export_ui_edit_item_wizard_form'] = array(
      'render element' => 'form',
      'function' => 'maps_admin_ctools_export_ui_edit_item_wizard_form',
      'theme path' => drupal_get_path('theme', 'maps_admin'),
    );
  }
}

/**
 * Implements hook_page_alter().
 */
function maps_admin_process_page($page) {
  $font_source =  theme_get_setting('font_source', 'maps_admin');
  
  switch ($font_source) {
    case 'local':
      drupal_add_css(drupal_get_path('theme', 'maps_admin') . '/css/fonts.css', array('group' => CSS_THEME, 'every_page' => TRUE, 'weight' => -10));
      break;
    case 'remote':
      drupal_add_css('http://fonts.googleapis.com/css?family=Source+Sans+Pro:300,400,600,300italic,400italic,600italic', array('type' => 'external'));
      break;
  }
  
  // Viewport for mobile devices.
  drupal_add_html_head(
    array(
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'viewport',
        'content' => 'width=device-width, initial-scale=1, maximum-scale=1',
      )
    ),
    'viewport');

  // Horizontal scroll using mouse wheel.
  if (module_exists('libraries') && $jmw_path = libraries_get_path('jquery-mousewheel')) {
    if (file_exists($jmw_file = $jmw_path . '/jquery.mousewheel.js')) {
      drupal_add_js($jmw_file);
      drupal_add_js(array('mapsAdmin' => array('scrollHorz' => '#main_content')), 'setting');
    }
  }
}

/**
 * Override the default theme for primary and secondary local tasks.
 *
 * @ingroup themeable
 * @see theme_menu_local_tasks()
 * @see menu_local_tasks()
 */
function maps_admin_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs primary">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<div class="secondary_wrapper" /><ul class="tabs secondary">';
    $variables['secondary']['#suffix'] = '</ul></div>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function maps_admin_form_field_ui_field_edit_form_alter(&$form, &$form_state) {
  $form['field']['#collapsible'] = TRUE;
  $form['field']['#collapsed'] = TRUE;
}

/**
 * Render HTML for cTools UI wizard form.
 *
 * Note that the theme function is not provided by the cTools module.
 *
 * @ingroup themeable
 */
function maps_admin_ctools_export_ui_edit_item_wizard_form($variables) {
  $header = $content = $buttons = '';
  $form = $variables['form'];

  foreach (element_children($form, TRUE) as $key) {
    $output = drupal_render($form[$key]);

    if ($key == 'ctools_trail') {
      $header .= $output;
    }
    elseif ($key == 'buttons') {
      $buttons .= $output;
    }
    else {
      $content .= $output;
    }
  }//end foreach

  return $header . '<div class="clearfix">' . $content . '</div>' . $buttons;
}
