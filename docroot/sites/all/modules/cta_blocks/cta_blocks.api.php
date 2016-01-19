<?php

/**
 * @file
 * API Documentation.
 */

/**
 * Definition of hook_default_cta_blocks().
 *
 * Demonstrate how to programatically provide default cta blocks.
 */
function YOUR_MODULE_default_cta_blocks() {
  $export = array();

  $block               = new stdClass;
  $block->api_version  = 1;
  // Unique identifier.
  $block->machine_name = 'test_block';
  // Human-readable name.
  $block->name         = 'Test Block';
  // Title value, displayed in rendered block. Required.
  $block->title        = 'Test Block Title';
  // Subtitle value, displayed in rendered block. Optional.
  $block->subtitle     = 'Subtitle';
  // Full URL or internal path used to link rendered block. Required.
  $block->link         = '<front>';

  $export[$block->machine_name] = $block;

  return $export;
}
