<?php

/**
 * @file
 * The ctools plugin.
 *
 * This file contains the plugin definition as well as the callback functions
 * to render the pane and to provide the pane visualization in the admin
 * interface.
 */

$bundles = paragraphs_bundle_load();
$subtypes = array();
foreach ($bundles as $bundle) {
  $subtypes[$bundle->bundle] = array(
    'title' => $bundle->name,
    'description' => t('Adds a %bundle paragraph item.', array('%bundle' => $bundle->name)),
    'category' => t('Paragraph Panes'),
  );
}

$plugin = array(
  'title' => t('Paragraph panes'),
  'edit form' => 'paragraph_panes_edit_form',
  'render callback' => 'paragraph_panes_render',
  'content types' => $subtypes,
  'admin info' => 'paragraph_panes_admin_info',
);

/**
 * The 'admin info' callback for panel pane.
 */
function paragraph_panes_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass();
    $block->title = !empty($conf['override_title']) ?
      $conf['override_title_text']
      : t('Paragraph item <em>%type</em>.', array('%type' => $subtype));
    // TODO add description?
    $block->content = '';
    return $block;
  }
}

/**
 * Run-time rendering of the body of the block (content type).
 *
 * This function will create a 'run-time' entity, set the field values
 * using the pane configuration and render the entity view.
 */
function paragraph_panes_render($subtype, $conf, $panel_args, $context = NULL) {
  // Create the entity.
  $entity = entity_create('paragraphs_item', array(
    'field_name' => NULL,
    'bundle' => $subtype,
  ));

  // Add all fields from the pane configuration.
  foreach ($conf as $field_name => $values) {
    if (strpos($field_name, 'field_') === 0) {
      foreach ($values as &$value) {
        if (isset($value['fid']) && !isset($value['uri'])) {
          $value = (array) file_load($value['fid']);
        }
        if (isset($value['serialized_entity'])) {
          $value['entity'] = unserialize($value['serialized_entity']);
          unset($value['serialized_entity']);
        }
      }
      $entity->$field_name = array(LANGUAGE_NONE => $values);
    }
  }

  // Add the rendered entity to the block's content.
  $block = new stdClass();
  $block->title = '';
  $view = entity_view('paragraphs_item', array($entity));
  $block->content = drupal_render($view);

  return $block;
}
