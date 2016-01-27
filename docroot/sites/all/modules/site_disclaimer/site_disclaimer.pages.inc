<?php

/**
 * @file
 * Page callbacks for the Site Disclaimer module.
 */

/**
 * Display the Site Disclaimer, when a user needs to confirm them after registering.
 */
function site_disclaimer_confirm($form, $form_state, $langcode = NULL, $preview = FALSE) {
  $title = check_plain(variable_get('site_disclaimer_title', SITE_DISCLAIMER_DEFAULT_TITLE));
  if (!$preview) {
    drupal_set_title($title);
  }

  // Will we show the full text of the Terms or only provide a link to the Terms.
  // Reading the checkbox label to find out.
  $nid = _site_disclaimer_nid_translated(variable_get('site_disclaimer_node_id', ''), $langcode);
  $checkbox_label = filter_xss_admin(_site_disclaimer_variable_get_translated('site_disclaimer_checkbox_label', SITE_DISCLAIMER_DEFAULT_CHECKBOX_LABEL, $langcode));
  $use_link = (strpos($checkbox_label, '@link') !== FALSE);
  $use_named_links = (strpos($checkbox_label, '@&quot;') !== FALSE);

  // If there is no node to show and no links in the $checkbox_label, we don't show the terms form.
  if (!$use_link && !$use_named_links && !$nid) {
    return array('#markup' => t('There were no changes.'));
  }

//@todo: Check user's last version and if he/she already accepted the latest, redirect to ...

  $changes = filter_xss_admin(_site_disclaimer_variable_get_translated('site_disclaimer_version_details', '', $langcode));
  if (empty($changes)) {
    $changes = '.';
  }
  else {
    $changes = ':<br />' . $changes . '<br /><br />';
  }
  $form = array();
  $form['intro'] = array(
    '#type' => 'item',
    '#markup' => t('There were recent changes to the %terms!changes Please review the new %terms below. You must accept the following to continue.',
      array(
        '%terms' => $title,
        '!changes' => $changes,
      )),
  );
  site_disclaimer_form_user_register_form_alter($form, $form_state, 'site_disclaimer_confirm', $langcode, $preview);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue'),
    '#weight' => 50,
  );

  return $form;
}

/**
 * Submit handler for site_disclaimer_confirm()
 */
function site_disclaimer_confirm_submit($form, &$form_state) {
  global $user;
  $edit = array(
    'data' => array('site_disclaimer' => $form_state['values']['I_agree']),
  );
  user_save($user, $edit);
}

