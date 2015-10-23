<?php

function stop_spam_external_links_report() {
  if (empty($_POST)) {
    _stop_spam_external_links_unprocessed();
  }

  $content = array(
    'operations' => drupal_get_form('stop_spam_links_operations_form'),
    'settings' => drupal_get_form('stop_spam_report_settings_form'),
    'report' => _stop_spam_external_links_summary(),
  );
  
  return $content;
}

function stop_spam_links_operations_form($state) {
  $last_updates = array();
  
  $types = array(
    'comment' => t('Comments'),
    'node' => t('Nodes'),
  );
  
  foreach ($types as $type => $title) {
    $last_update = variable_get('stop_spam_links_updated_' . $type, 0);
    
    $last_updates[] = t('!title : !updated', array(
      '!title' => $title,
      '!updated' => $last_update
        ? t('!updated ago', array('!updated' => format_interval(REQUEST_TIME - $last_update)))
        : t('Never'),
    ));
  }

  $content_types = node_type_get_types();
  $indexable_types = variable_get('stop_spam_links_index_types', array());
  
  foreach ($indexable_types as $index => $type) {
    $indexable_types[$index] = isset($content_types[$type]->name)
      ? $content_types[$type]->name
      : t('Broken type (@type)', array('@type' => $type));
  }
  
  $indexable_types = array('_comment' => t('Comment')) + $indexable_types;

  $form = array(
    'index_rebuild' => array(
      '#type' => 'fieldset',
      '#title' => t('Links index'),
      '#description' => t('Index update can take a lot of time and will be performed using batch processing.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,

      'last_update' => array(
        '#type' => 'item',
        '#title' => t('Last indexed content was updated'),
        '#markup' => theme('item_list', array('items' => $last_updates)),
      ),

      'indexable_types' => array(
        '#type' => 'item',
        '#title' => t('Following content types are indexed'),
        '#markup' => theme('item_list', array('items' => $indexable_types)),
        '#description' => t('You can change this list in !settings', array(
          '!settings' => l(t('settings'), 'admin/config/system/stop_spam'),
        )),
      ),

      'actions' => array(
        '#type' => 'actions',
        'clear' => array(
          '#type' => 'submit',
          '#value' => t('Clear'),
          '#submit' => array(
            '_stop_spam_links_operations_form_clear',
          ),
          '#suffix' => ' ',
        ),
        'update' => array(
          '#type' => 'submit',
          '#value' => t('Update'),
          '#submit' => array(
            '_stop_spam_links_operations_form_update',
          ),
          '#suffix' => ' ',
        ),
      ),
    ),
  );

  return $form;
}

function stop_spam_report_settings_form($state) {
  $form = array(
    'report_settings' => array(
      '#type' => 'fieldset',
      '#title' => t('Report settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,

      'stop_spam_report_ignore_hosts' => array(
        '#type' => 'textarea',
        '#title' => t('Hosts to hide in summary'),
        '#default_value' => implode("\n", variable_get('stop_spam_report_ignore_hosts', array())),
      ),

      'stop_spam_report_hosts_per_page' => array(
        '#type' => 'select',
        '#title' => t('Hosts per page'),
        '#options' => drupal_map_assoc(array('10', '25', '50', '100')),
        '#default_value' => variable_get('stop_spam_report_hosts_per_page', '25'),
      ),
    ),
  );

  $form = system_settings_form($form);

  // Move buttons to the fieldset.
  $form['report_settings']['actions'] = $form['actions'];
  unset($form['actions']);

  // Add our pre-submit handler.
  array_unshift($form['#submit'], '_stop_spam_report_settings_form_pre_submit');

  return $form;
}

function _stop_spam_report_settings_form_pre_submit($form, &$state) {
  $hosts = &$state['values']['stop_spam_report_ignore_hosts'];
  $hosts = preg_split('/\s*[\n\r]+\s*/', $hosts, NULL, PREG_SPLIT_NO_EMPTY);
  
  foreach ($hosts as &$host) {
    $host = _stop_spam_remove_www($host);
  }
  
  unset($hosts, $host);
}

function _stop_spam_links_operations_form_clear($form, &$state) {
  if (db_query('TRUNCATE TABLE {stop_spam_links}')) {
    _stop_spam_links_last_update_reset();
    drupal_set_message(t('Links index was cleared.'));
  }
  else {
    drupal_set_message(t('Unable to clear links index!'));
  }
}

function _stop_spam_links_last_update_reset() {
  foreach (array('comment', 'node') as $type) {
    variable_del('stop_spam_links_updated_' . $type);
  }
}

function _stop_spam_links_operations_form_update($form, &$state) {
  if (!lock_acquire('stop_spam_links_index_update')) {
    drupal_set_message(t('Links index update is already in progress.'), 'error');
    return;
  }

  if (!db_query('UPDATE {stop_spam_links} SET is_new = 0')) {
    drupal_set_message(t('Unable to reset "new" flag. Index update was cancelled.'), 'error');
  }

  $operations = array();
  $content_types = variable_get('stop_spam_links_index_types', array());
  
  if (!empty($content_types)) {
    $operations[] = array('_stop_spam_batch_parse_links', array(array(
      'type' => 'node',
      'content_types' => $content_types,
      'limit' => 25,
    )));
  }
  
  $operations[] = array('_stop_spam_batch_parse_links', array(array(
    'type' => 'comment',
    'content_types' => NULL,
    'limit' => 100,
  )));

  $batch = array(
    'title' => t('Updating links index'),
    'operations' => $operations,
    'finished' => '_stop_spam_batch_parse_links_finished',
    'file' => drupal_get_path('module', 'stop_spam') . '/stop_spam.report.inc',
  );
  
  batch_set($batch);
}

/**
 * Parses URLs from entities of passed type.
 *
 * @param array $vars
 *   Keys are:
 *   - type: entity type
 *   - limit: number of entities to process on each iteration
 *   - content_types: array with node types to index, for comments just an empty
 *     array
 * @param array $context
 */
function _stop_spam_batch_parse_links($vars, &$context) {
  // Too smart :) it confuses my IDE.
  extract($vars);

  if (empty($context['sandbox'])) {
    $context['sandbox'] = array(
      'progress' => 0,
      'offset' => 0,
      'current_id' => 0,
      'last_updated' => variable_get('stop_spam_links_updated_' . $type, 0),
    );

    $totals = _stop_spam_batch_count_total($type, $context['sandbox']['last_updated'], $content_types);

    $context['sandbox']['total'] = $totals->rows;
    $context['results'][$type] = array(
      'links' => 0,
      'total' => $totals->entities,
      'last_updated' => $context['sandbox']['last_updated'],
    );

    if (empty($totals->rows)) {
      $context['message'] = t('Nothing to index');
      return;
    }
  }
  
  $sandbox = &$context['sandbox'];

  $offset = &$sandbox['offset'];
  $last_updated = &$sandbox['last_updated'];

  $res = _stop_spam_batch_fetch_next_portion($type, $last_updated, $content_types, $offset, $limit);
  $ids = array();
  $links = array();
  $subfields = _stop_spam_body_subfields($type);
  
  foreach ($res as $entity) {
    _stop_spam_field_parse_links($links, $entity, $subfields);

    $ids[] = $entity->id;
    
    $sandbox['progress']++;
    
    if ($entity->updated != $last_updated) {
      $offset = 1;
      $last_updated = $entity->updated;
    }
    else {
      $offset++;
    }
  }

  // Cleanup links of parsed nodes.
  if (!empty($ids)) {
    db_delete('stop_spam_links')
      ->condition('entity_type', $type)
      ->condition('entity_id', $ids)
      ->execute();
  }

  // Save links.
  if (!empty($links)) {
    $query = db_insert('stop_spam_links');
    $query->fields(array(
      'entity_type',
      'entity_id',
      'link_id',
      'field',
      'host',
      'url',
      'timestamp',
      'is_new',
    ));
    
    foreach ($links as $entity_id => $entity_links) {
      foreach ($entity_links as $link_id => $link) {
        $query->values(array(
          'entity_type' => $type,
          'entity_id' => $entity_id,
          'link_id' => $link_id,
          'field' => $link['field'],
          'host' => $link['host'],
          'url' => $link['url'],
          'timestamp' => $link['updated'],
          'is_new' => 1,
        ));
      }

      $context['results'][$type]['links'] += count($entity_links);
    }
    
    $query->execute();
  }

  if (!empty($ids) && $sandbox['progress'] < $sandbox['total']) {
    $context['finished'] = $sandbox['progress'] / $sandbox['total'];
  }
  else {
    // Start from the next second after the last update time found here if at
    // least one entity was processed.
    if ($sandbox['progress'] > 0) {
      variable_set('stop_spam_links_updated_' . $type, $last_updated + 1);
    }

    $context['finished'] = 1;
  }
}

function _stop_spam_batch_parse_links_finished($success, $results, $operations) {
  $types = array(
    'node' => t('nodes'),
    'comment' => t('comments'),
  );

  if ($success) {
    foreach ($types as $type => $label) {
      $links = !empty($results[$type]['links']) ? $results[$type]['links'] : 0;
      $total = !empty($results[$type]['total']) ? $results[$type]['total'] : 0;
      drupal_set_message(t('Extracted @links links from @total @entities.', array(
        '@links' => $links,
        '@total' => $total,
        '@entities' => $label,
      )));
    }
  }
  else {
    drupal_set_message(t('Finished with an error.'), 'error');
  }
}

function _stop_spam_batch_count_total($type, $last_updated, $content_types = NULL) {
  //@TODO: remove hard-coding of fields, use field API to get them.
  switch ($type) {
    case 'comment':
      $id = 't.cid';
      $field_table = 'field_data_comment_body';
      $on = 't.cid = ft.entity_id';
      break;

    default:
      $id = 't.nid';
      $field_table = 'field_data_body';
      $on = 't.nid = ft.entity_id AND t.vid = ft.revision_id';
      break;
  }

  $args = array();
  $sql  = "SELECT COUNT(DISTINCT $id) as entities, COUNT(*) as rows";
  $sql .= " FROM {{$type}} t";
  $sql .= " INNER JOIN {{$field_table}} ft ON $on AND ft.entity_type = :type AND ft.deleted = 0";
  $sql .= " WHERE t.changed >= :last_updated";
  
  if (!empty($content_types)) {
    $sql .= " AND t.type IN(:content_types)";
    $args += array(
      ':content_types' => $content_types,
    );
  }
  
  $args += array(
    ':type' => $type,
    ':last_updated' => $last_updated,
  );

  $result = db_query($sql, $args);
  
  return $result->fetchObject();
}

function _stop_spam_batch_fetch_next_portion($type, $last_updated, $content_types, $offset, $limit) {
  switch ($type) {
    case 'comment':
      $id = 't.cid';
      $field = 'comment_body';
      $on = 't.cid = ft.entity_id';
      break;
    
    default:
      $id = 't.nid';
      $field = 'body';
      $on = 't.nid = ft.entity_id AND t.vid = ft.revision_id';
      break;
  }
  
  $args = array();
  $sql  = "SELECT $id as id, t.changed as updated, ft.{$field}_format as format";
  
  foreach (_stop_spam_body_subfields($type) as $subfield) {
    $sql .= ", ft.{$field}_{$subfield} as $subfield";
  }
  
  $sql .= " FROM {{$type}} t";
  $sql .= " INNER JOIN {field_data_{$field}} ft ON $on AND ft.entity_type = :type AND ft.deleted = 0";
  $sql .= " WHERE changed >= :last_updated";
  
  if (!empty($content_types)) {
    $sql .= " AND t.type IN(:content_types)";
    $args += array(
      ':content_types' => $content_types,
    );
  }
  
  $sql .= " ORDER BY t.changed ASC";
  
  $args += array(
    ':type' => $type,
    ':last_updated' => $last_updated,
  );
  
  $results = db_query_range($sql, $offset, $limit, $args);
  
  return $results;
}

function _stop_spam_field_parse_links(&$links, $entity, $subfields) {
  foreach ($subfields as $subfield) {
    $content = $entity->{$subfield};
    
    if (empty($content)) {
      continue;
    }

    $content = check_markup($content, $entity->format);
    $tags = _stop_spam_extract_attributes($content, array('a' => 'href', 'img' => 'src'));
    $current_host = _stop_spam_current_host();
    
    foreach ($tags as $tag => $urls) {
      foreach ($urls as $url) {
        $host = @parse_url($url, PHP_URL_HOST);
        
        if (!empty($host) && $current_host !== FALSE) {
          $host = _stop_spam_remove_www($host);
          
          if ($host !== $current_host) {
            $links[$entity->id][] = array(
              'field' => 'body.' . $subfield,
              'host' => $host,
              'url' => $url,
              'updated' => $entity->updated,
            );
          }
        }
      }
    }
  }
}

function _stop_spam_external_links_unprocessed() {
  $node_types = variable_get('stop_spam_links_index_types', array());

  //@TODO: remove hard-coding of fields.
  $sql  = '(SELECT COUNT(DISTINCT c.cid) as count, :comment as type';
  $sql .= ' FROM {comment} c';
  $sql .= ' INNER JOIN {field_data_comment_body} fc ON fc.entity_type = :comment AND fc.entity_id = c.cid AND fc.deleted = 0';
  $sql .= ' WHERE c.status != 0 AND c.changed >= :updated_comment)';
  $args = array(
    ':comment' => 'comment',
    ':updated_comment' => variable_get('stop_spam_links_updated_comment', 0),
  );

  if (!empty($node_types)) {
    $sql .= ' UNION ALL ';
    $sql .= '(SELECT COUNT(DISTINCT n.nid) as count, :node as type';
    $sql .= ' FROM {node} n';
    $sql .= ' INNER JOIN {field_data_body} fn ON fn.entity_type = :node AND fn.entity_id = n.nid AND fn.revision_id = n.vid AND fn.deleted = 0';
    $sql .= ' WHERE n.status != 0 AND n.type IN (:node_types) AND n.changed >= :updated_node)';
    $args += array(
      ':node' => 'node',
      ':updated_node' => variable_get('stop_spam_links_updated_node', 0),
      ':node_types' => $node_types,
    );
  }

  $results = db_query($sql, $args);
  foreach ($results as $row) {
    if (!empty($row->count)) {
      switch ($row->type) {
        case 'node':
          $title = format_plural($row->count, '1 node', '@count nodes');
          break;

        case 'comment':
          $title = format_plural($row->count, '1 comment', '@count comments');
          break;
      }

      drupal_set_message(t('!count left unprocessed.', array('!count' => $title)), 'warning', FALSE);
    }
  }
}

function _stop_spam_external_links_summary() {
  $ignored_hosts = variable_get('stop_spam_report_ignore_hosts', array());

  $header = array(
    array(
      'data' => t('Host'),
      'field' => 'l.host',
    ),
    array(
      'data' => t('Links in Nodes'),
      'field' => 'nodes',
    ),
    array(
      'data' => t('Links in Comments'),
      'field' => 'comments',
    ),
    array(
      'data' => t('Last update'),
      'field' => 'last_update',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Fresh'),
      'field' => 'has_new',
    ),
  );

  $query = db_select('stop_spam_links', 'l');
  $query->addField('l', 'host');
  $query->addExpression('COUNT(c.cid)', 'comments');
  $query->addExpression('COUNT(n.nid)', 'nodes');
  $query->addExpression('MAX(l.timestamp)', 'last_update');
  $query->addExpression('MAX(l.is_new)', 'has_new');
  $query->leftJoin('comment', 'c', 'l.entity_type = :comment AND c.cid = l.entity_id', array(':comment' => 'comment'));
  $query->leftJoin('node', 'n', 'l.entity_type = :node AND n.nid = l.entity_id', array(':node' => 'node'));
  $query->condition(
    db_or()
      ->condition('n.status', 0, '!=')
      ->isNull('n.nid')
  );
  $query->condition(
    db_or()
      ->condition('c.status', 0, '!=')
      ->isNull('c.cid')
  );

  $count_query = db_select('stop_spam_links', 'l');
  $count_query->addExpression('COUNT(DISTINCT l.host)', 'hosts');

  if (!empty($ignored_hosts)) {
    $query->condition('l.host', $ignored_hosts, 'NOT IN');
    $count_query->condition('l.host', $ignored_hosts, 'NOT IN');
  }

  $query->groupBy('l.host');

  $per_page = variable_get('stop_spam_report_hosts_per_page', '25');
  $query = $query->extend('PagerDefault');
  $query->limit($per_page);
  $query->setCountQuery($count_query);

  $query = $query->extend('TableSort');
  $query->orderByHeader($header);

  $results = $query->execute();
  $rows = array();
  
  foreach ($results as $host) {
    $rows[] = array(
      l($host->host, 'http://' . $host->host, array(
        'attributes' => array(
          'class' => 'external-link',
          'target' => '_blank',
        ),
      )),
      $host->nodes ? l($host->nodes, 'admin/reports/stop_spam/nodes/' . $host->host, array('html' => TRUE)) : '0',
      $host->comments ? l($host->comments, 'admin/reports/stop_spam/comments/' . $host->host, array('html' => TRUE)) : '0',
      format_date(!empty($host->last_update) ? $host->last_update : 0, 'custom', 'd M Y H:i'),
      !empty($host->has_new) ? t('Yes') : t('No'),
    );
  }

  $output = array(
    'table' => array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    ),
    'pager' => array(
      '#theme' => 'pager',
    ),
  );

  return $output;
}
