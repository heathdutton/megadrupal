<?php
/**
 * @file
 * Export interface plugin for RoyalSlider.
 */


/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'royalslider_optionset',  // As defined in hook_schema().
  'access' => 'administer royalslider',  // Define a permission users must have to access these pages.

  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/config/media',
    'menu item' => 'royalslider',
    'menu title' => 'RoyalSlider',
    'menu description' => 'Administer RoyalSlider presets.',
  ),

  // Define user interface texts.
  'title singular' => t('Option Set'),
  'title plural' => t('Option Sets'),
  'title singular proper' => t('RoyalSlider Option Set'),
  'title plural proper' => t('RoyalSlider Option Sets'),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'royalslider_ctools_export_ui_form',
    'validate' => 'royalslider_ctools_export_ui_form_validate',
    'submit' => 'royalslider_ctools_export_ui_form_submit',
  ),
);

/**
 * Export UI form
 */
function royalslider_ctools_export_ui_form(&$form, &$form_state) {
  // Load the admin form include.
  module_load_include('inc', 'royalslider', 'royalslider.admin');

  // Make optionset reference in form_state.
  $form_state['optionset'] = &$form_state['item'];

  // Load the configuration form.
  $form = drupal_retrieve_form('royalslider_form_optionset_edit', $form_state);
}

/**
 * Validation handler
 */
function royalslider_ctools_export_ui_form_validate(&$form, &$form_state) {
  // @todo
}

/**
 * Submit handler
 */
function royalslider_ctools_export_ui_form_submit(&$form, &$form_state) {
  // Edit the reference to $form_state['optionset'] which will in turn
  // reference $form_state['item'] which is what CTools is looking for.
  $optionset = &$form_state['optionset'];
  $optionset->skin = $form_state['values']['skin'];
  $optionset->title = $form_state['values']['title'];
  $optionset->imagestyle_fullscreen = $form_state['values']['image_style']['fullscreen'];
  $optionset->imagestyle_normal = $form_state['values']['image_style']['normal'];
  $optionset->imagestyle_thumbnail = $form_state['values']['image_style']['thumbnail'];

  // Assign the values to the option set.
  $optionset->options = _royalslider_optionset_defaults();

  // Save all the values for the option set.
  foreach ($optionset->options as $key => $value) {
    if (array_key_exists($key, $form_state['values'])) {
      $optionset->options[$key] = $form_state['values'][$key];
    }
  }
}
