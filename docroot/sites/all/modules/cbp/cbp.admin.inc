<?php
function cbp_configuration()
{
   if (!module_exists('honeypot'))
   {
      drupal_set_message("You should STRONGLY consider installing the <a href='http://drupal.org/project/honeypot'>Honeypot module</a> as well to gain the benefits of the honeypot + cbp integration.  Doing this will turn your honeypot blocks into cbp blocks!  This is a tremendous tool for finding all those bad guys even quicker :).  But dont worry, it wont block based on honeypot_timestamp type of blocks.  It only blocks with types of 'honeypot' which means that it detected something filled in the hidden 'honeypot' field on a form.  Only a bot would do something that foolish.  What a great way to stop spam!");
   }
   $contents[] = drupal_get_form('cbp_configuration_settings');
   $contents[] = drupal_get_form('cbp_configuration_test_connection');

   return $contents;
}

function cbp_configuration_test_connection()
{
   $form['test_connection'] = array
   (
      '#type' => 'submit',
      '#title' => t('Test Connection'),
      '#description' => t('Test connection to the Crowdsource server'),
      '#help' => 'ewrtwet',
      '#value' => t('Test Connection to Crowdsource Server'),
      '#submit' => array('cpb_test_server_connection_submit'),
      '#method' => 'POST',
   );

   return $form;
}

function cbp_configuration_settings()
{
   $cbp_settings = variable_get('cbp_settings', array('remote' => array(CBP_REMOTE_SEND_ENABLED => CBP_REMOTE_SEND_ENABLED, CBP_REMOTE_RECEIVE_ENABLED => CBP_REMOTE_RECEIVE_ENABLED)));
   $form['configuration_settings'] = array
   (
      'remote' => array
      (
         '#title' => t('Send and receive data from/to Crowdsource Server'),
         '#type' => 'checkboxes',
         '#options' => array
         (
            CBP_REMOTE_SEND_ENABLED => t('Send to Crowdsource Server - Enable this if you would like send data to the crowd source network'),
            CBP_REMOTE_RECEIVE_ENABLED => t('Receive from Crowdsource Server - Enable this if you would like get data from the crowd source network'),
         ),
         '#default_value' => $cbp_settings['remote'],
      ),
      'submit' => array
      (
         '#type' => 'submit',
         '#value' => t('Save'),
         '#submit' => array('cbp_configuration_settings_submit'),
      )
   );
   return $form;
}

function cbp_configuration_settings_submit($form, &$form_state)
{
   $cbp_settings['remote'] = $form_state['values']['remote'];
   variable_set('cbp_settings', $cbp_settings);
   drupal_set_message("CBP settings saved.");
   watchdog('crowd brutefore protection', 'CBP settings saved.');
}

function cpb_test_server_connection_submit($form, &$form_state)
{
      cbp_check_connection($debug = TRUE);
}

function cbp_whitelist()
{
   $default_ip = arg(5);
   if (!filter_var($default_ip, FILTER_VALIDATE_IP))
   {
      $default_ip = '';
   }
   $rows = array();
   $header = array(t('Whitelisted IP addresses'), t('Operations'));
   $result = db_query('SELECT * FROM {cbp_whitelist}');
   foreach ($result as $ip)
   {
      $rows[] = array
      (
         $ip->ip,
         l(t('delete'), "admin/config/people/ip-whitelist/delete/{$ip->ip}"),
      );
   }

   $build['cbp_whitelist_form'] = drupal_get_form('cbp_whitelist_form', $default_ip);
   $build['cbp_whitelist_table'] = array
   (
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No whitelisted IP addresses available.'),
   );

  return $build;
}

/**
 * Define the form for whitelisting IP addresses.
 *
 * @ingroup forms
 * @see system_ip_whitelist_form_validate()
 * @see system_ip_whitelist_form_submit()
 */
function cbp_whitelist_form($form, $form_state, $default_ip)
{
   $form['ip'] = array
   (
      '#title' => t('IP address'),
      '#type' => 'textfield',
      '#size' => 48,
      '#maxlength' => 40,
      '#default_value' => $default_ip,
      '#description' => t('Enter a valid IP address.'),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array
  (
    '#type' => 'submit',
    '#value' => t('Add'),
  );
  $form['#submit'][] = 'cbp_whitelist_form_submit';
  $form['#validate'][] = 'cbp_whitelist_form_validate';
  return $form;
}

function cbp_whitelist_form_validate($form, &$form_state)
{
   $ip = trim($form_state['values']['ip']);
   if (db_query("SELECT * FROM {cbp_whitelist} WHERE ip = :ip", array(':ip' => $ip))->fetchField())
   {
      form_set_error('ip', t('This IP address is already whitelisted.'));
   }
   elseif (filter_var($ip, FILTER_VALIDATE_IP, FILTER_FLAG_NO_RES_RANGE) == FALSE)
   {
      form_set_error('ip', t('Enter a valid IP address.'));
   }
}

function cbp_whitelist_form_submit($form, &$form_state)
{
   $ip = trim($form_state['values']['ip']);
   db_insert('cbp_whitelist')->fields(array('ip' => $ip))->execute();
   cbp_purge_bans($ip); //This purges both flood AND blocked_ips tables
   drupal_set_message(t('The IP address %ip has been whitelisted.', array('%ip' => $ip)));
   watchdog('crowd brutefore protection', t('The IP address %ip has been whitelisted.', array('%ip' => $ip)));
   $form_state['redirect'] = 'admin/config/people/ip-whitelist';
  return;
}

/**
 * IP deletion confirm page.
 *
 * @see cbp_whitelist_delete_submit()
 */
function cbp_whitelist_delete($form, &$form_state, $ip)
{
   $form['whitelisted_ip'] = array
   (
      '#type' => 'value',
      '#value' => $ip,
  );
  return confirm_form($form, t('Are you sure you want to delete %ip?', array('%ip' => $ip)), 'admin/config/people/ip-whitelist', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Process cbp_whitelist_delete form submissions.
 */
function cbp_whitelist_delete_submit($form, &$form_state)
{
   $whitelisted_ip = $form_state['values']['whitelisted_ip'];
   db_delete('cbp_whitelist')->condition('ip', $whitelisted_ip)->execute();
   watchdog('crowd brutefore protection', 'Deleted Whitelisted IP %ip', array('%ip' => $whitelisted_ip));
   drupal_set_message(t('The IP address %ip was deleted.', array('%ip' => $whitelisted_ip)));
   $form_state['redirect'] = 'admin/config/people/ip-whitelist';
}