<?php

/**
 * @file
 * Form used to trigger the import process.
 */

/**
 * The form used to kickoff the import batch.
 */
function _potx_exportables_import_form($form, &$form_state) {
  $form['introduction'] = array(
    '#type' => 'html_tag',
    '#tag' => 'p',
    '#value' => t('Click on the button below to import all po files defined in modules. Remember: PO files that appear to have changed will be processed again.'),
    '#weight' => -100,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  $form['#submit'][] = '_potx_exportables_import_data';
  return $form;
}
