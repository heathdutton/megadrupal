<?php

/**
 * @file
 *
 * Plugin to provide a profile2_edit_form context
 *
 * @thanks to : http://internetdevels.com/blog/ctools-context-plugin
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Profile2 edit form"),
  'description' => t('Profile2 edit form.'),
  'context' => 'profile2_panels_context_create_profile2_edit_form',
  'edit form' => 'profile2_panels_context_profile2_edit_form_settings_form',
  'defaults' => array('pid' => ''),
  'convert list' => 'profile2_panels_context_profile2_edit_convert_list',
  'convert' => 'profile2_panels_context_profile2_edit_convert',
  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter the PID of a Profile2 for this argument:'),
  ),
);

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function profile2_panels_context_create_profile2_edit_form($empty, $profile2 = NULL, $conf = FALSE) {

  // We want to create the taxonomy add form, so we need to add the form contexts too.
  $context = new ctools_context(array(
    'form',
    'profile2_edit_form',
    'entity:profile2',
  ));
  $context->keyword = 'profile2_edit';
  $context->plugin = 'profile2_edit_form';

  if ($empty) {
    return $context;
  }


  // Profile2 data might not be initialized.
  // Save empty profile's. Not clean, but we need a ID later.
  if (isset($profile2->is_new) && empty($profile2->pid)) {
    $profile2->save();
    if (empty($profile2->pid)) {
      drupal_set_message("Could not load profile2", 'error');
      return "";
    }
  }
  $context->data = $profile2;


  // Profile2's submodule 'profile2_page' is used for form handling.
  module_load_include('inc', 'profile2_page', 'profile2_page');


  // For AJAX callbacks the form-file (profile2_page.inc) must be included in
  // the $formstate.
  // This piece of information can let other modules know that more files
  // need to be included if this form is loaded from cache:
  $file = drupal_get_path('module', 'profile2_page') . '/profile2_page.inc';
  $form_state['build_info']['files'] = array($file);

  // @todo: Redirect users back from where they edit the form.



  // We render the form with entityapi.
  $form = entity_ui_get_form('profile2', $profile2, 'edit', $form_state);


  // Form with correct $formstate is there, now add it to the context.
  $context->form_id = $form['#form_id'];
  $context->form = $form;
  // Empty form_title (otherwise will produce error).
  $context->form_title = "";


  return $context;
}

function profile2_panels_context_profile2_edit_form_settings_form($form, &$form_state) {

  $conf = &$form_state['conf'];

  $form['profile2'] = array(
    '#title' => t('Profile2'),
    '#type' => 'textfield',
    '#maxlength' => 512,
    '#autocomplete_path' => 'ctools/autocomplete/profile2',
    '#weight' => -10,
  );

  return $form;
}

function profile2_panels_context_profile2_edit_form_settings_form_submit($form, &$form_state) {
  $form_state['conf']['profile2'] = $form_state['values']['profile2'];
}

/**
 * Provide a list of ways that this context can be converted to a string.
 */
function profile2_panels_context_profile2_edit_convert_list() {
  // Pass through to the "profile" context convert list.
  $plugin = ctools_get_context('entity:profile2');
  return ctools_context_entity_convert_list($plugin);
}

/**
 * Convert a context into a string.
 */
function profile2_panels_context_profile2_edit_convert($context, $type) {
  if ($type == 'profile2') {
    return $context->data['profile2'];
  }
  // Pass through to the "profile" context convert list.
  $plugin = ctools_get_context('entity:profile2');
  return ctools_context_entity_convert($context, $type);
}
