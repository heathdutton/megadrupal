<?php

$plugin = array(
  'title' => t("Entity from token"),
  'description' => t('Entity object from token.'),
  'context' => 'ctools_token_entity_context_context_create_entity',
  'edit form' => 'ctools_token_entity_context_context_entity_settings_form',
  'defaults' => array('entity_id' => '', 'extract_number' => FALSE),
  'convert list' => 'ctools_token_entity_context_context_entity_convert_list',
  'convert' => 'ctools_token_entity_context_context_entity_convert',
  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter the ID of an entity or the token containing entity ID.'),
  ),
  'get child' => 'ctools_token_entity_context_context_entity_get_child',
  'get children' => 'ctools_token_entity_context_context_entity_get_children',
);

function ctools_token_entity_context_context_entity_get_child($plugin, $parent, $child) {
  $plugins = ctools_token_entity_context_context_entity_get_children($plugin, $parent);
  return $plugins[$parent . ':' . $child];
}

function ctools_token_entity_context_context_entity_get_children($plugin, $parent) {
  $entities = entity_get_info();
  $plugins = array();
  foreach ($entities as $entity_type => $entity) {
    $child_plugin = $plugin;
    $child_plugin['title'] = $entity['label'] .  ' (' . t('from token') . ')';
    $child_plugin['keyword'] = $entity_type;
    $child_plugin['context name'] = $entity_type;
    $child_plugin['name'] = $parent . ':' . $entity_type;
    $child_plugin['description'] = t('Creates @entity context from a token-based entity ID.', array('@entity' => $entity_type));
    $child_plugin_id = $parent . ':' . $entity_type;
    drupal_alter('ctools_entity_context', $child_plugin, $entity, $child_plugin_id);
    $plugins[$child_plugin_id] = $child_plugin;
  }
  drupal_alter('ctools_entity_contexts', $plugins);
  return $plugins;
}

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function ctools_token_entity_context_context_create_entity($empty, $data = NULL, $conf = FALSE, $plugin = NULL) {
  $entity_type = $plugin['keyword'];
  $entity_info = entity_get_info($entity_type);
  $context = new ctools_context(array('entity:' . $entity_type, 'entity', $entity_type));
  $context->plugin = $plugin['name'];
  $context->keyword = $entity_type;

  if ($empty) {
    return $context;
  }

  $id = 0;
  if ($conf && isset($data['entity_id'])) {
    $id = $data['entity_id'];
    $token_data = array();
    if ($current_node = menu_get_object('node')) {
      $token_data = array('node' => $current_node);
    }
    $id = token_replace($id, $token_data, array('clear' => TRUE));
    if (!empty($data['extract_number'])) {
      $matches = array();
      // Extract first integer.
      if (preg_match('/([0-9]+)/', $id, $matches)) {
        $id = $matches[1];
      }
    }
    $data = entity_load($entity_type, array($id));
    $data = !empty($data[$id]) ? $data[$id] : FALSE;
  }
  elseif (is_object($data)) {
    $ids = entity_extract_ids($entity_type, $data);
    $id = $ids[0];
  }

  if (!empty($data)) {
    $context->data = $data;
    if (!empty($entity_info['entity keys']['label'])) {
      $context->title = $data->{$entity_info['entity keys']['label']};
    }
    $context->argument = $id;

    if ($entity_info['entity keys']['bundle']) {
      $context->restrictions['type'] = array($data->{$entity_info['entity keys']['bundle']});
    }
  }

  return $context;
}

function ctools_token_entity_context_context_entity_settings_form($form, &$form_state) {
  $plugin = $form_state['plugin'];
  $conf = $form_state['conf'];

  $form['entity_id'] = array(
    '#title' => t('Token for entity ID'),
    '#description' => t('Enter the ID of a @entity entity or the token containing entity ID.', array('@entity' => $plugin['keyword'])) . '<br />' .
      t('You may use tokens from token.module.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#size' => 80,
    '#maxlength' => 512,
    '#default_value' => $conf['entity_id'],
  );

  $form['extract_number'] = array(
    '#title' => t('Extract number'),
    '#description' => t('Extract the first number from token value for using it as entity ID. This may be useful for destination URL parameter, the token for this is [current-page:query:destination].'),
    '#type' => 'checkbox',
    '#default_value' => $conf['extract_number'],
  );

  $form['available_tokens'] = array(
    '#value' => 'Browse available tokens',
    '#theme' => 'token_tree',
    '#token_types' => array('node'),
    '#dialog' => TRUE,
  );

  return $form;
}

function ctools_token_entity_context_context_entity_settings_form_submit($form, &$form_state) {
  $form_state['conf']['entity_id'] = $form_state['values']['entity_id'];
  $form_state['conf']['extract_number'] = $form_state['values']['extract_number'];
}

/**
 * Provide a list of ways that this context can be converted to a string.
 */
function ctools_token_entity_context_context_entity_convert_list($plugin) {
  $list = array();

  $entity = entity_get_info($plugin['context name']);
  if (isset($entity['token type'])) {
    $token = $entity['token type'];
  }
  else {
    $token = $plugin['context name'];
  }

  // Hack: we need either token.module or a core fix for this to work right,
  // until then, we just muscle it.
  if ($token == 'taxonomy_term') {
    $token = 'term';
  }

  $tokens = token_info();
  if (isset($tokens['tokens'][$token])) {
    foreach ($tokens['tokens'][$token] as $id => $info) {
      if (!isset($list[$id])) {
        $list[$id] = $info['name'];
      }
    }
  }
  return $list;
}

/**
 * Convert a context into a string.
 */
function ctools_token_entity_context_context_entity_convert($context, $type, $options = array()) {
  $entity_type = $context->type[2];
  $entity = entity_get_info($entity_type);

  if (isset($entity['token type'])) {
    $token = $entity['token type'];
  }
  else {
    $token = $entity_type;
  }

  // Hack: we need either token.module or a core fix for this to work right,
  // until then, we just muscle it.
  if ($token == 'taxonomy_term') {
    $token = 'term';
  }

  $tokens = token_info();

  $values = token_generate($token, array($type => $type), array($token => $context->data), $options);
  if (isset($values[$type])) {
    return $values[$type];
  }
}
