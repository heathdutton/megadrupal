<?php

/**
 * @file
 * Admin pages.
 */


/**
 * Settings page
 */
function lingwo_entry_admin_settings($form, &$form_state) {
  $settings = LingwoEntrySettings::get();

  // we call our own hook_lingwo_language_form to build the pos_allowed_values form element
  $form = _lingwo_language_form('lingwo_entry_lingwo_language_form');

  $form[$settings->name('node_title_munging')] = array(
    '#type' => 'checkboxes',
    '#title' => t('Add extra information to the node title'),
    '#options' => array(
      'language' => t('Language'),
      'pos' => t('Part of speech'),
    ),
    '#default_value' => $settings->node_title_munging,
    '#description' => t('Activate this option to add extra information to the node title.  To include the same information in more customized way, disable this option and implement it in your theme.'),
  );

  $form[$settings->name('lookup_path')] = array(
    '#type' => 'textfield',
    '#title' => t('Lookup path'),
    '#default_value' => $settings->lookup_path,
    '#description' => t('The drupal path for dictionary lookup. Do not include a trailing slash. Leave empty to disable the lookup path.'),
    '#field_prefix' => url(NULL, array('absolute' => FALSE)) . (variable_get('clean_url', 0) ? '' : '?q='),
  );

  $form[$settings->name('redirect_node')] = array(
    '#type' => 'checkbox',
    '#title' => t('Redirect entry nodes to lookup path'),
    '#default_value' => $settings->redirect_node,
    '#description' => t('If you activate this option, by going to an entry nodes <em>node/nid</em> page, you will be redirected to the same page as <em>&lt;Lookup path&gt;/Headword</em>.'),
  );

  $form = system_settings_form($form);
  // Rebuild the menu after updating the settings.
  $form['#submit'][] = 'menu_rebuild';
  return $form;
}

