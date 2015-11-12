<?php

/**
 * @file
 * Views wizard for entity gallery views.
 */

$plugin = array(
  'name' => 'entity_gallery',
  'base_table' => 'entity_gallery',
  'created_column' => 'created',
  'available_sorts' => array(
    'title:DESC' => t('Title')
  ),
  'path' => drupal_get_path('module', 'entity_gallery') . '/plugins/views_wizard',
  'form_wizard_class' => array(
    'file' => 'views_ui_entity_gallery_views_wizard.class.php',
    'class' => 'ViewsUiEntityGalleryViewsWizard',
  ),
  'title' => t('Galleries'),
  'filters' => array(
    'status' => array(
      'value' => ENTITY_GALLERY_PUBLISHED,
      'table' => 'entity_gallery',
      'field' => 'status',
    ),
  ),
  'path_field' => array(
    'id' => 'egid',
    'table' => 'entity_gallery',
    'field' => 'egid',
    'exclude' => TRUE,
    'link_to_entity_gallery' => FALSE,
    'alter' => array(
      'alter_text' => 1,
      'text' => 'gallery/[egid]',
    ),
  ),
);
