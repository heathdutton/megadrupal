<?php

$plugin = array(
  'name' => 'Mailhandler Single Mailbox "Plus" Address Generator',
  'module' => 'mailhandler_singlemailbox',
  'label' => t('Plus addressing'),
  'file' => 'MailhandlerSinglemailboxPlusAddressGenerator.class.php',
  'handler' => array(
    'class' => 'MailhandlerSinglemailboxPlusAddressGenerator',
  ),
);