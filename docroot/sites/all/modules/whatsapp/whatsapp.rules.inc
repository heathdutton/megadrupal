<?php

/**
 * @file
 * Rules integration for the WhatsApp module.
 */

/**
 * Implements hook_rules_action_info().
 */
function whatsapp_rules_action_info() {
  $actions = array();

  $actions['whatsapp_rules_send_message'] = array(
    'label' => t('Send a WhatsApp message'),
    'parameter' => array(
      'to' => array(
      'type' => 'text',
      'label' => t('To'),
      'description' => t('Destionation where the messages will be sent to.'),
      ),
      'message' => array(
        'type' => 'text',
        'label' => t('Message'),
        'description' => t("The WhatsApp's message body."),
        'translatable' => TRUE,
      ),
    ),
    'group' => t('WhatsApp'),
  );

  return $actions;
}

/**
 * Action Implementation: Send WhatsApp message.
 */
function whatsapp_rules_send_message($to, $message) {
  $wc = new WhatsappController();
  $wc->init();
  $success = FALSE;
  $wc->setDestination($to);
  $success = $wc->sendMessage(drupal_html_to_text($message));

  if ($success) {
    $wc->WaitforReceipt();
    watchdog('whatsapp', 'Successfully sent WhatsApp to %recipient', array('%recipient' => $to));
  }
}
