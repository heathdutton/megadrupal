<?php
/**
 * Interface definition for a Mailhandlersinglemailbox Address Generator
 * Ensure that those building a ctools plugin of this type have a generate method
 * or email generation won't work.
 */
interface MailhandlerSinglemailboxAddressGenerator {
  public function generate($base = '');
}

/**
 * Abstract email address generation class.
 * All generators need a 'generate' method.
 */
abstract class MailhandlerSinglemailboxAddressGeneratorBase implements MailhandlerSinglemailboxAddressGenerator{
  // Provide default characters that are used in generating random strings.
  public $vals = '1234567890abcdefghijklmnopqrstuvwxyz';
  public $pallet, $random_part, $domain;
  /**
   * Constructor
   */
  function __construct(){
    // @TODO length shouldn't be hard coded here - and probably should come from
    // some sort of configuration - or passed into the constructor.
    // Then again nobody is being forced to extend this class.  But the default
    // plugins that come with Mailhandler Singlemailbox do extend this.
    $this->pallet = str_split($this->vals);
    $length = 8;
    $this->random_part = '';
    while (strlen($this->random_part) < $length) {
      $this->random_part .= $this->pallet[mt_rand(0, strlen($this->vals) - 1)];
    }
    $this->domain = mailhandler_singlemailbox_get_default_mailbox_domain();
  }

  /**
   * Implement generate interface
   */
  public function generate($base = '') {}
}