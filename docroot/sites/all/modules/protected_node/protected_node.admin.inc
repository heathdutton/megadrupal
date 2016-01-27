<?php

/**
 * @file
 * Protected node overview page.
 */

/**
 * List all protected content.
 *
 * This is copied quite liberally from node_admin_nodes().
 */
function protected_node_content_list() {
  // Enable language column if entity translation module is enabled or if we
  // have any node with language.
  $multilanguage = (module_exists('translation') || module_exists('entity_translation') || db_query_range("SELECT 1 FROM {node} WHERE language <> :language", 0, 1, array(':language' => LANGUAGE_NONE))->fetchField());

  // Build the sortable table header.
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'n.title'),
    'type' => array('data' => t('Type'), 'field' => 'n.type'),
    'author' => t('Author'),
    'status' => array('data' => t('Status'), 'field' => 'n.status'),
    'changed' => array(
      'data' => t('Updated'),
      'field' => 'n.changed',
      'sort' => 'desc',
    ),
  );
  if ($multilanguage) {
    $header['language'] = array('data' => t('Language'), 'field' => 'n.language');
  }
  $header['operations'] = array('data' => t('Operations'));

  $query = db_select('node', 'n')->extend('PagerDefault')->extend('TableSort');

  // Only list content that has been protected.
  $query->join('protected_nodes', 'p', 'n.nid = p.nid');
  $query->condition('p.protected_node_is_protected', 1);

  if (!user_access('bypass node access')) {
    // If the user is able to view their own unpublished nodes, allow them
    // to see these in addition to published nodes. Check that they actually
    // have some unpublished nodes to view before adding the condition.
    if (user_access('view own unpublished content') && $own_unpublished = db_query('SELECT nid FROM {node} WHERE uid = :uid AND status = :status', array(':uid' => $GLOBALS['user']->uid, ':status' => 0))->fetchCol()) {
      $query->condition(db_or()
        ->condition('n.status', 1)
        ->condition('n.nid', $own_unpublished, 'IN')
      );
    }
    else {
      // If not, restrict the query to published nodes.
      $query->condition('n.status', 1);
    }
  }
  $nids = $query
    ->fields('n', array('nid'))
    ->limit(50)
    ->orderByHeader($header)
    ->addTag('node_access')
    ->execute()
    ->fetchCol();
  $nodes = node_load_multiple($nids);

  // Prepare the list of nodes.
  $languages = language_list();
  $destination = drupal_get_destination();
  $options = array();
  foreach ($nodes as $node) {
    $langcode = entity_language('node', $node);
    $l_options = $langcode != LANGUAGE_NONE && isset($languages[$langcode]) ? array('language' => $languages[$langcode]) : array();
    $options[$node->nid] = array(
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $node->title,
          '#href' => 'node/' . $node->nid,
          '#options' => $l_options,
          '#suffix' => ' ' . theme('mark', array('type' => node_mark($node->nid, $node->changed))),
        ),
      ),
      'type' => check_plain(node_type_get_name($node)),
      'author' => theme('username', array('account' => $node)),
      'status' => $node->status ? t('published') : t('not published'),
      'changed' => format_date($node->changed, 'short'),
    );
    if ($multilanguage) {
      if ($langcode == LANGUAGE_NONE || isset($languages[$langcode])) {
        $options[$node->nid]['language'] = $langcode == LANGUAGE_NONE ? t('Language neutral') : $languages[$langcode]->name;
      }
      else {
        $options[$node->nid]['language'] = t('Undefined language (@langcode)', array('@langcode' => $langcode));
      }
    }
    // Build a list of all the accessible operations for the current node.
    $operations = array();
    if (node_access('update', $node)) {
      $operations['edit'] = array(
        'title' => t('edit'),
        'href' => 'node/' . $node->nid . '/edit',
        'query' => $destination,
      );
    }
    if (node_access('delete', $node)) {
      $operations['delete'] = array(
        'title' => t('delete'),
        'href' => 'node/' . $node->nid . '/delete',
        'query' => $destination,
      );
    }
    $options[$node->nid]['operations'] = array();
    if (count($operations) > 1) {
      // Render an unordered list of operations links.
      $options[$node->nid]['operations'] = array(
        'data' => array(
          '#theme' => 'links__node_operations',
          '#links' => $operations,
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
    }
    elseif (!empty($operations)) {
      // Render the first and only operation as a link.
      $link = reset($operations);
      $options[$node->nid]['operations'] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => $link['title'],
          '#href' => $link['href'],
          '#options' => array('query' => $link['query']),
        ),
      );
    }
  }

  $form['nodes'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
    '#empty' => t('No protected content available.'),
  );

  $form['pager'] = array('#markup' => theme('pager'));
  return $form;
}
