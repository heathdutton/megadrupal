<?php

$plugin = array(
  'title' => t('Entity'),
  'category' => t('Entity list'),
  'description' => t('Displays an entity from an entity list.'),
  'required context' => new ctools_context_required(t('List'), 'entitylist'),
  'defaults' => array(
    'index' => 1,
    'view_mode' => '',
    'use_pager' => FALSE,
    'pager_id' => 0,
  ),
  'admin title' => 'entitylist_ctools_entity_admin_title',
  'admin info' => 'entitylist_ctools_entity_admin_info',
  'render callback' => 'entitylist_ctools_entity_render',
  'edit form' => 'entitylist_ctools_entity_form',
);

/**
 * Render callback.
 */
function entitylist_ctools_entity_render($subtype, $conf, $panel_args, $context) {
  // Get the list.
  $list = entitylist_list_load_from_context($context);
  // The "readable" index starts at 1 while the "real" index starts at 0.
  $index = $conf['index'] - 1;
  $entity = $list->entity($index);

  $block = new stdClass();
  $block->module  = 'entitylist';
  // Provide integration with the cachetags plugin.
  $tags = $list->cacheTags(TRUE);
  $block->cache_tags = $block->request_tags = $tags;

  if (empty($entity)) {
    $block->content = '';
    return $block;
  }

  $entity_type = $list->entityType();
  // TODO: Would be nice to avoid entity_get_info() here. The query handler
  // should theoritically already know about the entity id. Can we pass that
  // along somehow?
  $entity_info = entity_get_info($entity_type);
  $entity_id = $entity->{$entity_info['entity keys']['id']};

  // TODO: Is there a more sustainable way to render entities in a generic way,
  // without depending on entity.module and entity_view()?
  $view_function = $entity_type . '_view';
  if (strstr($conf['view_mode'], ':') === FALSE) {
    $view_mode = $conf['view_mode'];
  }
  else {
    list(, $view_mode) = explode(':', $conf['view_mode']);
  }
  if ($entity_type == 'comment') {
    $node = node_load($entity->nid);
    $block->content = $view_function($entity, $node, $view_mode);
  }
  else {
    $block->content = $view_function($entity, $view_mode);
  }

  // Also add the entity that is being rendered.
  $block->cache_tags[$entity_type][] = $entity_id;
  $block->request_tags[$entity_type][] = $entity_id;

  return $block;
}

/**
 * Edit form callback.
 */
function entitylist_ctools_entity_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $index_options = array();
  for ($i = 1; $i <= 50; $i++) {
    $index_options[$i] = t("$i");
  }
  $form['index'] = array(
    '#type' => 'select',
    '#title' => t('Index'),
    '#options' => $index_options,
    '#default_value' => $conf['index'],
  );

  $view_mode_options = array();
  $entity_infos = entity_get_info();
  $generic_view_modes = array();
  foreach ($entity_infos as $entity_type => $entity_info) {
    $view_modes = array();
    foreach ($entity_info['view modes'] as $view_mode => $view_mode_info) {
      $view_modes[$entity_type . ':' . $view_mode] = $view_mode_info['label'];
      $generic_view_modes[$view_mode] = $view_mode_info['label'];
    }
    if (!empty($view_modes)) {
      $view_mode_options[$entity_info['label']] = $view_modes;
    }
  }

  $view_mode_options['Generic view modes'] = $generic_view_modes;

  $form['view_mode'] = array(
    '#type' => 'select',
    '#title' => t('View mode'),
    '#options' => $view_mode_options,
    '#default_value' => $conf['view_mode'],
  );
  return $form;
}

function entitylist_ctools_entity_form_submit($form, &$form_state) {
  $conf = &$form_state['conf'];
  // Fetch pager settings from the context.
  $context = $form_state['contexts'][$conf['context']];
  $conf['use_pager'] = !empty($context->use_pager) ? $context->use_pager : FALSE;
  $conf['pager_id'] = isset($context->pager_id) ? (int)$context->pager_id : 0;
  // Save values from the form.
  foreach (array('index', 'view_mode') as $key) {
    $conf[$key] = $form_state['values'][$key];
  }
}

/**
 * Admin title callback.
 */
function entitylist_ctools_entity_admin_title($subtype, $conf, $context) {
  return t('Item @index from "@list"', array('@index' => $conf['index'], '@list' => $context->identifier));
}

/**
 * Admin info callback.
 */
function entitylist_ctools_entity_admin_info($subtype, $conf, $context) {
  if (strstr($conf['view_mode'], ':') === FALSE) {
    $view_mode = $conf['view_mode'];
    $title = $view_mode;
    $type = 'Generic';
  }
  else {
    list($entity_type, $view_mode) = explode(':', $conf['view_mode']);
    $entity_info = entity_get_info($entity_type);
    $title = check_plain($entity_info['view modes'][$view_mode]['label']);
    $type = $entity_info['label'];
  }
  $block = new stdClass();
  $block->title = $title;
  $block->content = t('@mode for @type', array('@mode' => $block->title, '@type' => $type));
  return $block;
}
