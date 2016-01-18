<?php 

/**
 * @file
 * Administrative functions to configure Mass messaging.
 */

/**
 * Callback for admin/config/mass_messaging/configuration.
 */
function mass_messaging_settings_form($form, &$form_state) {
  
  $form = array();
    
  $form['mass_messaging_cron'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mass messaging Cron Settings'),
    '#collapsible' => TRUE,
    '#description' => t('The mass messaging queue is processed by the cron. This is required because it can take a long time to send big chunks of mails. Each queue item has a view associated. The cron takes the queue item with the least activity in the past and take a batch (subset of the items in the view) and process it. If the queue item is fully processed it is removed from the queue. Then we start again by selecting the queue item with the least activity in the past. With this mechanisme we ensure that all queues get processed in parallel.'),
  );
  $form['mass_messaging_cron']['mass_messaging_cron_percentage'] = array(
    '#type' => 'textfield',
    '#title' => t('Cron Percentage'),
    '#default_value' => variable_get('mass_messaging_cron_percentage', '80'),
    '#description' => t('The cron percentage defines the percentage of the available time to be used for the mass messaging cron to process messages. Default 80, so other application get also a chance to process anything.')
  );

  $form['mass_messaging_cron']['mass_messaging_queue_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Queue Timeout'),
    '#default_value' => variable_get('mass_messaging_queue_timeout', '180'),
    '#description' => t('This timeout indicates how long a queue cannot have any activity until it is seen as broken. After each batch the queue is updated.')
  );
  
  $form['mass_messaging_cron']['mass_messaging_batch_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Batch Size'),
    '#default_value' => variable_get('mass_messaging_batch_size', '100'),
    '#description' => t('With the batch size you can control the number of messages processed in one batch.')
  );
  
  $form['mass_messaging_integration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mass messaging Mail Integration'),
    '#collapsible' => TRUE,
  );
  
  $form['mass_messaging_integration']['mass_messaging_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug Mode'),
  //  '#options' => array('mimemail' => t('Mime Mail'), 'debug' => t('Debug (Watchdog)'), ),
    '#default_value' => variable_get('mass_messaging_debug', '0'),
    '#description' => t('If you activate the debug mode the message are not send. They get only logged in the watchdog.')
  );
  
  
  
  return system_settings_form($form);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function mass_messaging_settings_form_validate($form, &$form_state) {
  // TODO: Check if some validation must be done.
}


function mass_messaging_queue_form($form, &$form_state) {
  // TODO: Implement the Queue Form
  return '';
}
