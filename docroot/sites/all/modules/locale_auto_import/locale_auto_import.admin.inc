<?php
/**
 * @file
 * Administration form file for locale_auto_import
 */

/**
 * Include ".inc" file to get the constants.
 */
require_once 'locale_auto_import.inc';

/**
 * Define form (admin/config/regional/translate/auto_import).
 */
function locale_auto_import_form($form, &$form_state) {
  $form['import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import translation'),
  );
  // ----------------------------------------------
  // Text groups
  // ----------------------------------------------
  $form['import']['group'] = array(
    '#type' => 'item',
    '#title' => t('Text groups'),
    '#markup' => '<dl>',
    '#description' => t('Use the system name of the text group in the name of your .po file before the language code to make it automatically import in the right text group.<br />Eg.: my_feature.views.fr.po'),
  );
  // Get the list of text groups.
  $text_groups = module_invoke_all('locale', 'groups');
  // Format it for references in the form.
  foreach ($text_groups as $system_name => $display_name) {
    $form['import']['group']['#markup'] .= "<dt>$system_name</dt><dd>$display_name</dd>";
  }
  $form['import']['group']['#markup'] .= '</dl>';

  // ----------------------------------------------
  // Modules
  // ----------------------------------------------
  // Build the table header.
  $header = array(
    'module_name' => array('data' => t('Module')),
    'machine_readable_name' => array('data' => t('Machine readable name')),
  );
  // Build the rows.
  $options = array();
  // Search for all modules containing a folder "auto_translations"
  $modules_list = module_list();
  foreach ($modules_list as $module_name) {
    // Made the path of the "auto_translations" folder.
    $module_path_po = drupal_get_path('module', $module_name) . '/' . LOCALE_AUTO_IMPORT_PO_FOLDER;
    // Check if the "auto_translations" folder is present in the module folder.
    if (file_exists($module_path_po) === TRUE && is_dir($module_path_po) === TRUE) {
      // Get info for the module.
      $module = system_get_info('module', $module_name);
      // Get the display name and system name.
      $options[$module_name] = array(
        'module_name' => check_plain($module['name']),
        'machine_readable_name' => check_plain($module_name),
      );
    }
  }
  // Build the tableselect.
  $form['import']['modules_list'] = array(
    '#prefix' => '<label>' . t('Modules') . '</label>',
    '#suffix' => '<div class="description">' . t('List all the module which an "@LOCALE_AUTO_IMPORT_PO_FOLDER" folder has been detected. Check the ones you want to import the .po file(s) from.', array('@LOCALE_AUTO_IMPORT_PO_FOLDER' => LOCALE_AUTO_IMPORT_PO_FOLDER)) . '</div>',
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No "@LOCALE_AUTO_IMPORT_PO_FOLDER" folder has been detected within your modules.', array('@LOCALE_AUTO_IMPORT_PO_FOLDER' => LOCALE_AUTO_IMPORT_PO_FOLDER)),
  );

  // ----------------------------------------------
  // Mode
  // ----------------------------------------------
  $form['import']['mode'] = array(
    '#type' => 'radios',
    '#title' => t('Mode'),
    '#default_value' => LOCALE_AUTO_IMPORT_DEFAULT_MODE,
    '#options' => array(
      LOCALE_IMPORT_OVERWRITE => t('Strings in the uploaded file replace existing ones, new ones are added. The plural format is updated.'),
      LOCALE_IMPORT_KEEP => t('Existing strings and the plural format are kept, only new strings are added.'),
    ),
  );

  // ----------------------------------------------
  // Submit
  // ----------------------------------------------
  $form['import']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return($form);
}

/**
 * Validation of the form.
 */
function locale_auto_import_form_validate($form, &$form_state) {
  $error = TRUE;
  foreach ($form_state['values']['modules_list'] as $value) {
    if ($value !== 0) {
      $error = FALSE;
    }
  }
  if ($error) {
    form_set_error('modules_list', t('At least one module must be selected.'));
  }
}

/**
 * Process the locale_auto_import import form submission.
 */
function locale_auto_import_form_submit($form, &$form_state) {
  // Form: Made the list of all selected modules.
  $components = array();
  foreach ($form_state['values']['modules_list'] as $module_name => $value) {
    if ($value !== 0) {
      $components[$module_name] = $module_name;
    }
  }
  // Form: Mode value.
  $mode = $form_state['values']['mode'];
  // Imports translations of a list of modules.
  locale_auto_import_system_update($components, $mode);

  return;
}
