<?php
/**
 * @file mailhandler_singlemailbox.admin.inc
 *    Administration pages for mailhandler_singlemailbox.
 */

/**
 * Menu callback for administration form
 */
function mailhandler_singlemailbox_admin_form() {
  // Load all available mailhandler mailboxes.
  $mailboxes = mailhandler_mailbox_load_all(FALSE);
  // Create options for a select box.
  $options = array();
  foreach ($mailboxes as $mailbox) {
    $options[$mailbox->mail] = $mailbox->settings['name'];
  }
  if (empty($options)) {
    drupal_set_message(t('There are no mailhandler mailboxes configured. You must add a !adminlink before a default mailbox can be selected.', array('!adminlink' => l(t('mailbox'),'admin/structure/mailhandler'))), 'warning');
  }
  $form['mailhandler_singlemailbox']['default_mailbox_name'] = array(
    '#type' => 'select',
    '#title' => t('Select the default mailbox'),
    '#description' => t('The mailbox that stores and processes content sent to this site.'),
    '#options' => $options,
    '#required' => TRUE,
    '#default_value' => variable_get('mailhandler_singlemailbox_default_mailbox_name', NULL),
  );

  if ($scheme_options = mailhandler_singlemailbox_get_address_plugin_options()) {
    $form['mailhandler_singlemailbox']['addressing_scheme'] = array(
      '#type' => 'select',
      '#title' => t('Select an e-mail address format'),
      '#description' => t('Use plus addressing if you want email to go into distributed folders as opposed to one default folder (inbox). !learnmore.', array('!learnmore' => l('Learn more', 'admin/help/mailhandler_singlemailbox'))),
      '#options' => $scheme_options,
      '#default_value' => variable_get('mailhandler_singlemailbox_addressing_scheme', NULL),
      '#required' => TRUE,
    );
  }
  else {
    drupal_set_message(t('There are no address generation plugins installed.  There should at least be a default provided by the mailhandler_singlemailbox module.  Contact your site administrator to help sort this out.'), 'warning');
  }

  $form['mailhandler_singlemailbox']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}


function mailhandler_singlemailbox_get_address_plugin_options() {

  ctools_include('plugins');
  $addressing_schemes = ctools_get_plugins('mailhandler_singlemailbox', 'mailhandler_singlemailbox_address_generator');
  if (is_array($addressing_schemes)) {
    foreach ($addressing_schemes as $id => $addressing_scheme){
      $scheme_options[$id] = $addressing_scheme['label'];
    }
    return $scheme_options;
  }
  else {
    return FALSE;
  }
}

/**
 * Store the name of the default mailhandler mailbox.
 */
function mailhandler_singlemailbox_admin_form_submit(&$form, &$form_state) {
  if ($form_state['values']['default_mailbox_name']) {
    variable_set('mailhandler_singlemailbox_default_mailbox_name', $form_state['values']['default_mailbox_name']);
  }
  if ($form_state['values']['addressing_scheme']) {
    variable_set('mailhandler_singlemailbox_addressing_scheme', $form_state['values']['addressing_scheme']);
  }
  mailhandler_singlemailbox_set_feeds_source($form_state['values']['default_mailbox_name']);
  drupal_set_message(t('Your settings have been saved.'), 'status');
}

/**
 * Set the feed source. This function defines a hook for setting the importer
 * name.  The hook allows us to use an alternate feeds importer (other than
 * "mailhandler_media_nodes" defined in mail_media module).
 *
 * @param $mailbox
 *    the mailbox to set in the feeds source
 */
function mailhandler_singlemailbox_set_feeds_source($mailbox) {
  foreach (module_implements('mailhandler_singlemailbox_feed_info') as $module) {
    $source_name = module_invoke($module, 'mailhandler_singlemailbox_feed_info');
    $source_config = array(
      'MailhandlerFetcher' => array(
          'mailbox' => $mailbox,
      ),
      'MailhandlerParser' => array(
          'if_auth_fails' => 'remove',
      ),
    );
    $source = feeds_source($source_name);
    $source->addConfig($source_config);
    $source->schedule();
    $source->save();
  }
}