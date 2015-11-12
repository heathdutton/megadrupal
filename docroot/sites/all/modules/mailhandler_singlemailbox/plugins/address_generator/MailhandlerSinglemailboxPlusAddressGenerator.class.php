<?php

class MailhandlerSinglemailboxPlusAddressGenerator extends MailhandlerSinglemailboxAddressGeneratorBase {
  /**
   * Implement generate interface
   * @param $base string
   * @return email address in the form mailboxaddress+base-RANDOMTEXT@domain
   * Plus address generator best used with a mailboxes that support plus addressing.
   */
  public function generate($base = '') {
    //remove invalid characters from the base.
    $base = str_replace(' ', '-', $base);
    $default_mailbox_name = variable_get('mailhandler_singlemailbox_default_mailbox_name', NULL);
    $mailbox =  mailhandler_mailbox_load($default_mailbox_name);
    $position = strpos($mailbox->settings['mailbox_address'], '@' . $this->domain);
    $default_baseaddress = substr($mailbox->settings['mailbox_address'], 0, $position);
    $new_address = strtolower($default_baseaddress . '+' . $base . '-' . $this->random_part . '@' . $this->domain);
    return $new_address;
  }
}