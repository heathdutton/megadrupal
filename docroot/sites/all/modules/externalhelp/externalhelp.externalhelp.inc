<?php

/**
 * @file
 * External help for the External help module. Recursion warning!
 */

/**
 * Implements hook_externalhelp().
 */
function externalhelp_externalhelp() {
  return array(
    'project_page' => array(
      'label' => t('The External Help project page'),
      'url' => 'http://drupal.org/project/externalhelp',
    ),
    'help_import' => array(
      'label' => t('About having external help as inline help'),
      'url' => 'http://drupal.org/node/1031972',
    ),
    'online_docs' => array(
      'label' => t('Shape of future Drupal online documentation'),
      'url' => 'http://drupal.org/node/1291058',
    ),
    'externalise_strings' => array(
      'label' => t('Discussion about moving interface strings out of code'),
      'url' => 'http://groups.drupal.org/node/154394',
    ),
  );
}
