<?php

/**
 * @file
 * "False" condition plugin.
 */

$plugin = array(
  'title' => t('Boolean false'),
  'description' => t(''),
  'evaluate' => repeat(FALSE),
  'reason' => repeat(t('Boolean false')),
  'settings' => repeat(array()),
  'settings include' => '',
  'identifier token' => 'false',
  'help' => repeat(t('Boolean false condition type: [condition false] ' .
  'always evaluates to false.')),
);
