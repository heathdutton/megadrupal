<?php

class MailhandlerSinglemailboxDefaultAddressGenerator extends MailhandlerSinglemailboxAddressGeneratorBase {
  /**
   * Implement generate inteface
   * @param $base string
   * @return email address in the form $base-RANDOMTEXT@domain
   * Basic address generator best used with a catch all mailbox.
   */
  public function generate($base = '') {
    //remove invalid characters from the base.
    $base = str_replace(' ', '-', $base);
    $new_address = strtolower($base . '-' . $this->random_part . '@' . $this->domain);
    return $new_address;
  }
}