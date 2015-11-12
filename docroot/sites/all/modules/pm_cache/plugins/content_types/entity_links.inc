<?php

/**
 * @file
 * Handle rendering entity fields as panes.
 */

$plugin = array(
  'title' => t('Entity links'),
  'content type' => 'pm_cache_entity_links_content_type_content_type',
  'defaults' => array(
    'override_title' => FALSE,
    'override_title_text' => '',
    'view_mode' => '',
  ),
);

/**
 * Just one subtype.
 */
function pm_cache_entity_links_content_type_content_type($subtype) {
  $types = pm_cache_entity_links_content_type_content_types();
  if (isset($types[$subtype])) {
    return $types[$subtype];
  }
}

/**
 * Return all field content types available.
 */
function pm_cache_entity_links_content_type_content_types() {
  $types = &drupal_static(__FUNCTION__, array());
  if (!empty($types)) {
    return $types;
  }

  $entity_types = entity_get_info();
  foreach ($entity_types as $entity_type => $info) {
    // We check that this entity type can be build through Drupal 7's loosely
    // defined entity system.
    if (!isset($types[$entity_type]) && function_exists($entity_type . '_build_content')) {
      $types[$entity_type] = array(
        'category' => t($info['label']),
        'title' => t('@type links, AJAX', array('@type' => t($info['label']))),
        'description' => t('@type links, rendered asynchronously to enable better caching.', array('@type' => t($info['label']))),
        'required context' => new ctools_context_required(t($info['label']), 'entity:' . $entity_type),
      );
    }
  }

  return $types;
}

/**
* Render the custom content type.
*/
function pm_cache_entity_links_content_type_render($subtype, $conf, $panel_args, $context) {
  if (!empty($context) && empty($context->data)) {
    return;
  }
  $block = new stdClass();
  $block->module = $subtype;
  $entity = isset($context->data) ? clone($context->data) : NULL;

  if (empty($entity)) {
    $block->delta = 'placeholder';
    $block->content = 'placeholder';
  }
  else {
    list($id) = entity_extract_ids($subtype, $entity);
    $block->delta  = $id;
    $element = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'entity-links',
          'entity-links-' . drupal_html_class($subtype),
        ),
        'data-entity-type' => $subtype,
        'data-entity-id' => $id,
        'data-view-mode' => $conf['view_mode'],
      ),
      // We render the links for anonymous users so that only links for
      // authenticated users have to be fetched over AJAX. This assumed that
      // the links for anonymous users doesn't depend on session data.
      'links' => pm_cache_get_entity_links($subtype, $entity, $conf['view_mode'], drupal_anonymous_user()),
    );
    $block->content = $element;
  }
  return $block;
}

/**
 * Returns an edit form for the custom type.
 */
function pm_cache_entity_links_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $entity_type = $form_state['subtype_name'];

  $entity = entity_get_info($entity_type);
  $build_mode_options = array();
  foreach ($entity['view modes'] as $mode => $option) {
    $build_mode_options[$mode] = $option['label'];
  }

  $form['view_mode'] = array(
    '#title' => t('View mode'),
    '#type' => 'select',
    '#description' => t('Select the view mode to render the entity links for.'),
    '#options' => $build_mode_options,
    '#default_value' => $conf['view_mode'],
  );

  return $form;
}

function pm_cache_entity_links_content_type_edit_form_submit($form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
* Returns the administrative title for a type.
*/
function pm_cache_entity_links_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" links, AJAX', array('@s' => $context->identifier));
}
