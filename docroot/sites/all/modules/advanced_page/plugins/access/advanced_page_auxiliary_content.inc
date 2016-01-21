<?php
/**
 * @file
 * Advanced Page Auxiliary Content
 * Plugin to provide access/visiblity for Advanced Page has auxiliary content.
 */

/**
 * Plugins are described by creating a $plugin array which will
 * be used by the system that includes the file.
 */
$plugin = array(
  'title' => t('Advanced Page: Auxiliary Content'),
  'description' => t('Returns true if this advanced page has auxiliary content.'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'callback' => 'advanced_page_auxiliary_content_access_check',
  'settings form' => 'advanced_page_auxiliary_content_access_settings_form',
  'summary' => 'advanced_page_auxiliary_content_access_summary',
);

/**
 * Settings form for the 'advanced_page_auxiliary_content' access plugin.
 */
function advanced_page_auxiliary_content_access_settings_form(&$form, &$form_state, $conf) {
  return $form;
}

/**
 * Custom callback defined by 'callback' in the $plugin array.
 *
 * Check for access.
 */
function advanced_page_auxiliary_content_access_check($conf, $context) {
  $node = $context->data;
  $items = field_get_items('node', $node, 'field_advanced_page_aux_content');
  if ($items) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Describe an instance of this plugin.
 *
 * Provide a summary description based upon the number of auxiliary
 * content items.
 */
function advanced_page_auxiliary_content_access_summary($conf, $context) {
  return t('The "Auxiliary Content" field of the node being viewed is not empty.');
}
