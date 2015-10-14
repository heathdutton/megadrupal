<?php
/**
 * @file
 * MailhandlerAuthenticateSendto class.
 */

$plugin = array(
  'name' => '"To" address authentication',
  'description' => 'Authenticate messages where the "To" address is within a preauthorized list.',
  'handler' => array(
    'class' => 'MailhandlerAuthenticateSendto',
    'parent' => 'MailhandlerAuthenticate',
  ),
  'file' => 'MailhandlerAuthenticateSendto.class.php',
);
