<?php
/**
 * @file
 * Profanity export UI plugin.
 */

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  // As defined in hook_schema().
  'schema' => 'profanity_list',
  // Define a permission users must have to access these pages.
  'access' => 'administer profanity',

  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/config/content',
    'menu item' => 'profanity',
    'menu title' => 'Profanity',
    'menu description' => 'Administer profanity lists.',
  ),

  // Define user interface texts.
  'title singular' => t('list'),
  'title plural' => t('lists'),
  'title singular proper' => t('Profanity list'),
  'title plural proper' => t('Profanity lists'),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'profanity_ctools_export_ui_form',
    // 'submit' and 'validate' are also valid callbacks.
  ),
);

/**
 * Implements hook_ctools_export_ui_form().
 */
function profanity_ctools_export_ui_form(&$form, &$form_state) {
  $list = $form_state['item'];

  $form['words'] = array(
    '#type' => 'textarea',
    '#title' => t('Words'),
    '#default_value' => $list->words,
    '#required' => TRUE,
    '#description' => t('The words to match against, separated by commas.'),
  );

  $form['replacement_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Replacement mode'),
    '#default_value' => $list->replacement_mode,
    '#required' => TRUE,
    '#options' => array(
      0 => t('Replace found words with a character repeated to match the length of the original word'),
      1 => t('Replace found words with a phrase'),
    ),
  );

  $form['replacement_character'] = array(
    '#type' => 'textfield',
    '#title' => t('Replacement character'),
    '#default_value' => $list->replacement_character,
    '#maxlength' => 2,
    '#size' => 6,
    '#states' => array(
      'required' => array(
        ':input[name="replacement_mode"]' => array('value' => 0),
      ),
      'visible' => array(
        ':input[name="replacement_mode"]' => array('value' => 0),
      ),
    ),
  );

  $form['replacement_phrase'] = array(
    '#type' => 'textfield',
    '#title' => t('Replacement phrase'),
    '#default_value' => $list->replacement_phrase,
    '#maxlength' => 128,
    '#states' => array(
      'required' => array(
        ':input[name="replacement_mode"]' => array('value' => 1),
      ),
      'visible' => array(
        ':input[name="replacement_mode"]' => array('value' => 1),
      ),
    ),
  );

  $form['case_sensitive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Match words as case sensitive'),
    '#default_value' => $list->case_sensitive,
  );

  $form['match_partial'] = array(
    '#type' => 'checkbox',
    '#title' => t('Match partial occurances of words'),
    '#default_value' => $list->match_partial,
  );

  $form['#submit'][] = 'profanity_crud_form_submit';
}
