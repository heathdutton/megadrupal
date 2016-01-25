<?php
/**
 * @file
 * MailhandlerCommandsMailMedia class.
 */

$plugin = array(
  'name' => 'Mail Media link processor',
  'description' => 'Provides email body links to shared media sites as mapping sources.',
  'handler' => array(
    'class' => 'MailhandlerCommandsMailMediaLink',
    'parent' => 'MailhandlerCommands',
  ),
  'file' => 'MailhandlerCommandsMailMediaLink.class.php',
  'weight' => 0,
);