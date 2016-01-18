<?php

/**
 * Menu callback: confirm rebuilding of relations.
 */
function menu_link_node_menu_admin_populate_confirm() {
  return confirm_form(array(), t('Are you sure you want to populate the menu link field of nodes?'),
                  'admin/reports/status', t('This action populates the default menu link field on nodes with existing menu links, and may be a lengthy process.'), t('Populate'), t('Cancel'));
}

/**
 * Submit handler that populates the default menu link field on nodes with existing menu links.
 */
function menu_link_node_menu_admin_populate_confirm_submit($form, &$form_state) {
  $operations = array();
  foreach (node_type_get_types() as $info) {
    $instance = field_read_instance('node', MENU_LINK_DEFAULT_FIELD, $info->type);
    if (!empty($instance)) {
      $operations[] = array('menu_link_node_menu_admin_populate_batch_operation', array($instance));
    }
  }

  if (!empty($operations)) {
    $batch = array(
      'title' => t('Populating the ":menu_link_field" field.', array(':menu_link_field' => MENU_LINK_DEFAULT_FIELD)),
      'file' => drupal_get_path('module', 'menu_link_node_menu') . '/menu_link_node_menu.admin.inc',
      'operations' => $operations,
    );
    batch_set($batch);
  }
  $form_state['redirect'] = 'admin/reports/status';
}

/**
 * Batch operation: seize and store menu links of nodes in the MENU_LINK_DEFAULT_FIELD.
 *
 * This is a multistep operation, iterating all nodes with a menu link by
 * packs of 20.
 */
function menu_link_node_menu_admin_populate_batch_operation($instance, &$context) {
  $default_menu = !empty($instance['default value'][0]['menu_name']) ? $instance['default value'][0]['menu_name'] : 'main-menu';

  $query = db_select('menu_links', 'ml');
  $query->fields('ml', array('mlid', 'menu_name', 'plid', 'link_path', 'link_title', 'options', 'hidden', 'expanded', 'weight'));
  $query->addExpression('ml.menu_name = :default_menu', 'default_menu', array(':default_menu' => $default_menu));
  $query->addField('n', 'nid');
  $query->innerJoin('node', 'n', "ml.link_path = CONCAT('node/', n.nid)");
  $query->condition('ml.module', 'menu');
  $query->condition('ml.menu_name', $instance['settings']['menu_options']);
  $query->condition('ml.router_path', 'node/%');
  $query->condition('n.type', $instance['bundle']);
  $query->groupBy('ml.link_path');
  $query->orderBy('default_menu', 'DESC');
  $query->orderBy('ml.mlid');

  if (empty($context['sandbox'])) {
    // Initiate multistep processing.
    $context['sandbox']['count'] = 0;
    $context['sandbox']['current'] = 0;
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = $query->countQuery()->execute()->fetchField();
  }

  // Process the next 20 menu links/nodes.
  $limit = 20;
  $menu_links = $query
    ->condition('ml.mlid', $context['sandbox']['current'], '>')
    ->range(0, $limit)
    ->execute()->fetchAllAssoc('nid', PDO::FETCH_ASSOC);

  $nodes = node_load_multiple(array_keys($menu_links));
  foreach ($nodes as $nid => $node) {
    // To preserve database integrity, only populate the menu link field if the
    // node loads successfully.
    if (!empty($node) && empty($node->{MENU_LINK_DEFAULT_FIELD}[LANGUAGE_NONE])) {
      $menu_links[$nid]['options'] = unserialize($menu_links[$nid]['options']);
      $node->{MENU_LINK_DEFAULT_FIELD}[LANGUAGE_NONE][0] = $menu_links[$nid];

      node_save($node);
      $context['sandbox']['count']++;
    }
    $context['sandbox']['current'] = $menu_links[$nid]['mlid'];
  }
  $context['sandbox']['progress'] += $limit;

  // Multistep processing: report progress.
  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  else {
    $types = node_type_get_names();
    drupal_set_message(t('Populated the ":menu_link_field" field of :count posts of type :node_type.', array(':menu_link_field' => MENU_LINK_DEFAULT_FIELD, ':count' => $context['sandbox']['count'], ':node_type' => $types[$instance['bundle']])));
  }
}
