<?php

/**
 * @file
 * Et_action plugin file.
 */

$plugin = array(
  'title' => t('Translation Set'),
  'description' => t('Set source language for entity.'),
  'handler' => array(
    'class' => 'EntityTranslationActionsSet',
    'parent' => 'EntityTranslationActionsBasic',
  ),
  'single' => TRUE,
  'icon' => 'no-icon.png',
  'weight' => 2,
);
