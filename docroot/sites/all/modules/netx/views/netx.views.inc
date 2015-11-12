<?php

/**
 * @file
 * Views integration for the netx module.
 */

/**
 * Implements hook_views_data().
 */
function netx_views_data() {

  // Advertise this table as a possible base table.
  $data['file_metadata']['table']['base'] = array(
    'field' => 'fid',
    'title' => t('File metadata'),
    'help' => t("Files metadata maintained by Drupal and various modules."),
    'defaults' => array(
      'field' => 'name'
    ),
  );
  $data['file_metadata']['table']['entity type'] = 'file';
  $data['file_metadata']['table']['group']  = t('File metadata');

  // File metadata name.
  $data['file_metadata']['name'] = array(
    'title' => t('File metadata name.'),
    'help' => t('The name of metadata attribute.'),
    'field' => array(
      'handler' => 'views_handler_field_file',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // File metadata value.
  $data['file_metadata']['value'] = array(
    'title' => t('File metadata value.'),
    'help' => t('The value of metadata attribute.'),
    'field' => array(
      'handler' => 'views_handler_field_serialized',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Relationship: fid.
  $data['file_metadata']['fid'] = array(
    'title' => t('File metadata ID'),
    'help' => t('File ID metadata tied to.'),
    'real field' => 'fid',
    'relationship' => array(
      'title' => t('File metadata ID'),
      'label' => t('File metadata ID'),
      'base' => 'file_managed',
      'entity type' => 'file',
      'base field' => 'fid',
      'handler' => 'views_handler_relationship',
      'relationship field' => 'fid',
    ),
  );

  $data['file_metadata']['table']['join'] = array(
    // Link ourself to the {file_managed} table so we can provide file->entity relationships.
    'file_managed' => array(
      'field' => 'fid',
      'left_field' => 'fid',
    ),
  );

  return $data;
}
