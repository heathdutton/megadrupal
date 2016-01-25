<?php
/**
 * @file
 * openedu_linkblock.bean.inc
 */

/**
 * Implements hook_bean_admin_ui_types().
 */
function openedu_linkblock_bean_admin_ui_types() {
  $export = array();

  $bean_type = new stdClass();
  $bean_type->disabled = FALSE; /* Edit this to true to make a default bean_type disabled initially */
  $bean_type->api_version = 5;
  $bean_type->name = 'link_block';
  $bean_type->label = 'Link block';
  $bean_type->options = '';
  $bean_type->description = '';
  $export['link_block'] = $bean_type;

  return $export;
}
