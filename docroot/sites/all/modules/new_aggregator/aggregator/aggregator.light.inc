<?php
// $Id: aggregator.light.inc,v 1.1 2008/07/21 12:15:44 aronnovak Exp $
/**
 * @file
 *   Aggregator Light processor related functions.
 */

/**
 * Implementation of hook_block().
 *
 * Generates blocks for the latest news items in each category.
 */
function aggregator_block($op = 'list', $delta = '', $edit = array()) {
  if ($op == 'list') {
    $block = array();
    $block['aggregator-latest']['info'] = t('Latest aggregated light feed items');
    $terms = _aggregator_collect_terms();
    foreach ($terms as $tid => $name) {
      $block['aggregator-' . $tid]['info'] = t('Latest items with !term  term', array('!term' => $name));
    }
    return $block;
  }
  else if ($op == 'configure') {
    $value = variable_get($delta . '-block', 5);
    $form['block'] = array('#type' => 'select', '#title' => t('Number of news items in block'), '#default_value' => $value, '#options' => drupal_map_assoc(array(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20)));
    return $form;
  }
  else if ($op == 'save') {
    variable_set($delta . '-block', $edit['block']);
  }
  else if ($op == 'view') {
    if (user_access('access content')) {
      $tid = array_pop(explode('-', $delta));
      $query_args = array();
      if ($tid == 'latest') {
        $title = t('Latest items');
        $query = "SELECT title, link FROM {aggregator_item} ORDER BY timestamp DESC";
      }
      else if(is_numeric($tid)) {
        $term = taxonomy_get_term($tid);
        $title = t('Latest items with !term term', array('!term' => $term->name));
        $query = "SELECT i.title, i.link FROM {aggregator_item} i LEFT JOIN {term_node} t ON t.nid = i.nid WHERE t.tid = %d ORDER BY timestamp DESC";
        $query_args = array($tid);
      }
      $result = db_query_range($query, $query_args, 0, variable_get($delta . '-block', 5));
      $items = array();
      while ($item = db_fetch_object($result)) {
        $items[] = theme('aggregator_block_item', $item);
      }
      $block['subject'] = check_markup($title, 4);
      $block['content'] = theme('item_list', $items);
      return $block;
    }
  }
}

/**
 * Implementation of hook_aggregator_process().
 * 
 * @param $op
 *   'save' The feed items should be updated or saved.
 *   'info' Metadata about the processor
 * @param $node
 *   The feed-node object.
 */
function aggregator_aggregator_process($op, $node = NULL) {
  switch ($op) {
    case 'save':
      if (is_array($node->feed->items)) {
        $age = variable_get('aggregator_clear_' . $node->type, 9676800);
        $new_items = 0;
        foreach ($node->feed->items as $item) {
          // Avoid to create already expired items
          if ($item->unique === TRUE) {
            if (time() - $item->timestamp < $age) {
              db_query("INSERT INTO {aggregator_item} (nid, title, link, author, description, timestamp, guid) VALUES (%d, '%s', '%s', '%s', '%s', %d, '%s')", $node->nid, $item->title, $item->link, '', $item->description, $item->timestamp, $item->guid);
              $new_items++;
            }
          }
          else {
            db_query("UPDATE {aggregator_item} SET title = '%s', author = '%s', description = '%s', timestamp = %d, guid = '%s' WHERE iid = '%s'", $item->title, '', $item->description, $item->timestamp, $item->guid, $item->unique);
          }
        }
        return $new_items;
      }
      break;
    case 'unique':
      foreach ($node->feed->items as $k => $item) {
        if (!empty($item->guid) && strlen($item->guid) < 256) {
          $iid = db_result(db_query("SELECT iid FROM {aggregator_item} WHERE nid = %d AND guid = '%s'", $node->nid, $item->guid));
        }
        else if ($item->link && $item->link != $node->feed->link && $item->link != $node->feed->link) {
          $iid = db_result(db_query("SELECT iid FROM {aggregator_item} WHERE nid = %d AND link = '%s'", $node->nid, $item->link));
        }
        else {
          $iid = db_result(db_query("SELECT iid FROM {aggregator_item} WHERE nid = %d AND title = '%s'", $node->nid, $item->title));
        }
        $node->feed->items[$k]->unique = ($iid == FALSE ? TRUE : $iid);
      }
      break;
    case 'info':
      return array(
      'title' => t('Aggregator Light'),
      'description' => t('Provides lightweight item processor for Aggregator.'),
      );
  }
}

/**
 * Per-content-type settings for Aggregator Light processor.
 */
function _aggregator_light_form_settings(&$form) {
  $type = $form['type']['#value'];
  $info = module_invoke('aggregator', 'aggregator_process', 'info');
  $form['modules']['aggregator'] = array(
    '#type' => 'fieldset',
    '#title' => t('Aggregator Light processor settings'),
    '#description' => $info['description'],
    '#collapsible' => TRUE,
    '#collapsed' => !aggregator_is_enabled('aggregator', $type),
  );
  $vocabs = taxonomy_get_vocabularies();
  $vids = array();
  $vids[0] = t('None');
  foreach ($vocabs as $vid => $vocab) {
    $vids[$vid] = $vocab->name;
  }
  $period = drupal_map_assoc(array(3600, 10800, 21600, 32400, 43200, 86400, 172800, 259200, 604800, 1209600, 2419200, 4838400, 9676800), 'format_interval');
  $form['modules']['aggregator']['aggregator_vid_' . $type] = array(
    '#type' => 'select',
    '#title' => t('Vocabulary'),
    '#multiple' => FALSE,
    '#options' => $vids,
    '#description' => t('Select a vocabulary for categorizing feed items.'),
    '#default_value' => variable_get('aggregator_vid_' . $type, 0),
  );
  $form['modules']['aggregator']['aggregator_clear_' . $type] = array(
    '#type' => 'select',
    '#title' => t('Discard items older than'),
    '#default_value' => variable_get('aggregator_clear_' . $type, 9676800),
    '#options' => $period,
    '#description' => t('The length of time to retain feed items before discarding. (Requires a correctly configured <a href="@cron">cron maintenance task</a>.)', array('@cron' => url('admin/reports/status'))),
  );
  $formats = filter_formats();
  foreach ($formats as $k => $format) {
    $formats[$k] = $format->name;
  }
  $form['modules']['aggregator']['aggregator_input_filter_' . $type] = array(
    '#type' => 'select',
    '#title' => t('Input format for item description'),
    '#default_value' => variable_get('aggregator_input_filter_' . $type, FILTER_FORMAT_DEFAULT),
    '#options' => $formats,
    '#description' => t('You can specify here the allowed elements in the feed items.'),
  );
  $items = array(0 => t('none')) + drupal_map_assoc(array(3, 5, 10, 15, 20, 25), '_aggregator_items');
  $form['modules']['aggregator']['aggregator_summary_items_' . $type] = array(
    '#type' => 'select',
    '#title' => t('Items shown in sources and categories pages') ,
    '#default_value' => variable_get('aggregator_summary_items_' . $type, 3),
    '#options' => $items,
    '#description' => t('Number of feed items displayed in feed and category summary pages.'),
  );
}

/**
 * Delete expired items.
 */
function _aggregator_light_delete_expired() {
  $types = node_get_types();
  // Delete expired light items.
  foreach ($types as $type) {
    if (variable_get('aggregator_feed_' . $type->type, FALSE) && aggregator_is_enabled('aggregator', $type->type)) {
      $result = db_query("SELECT l.iid FROM {aggregator_item} l LEFT JOIN {node} n ON l.nid = n.nid WHERE n.type = '%s'", $type->type);
      $iids = array();
      while ($item = db_fetch_array($result)) {
        $iids[] = $item['iid'];
      }
      if (count($iids) > 0) {
        $age = variable_get('aggregator_clear_' . $type->type, 9676800);
        db_query('DELETE FROM {aggregator_item} WHERE iid IN (' . implode(', ', $iids) . ') AND %d - timestamp > %d', time(), $age);
      }
    }
  }
}
