<?php

/**
 * @file
 * Provides a panels pane that shows user information.
 */

$plugin = array(
  'title' => t('OpenAtrium Contextual Help'),
  'description' => t('A single button that contains a dropdown menu exposing relevant contextual help.'),
  'single' => TRUE,
  'category' => array(t('Open Atrium'), -9),
  'render callback' => 'atrium_contextual_help_button_render',
);

/**
 * Render callback for the panel.
 */
function atrium_contextual_help_button_render($subtype, $conf, $args, $context = NULL) {

  // Get our view
  $display_id = 'modal_links_block';
  $view = views_get_view('contextual_help');
  if (!$view || !$view->access($display_id)) {
    return;
  }
  $view->set_display($display_id);
  $view->preview();

  $classes = '';
  if ($view->total_rows) {
    $classes .= 'contextual-help-exists';
  }

  // Include css
  drupal_add_css(drupal_get_path('module', 'atrium_contextual_help') . '/atrium_contextual_help.css');

  $vars['contextual_help'] = views_embed_view('contextual_help', 'modal_links_block');
  $vars['title'] = t('Contextual Help');
  $vars['oa_toolbar_btn_class'] = $classes;
  $vars['oa_toolbar_btn_class'] .= variable_get('oa_toolbar_style', 0) ? '' : ' btn-inverse';

  // Build our block
  $block = new stdClass();
  $block->title = '';
  $block->content = theme('atrium_contextual_help_button', $vars);

  return $block;
}
