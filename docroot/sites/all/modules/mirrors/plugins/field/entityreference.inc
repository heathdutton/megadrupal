<?php

/**
 * @file
 * Mirrors field plugin declaration.
 */

/**
 * Field Declaration: entity_reference.
 */
$plugin = array(
  'title' => t('Entity Reference'),
  'category' => 'Field',
  'views' => array(
    'settings' => array('link' => 0),
  ),
  'feeds' => array(
    'target' => 'SELF:label',
  ),
);
