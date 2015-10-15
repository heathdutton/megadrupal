<?php

/**
 * @file
 * For the admin pages.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function lingwo_pron_admin_settings_form($form, &$form_state) {
  $settings = &LingwoPronSettings::get();
  $form = array();

  $form[$settings->name('enabled_fields')] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enabled fields'),
    '#options' => array(
      'ipa' => t('IPA: International Phonetic Alphabet'),
      'tag' => t('Tag: Marks the pronuncation in some way (for example, dialect: US or UK)'),
      'audio' => t('Audio: An audio file demonstrating the pronuncation'),
    ),
    '#default_value' => $settings->enabled_fields,
  );

  $form[$settings->name('audio_file_path')] = array(
    '#type' => 'textfield',
    '#title' => t('Audio file path'),
    '#description' => t('Optional subdirectory within the "%directory" directory where files will be stored. Do not include preceding or trailing slashes.', array('%directory' => variable_get('file_directory_path', 'files') . '/')),
    '#default_value' => $settings->audio_file_path,
  );

  $form[$settings->name('audio_file_extensions')] = array(
    '#type' => 'textfield',
    '#title' => t('Permitted upload file extensions'),
    '#description' => t('Extensions a user can upload to this field. Separate extensions with a space and do not include the leading dot. Leaving this blank will allow users to upload a file with any extension.'),
    '#default_value' => $settings->audio_file_extensions,
  );

  $form[$settings->name('audio_file_size_max')] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum upload size per file'),
    '#description' => t('Specify the size limit that applies to each file separately. Enter a value like "512" (bytes), "80K" (kilobytes) or "50M" (megabytes) in order to restrict the allowed file size. If you leave this empty the file sizes will be limited only by PHP\'s maximum post and file upload sizes (current limit <strong>%limit</strong>).', array('%limit' => format_size(file_upload_max_size()))),
    '#default_value' => $settings->audio_file_size_max,
  );

  // merge in the tag values from our hook_lingwo_language_form().
  $form = array_merge($form, _lingwo_language_form('lingwo_pron_lingwo_language_form'));

  return system_settings_form($form);
}

