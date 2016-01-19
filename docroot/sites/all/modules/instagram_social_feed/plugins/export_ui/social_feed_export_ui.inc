<?php
/**
 * @file
 */

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'instagram_social_feeds',  // As defined in hook_schema().
  'access' => 'administer instagram_social_feed settings',  // Define a permission users must have to access these pages.
  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/config/services',
    'menu item' => 'instagram_social_feed',
    'menu title' => 'Manage Instagram feeds',
    'menu description' => 'Administer Instagram feeds.',
  ),
  // Define user interface texts.
  'title singular' => t('preset'),
  'title plural' => t('presets'),
  'title singular proper' => t('Instagram feed'),
  'title plural proper' => t('Instagram feeds'),
  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'instagram_social_feed_ctools_export_ui_form',
  ),
);

/**
 * Define the preset add/edit form.
 */
function instagram_social_feed_ctools_export_ui_form(&$form, &$form_state) {
  $preset = $form_state['item'];
  $form['feed_type'] = array(
    '#type' => 'select',
    '#title' => t('Feed type'),
    '#default_value' => $preset->feed_type,
    '#required' => TRUE,
    '#options' => instagram_social_feed_get_feed_type_options(),
  );
  $form['search_term'] = array(
    '#type' => 'textfield',
    '#title' => t('Search term (if applicable)'),
    '#description' => t("Leave blank if selecting user feed or user's own photos"),
    '#default_value' => $preset->search_term,
  );
  $form['auto_publish'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto-publish?'),
    '#default_value' => $preset->auto_publish,
    '#required' => FALSE,
  );
  $form['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable this feed?'),
    '#default_value' => $preset->enabled,
    '#required' => FALSE,
  );
}
