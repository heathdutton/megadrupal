<?php
/**
 * @file
 * Functions relating to menu callbacks for the Felix module.
 */

/**
 * Page callback for the add block page.
 */
function felix_add_block_page() {
  if (empty($_GET['region']) || empty($_GET['path'])) {
    return MENU_NOT_FOUND;
  }

  if (!$region = _felix_region_by_name($_GET['region'])) {
    return MENU_NOT_FOUND;
  }

  if (!empty($_GET['nodetype'])) {
    $salt = $_GET['nodetype'] . $region->name;
    if (drupal_get_token($salt) != $_GET['token']) {
      return MENU_ACCESS_DENIED;
    }

    if (!empty($_GET['viewmode'])) {
      return _felix_add_block_nodetype_page($region, $_GET['nodetype'], $_GET['path'], $_GET['viewmode']);
    }
    else {
      $query = db_select('felix_region', 'r')->condition('r.name', $region->name);
      $query->join('felix_block_set_viewmode', 'vm', 'r.block_set = vm.block_set');
      $query->fields('vm', array('viewmode'));
      $viewmodes = $query->execute()->fetchCol();
      if (count($viewmodes) == 1) {
        // Skip the viewmodes page if there is only one viewmode to choose from.
        return _felix_add_block_nodetype_page($region, $_GET['nodetype'], $_GET['path'], $viewmodes[0]);
      }
      else {
        return _felix_add_block_viewmode_page($region, $_GET['nodetype'], $_GET['path'], $_GET['token']);
      }
    }
  }
  elseif (!empty($_GET['module']) && !empty($_GET['delta'])) {
    $salt = $_GET['module'] . $_GET['delta'] . $region->name;
    if (drupal_get_token($salt) != $_GET['token']) {
      return MENU_ACCESS_DENIED;
    }
    $block = new stdClass();
    $block->module = $_GET['module'];
    $block->delta = $_GET['delta'];
    $block->region = $_GET['region'];
    $block->path = $_GET['path'];
    $block->data = array();
    $config = module_invoke_all('felix_block_config', $block);
    if ($config || TRUE) {
      $block = new stdClass();
      $block->region = $_GET['region'];
      $block->module = $_GET['module'];
      $block->delta = $_GET['delta'];
      $block->data = array();
      return drupal_get_form('felix_attributes_form', $region, $block, $_GET['path']);
    }
    else {
      return _felix_add_block($region, $_GET['path'], $_GET['module'], $_GET['delta']);
    }
  }
  elseif (!empty($_GET['nid'])) {
    $salt = $_GET['nid'] . $region->name;
    if (drupal_get_token($salt) != $_GET['token']) {
      return MENU_ACCESS_DENIED;
    }
    $data = array('viewmode' => $_GET['viewmode']);
    return _felix_add_block($region, $_GET['path'], 'node', $_GET['nid'], 'node/' . $_GET['nid'], $data);
  }
  else {
    // Show the list of possible blocks and nodetypes.
    return _felix_add_block_list_page($region, $_GET['path']);
  }
}

function _felix_add_block_viewmode_page($region, $nodetype, $path, $token) {
  global $language;

  $query = db_select('felix_region', 'r')->condition('r.name', $region->name);
  $query->join('felix_block_set_viewmode', 'vm', 'r.block_set = vm.block_set');
  $query->fields('vm', array('viewmode'));
  $view_modes = $query->execute()->fetchCol();

  $entity_info = entity_get_info('node');
  $node_types = node_type_get_names();

  $output = array(
    'header' => array(
      '#theme' => 'html_tag',
      '#tag' => 'h2',
      '#value' => t('View mode for !nodetype:', array('!nodetype' => $node_types[$nodetype])),
    ),
    'links' => array(
      '#theme' => 'links',
      '#links' => array(),
    ),
  );

  foreach ($view_modes as $view_mode) {
    $output['links']['#links'][] = array(
      'title' => $entity_info['view modes'][$view_mode]['label'],
      'href' => 'felix-blocks/add',
      'query' => array(
        'region' => $region->name,
        'nodetype' => $nodetype,
        'path' => $path,
        'viewmode' => $view_mode,
        'token' => $token,
      ),
    );
  }

  return $output;
}

function _felix_add_block_nodetype_page($region, $nodetype, $path, $viewmode) {
  global $language;

  $output = '';

  $output .= '<h2>' . t('Search node') . '</h2>';
  $form = drupal_get_form('felix_add_node_form', $region, $nodetype, $path, $viewmode);
  $output .= drupal_render($form);

  $output .= '<h2>' . t('Recent content') . '</h2>';
  $language_condition = db_or()
    ->condition('n.language', LANGUAGE_NONE)
    ->condition('n.language', $language->language);
  $q = db_select('node', 'n')
          ->fields('n', array('nid', 'type', 'title', 'created'))
          ->fields('u', array('name'))
          ->condition('n.type', $nodetype)
          ->condition($language_condition)
          ->orderBy('n.changed', 'DESC')
          ->range(0, 10)
          ->addTag('node_access');
  $q->join('users', 'u', 'n.uid = u.uid');
  $nodes = $q->execute();
  $items = array();

  $table = array(
    '#theme' => 'table',
    '#header' => array(
      t('Title'),
      t('Author'),
      t('Created'),
    ),
    '#rows' => array(),
    '#nodes' => array(),
  );

  while ($node = $nodes->fetchObject()) {
    $table['#nodes'][] = $node;

    $url = l($node->title, 'felix-blocks/add', array(
      'query' => array(
        'nid' => $node->nid,
        'region' => $region->name,
        'path' => $path,
        'token' => drupal_get_token($node->nid . $region->name),
        'viewmode' => $viewmode,
      ),
    ));

    $table['#rows'][] = array(
      $url,
      $node->name,
      format_date($node->created, 'short'),
    );
  }

  // Allow other modules to alter the recent content table.
  drupal_alter('felix_recent_content', $table);

  if (count($table['#rows'])) {
    $output .= drupal_render($table);
  }
  else {
    $output .= '<p>' . t('No content found.') . '</p>';
  }

  return $output;
}

/**
 * Autocomplete callback function.
 */
function _felix_autocomplete($type) {
  $search = $_GET['s'];

  $results = array();
  $nodes = db_select('node', 'n')
    ->fields('n', array('nid', 'title', 'changed'))
    ->condition('n.type', $type)
    ->condition('n.title', "$search%", 'LIKE')
    ->orderBy('n.changed', 'desc')
    ->range(0, 10)
    ->addTag('node_access')
    ->execute()
    ->fetchAll();
  foreach ($nodes as $node) {
    $results[] = array(
      'nid' => $node->nid,
      'title' => check_plain($node->title),
      'value' => $node->title . ' (#' . $node->nid . ')',
      'description' => t('Last modified: @date', array('@date' => format_date($node->changed))),
      'addClass' => NULL,
    );
  }

  // Let other modules alter the autocomplete results.
  drupal_alter('felix_autocomplete', $results);

  if (empty($results)) {
    $results = array(array(
      'title' => t('No results'),
      'addClass' => 'status-notice',
      'disabled' => TRUE,
    ));
  }

  drupal_json_output($results);
  drupal_exit();
}

/**
 * Form to add a node to a region.
 */
function felix_add_node_form($form, &$form_state, $region, $nodetype, $path, $viewmode) {
  $form['#felix_region'] = $region;
  $form['#felix_nodetype'] = $nodetype;
  $form['#felix_path'] = $path;
  $form['#felix_viewmode'] = $viewmode;

  $form['node'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#autocomplete_path' => 'felix-blocks/autocomplete-node/' . $nodetype,
    '#required' => TRUE,
  );

  if (module_exists('linkit')) {
    // Check which version of linkit is currently installed.
    $added = FALSE;
    foreach (array('bac', 'linkit_bac') as $library) {
      if ($added = drupal_add_library('linkit', $library)) {
        break;
      }
    }

    if ($added) {
      // Use the better autocomplete widget from linkit.
      drupal_add_library('linkit', 'linkit_bac');
      drupal_add_js(drupal_get_path('module', 'felix') . '/felix.js');
      drupal_add_js(array('felix' => array('nodetype' => $nodetype)), 'setting');
      unset($form['node']['#autocomplete_path']);
      $form['node']['#prefix'] = '<div id="felix-search-node">';
      $form['node']['#suffix'] = '</div>';
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  return $form;
}

/**
 * Validate callback for felix_add_node_form.
 */
function felix_add_node_form_validate($form, &$form_state) {
  if (!preg_match('/\\(\\#([0-9]+)\\)$/s', $form_state['values']['node'], $match)) {
    form_set_error('node', t('Invalid format'));
  }
  else {
    $nodetype = db_select('node', 'n')
            ->fields('n', array('type'))
            ->condition('n.nid', $match[1])
            ->execute()
            ->fetchField();
    if ($nodetype !== $form['#felix_nodetype']) {
      form_set_error('node', t('Node not found'));
    }
  }
}

/**
 * Submit callback for felix_add_node_form.
 */
function felix_add_node_form_submit($form, &$form_state) {
  preg_match('/\\(\\#([0-9]+)\\)$/s', $form_state['values']['node'], $match);
  $nid = (int) $match[1];
  $data = array('viewmode' => $form['#felix_viewmode']);
  _felix_add_block($form['#felix_region'], $form['#felix_path'], 'node', $nid, "node/$nid", $data);
}

function felix_add_block_config_form($form, &$form_state, $region, $module, $delta, $path) {
  $form['#felix_region'] = $region;
  $form['#felix_module'] = $module;
  $form['#felix_delta'] = $delta;
  $form['#felix_path'] = $path;

  $config = module_invoke($module, 'block_configure', $delta);
  if ($config) {
    $form['block_config'] = $config;
  }

  $form['buttons'] = array(
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
    'cancel' => array(
      '#markup' => l(t('cancel'), $path),
    ),
  );

  return $form;
}

function _felix_add_block($region, $path, $module, $delta, $detail_path = NULL, $data = array()) {
  if ($module == 'node' && is_numeric($delta) && module_exists('translation')) {
    // Add the source translation.
    $tnid = db_select('node', 'n')
      ->fields('n', array('tnid'))
      ->condition('n.nid', $delta)
      ->execute()
      ->fetchField();
    $delta = $tnid ? $tnid : $delta;
    $detail_path = "node/$delta";
  }

  $hash = _felix_get_hash($region, $path);

  $block = new stdClass();
  $block->region = $region->name;
  $block->weight = _felix_new_block_weight($region->name);
  $block->hash = $hash->hash;
  $block->module = $module;
  $block->delta = $delta;
  $block->detail_path = $detail_path;
  $block->data = $data;
  $block->hash_parts = $hash->parts;
  $block->exclude = serialize(array());

  foreach (module_implements('felix_block_insert') as $module) {
    $hook = "{$module}_felix_block_insert";
    $hook($block, $path);
  }

  $block->data = serialize($block->data);
  $block->hash_parts = serialize($block->hash_parts);

  drupal_write_record('felix_block', $block);
  drupal_set_message(t('Succesfully added a new block to the region %region.', array('%region' => $region->title)));

  _felix_expire_pagecache($path);

  drupal_goto($path);
}

function _felix_add_block_list_page($region, $path) {
  // Build a list of all possible items (blocks or nodetypes).
  $items = array();

  // Add list of nodetypes.
  $query = db_select('felix_region', 'r')->condition('r.name', $region->name);
  $nodetype_table = $query->join('felix_block_set_nodetype', 'nt', 'r.block_set = nt.block_set');
  $query->fields('nt', array('nodetype'))->orderBy('nt.nodetype');
  $nodetypes = $query->execute()->fetchCol();
  if ($nodetypes) {
    $nodetype_names = node_type_get_names();
    $items['_nodetypes'] = array();
    foreach ($nodetypes as $nodetype) {
      $items['_nodetypes'][] = array(
        'title' => $nodetype_names[$nodetype],
        'href' => 'felix-blocks/add',
        'query' => array(
          'nodetype' => $nodetype,
          'region' => $region->name,
          'path' => $path,
          'token' => drupal_get_token($nodetype . $region->name),
        ),
      );
    }
  }

  // Add blocks.
  $blocks = db_select('felix_block_set_block', 'b')
          ->fields('b', array('module', 'delta'))
          ->condition('b.block_set', $region->block_set)
          ->orderBy('b.module')
          ->execute()
          ->fetchAll();
  $modules = array();
  $deltas = array();
  foreach ($blocks as $block) {
    if (!isset($deltas[$block->module])) {
      $deltas[$block->module] = array();
    }
    $deltas[$block->module][] = $block->delta;
  }
  $modules = array_keys($deltas);
  foreach ($modules as $module) {
    $info = module_invoke($module, 'block_info');
    foreach ($info as $delta => $block) {
      if (!in_array($delta, $deltas[$module]) && !in_array('*', $deltas[$module])) {
        continue;
      }
      if (!isset($items[$module])) {
        $items[$module] = array();
      }
      $items[$module][] = array(
        'title' => $info[$delta]['info'],
        'href' => 'felix-blocks/add',
        'query' => array(
          'module' => $module,
          'delta' => $delta,
          'region' => $region->name,
          'path' => $path,
          'token' => drupal_get_token($module . $delta . $region->name),
        ),
      );
    }
  }

  $output = '';
  foreach ($items as $name => $links) {
    if ($name == '_nodetypes') {
      $name = t('Nodetype');
    }
    else {
      $info = system_get_info('module', $name);
      $name = $info['name'];
    }

    $output .= '<h2>' . check_plain($name) . '</h2>';
    $output .= theme('links', array('links' => $links));
  }
  return $output;
}

/**
 * Menu callback for autocomplete node fields.
 */
function felix_autocomplete_node($nodetype, $text) {
  global $language;
  $matches = array();
  $language_condition = db_or()
    ->condition('n.language', LANGUAGE_NONE)
    ->condition('n.language', $language->language);
  $nodes = db_select('node', 'n')
    ->fields('n', array('nid', 'title'))
    ->condition('n.type', $nodetype)
    ->condition('n.title', db_like($text) . '%', 'LIKE')
    ->condition($language_condition)
    ->orderBy('n.changed', 'DESC')
    ->range(0, 10)
    ->addTag('node_access')
    ->execute()
    ->fetchAll();
  foreach ($nodes as $node) {
    $matches["{$node->title} (#{$node->nid})"] = check_plain($node->title);
  }
  drupal_json_output($matches);
}

/**
 * Page callback for block actions.
 *
 * This function is only accessible with the following GET parameters:
 *
 * - region
 * - path
 * - module
 * - delta
 * - hash
 * - token => drupal_get_token(region module delta hash)
 *
 * @param string $action One of 'move-up', 'move-down', 'attributes' or 'remove'
 */
function felix_block_action($action) {
  if (empty($_GET['region'])
    || empty($_GET['fbid'])
    || empty($_GET['path'])
    || empty($_GET['module'])
    || empty($_GET['delta'])
    || !isset($_GET['hash'])
    || empty($_GET['token'])) {
    return MENU_NOT_FOUND;
  }

  if (!$region = _felix_region_by_name($_GET['region'])) {
    return MENU_NOT_FOUND;
  }

  $block = db_select('felix_block', 'cb')
          ->fields('cb')
          ->condition('cb.fbid', $_GET['fbid'])
          ->execute()
          ->fetchObject();
  if (!$block) {
    return MENU_NOT_FOUND;
  }

  $block->data = unserialize($block->data);
  $block->hash_parts = unserialize($block->hash_parts);
  $block->exclude = unserialize($block->exclude);

  $token = drupal_get_token($_GET['fbid'] . $_GET['region'] . $_GET['module'] . $_GET['delta'] . $_GET['hash']);

  if ($token !== $_GET['token']) {
    return MENU_ACCESS_DENIED;
  }

  switch ($action) {
    case 'move-up':
    case 'move-down':
      return _felix_block_action_move($action, $block, $_GET['path']);
    case 'attributes':
      return drupal_get_form('felix_attributes_form', $region, $block, $_GET['path']);
    case 'hide':
    case 'show':
      return _felix_block_action_hide($action, $block, $_GET['path']);
    case 'remove':
      return drupal_get_form('felix_remove_form', $region, $block, $_GET['path']);
  }
}

function _felix_block_action_move($action, $block, $path) {
  $candidate = db_select('felix_block', 'cb')
          ->fields('cb', array('fbid', 'region', 'hash', 'module', 'delta', 'weight'))
          ->condition('cb.region', $block->region)
          ->condition('cb.hash', $block->hash)
          ->condition('cb.weight', $block->weight, $action == 'move-up' ? '<' : '>')
          ->orderBy('cb.weight', $action == 'move-up' ? 'DESC' : 'ASC')
          ->range(0, 1)
          ->execute()
          ->fetchObject();

  if ($candidate) {
    // Swap the block with the candidate.
    db_update('felix_block')
            ->fields(array('weight' => $candidate->weight))
            ->condition('fbid', $block->fbid)
            ->execute();
    db_update('felix_block')
            ->fields(array('weight' => $block->weight))
            ->condition('fbid', $candidate->fbid)
            ->execute();
  }
  else {
    drupal_set_message(t('Unable to move the block.'), 'warning');
  }
  _felix_expire_pagecache($path);
  drupal_goto($path);
}

function _felix_block_action_hide($action, $block, $path) {
  if ($action == 'hide') {
    $block->exclude = array_unique(array_merge($block->exclude, array($path)));
  }
  else {
    $block->exclude = array_values(array_diff($block->exclude, array($path)));
  }
  db_update('felix_block')
    ->fields(array('exclude' => serialize($block->exclude)))
    ->condition('fbid', $block->fbid)
    ->execute();
  _felix_expire_pagecache($path);
  drupal_goto($path);
}

function felix_attributes_form($form, &$form_state, $region, $block, $path) {
  $form['#felix_region'] = $region;
  $form['#felix_block'] = $block;
  $form['#felix_path'] = $path;

  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => isset($block->data['subject']) ? $block->data['subject'] : '',
    '#description' => t('Leave empty to use the default subject.'),
  );

  if ($block->module == 'node') {
    $query = db_select('felix_region', 'r')->condition('r.name', $region->name);
    $query->join('felix_block_set_viewmode', 'vm', 'r.block_set = vm.block_set');
    $query->fields('vm', array('viewmode'));
    $viewmodes = $query->execute()->fetchCol();
    if (isset($block->data['viewmode']) && !in_array($block->data['viewmode'], $viewmodes)) {
      $viewmodes[] = $block->data['viewmode'];
    }
    if (count($viewmodes) > 1) {
      sort($viewmodes);
      $form['viewmode'] = array(
        '#type' => 'radios',
        '#title' => t('Viewmode'),
        '#options' => drupal_map_assoc($viewmodes, 'felix_viewmode_name'),
        '#default_value' => isset($block->data['viewmode']) ? $block->data['viewmode'] : 'teaser',
      );
    }
  }

  // Use "clone" to prevent the hooks from altering the $block object.
  $config = module_invoke_all('felix_block_config', clone $block);
  if ($config) {
    $form['#felix_block_config'] = TRUE;
    $form['block_config'] = $config;
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );

  return $form;
}

function felix_attributes_form_submit($form, &$form_state) {
  $block = $form['#felix_block'];

  $block->data['subject'] = $form_state['values']['subject'];
  if (isset($form_state['values']['viewmode'])) {
    $block->data['viewmode'] = $form_state['values']['viewmode'];
  }

  foreach (module_implements('felix_block_update') as $module) {
    $hook = "{$module}_felix_block_update";
    $hook($block, $form_state['values']);
  }

  $hash = _felix_get_hash($form['#felix_region'], $form['#felix_path']);
  if (empty($block->hash)) {
    $new = TRUE;
    $block->hash = $hash->hash;
    $block->hash_parts = $hash->parts;
  }
  else {
    $new = FALSE;
  }

  // Save block config.
  if (!empty($form['#felix_block_config'])) {
    $config = module_invoke_all('felix_block_config_save', $block, $form_state['values']);
    if (is_array($config)) {
      $block->data['config'] = $config;
    }
  }

  if ($new) {
    _felix_add_block($form['#felix_region'], $form['#felix_path'], $block->module, $block->delta, NULL, $block->data);
  }
  else {
    $block->data = serialize($block->data);
    $block->hash_parts = serialize($block->hash_parts);
    $block->exclude = serialize($block->exclude);
    drupal_write_record('felix_block', $block, array('fbid'));

    _felix_expire_pagecache($form['#felix_path']);

    drupal_set_message(t('The block settings have been saved succesfully.'));
    drupal_goto($form['#felix_path']);
  }
}

function felix_remove_form($form, &$form_state, $region, $block, $path) {
  $block = _felix_render_block($block);

  $form['#felix_region'] = $region;
  $form['#felix_block'] = $block;
  $form['#felix_path'] = $path;

  $form['info'] = array(
    '#markup' => '<p>' . t('Are you sure you want to delete the block %subject from %region?', array('%subject' => $block->subject, '%region' => $region->title)) . '</p>',
  );

  if ($block->module == 'node') {
    if (($node = node_load($block->delta)) && node_access('delete', $node)) {
      $form['delete_node'] = array(
        '#type' => 'checkbox',
        '#title' => t('Also delete the node'),
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Remove'),
  );

  $form['cancel'] = array(
    '#markup' => l(t('cancel'), $path),
  );

  return $form;
}

function felix_remove_form_submit($form, &$form_state) {
  $region = $form['#felix_region'];
  $block = $form['#felix_block'];
  $path = $form['#felix_path'];

  db_delete('felix_block')
    ->condition('fbid', $block->fbid)
    ->execute();

  foreach (module_implements('felix_block_delete') as $module) {
    $hook = "{$module}_felix_block_delete";
    $hook($block);
  }

  if ($block->module == 'node' && !empty($form_state['values']['delete_node'])) {
    node_delete($block->delta);
  }

  _felix_expire_pagecache($path);

  drupal_set_message(t('The block %subject has been deleted from region %region.', array('%subject' => $block->subject, '%region' => $region->title)));
  drupal_goto($path);
}

/**
 * Convert a viewmode name to user friendly name.
 */
function felix_viewmode_name($name) {
  $entity_info = entity_get_info('node');
  if (isset($entity_info['view modes'][$name])) {
    return $entity_info['view modes'][$name]['label'];
  }
  else {
    return $name;
  }
}

/**
 * Expire caches for given page.
 *
 * @param string $path
 */
function _felix_expire_pagecache($path) {
  // Expire Drupal pagecache.
  cache_clear_all(url($path, array('absolute' => TRUE)), 'cache_page');

  // Expire external caches.
  if (function_exists('expire_cache_derivative')) {
    expire_cache_derivative(array($path));
  }
}
