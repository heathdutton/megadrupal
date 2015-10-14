<?php

/**
 * Page callback for adding an entity.
 *
 * Return a list of available entities to add or the add entity page if there's
 * only a single entity type.
 */
function _entityspice_entity_bundles_list_page($etype) {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);

  // Bypass the entity/add listing if only one entity bundle is available.
  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }

  $vars = array('content' => $content, 'entity_type' => $etype);
  $output = theme('entityspice_entity_add_list', $vars);

  return $output;
}

/**
 * Page callback for entity overview page.
 */
function _entityspice_entity_page($etype, array $overrides = array()) {
// Setup the header for both the query and table.
  $header = array(
    'eid' => array(
      'field' => 'eid',
      'type' => 'property',
      'data' => 'ID',
      'specifier' => 'eid',
    ),
    'title' => array(
      'field' => 'title',
      'type' => 'property',
      'data' => 'Title',
      'specifier' => 'title',
    ),
    'owner' => array(
      'field' => 'uid',
      'type' => 'property',
      'data' => 'Owner',
      'specifier' => 'uid',
    ),
  );

  $info = _entityspice_getinfo_override($etype, $overrides);
  $has_bundle = $info['has bundle'];
  $has_revision = $info['has revision'];

  if($has_bundle) {
    $header['bundle'] = array(
      'field' => 'bundle',
      'type' => 'property',
      'data' => 'Bundle',
      'specifier' => 'bundle',
    );
  }

  if($has_revision); {
    $header['updated'] = array(
      'field' => 'updated',
      'type' => 'property',
      'data' => 'Updated',
      'sort' => 'desc',
      'specifier' => 'updated',
    );
  }

  $header['operations'] = array('data' => t('Operations'));

  $result = FALSE;
  if (!isset($_POST['form_id'])) {
    $bundle = isset($_GET['bundle']) ? $_GET['bundle'] : '';
    $properties = isset($_GET['properties']) ? $_GET['properties'] : array();
    $fields = isset($_GET['fields']) ? $_GET['fields'] : array();
    $result = _entityspice_filter_query($etype, $header, $bundle, $properties, $fields);
  }

  $entities = array();
  if ($result) {
    $entities = entity_load($etype, array_keys($result[$etype]));
  }

  $rows = array();
  // @FIXME load "entity type" entity in a standard fashion.
  $callback = $etype . '_bundle_load';
  if (!empty($entities)) {
    $destination = drupal_get_destination();
    foreach ($entities as $id => $entity) {
      $uri = entity_uri($etype, $entity);
      $e_type = $callback($entity->bundle);
      $rows[$id] = array(
        'eid' => $id,
        'title' => array(
          'data' => array(
            '#type' => 'link',
            '#title' => _entityspice_get_entity_name($etype, $entity),
            '#href' => $uri['path'],
          ),
        ),
        'owner' => array(
          'data' => array(
            '#theme' => 'username',
            '#account' => user_load($entity->uid),
          ),
        ),
        'type' => check_plain($e_type->name),
      );

      if($has_revision) {
        $rows[$id]['updated'] = format_date($entity->updated, 'short');
      }

      // Build a list of all the accessible operations for the current sec.
      $ops = array();
      if (entity_access('update', $etype, $entity)) {
        $ops['edit'] = array(
          'title' => t('edit'),
          'href' => $uri['path'] . '/view/edit',
          'query' => $destination,
        );
      }
      if (entity_access('delete', $etype, $entity)) {
        $ops['delete'] = array(
          'title' => t('delete'),
          'href' => $uri['path'] . '/view/delete',
          'query' => $destination,
        );
      }
      if (count($ops) > 1) {
        // Render an unordered list of operations links.
        $rows[$id]['operations'] = array(
          'data' => array(
            '#theme' => 'links__node_operations',
            '#links' => $ops,
            '#attributes' => array('class' => array('links', 'inline')),
          ),
        );
      }
      elseif (!empty($ops)) {
        // Render the first and only operation as a link.
        $link = reset($ops);
        $rows[$id]['operations'] = array(
          'data' => array(
            '#type' => 'link',
            '#title' => $link['title'],
            '#href' => $link['href'],
            '#options' => array('query' => $link['query']),
          ),
        );
      }
      else {
        unset($header['operations']);
      }
    }
  }

  $output = array(
    'form' => drupal_get_form('entityspice_filter_form', $etype),
    'entities' => array(
      '#theme' => 'entityspice_entity_list',
      '#entities' => $entities,
      '#entity_type' => $etype,
      '#header' => $header,
      '#rows' => $rows,
    ),
  );
  return $output;
}

/**
 * Page callback for adding an entity.
 */
function _entityspice_entity_add_page($etype, $bundle, array $overrides = array()) {
  $info = _entityspice_getinfo($etype);

  $module = isset($overrides['module']) ? $overrides['module'] : $info['module'];
  $form_callback = isset($overrides['form callback']) ? $overrides['add form'] : $info['add form'];

  if(isset($overrides['form file'])) {
    $form_file = $overrides['form file'];
  }
  else {
    $dir = $info['forms file']['directory'] ? '/' . $info['forms file']['directory'] : '/';
    $file = $info['forms file']['file'];
    $suffix = $info['forms file']['suffix'];
    $form_file = $dir . $file . '.' . $suffix;
  }

  require_once drupal_get_path('module', $module) . $form_file;
  

  $entity = _entityspice_entity_create($etype, array('bundle' => $bundle));
  return drupal_get_form($form_callback, $entity);
}
