<?php
/**
 * @file
 * Functions that wrap Drupal up for WhatsApp.
 */

/**
 * Start the WhatsApp Connection.
 */
function whatsapp_smartconnection_start() {
  variable_set('whatsapp_status', 'CONNECTING');

  // Connect and begin listening.
  $wc = new WhatsappController();
  $wc->setDebug(TRUE)->init();
  if ($wc->prepare()) {
    echo "[+] Connected!\n";
    variable_set('whatsapp_status', 'CONNECTED');

    $received = FALSE;
    do {
      $wc->PollMessages();
      $msgs = $wc->GetMessages();
      foreach ($msgs as $m) {
        if ($m->_tag == 'message') {
          // @todo: Process inbound messages.
        }
      }
    } while(!$received);
  }

  echo "[+] Disconnected!\n";
  variable_set('whatsapp_status', 'DISCONNECTED');
}
