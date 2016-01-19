<?php

/**
 * @file
 * Hooks provided by Content callback module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Implements hook_content_callback_info().
 */
function hook_content_callback_info() {
  $callbacks = array();
  $callbacks['content_callback_example'] = array(
    'title' => t('Example'),
    'entity_types' => array('node'),
  );

  return $callbacks;
}

/**
 * Content callback function example.
 */
function content_callback_example_callback() {
  $elements = array();
  $elements['content'] = array('#markup' => t('This is a content callback example.'));

  return $elements;
}
