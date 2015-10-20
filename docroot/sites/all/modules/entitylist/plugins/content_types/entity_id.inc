<?php

/**
 * Implements hook_PLUGIN_ctools_content_types().
 */
function entitylist_entity_id_ctools_content_types() {
  return array(
    'title' => t('Lists'),
    'content type' => 'entitylist_entity_id_content_type_content_type',
  );
}

/**
 * Content type callback.
 */
function entitylist_entity_id_content_type_content_type($subtype, $plugin) {
  list($parent, $list_name, $entity_type, $entity_id) = explode(':', $subtype);
  $entities = entity_load($entity_type, array($entity_id));
  $entity = reset($entities);
  $list = entitylist_list_load($list_name);
  return _entitylist_entity_id_content_type_info($plugin, $list, $entity_type, $entity);
}

/**
 * Content types callback.
 */
function entitylist_entity_id_content_type_content_types($plugin) {
  $children = array();
  $lists = entitylist_list_load_all(array('content_type' => 1));
  foreach ($lists as $list) {
    $entity_type = $list->entityType();
    if ($entity_type) {
      $entities = $list->getEntities();
      foreach ($entities as $entity) {
        $info = _entitylist_entity_id_content_type_info($plugin, $list, $entity_type, $entity);
        $children[$info['name']] = $info;
      }
    }
  }
  return $children;
}

/**
 * Helper function to build content type info.
 */
function _entitylist_entity_id_content_type_info($plugin, $list, $entity_type, $entity) {
  list($entity_id, $revision_id, $bundle) = entity_extract_ids($entity_type, $entity);
  return array(
    'name' => $plugin['name'] . ':' . $list->name . ':' . $entity_type . ':' . $entity_id,
    'title' => check_plain(entity_label($entity_type, $entity)),
    'category' => t('List: @name (by ID)', array('@name' => $list->title)),
    'description' => filter_xss_admin($list->description),
    'required context' => new ctools_context_required(t('List'), 'entitylist', array('name' => $list->name)),
    'defaults' => array(
      'view_mode' => '',
      'use_pager' => FALSE,
      'pager_id' => 0,
    ),
    'render callback' => 'entitylist_ctools_entity_index_render',
  );
}

/**
 * Render callback.
 */
function entitylist_entity_id_content_type_render($subtype, $conf, $panel_args, $context) {
  list($parent, $list_name, $entity_type, $entity_id) = explode(':', $subtype);
  $list = entitylist_list_load_from_context($context);
  // TODO: register what entity is loaded for what list, so that we can adjust
  // indexed entity content types accordingly. Can be used for having a
  // partially "locked" list of entities.
  // @see entitylist_entity_index_content_type_render().
  $entities = entity_load($entity_type, array($entity_id));
  $entity = reset($entities);

  $block = new stdClass();
  $block->module  = 'entitylist';
  // Provide integration with the cachetags plugin.
  $tags = $list->cacheTags(TRUE);
  $block->cache_tags = $block->request_tags = $tags;
  // Also add the entity that is being rendered.
  $block->cache_tags[$entity_type][] = $entity_id;
  $block->request_tags[$entity_type][] = $entity_id;

  if (empty($entity)) {
    $block->content = '';
    return $block;
  }

  // TODO: Is there a more sustainable way to render entities in a generic way,
  // without depending on entity.module and entity_view()?
  $function = $entity_type . '_view';
  if ($entity_type == 'comment') {
    $node = node_load($entity->nid);
    $block->content = $function($entity, $node, $conf['view_mode']);
  }
  else {
    $block->content = $function($entity, $conf['view_mode']);
  }

  return $block;
}

/**
 * Edit form callback.
 */
function entitylist_entity_id_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  list($parent, $list_name, $entity_type, $entity_id) = explode(':', $form_state['subtype_name']);
  $entity_info = entity_get_info($entity_type);

  // TODO: Integrate with Panelizer here, and show layouts instead of select.
  $options = array();
  foreach ($entity_info['view modes'] as $view_mode => $view_mode_info) {
    $options[$view_mode] = $view_mode_info['label'];
  }
  $form['view_mode'] = array(
    '#type' => 'select',
    '#title' => t('View mode'),
    '#options' => $options,
    '#default_value' => $conf['view_mode'],
  );
  return $form;
}

/**
 * Edit form submit callback.
 */
function entitylist_entity_id_content_type_edit_form_submit($form, &$form_state) {
  $conf = &$form_state['conf'];
  $context = $form_state['contexts'][$conf['context']];
  // Fetch pager settings from the context and set them in the $conf array.
  // This is needed for the cache plugin to be able to cache paged lists
  // properly.
  $conf['use_pager'] = !empty($context->use_pager) ? $context->use_pager : FALSE;
  $conf['pager_id'] = isset($context->pager_id) ? (int)$context->pager_id : 0;
  foreach (array('view_mode') as $key) {
    $conf[$key] = $form_state['values'][$key];
  }
}

/**
 * Admin title callback.
 */
function entitylist_entity_id_content_type_admin_title($subtype, $conf, $context) {
  list($parent, $list_name, $entity_type, $entity_id) = explode(':', $subtype);
  $entities = entity_load($entity_type, array($entity_id));
  $entity = reset($entities);
  return check_plain(entity_label($entity_type, $entity));
}

/**
 * Admin info callback.
 */
function entitylist_entity_id_content_type_admin_info($subtype, $conf, $context) {
  list($parent, $list_name, $entity_type, $entity_id) = explode(':', $subtype);
  $entities = entity_load($entity_type, array($entity_id));
  $entity = reset($entities);
  $entity_info = entity_get_info($entity_type);

  $view_mode_label = check_plain($entity_info['view modes'][$conf['view_mode']]['label']);
  $entity_label = check_plain(entity_label($entity_type, $entity));

  $block = new stdClass();
  $block->title = $view_mode_label;
  $block->content = t('Displays %entity_label with view mode %view_mode_label.', array('%entity_label' => $entity_label, '%view_mode_label' => $view_mode_label));
  return $block;
}
