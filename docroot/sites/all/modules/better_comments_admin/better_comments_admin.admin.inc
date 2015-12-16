<?php
/**
 * @file
 */

/**
 * Form callback for Better comments administration settings form.
 */
function better_comments_admin_settings_form($form, &$form_state) {
  $settings = better_comments_admin_settings_defaults();

  $form['better_comments_admin'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );

  $form['better_comments_admin']['disable_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override default comments administration pages?'),
    '#description' =>
      t('This will remove the default menu entries for the default comments administration pages and change the URL and title of the Better comments administration page.') . '<br />' .
      t('Warning: This is experimental and could have issues if the Page manager page has been overridden.'),
    '#default_value' => $settings['disable_default'],
  );

  $form['better_comments_admin']['auto_scroll'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keep comment preview at top of page?'),
    '#default_value' => $settings['auto_scroll'],
  );

  // Build list of available View modes.
  $entity_info = entity_get_info('comment');
  $view_modes = array();
  foreach ($entity_info['view modes'] as $key => $view_mode) {
    $view_modes[$key] = $view_mode['label'];
  }

  // Ensure default value is a valid option.
  $default_value = isset($view_modes[$settings['view_mode']]) ? $settings['view_mode'] : key($view_modes);

  $form['better_comments_admin']['view_mode'] = array(
    '#type' => 'select',
    '#title' => t('Comment preview view mode'),
    '#options' => $view_modes,
    '#default_value' => $default_value,
  );

  return system_settings_form($form);
}

/**
 * Implements hook_form_HOOK_alter().
 */
function better_comments_admin_form_better_comments_admin_settings_form_alter(&$form, $form_state) {
  $form['#submit'][] = 'better_comments_admin_settings_form_submit';
}

/**
 * Submit callback for Better comment administration settings form.
 */
function better_comments_admin_settings_form_submit($form, $form_state) {
  drupal_flush_all_caches();
}
