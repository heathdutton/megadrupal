<?php

/**
 * @file
 * HTML Import admin page.
 */

/**
 * Form API callback for administration settings form.
 */
function html_import_admin_form($form, &$form_state) {

  $form['html_import_import'] = array(
    '#type' => 'fieldset',
    '#title' => t('HTML Import - consolidate imported pages'),
    '#description' => t('Consolidate imported book pages and save them to a full-text field for index'),
  );
  $form['html_import_import']['html_import_advpubs_process'] = array(
    '#type' => 'submit',
    '#value' => t('Consolidate'),
    '#suffix' => '<span class="description">' . t('This could take time and might put an increased load on your server.') . '</span>',
  );
  $form['#submit'][] = '_html_import_import_submit';
  return $form;
}
