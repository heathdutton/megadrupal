<?php

/**
 * @file
 * API documentation for BLock export widget.
 */

/**
 * Alter list of available export formats
 *
 * @param array $formats
 * @param string $module Module of the block
 * @param string $delta Delta of the block
 */
function hook_block_export_widget_formats(array &$formats, $module, $delta) {
  switch ($module) {
    case 'block':
      $formats[t('Simple text block')] = array(
        'block_content' => t('Simple block content only'),
      );
      break;
  }
}

/**
 * Provide a renderable output of the block.
 *
 * @param array $block_export Configuration of the block export
 * @param array $args Additional arguments for the format
 * @return array Renderable array
 */
function hook_block_export_widget_format_FORMAT_NAME(array $block_export, array $args) {
  return array(
    '#markup' => 'block_content',
  );
}
