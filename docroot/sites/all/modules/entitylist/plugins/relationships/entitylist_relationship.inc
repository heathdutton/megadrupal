<?php

$plugin = array(
  'title' => t('List with arguments'),
  'description' => t('Creates a list context from arguments.'),
  'context' => 'entitylist_ctools_relationship_context',
  'get child' => 'entitylist_ctools_relationship_get_child',
  'get children' => 'entitylist_ctools_relationship_get_children',
  'edit form' => 'entitylist_ctools_relationship_settings_form',
);

function entitylist_ctools_relationship_get_child($plugin, $parent, $child) {
  $list = entitylist_list_load($child);
  $child = entitylist_ctools_context_info($list, $parent, TRUE);
  return $child;
}

function entitylist_ctools_relationship_get_children($plugin, $parent) {
  $children = array();
  $lists = entitylist_list_load_all();
  foreach ($lists as $list) {
    $info = entitylist_ctools_context_info($list, $parent, TRUE);
    if (!empty($info)) {
      $children[$info['name']] = $info;
    }
  }
  return $children;
}

function entitylist_ctools_relationship_context($contexts, $conf) {
  $context_keys = array_keys($conf['context']);
  if (empty($contexts[$context_keys[0]])) {
    return ctools_context_create_empty('entitylist', NULL);
  }

  if (isset($conf['name'])) {
    list($parent, $name) = explode(':', $conf['name']);
    $list = entitylist_list_load($name);
    $required_arguments = $list->requiredArguments();

    // Now parse the argument input in what format the list wants it's
    // arguments. It'll be either as full contexts or converters.
    $arguments = array();
    foreach ($required_arguments as $argument_key => $info) {
      $context_key = array_shift($context_keys);
      if (isset($contexts[$context_key])) {
        // Continue if we have an empty context and it's optional.
        if (!$info['required'] && isset($contexts[$context_key]->empty) && $contexts[$context_key]->empty) {
          continue;
        }
        if (strpos($argument_key, '.')) {
          list($context, $converter) = explode('.', $argument_key);
          $arguments[] = ctools_context_convert_context($contexts[$context_key], $converter);
        }
        else {
          $arguments[] = $contexts[$context_key];
        }
      }
    }

    // Set the arguments for the list.
    $list->setArguments($arguments);

    // Configure the pager.
    $range = $conf['range'];
    if (!empty($conf['use_pager']) && isset($conf['pager_id'])) {
      // Only configure the pager once.
      if (!$list->getPager()) {
        $list->setPager($range['limit'], $conf['pager_id']);
      }
    }
    // Configure the list with given range.
    else {
      $list->setRange($range['offset'], $range['limit']);
    }
  }

  if (!empty($list)) {
    $context = ctools_context_create('entitylist', $list);
    // @see entitylist_list_load_from_context().
    $context->setArguments = $list->getArguments();
    if (!empty($conf['use_pager']) && isset($conf['pager_id'])) {
      $context->use_pager = TRUE;
      $context->pager_id = (int)$conf['pager_id'];
    }
    return $context;
  }
}
