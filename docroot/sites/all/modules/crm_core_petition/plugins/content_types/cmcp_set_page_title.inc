<?php
/**
 * @file
 * Set page title for source summary report page.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Set page title'),
  'description' => t('Should be used only for source summary page.'),
  'category' => t('CRM Core Petition'),
  'edit form' => 'cmcp_set_page_title_form',
  'render callback' => 'cmcp_set_page_title_render',
  'admin info' => 'cmcp_set_page_title_admin_info',
  'defaults' => array(
    'text' => '',
  ),
  'all contexts' => TRUE,
);

/**
 * Administration info.
 */
function cmcp_set_page_title_admin_info($subtype, $conf, $contexts) {

  $block = new stdClass();
  $block->title = t('Set page title');
  $block->content = t('Should be used only for source summary page.');

  return $block;
}

/**
 * Settings form.
 */
function cmcp_set_page_title_form($form, &$form_state) {

  return $form;
}

/**
 * Submit handler for settings form.
 */
function cmcp_set_page_title_form_submit($form, &$form_state) {
}

/**
 * Render callback.
 */
function cmcp_set_page_title_render($subtype, $conf, $panel_args, $context = NULL) {

  if (!empty($panel_args[0])) {
    $title = t('Source summary for') . ' ' . check_plain($panel_args[0]);
    drupal_set_title($title);
  }
  $block = new stdClass();
  $block->title = '';
  $block->content = '';

  return $block;
}
