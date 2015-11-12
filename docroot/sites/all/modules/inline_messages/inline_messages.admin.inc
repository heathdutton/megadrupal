<?php
// $Id: inline_messages.admin.inc,v 1.3.4.1 2010/11/10 20:32:18 jsfwd Exp $

/**
 * @file
 * Administration settings page
 * - General
 * - jQuery.ScrollTo Settings
 * - Display Settings
 */
function inline_messages_admin_settings() {
  $tokens = array(
    '%form_id' => 'form_id'
  );

  /**
   * General
   */
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );

  // change the comment submit button text?
  $form['general']['inline_messages_form_comment_submit'] = array(
    '#type' => 'textfield',
    '#title' => 'Comment form submit button text',
    '#description' => t('Enter a custom label for the %what form submit button.', array('%what' => 'Comment')),
    '#default_value' => variable_get('inline_messages_form_comment_submit', ''),
  );
  if (!module_exists('comment')) {
    $form['general']['inline_messages_form_comment_submit']['#disabled'] = TRUE;
  }

  // hide the preview button?
  $form['general']['inline_messages_hide_preview'] = array(
    '#type' => 'checkbox',
    '#title' => 'Hide preview button',
    '#default_value' => variable_get('inline_messages_hide_preview', 0),
    '#description' => t('Hide the preview button for any form')
  );

  /**
   * jQuery.ScrollTo Settings
	 */
  $form['scrollto'] = array(
    '#type' => 'fieldset',
    '#title' => t('jQuery.ScrollTo Settings'),
    '#description' => t('You can add additional settings to add animation to page scrolling. Refer to the !link page for more information.', array('!link' => SCROLLTO_LINK)),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );

  $form['scrollto']['inline_messages_scrollto_path'] = array(
    '#type' => 'textfield',
    '#title' => 'Path to jQuery.ScrollTo plugin',
    '#description' => t('This module is packaged with %file or you can specify a path relative to your site root.', array('%file' => 'jquery.scrollTo-1.4.2-min.js')),
    '#default_value' => variable_get('inline_messages_scrollto_path', SCROLLTO_PATH_DEFAULT),
  );

  $form['scrollto']['inline_messages_scrollto_duration'] = array(
    '#type' => 'textfield',
    '#title' => t('Duration'),
    '#default_value' => variable_get('inline_messages_scrollto_duration', ''),
    '#description' => t('Length of the animation in milliseconds, none or 0 to jump immediately.'),
  );
  $form['scrollto']['inline_messages_scrollto_offset'] = array(
    '#type' => 'textfield',
    '#title' => t('Offset'),
    '#default_value' => variable_get('inline_messages_scrollto_offset', ''),
    '#description' => t('Number of pixels (positive or negative) to be added to the final target position.'),
  );

  /**
	 * Display Settings
   */
  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );

  $form['display']['inline_messages_display'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Use Inline Messages for the following'),
    '#default_value' => variable_get('inline_messages_display', array('site')),
    '#options' => array(
      'site' => t('Site'),
      'edit' => t('Content Editing'),
      'admin' => t('Administration')
      ),
    '#description' => theme('item_list', array('items' => array(
      t('Site: Node / Panel pages'),
      t('Content Editing: Editing existing posts or creating new ones'),
      t('Administration: Administration pages')
    )))
  );

  $form['display']['inline_messages_formids_options'] = array(
    '#type' => 'radios',
    '#title' => t('Custom settings'),
    '#default_value' => variable_get('inline_messages_formids_options', NULL),
    '#options' => array(
      'include' => t('Include'),
      'exclude' => t('Exclude')
    ),
    '#description' => t('Additionally you can choose to include or exclude forms by their %form_id.', $tokens),
  );

  $form['display']['inline_messages_formids'] = array(
    '#type' => 'textarea',
    '#title' => t('Form ids'),
    '#default_value' => variable_get('inline_messages_formids', NULL),
    '#description' => t('Enter one %form_id per line.', $tokens),
  );
  
  $form['#validate'][] = 'inline_messages_settings_validate';
  return system_settings_form($form);
}

/**
 * Validate
 */
function inline_messages_settings_validate($form, &$form_state) {
  // make sure there's a valid path to the jquery.scrollto plugin
  if (!file_exists($form_state['values']['inline_messages_scrollto_path'])) {
    form_error($form['inline_messages_scrollto']['inline_messages_scrollto_path'], t('Enter a valid path to the !link plugin.', array('!link' => SCROLLTO_LINK)));
  }

  // validate the jquery.scrollto settings
  if (!empty($form_state['values']['inline_messages_scrollto_duration']) && !is_numeric($form_state['values']['inline_messages_scrollto_duration'])) {
    form_error($form['inline_messages_scrollto']['inline_messages_scrollto_duration'], t('%setting must be numeric', array('%setting' => 'Duration')));
  }
  if (!empty($form_state['values']['inline_messages_scrollto_offset']) && !is_numeric($form_state['values']['inline_messages_scrollto_offset'])) {
    form_error($form['inline_messages_scrollto']['inline_messages_scrollto_offset'], t('%setting must be numeric', array('%setting' => 'Offset')));
  }
}