<?php

/**
 * @file
 * Uptolike Ctools Export UI file.
 */

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'uptolike_preset',
  'access' => 'administer uptolike',

  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/config/system',
    'menu item' => 'uptolike',
    'menu title' => 'Uptolike',
    'menu description' => 'Administer Uptolike presets.',
  ),
  // Define user interface texts.
  'title singular' => t('preset'),
  'title plural' => t('presets'),
  'title singular proper' => t('Uptolike preset'),
  'title plural proper' => t('Uptolike presets'),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'uptolike_ctools_export_ui_form',
    'submit' => 'uptolike_ctools_export_ui_form_submit',
    // 'submit' and 'validate' are also valid callbacks.
  ),
);

/**
 * Define the preset add/edit form.
 */
function uptolike_ctools_export_ui_form(&$form, &$form_state) {
  $preset = $form_state['item'];

  // Add Uptolike's configuration interface.
  $form['constructor'] = array(
    '#markup' => uptolike_constructor(),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'uptolike') . '/css/uptolike-admin.css'),
      'js' => array((drupal_get_path('module', 'uptolike') . '/scripts/scripts.js')),
    ),
  );
  $form['code'] = array(
    '#type' => 'hidden',
    '#default_value' => $preset->code,
  );
  $form['json'] = array(
    '#type' => 'hidden',
    '#default_value' => $preset->json,
    '#attached' => array(
      'js' => array(
        array(
          'data' => array(
            'Uptolike' => drupal_json_decode($preset->json),
          ),
          'type' => 'setting',
        ),
      ),
    ),
  );
}

/**
 * Submit export UI form.
 */
function uptolike_ctools_export_ui_form_submit(&$form, &$form_state) {
  if (!empty($form_state['values']['code'])) {
    // Remove <script> tag content.
    $code = preg_replace('/(<(script|style)\b[^>]*>).*?(<\/\2>)/is', "$1$3", $form_state['values']['code']);
    // Remove div tag. Not the best option.
    $code = str_replace(array('<div ', ' class="uptolike-buttons" ></div>'), array(), $code);
    // Add project ID for empty PID.
    $code = str_replace('data-pid=""', 'data-pid="' . uptolike_project_id() . '"', $code);
    // Remove <scripts> tags.
    $form_state['values']['code'] = trim(filter_xss_admin($code));
  }
}
