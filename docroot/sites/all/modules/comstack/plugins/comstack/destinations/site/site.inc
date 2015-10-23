<?php

$plugin = array(
  'title' => t('Site'),
  'description' => t('Messages will be recorded against the site, for use cases like a sitewide notification queue.'),
  'class' => 'ComstackDestination',
  'options' => array(
  ),
  'weight' => 3,
  'requires_storage' => TRUE,
);
