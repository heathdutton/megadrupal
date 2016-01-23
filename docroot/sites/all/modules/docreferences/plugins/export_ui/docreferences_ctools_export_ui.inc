<?php
/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'docreferences_reference',  // As defined in hook_schema(), mymodule_myobj.
  'access' => 'administer docreferences',  // Define a permission users must have to access these pages.

  // Define the menu item.
  'menu' => array(
    'menu item' => 'docreferences',
    'menu title' => t('Documentation References'),
    'menu description' => t('Administer docreferences.'),
  ),

  // Define user interface texts.
  'title singular' => t('reference'),
  'title plural' => t('references'),
  'title singular proper' => t('Reference'),
  'title plural proper' => t('References'),
  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'docreferences_ctools_export_ui_form',
    // 'submit' and 'validate' are also valid callbacks.
  ),
  // Use this to modify the default export UI interface.
  'handler' => array(
    'class' => 'docreferences_export_ui',
    'parent' => 'ctools_export_ui',
  ),
);

/**
* Define the preset add/edit form.
*/
function docreferences_ctools_export_ui_form(&$form, &$form_state) {
  $reference = $form_state['item'];

  // reference's configuration interface.
  // Ctools includes name field automatically on its own.
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Human readable reference title.'),
    '#default_value' => $reference->title,
    '#required' => true,
    '#maxlength' => 255,
  );
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Url'),
    '#description' => t('Link to documentation or other reference material.'),
    '#default_value' => $reference->url,
    '#required' => true,
    '#maxlength' => 255,
  );
  $form['category'] = array(
    '#type' => 'textfield',
    '#title' => t('Category'),
    '#description' => t('Organize references by categories.'),
    '#default_value' => $reference->category,
    '#maxlength' => 255,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    //'#description' => t(''),
    '#default_value' => $reference->description,
    '#maxlength' => 255,
  );
  $form['weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight'),
    //'#description' => t(''),
    '#default_value' => $reference->weight,
    '#maxlength' => 10,
  );

}
