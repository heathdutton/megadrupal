<?php
/**
 * @file
 * MailhandlerAuthenticateSendto class -- a mailhandler auth plugin.
 */

/**
 * MailhandlerAuthenticateSendto is a Drupal Mailhandler authentication plugin
 * that authenticates incoming mail based on the "To:" header.  If the "To:"
 * header is in a list of authorized recipient email addresses, the message
 * will be authenticated.
 */
class MailhandlerAuthenticateSendto extends MailhandlerAuthenticate {

  /**
   * Perform the authentication check based on "To:" address.
   * 
   * @param $message
   * @param $mailbox 
   */
  public function authenticate(&$message, $mailbox) {
    $authenticated_uid = 0;    // by default, set uid of message to 0
    $toaddress = $this->get_toaddress($message);
    if (isset($toaddress)) {
      $valid_addresses = $this->get_valid_address_list();
      if (isset($valid_addresses[$toaddress])) {
        $authenticated_uid = $valid_addresses[$toaddress];
      }
    }
    return $authenticated_uid;
  }
  
  /**
   * Return the "to address" from the headers. Note: will not return "toad dress".
   * 
   * @param $message
   *   the message retrieved by Mailhandler
   */
  private function get_toaddress($message) {
    $toaddress = '';
    if (isset($message['header'])) {
      $header = $message['header'];
      if (isset($header->toaddress)) {
        $toaddress = $header->toaddress;
      }
    }
    preg_match('/\<(.+?\@.+?)\>/', $toaddress, $matches);
    if (isset($matches[1])) {
      $toaddress = $matches[1];
    }
    return strtolower($toaddress);
  }
  
  /**
   * Return a list of valid addresses where key = email address and value = associated UID.
   * EG. $array['admin@example.com'] = 1
   * 
   * @return list of valid addresses
   */
  private function get_valid_address_list() {
    
    // Define a hook -- this plugin is intended to be used with mailhandler_singlebox
    // but it could be implemented using another email address to uid mapping function.
    $addresses = array();
    foreach (module_implements('mailhandler_sendto_addresses') as $module) {
      $addresses += module_invoke($module, 'mailhandler_sendto_addresses');
    }
    return $addresses;
  }
}
