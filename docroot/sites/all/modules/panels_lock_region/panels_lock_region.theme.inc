<?php
/**
 * @file
 * Implementation of hooks related to theme.
 */

/**
 * Implements hook_preprocess_HOOK().
 */
function template_preprocess_panels_lock_region_region_buttons(&$vars) {
  $region_id = $vars['region_id'];
  $display = $vars['display'];
  $renderer = $vars['renderer'];
  $vars['links'] = '';

  // Load lock status.
  $lock = $display->panel_settings['region_lock'][$region_id];

  // Add option to add items in the IPE.
  $vars['links']['add-pane'] = array(
    'title' => '<span>' . t('Add new pane') . '</span>',
    'href' => $renderer->get_url('select-content', $region_id),
    'attributes' => array(
      'class' => array('ctools-use-modal', 'add', 'panels-ipe-hide-bar'),
      'title' => t('Add new pane'),
    ),
    'html' => TRUE,
  );

  // Create lock link.
  $vars['links']['lock-region'] = array(
    'title' => '<span>' . t('Lock this region') . '</span>',
    'href' => $renderer->get_url('change-lock', $region_id, 'lock'),
    'html' => TRUE,
    'attributes' => array(
      'class' => array('use-ajax', 'lock'),
      'title' => t('Lock this region'),
      'style' => $lock == 'locked' ? 'display: none;' : '',
    ),
  );

  // Create unlock link.
  $vars['links']['unlock-region'] = array(
    'title' => '<span>' . t('Unlock this region') . '</span>',
    'href' => $renderer->get_url('change-lock', $region_id, 'unlock'),
    'html' => TRUE,
    'attributes' => array(
      'class' => array('use-ajax', 'unlock'),
      'title' => t('Unlock this region'),
      'style' => $lock == 'unlocked' ? 'display: none;' : '',
    ),
  );

  $context = array(
    'region_id' => $region_id,
    'display' => $display,
    'renderer' => $renderer,
  );

  drupal_alter('panels_lock_region_region_links', $vars['links'], $context);
}

/**
 * Theme function for add lock/unlock button.
 */
function theme_panels_lock_region_region_buttons($vars) {
  $attributes = array(
    'class' => array('panels-ipe-linkbar', 'inline'),
  );

  $links = theme('links', array('links' => $vars['links'], 'attributes' => $attributes));

  return '<div class="panels-ipe-newblock panels-ipe-on">' . $links . '</div>';
}
