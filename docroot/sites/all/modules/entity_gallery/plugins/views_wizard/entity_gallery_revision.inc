<?php

/**
 * @file
 * Views wizard for entity gallery revision views.
 */

$plugin = array(
  'name' => 'entity_gallery_revision',
  'base_table' => 'entity_gallery_revision',
  'created_column' => 'timestamp',
  'path' => drupal_get_path('module', 'entity_gallery') . '/plugins/views_wizard',
  'form_wizard_class' => array(
    'file' => 'views_ui_entity_gallery_revision_views_wizard.class.php',
    'class' => 'ViewsUiEntityGalleryRevisionViewsWizard',
  ),
  'title' => t('Galleries revisions'),
  'filters' => array(
    'status' => array(
      'value' => '1',
      'table' => 'entity_gallery', // @todo - unclear if this should be entity_gallery or entity_gallery_revision
      'field' => 'status',
    ),
  ),
  'path_field' => array(
    'id' => 'vid',
    'table' => 'entity_gallery_revision',
    'field' => 'vid',
    'exclude' => TRUE,
    'alter' => array(
      'alter_text' => 1,
      'text' => 'gallery/[egid]/revisions/[vid]/view',
    ),
  ),
  'path_fields_supplemental' => array(
    array(
      'id' => 'egid',
      'table' => 'entity_gallery',
      'field' => 'egid',
      'exclude' => TRUE,
      'link_to_entity_gallery' => FALSE,
    ),
  ),
);
