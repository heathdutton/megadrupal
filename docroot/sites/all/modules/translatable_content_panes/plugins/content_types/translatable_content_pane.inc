<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Translatable content'),
  'single' => TRUE,
  'defaults' => array(
    'id' => '',
  ),
  'description' => t('Add new translatable custom content.'),
  'category' => t('Custom'),
  'top level' => TRUE,
  'no title override' => TRUE,
  'edit text' => t('Edit content'),
);

/**
 * The form to add or edit the content.
 */
function translatable_content_panes_translatable_content_pane_content_type_edit_form($form, &$form_state) {
  if (!empty($form_state['conf']['id'])) {
    $pane = translatable_content_panes_get_pane($form_state['conf']['id'], TRUE);
  }
  else {
    $pane = array();
  }

  module_load_include('inc', 'translatable_content_panes', 'translatable_content_panes.admin');
  $form += translatable_content_panes_form_builder($pane);

  return $form;
}

/**
 * Helper function to generate an ID for our entities
 */
function _translatable_content_panes_generate_uuid() {
  // The field names refer to RFC 4122 section 4.1.2.
  return sprintf('%04x%04x-%04x-4%03x-%04x-%04x%04x%04x',
    // 32 bits for "time_low".
    mt_rand(0, 65535), mt_rand(0, 65535),
    // 16 bits for "time_mid".
    mt_rand(0, 65535),
    // 12 bits after the 0100 of (version) 4 for "time_hi_and_version".
    mt_rand(0, 4095),
    bindec(substr_replace(sprintf('%016b', mt_rand(0, 65535)), '10', 0, 2)),
    // 8 bits, the last two of which (positions 6 and 7) are 01, for "clk_seq_hi_res"
    // (hence, the 2nd hex digit after the 3rd hyphen can only be 1, 5, 9 or d)
    // 8 bits for "clk_seq_low" 48 bits for "node".
    mt_rand(0, 65535), mt_rand(0, 65535), mt_rand(0, 65535)
  );
}

/**
 * Submit handler for the above form.
 */
function translatable_content_panes_translatable_content_pane_content_type_edit_form_submit($form, &$form_state) {
  // Do we need to create a new pane?
  if (empty($form_state['conf']['id'])) {
    // Generate a new uuid.
    $form_state['conf']['id'] = _translatable_content_panes_generate_uuid();

    // Save the new pane to the database.
    $record = array(
      'id' => $form_state['conf']['id'],
      'admin_title' => $form_state['values']['admin_title'],
      'code_content' => $form_state['values']['code_content'],
    );
    drupal_write_record('translatable_content_pane', $record);
  }

  // Use the helper from the admin form.
  module_load_include('inc', 'translatable_content_panes', 'translatable_content_panes.admin');
  translatable_content_panes_save_pane_content($form_state['conf']['id'], $form_state['values']);
}

/**
 * Returns the administrative title for a translatable content pane.
 */
function translatable_content_panes_translatable_content_pane_content_type_admin_title($subtype, $conf) {
  $pane = translatable_content_panes_get_pane($conf['id']);
  if (!empty($pane)) {
    return $pane['admin_title'];
  }
}

/**
 * Outputs the content for the translatable content pane.
 */
function translatable_content_panes_translatable_content_pane_content_type_render($subtype, $conf, $panel_args) {
  $pane = translatable_content_panes_get_pane($conf['id'], TRUE);
  if (empty($pane)) {
    return;
  }

  // Ensure a language key.
  $pane += array('language' => LANGUAGE_NONE);

  // If the locale module is on.
  if (module_exists('locale')) {
    $langcode = $GLOBALS['language']->language;

    // Is there a specific language version?
    if (isset($pane['translations'][$langcode])) {
      // Change the pane's content to the translated version.
      $pane['title'] = $pane['translations'][$langcode]['title'];
      $pane['content'] = $pane['translations'][$langcode]['content'];
      $pane['format'] = $pane['translations'][$langcode]['format'];
    }
  }

  // The returned block.
  $block = new stdClass();
  $block->module = 'translatable_content_panes';
  $block->delta = $pane['id'];
  $block->title = check_plain($pane['title']);
  $block->content = array(
    'content' => array(
      '#theme' => 'translatable_content_pane',
      '#pane' => $pane,
      '#content' => check_markup($pane['content'], $pane['format'], $pane['language']),
    ),
    '#contextual_links' => array(
      'content' => array('admin/content/translatable-content-panes', array($pane['id'])),
    ),
  );

  return $block;
}
