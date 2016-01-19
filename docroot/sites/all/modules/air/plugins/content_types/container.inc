<?php

/**
 * @file
 */

// Plugin definition
$plugin = array(
  'title' => t('AIR Container'),
);

/**
 * Returns all available entity collections as a content type.
 */
function air_container_content_type_content_types() {
  $types = array();
  $defaults = array(
    'description' => t('Display a empty container.'),
    'category' => t('AIR'),
    'defaults' => array(
      'placement' => NULL,
    ),
  );

  $entity_types = air_get_all_reference_fields();
  foreach ($entity_types as $entity_type => $fields) {
    foreach ($fields as $field) {
      $type = $defaults;
      $type['title'] = t('AIR %entity_type:%referencefield', array('%entity_type' => $entity_type, '%referencefield' => $field));
      $type['defaults']['field_name'] = $field;
      $type['defaults']['entity_type'] = $entity_type;
      $type['defaults']['style'] = 'default';
      $type['defaults']['show_default'] = 0;
      $type['required context'] = array();
      $type['required context'][] = new ctools_context_required($entity_type, $entity_type);
      $types['air:' . $entity_type . ':' . $field] = $type;
    }
  }
  return $types;
}


/**
 * Render callback.
 *
 */
function air_container_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = 'air';
  $block->delta = 0;
  $block->title = '';
  $block->content = array();

  $entity = $context[0]->data;
  if (!$entity) return $block;

  if (!air_check_referred_data_exists(entity_id($conf['entity_type'], $entity), $conf['entity_type'], $conf['field_name'])) {
    return $block;
  }

  if (isset($conf['placement'])) {
    $placement = $conf['placement'];
  }


  $block->content['wrapper'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('air-container'),
    ),
  );

  if (user_access('configure air settings')) {
    drupal_add_css(drupal_get_path('module', 'air') . '/css/air.css');
    drupal_add_js(drupal_get_path('module', 'air') . '/js/air.js');
    $block->content['wrapper']['edit_link'] = array(
      '#markup' => air_edit_link_render($entity, $conf),
    );
  }
  $entity_referece_ids = air_get_referred_data_from_entity(entity_id($conf['entity_type'], $entity), $conf['entity_type'], $conf['field_name']);

  $air_settings = air_get_entities($entity, $conf, $entity_referece_ids);

  if (!empty($air_settings)) {
    $entity_array = air_render_entities($air_settings, $conf['entity_type']);
    $style = $conf['style'];
    $entity_array_rendered = air_render_themed_items($style, $entity_array);
    $block->content['wrapper']['rendered_entities'] = array(
      '#markup' => $entity_array_rendered,
      '#prefix' => '<div class="air-rendered-entities">',
      '#suffix' => '</div>',
    );
  }

  return $block;
}

/**
 * @param $form
 * @param $form_state
 * @return mixed
 */
function air_container_content_type_edit_form($form, &$form_state) {
  $settings = $form_state['conf'];
  $form += air_formatter_settings_form($settings);

  return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function air_container_content_type_edit_form_submit($form, &$form_state) {
  if (empty($form_state['conf']['placement'])) {
    $form_state['conf']['placement'] = uniqid();
  }
  // Copy everything from our defaults.
  foreach (array('style', 'show_default') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Returns the administrative title for a type.
 */
function air_container_admin($subtype, $conf, $context) {
  return t('AIR Container: @subtype', array('@subtype' => $subtype));
}