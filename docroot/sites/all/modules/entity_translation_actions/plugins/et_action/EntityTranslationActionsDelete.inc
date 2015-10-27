<?php

/**
 * @file
 * Et_action plugin file.
 */

$plugin = array(
  'title' => t('Translation Delete'),
  'description' => t('Delete existing translations of entity.'),
  'handler' => array(
    'class' => 'EntityTranslationActionsDelete',
    'parent' => 'EntityTranslationActionsBasic',
  ),
  'single' => TRUE,
  'icon' => 'no-icon.png',
  'weight' => 3,
);
