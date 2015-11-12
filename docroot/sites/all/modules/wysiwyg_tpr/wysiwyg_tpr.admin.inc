<?php

/**
 * @file
 * Administrative functions for "WYSIWYG toolbar per role".
 */

/**
 * Form builder for Wysiwyg profile form.
 */
function wysiwyg_tpr_profile_form($form, &$form_state, $profile, $rid) {
  module_load_include('inc', 'wysiwyg', 'wysiwyg.admin');

  $form['role'] = array('#type' => 'value', '#value' => 'all');
  if ($rid === 'all') {
    // Use default profile settings.
    return wysiwyg_profile_form($form, $form_state, $profile);
  }

  // Merge in defaults.
  $profile = (array) $profile;
  $profile += array(
    'format' => '',
    'editor' => '',
  );
  if (empty($profile['settings'])) {
    $profile['settings'] = array();
  }
  $profile['settings'] += array(
    'default' => TRUE,
    'user_choose' => FALSE,
    'show_toggle' => TRUE,
    'theme' => 'advanced',
    'language' => 'en',
    'access' => 1,
    'access_pages' => "node/*\nuser/*\ncomment/*",
    'buttons' => array(),
    'toolbar_loc' => 'top',
    'toolbar_align' => 'left',
    'path_loc' => 'bottom',
    'resizing' => TRUE,
    // Also available, but buggy in TinyMCE 2.x: blockquote,code,dt,dd,samp.
    'block_formats' => 'p,address,pre,h2,h3,h4,h5,h6,div',
    'verify_html' => TRUE,
    'preformatted' => FALSE,
    'convert_fonts_to_spans' => TRUE,
    'remove_linebreaks' => TRUE,
    'apply_source_formatting' => FALSE,
    'paste_auto_cleanup_on_paste' => FALSE,
    'css_setting' => 'theme',
    'css_path' => NULL,
    'css_classes' => NULL,
  );
  $profile = (object) $profile;

  $formats = filter_formats();
  $editor = wysiwyg_get_editor($profile->editor);
  $role = user_role_load($rid);
  $form['role']['#value'] = $role;

  // Load custom profile.
  if (($custom_profile = wysiwyg_tpr_profile_load($profile->format, $rid)) !== FALSE) {
    $profile = $custom_profile;
  }

  // Get original profile form with custom profile settings.
  $form = wysiwyg_profile_form($form, $form_state, $profile);
  drupal_set_title(t('%editor profile for %format, role %role_name', array('%editor' => $editor['title'], '%format' => $formats[$profile->format]->name, '%role_name' => $role->name)), PASS_THROUGH);

  return $form;
}

/**
 * Submit callback for WYSIWYG tpr profile form.
 *
 * This is basically a clone of wysiwyg_profile_form_submit() but stores the
 * settings per role.
 *
 * @see wysiwyg_tpr_profile_form()
 * @see wysiwyg_profile_form_submit()
 */
function wysiwyg_tpr_profile_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  if (isset($values['buttons'])) {
    // Store only enabled buttons for each plugin.
    foreach ($values['buttons'] as $plugin => $buttons) {
      $values['buttons'][$plugin] = array_filter($values['buttons'][$plugin]);
    }
    // Store only enabled plugins.
    $values['buttons'] = array_filter($values['buttons']);
  }
  // Remove any white-space from 'block_formats' setting, since editor
  // implementations rely on a comma-separated list to explode().
  $values['block_formats'] = preg_replace('@\s+@', '', $values['block_formats']);

  // Remove input format name.
  $format = $values['format'];
  $input_format = $values['input_format'];
  $editor = $values['editor'];
  $role = $values['role'];
  unset($values['format'], $values['input_format'], $values['editor'], $values['role']);

  // Remove FAPI values.
  // @see system_settings_form_submit()
  unset($values['submit'], $values['form_id'], $values['op'], $values['form_token'], $values['form_build_id']);

  if (empty($role) || empty($role->rid) || $role === 'all') {
    // Save settings in default table.
    db_merge('wysiwyg')
            ->key(array('format' => $format))
            ->fields(array(
              'editor' => $editor,
              'settings' => serialize($values),
            ))
            ->execute();
    wysiwyg_profile_cache_clear();
    drupal_set_message(t('Wysiwyg profile for %format has been saved.', array('%format' => $input_format)));

    $form_state['redirect'] = 'admin/config/content/wysiwyg';
  }
  else {
    // Store information in custom table.
    db_merge('wysiwyg_tpr')
            ->key(array('format' => $format, 'rid' => $role->rid))
            ->fields(array(
              'editor' => $editor,
              'rid' => $role->rid,
              'settings' => serialize($values),
            ))
            ->execute();
    // Clear internal caches.
    wysiwyg_tpr_profile_cache_clear();

    drupal_set_message(t('Wysiwyg profile for %format has been saved for role %role.', array('%format' => $input_format, '%role' => $role->name)));

    $form_state['redirect'] = "admin/config/content/wysiwyg/profile/{$format}";
  }
}
