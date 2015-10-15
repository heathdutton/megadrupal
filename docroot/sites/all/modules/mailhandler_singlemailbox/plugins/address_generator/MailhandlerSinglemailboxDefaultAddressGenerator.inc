<?php

$plugin = array(
  'name' => 'Mailhandler Single Mailbox Default Address Generator',
  'module' => 'mailhandler_singlemailbox',
  'label' => t('Default addressing'),
  'file' => 'MailhandlerSinglemailboxDefaultAddressGenerator.class.php',
  'handler' => array(
    'class' => 'MailhandlerSinglemailboxDefaultAddressGenerator',
  ),
);