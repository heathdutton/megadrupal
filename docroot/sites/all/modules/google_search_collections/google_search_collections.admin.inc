<?php

/**
 * @file
 * Contains admin hooks and callbacks for the Google Search Collections module.
 */


/**
 * Page callback for global admin settings.
 */
function google_search_collections_settings($form) {
  // Toggle to remove module-provided search path.
  $form['google_search_collections_unset_module_defaults'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide module-provided default search path(s)'),
    '#description' => t('Check this to hide default search paths provided by Google search modules (e.g. %example).', array(
      '!example' => 'search/gss',
    )),
    '#default_value' => variable_get('google_search_collections_unset_module_defaults', TRUE),
  );

  // The default search collection.
  $options = _google_search_collections_get_collection_options();
  $fallback = google_search_collections_get_default_collection();
  $form['google_search_collections_default_collection'] = array(
    '#type' => 'select',
    '#title' => t('Default collection'),
    '#description' => t('Controls which collection is used as the default (e.g. when searches are performed from the block form).'),
    '#required' => TRUE,
    '#default_value' => variable_get('google_search_collections_default_collection', $fallback),
    '#options' => $options,
  );

  $form['#submit'][] = 'google_search_collections_settings_submit';

  return system_settings_form($form);
}

/**
 * Submit handler for the Google Search Collections global admin settings form.
 */
function google_search_collections_settings_submit($form, &$form_state) {
  // Always rebuild the menu when .
  variable_set('menu_rebuild_needed', TRUE);
}

function _google_search_collections_get_collection_options() {
  $options = array();
  foreach (google_search_collections_get_definitions() as $path => $def) {
    $options[$path] = $def->title;
  }
  return $options;
}
