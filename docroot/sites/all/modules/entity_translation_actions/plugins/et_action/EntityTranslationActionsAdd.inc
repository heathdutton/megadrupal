<?php

/**
 * @file
 * Et_action plugin file.
 */

$plugin = array(
  'title' => t('Translation Add'),
  'description' => t('Copy existing translation for a new language.'),
  'handler' => array(
    'class' => 'EntityTranslationActionsAdd',
    'parent' => 'EntityTranslationActionsBasic',
  ),
  'single' => TRUE,
  'icon' => 'no-icon.png',
  'weight' => 1,
);
