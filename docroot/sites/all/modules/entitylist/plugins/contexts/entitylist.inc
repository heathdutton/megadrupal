<?php
/**
 * @file
 * Entity list context.
 */

$plugin = array(
  'title' => t('Entity list'),
  'description' => t('Adds an entity list to the page context.'),
  'context' => 'entitylist_ctools_context',
  'keyword' => 'list',
  'defaults' => array(
    'use_pager' => FALSE,
    'pager_id' => 0,
    'range' => array(
      'offset' => 0,
      'limit' => 10,
    ),
  ),
  'context name' => 'entitylist',
  'convert list' => array(
    'name' => t('Name'),
    'title' => t('Title'),
    'description' => t('Description'),
  ),
  'convert' => 'entitylist_ctools_context_convert',
  'edit form' => 'entitylist_ctools_context_edit_form',
  'get child' => 'entitylist_ctools_context_get_child',
  'get children' => 'entitylist_ctools_context_get_children',
);

function entitylist_ctools_context_get_child($plugin, $parent, $child) {
  $list = entitylist_list_load($child);
  $info = entitylist_ctools_context_info($list, $parent, FALSE);
  //return array_merge($plugin, $info);
  return $info;
}

function entitylist_ctools_context_get_children($plugin, $parent) {
  $children = array(
    'entitylist' => $plugin,
  );
  $children['entitylist']['no ui'] = TRUE;

  $lists = entitylist_list_load_all();
  foreach ($lists as $list) {
    $info = entitylist_ctools_context_info($list, $parent, FALSE);
    if (!empty($info)) {
      //$children[$info['name']] = array_merge($plugin, $info);
      $info['no required context ui'] = TRUE;
      // TODO: We should probably prefix the key with $parent or something here.
      $children[$info['name']] = $info;
    }
  }
  return $children;
}

function entitylist_ctools_context($empty, $data = NULL, $conf = FALSE, $plugin) {
  $context = new ctools_context('entitylist');
  $context->plugin = $plugin['name'];

  if ($empty) {
    return $context;
  }

  // This is a configured context, i.e. one without other required
  // arguments (contexts).
  if ($conf) {
    if (is_array($data) && isset($data['name'])) {
      list($plugin, $name) = explode(':', $data['name']);
      $range = $data['range'];
      $list = entitylist_list_load($name);
      // Configure the pager.
      if (!empty($data['use_pager']) && isset($data['pager_id'])) {
        $context->use_pager = TRUE;
        $context->pager_id = (int)$data['pager_id'];
        // Only configure the pager once.
        if (!$list->getPager()) {
          $list->setPager($range['limit'], $data['pager_id']);
        }
      }
      // Configure the list with given range.
      else {
        $list->setRange($range['offset'], $range['limit']);
      }
    }
  }
  else {
    $list = $data;
  }

  if (!empty($list)) {
    $context->data = $list;
    $context->title = $list->title;
    // TODO: Make sure that this gives enough granularity when setting up
    // caching per context. Probably a lot of cache lifetime to save right here.
    $context->argument = $list->name;
    // @see entitylist_list_load_from_context().
    $context->setArguments = array();
    $context->restrictions['name'] = array($list->name);
    return $context;
  }
}

function entitylist_ctools_context_edit_form_submit($form, &$form_state) {
  $conf = &$form_state['conf'];
  foreach (array('use_pager', 'pager_id', 'range') as $key) {
    if (isset($form_state['values'][$key])) {
      $conf[$key] = $form_state['values'][$key];
    }
  }
}

function entitylist_ctools_context_convert($context, $type) {
  switch ($type) {
    case 'name':
      return $context->data->name;
    case 'title':
      return $context->data->title;
    case 'description':
      return $context->data->description;
  }
}
