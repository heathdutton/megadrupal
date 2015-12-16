<?php

/**
 * @file
 * Et_action plugin file.
 */

$plugin = array(
  'title' => t('Translation Basic'),
  'description' => t('Provide basic class for all translation action plugins.'),
  'handler' => array(
    'class' => 'EntityTranslationActionsBasic',
  ),
  'single' => TRUE,
  'icon' => 'no-icon.png',
  'weight' => 0,
);
