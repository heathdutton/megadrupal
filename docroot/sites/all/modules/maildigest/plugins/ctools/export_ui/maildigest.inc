<?php
/**
 * @file
 * Allows Maildigest Digests to be exported.
 */

$plugin = array(
  'schema' => 'maildigest_digest',
  'access' => 'administer maildigest',
  'menu' => array(
    'menu prefix' => 'admin/build',
    'menu item' => 'maildigest',
    'menu title' => 'Maildigest Digests',
    'menu description' => 'Administer maildigest digests.',
    'items' => array(),
  ),
  'title singular' => t('maildigest digest'),
  'title singular proper' => t('Maildigest Digest'),
  'title plural' => t('maildigest digests'),
  'title plural proper' => t('Maildigest Digests'),
  'handler' => array(
    'class' => 'maildigest_digest_ui',
    'parent' => 'ctools_export_ui',
  ),
  'allowed operations' => array(
  ),
);
