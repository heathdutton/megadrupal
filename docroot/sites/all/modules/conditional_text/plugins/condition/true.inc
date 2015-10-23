<?php

/**
 * @file
 * "True" condition plugin.
 */

$plugin = array(
  'title' => t('Boolean true'),
  'description' => t(''),
  'evaluate' => repeat(TRUE),
  'reason' => repeat(t('Boolean true')),
  'settings' => repeat(array()),
  'settings include' => '',
  'identifier token' => 'true',
  'help' => repeat(t('Boolean true condition type: [condition true] ' .
  'always evaluates to true.')),
);
