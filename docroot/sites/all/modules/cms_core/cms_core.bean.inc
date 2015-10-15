<?php
/**
 * @file
 * cms_core.bean.inc
 */

/**
 * Implements hook_bean_admin_ui_types().
 */
function cms_core_bean_admin_ui_types() {
  $export = array();

  $bean_type = new stdClass();
  $bean_type->disabled = FALSE; /* Edit this to true to make a default bean_type disabled initially */
  $bean_type->api_version = 5;
  $bean_type->name = 'content_block';
  $bean_type->label = 'Content Block';
  $bean_type->options = '';
  $bean_type->description = 'Block entity that may contain textual content and media content such as images, video etc.';
  $export['content_block'] = $bean_type;

  return $export;
}
