<?php
/**
 * @file
 * Provide import/export functionality for gentle_reminder module.
 */

/**
 * Implements hook_features_api().
 */
function gentle_reminder_features_api() {
  return array(
    'gentle_reminder' => array(
      'name' => t('Gentle Reminder templates'),
      'default_hook' => 'default_gr_item_features',
      'default_file' => FEATURES_DEFAULTS_INCLUDED,
      'feature_source' => TRUE,
      'file' => drupal_get_path('module', 'gentle_reminder') . '/gentle_reminder.features.inc',
    )
  );
}

/**
 * Implements hook_features_export_options().
 */
function gentle_reminder_features_export_options() {
  $options = array();

  foreach (module_implements('gr_template') as $module) {
    $options[$module] = $module;
  }

  return $options;
}

/**
 * Implements hook_features_export().
 */
function gentle_reminder_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['gentle_reminder'] = 'gentle_reminder';

  $mailkeys = db_select('gentle_reminder_items', 'i')
    ->fields('i', array('mailkey'))
    ->execute()->fetchCol();

  foreach ($mailkeys as $mailkey) {
    $export['features']['gentle_reminder'][$mailkey] = $mailkey;
  }
}

/**
 * Implements hook_features_export_render().
 */
function gentle_reminder_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '  $templates = array();';
  $code[] = '';
  foreach ($data as $mailkey) {
    $templates = db_select('gentle_reminder_items', 'i')
      ->fields('i', array('mailkey', 'language', 'body'))
      ->condition('i.mailkey', $mailkey)
      ->execute();

    foreach ($templates as $template) {
      $code[] = "  // Exported gentle_reminder template: {$template->mailkey}-{$template->language}.";
      $code[] = "  \$templates['{$template->mailkey}']['{$template->language}'] = '{$template->body}';";
      $code[] = "";
    }
  }
  $code[] = '  return $templates;';
  $code = implode("\n", $code);

  return array('default_gr_item_features' => $code);
}

/**
 * Implements hook_features_revert().
 */
function gentle_reminder_features_revert($module) {
  gentle_reminder_features_rebuild($module);
}

/**
 * Implements of hook_features_rebuild().
 */
function gentle_reminder_features_rebuild($module) {
  if ($defaults = features_get_default('gentle_reminder', $module)) {
    foreach ($defaults as $mailkey => $default) {
      foreach ($default as $language => $body) {
        db_merge('gentle_reminder_items')
          ->key(array(
            'mailkey' => $mailkey,
            'language' => $language,
          ))
          ->fields(array(
            'mailkey' => $mailkey,
            'language' => $language,
            'body' => $body,
          ))
          ->execute();
      }
    }
  }
}
