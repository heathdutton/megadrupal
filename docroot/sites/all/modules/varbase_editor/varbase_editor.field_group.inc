<?php
/**
 * @file
 * varbase_editor.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function varbase_editor_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_caption_wrapper|file|image|wysiwyg';
  $field_group->group_name = 'group_caption_wrapper';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'wysiwyg';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Caption wrapper',
    'weight' => '1',
    'children' => array(
      0 => 'field_text',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'label' => 'Caption wrapper',
      'instance_settings' => array(
        'classes' => 'caption-wrapper',
        'element' => 'span',
        'show_label' => '0',
        'label_element' => '',
        'attributes' => '',
      ),
    ),
  );
  $export['group_caption_wrapper|file|image|wysiwyg'] = $field_group;

  return $export;
}
