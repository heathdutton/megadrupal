<?php

/**
 * @file
 * Chart display for Smart Chart.
 */

/**
 * Show the chart as form.
 */
function smart_chart_form($form, &$form_state, $item) {

  global $user;

  // Load functions.
  $file_path = module_load_include('inc', 'smart_chart', 'includes/smart_chart.functions');

  $form = array();

  // Add ctools ajax libraries.
  ctools_include('modal');
  ctools_include('ajax');
  ctools_modal_add_js();

  // Set page title.
  drupal_set_title($item->title);

  // Chart nid.
  $chart_nid = $item->nid;

  $form['#attributes']['id'] = 'chart-item-form';
  $form['#attributes']['class'][] = 'chart-item-form';

  // Main refresh button, hidden.
  $form['chart_container_' . $chart_nid] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'chart_container_' . $chart_nid,
      'class' => array('clearfix'),
    ),
  );
  $form['chart_container_' . $chart_nid]['actions'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('chart-actions' , 'chart-actions-main'),
    ),
  );
  $form['chart_container_' . $chart_nid]['actions']['chart_button_refresh_' . $chart_nid] = array(
    '#type' => 'button',
    '#value' => t('Refresh'),
    '#name' => 'chart_button_refresh_' . $chart_nid,
    '#attributes' => array(
      'class' => array('chart-button',
        'refresh', 'refresh_' . $chart_nid, 'hidden'),
    ),
  );

  // Set actions according to children.
  if (sc_item_has_children($chart_nid)) {
    // No actions at the moment.
  }
  else {
    // New button. Check user has permission.
    if (user_access('new smart chart item', $user)) {
      $href = 'chart/new/nojs/' . $chart_nid . '/' . $chart_nid;
      $form['chart_container_' . $chart_nid]['actions']['chart_button_new_' . $chart_nid]
        = _smart_chart_create_button('new', t('New'), t('New'), $href, '', $chart_nid, $chart_nid);
    }

    // Add button. Check user has permission.
    if (user_access('add smart chart item', $user)) {
      $href = 'chart/select/nojs/' . $chart_nid . '/' . $chart_nid;
      $form['chart_container_' . $chart_nid]['actions']['chart_button_add_' . $chart_nid]
        = _smart_chart_create_button('add', t('Add'), t('Add'), $href, '', $chart_nid, $chart_nid);
    }
  }

  // Get initial item.
  $first_nid = _smart_chart_get_first_item($chart_nid);

  if ($first_nid) {
    // Create opened items array if it does not exist.
    $form_state['opened_items'] = !isset($form_state['opened_items']) ?
      array($first_nid) : $form_state['opened_items'];

    if (isset($form_state['triggering_element']['#name'])) {
      $value = $form_state['triggering_element']['#name'];
      $value = str_replace('chart_button_expand_', '', $value);

      // Check that value exists in array and revove it.
      if (in_array($value, $form_state['opened_items'])) {
        $form_state['opened_items'] = array_diff($form_state['opened_items'], array($value));
      }
      else {
        $form_state['opened_items'][] = $value;
      }
    }
    // Get chart item for first step.
    $form = _smart_chart_get($form, $form_state, $first_nid, $chart_nid);
  }

  return $form;
}

/**
 * Get first item.
 *
 * @param $chart_nid
 *   Initial Chart nid.
 *
 * @return int
 *   Item nid.
 */
function _smart_chart_get_first_item($chart_nid) {
  // Find children of current item.
  $query = db_select('node', 'n')
    ->fields('n', array('nid'));
  $query->join('field_data_field_sc_parent', 'parent', 'parent.field_sc_parent_target_id = ' . $chart_nid);
  $query->join('field_data_field_sc_child', 'child', 'n.nid = child.field_sc_child_target_id and child.entity_id = parent.entity_id');

  // Creating hook to alter query.
  $query_altered = module_invoke_all('sc_first_item_query_alter', $chart_nid, $query);
  if ($query_altered) {
    $query = $query_altered;
  }

  $result = $query->execute()->fetchAssoc();

  return isset($result['nid']) ? $result['nid'] : FALSE;
}

/**
 * Get chart item structure.
 *
 * @param $form
 * @param $form_state
 * @param $item_nid
 *   Current item nid.
 * @param $chart_nid
 *   Initial chart nid.
 * @param $level
 *   Level of item.
 *
 * @return array
 *   Form.
 */
function _smart_chart_get($form, &$form_state, $item_nid, $chart_nid, $level = 1) {

  global $user;
  $item = node_load($item_nid);

  // Creating hook check access for item.
  $access = module_invoke_all('sc_item_access', $item, $user->uid);
  $access = is_bool($access) ? $access : TRUE;
  if (!$access) {
    return $form;
  }

  // Main container.
  $form['chart_container_' . $item_nid] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'chart_container_' . $item_nid,
      'class' => array('clearfix', 'chart-container'),
      'title' => $item->title,
    ),
  );

  // Set class according to level.
  if ($level == 1) {
    $form['chart_container_' . $item_nid]['#attributes']['class'][] = 'first';
  }
  elseif ($level > 1) {
    $form['chart_container_' . $item_nid]['#attributes']['class'][] = 'not-first';
  }

  // Level container.
  $form['chart_container_' . $item_nid]['level'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('chart-level'),
    ),
  );
  $form['chart_container_' . $item_nid]['level']['text'] = array(
    '#type' => 'markup',
    '#markup' => $level,
  );

  // Content containers.
  $form['chart_container_' . $item_nid]['chart_item_' . $item_nid] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('chart-content'),
      'id' => 'chart_item_' . $item_nid,
    ),
  );

  // Other buttons container.
  $form['chart_container_' . $item_nid]['actions'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('chart-actions'),
    ),
  );

  // If refresh button is clicked, clear cache for this item.
  // We need to do this before checking for cached content bellow.
  if (isset($form_state['clicked_button']['#name']) && strpos($form_state['clicked_button']['#name'], 'chart_button_refresh_') !== FALSE) {
    // Get id.
    $current_id = str_replace('chart_button_refresh_', '', $form_state['clicked_button']['#name']);
    // Clear cache for this nid.
    _smart_chart_clear_item_cache($current_id);
  }

  // Before recreating item, check cache.
  if ($cache = cache_get('smart_chart_item_' . $item_nid)) {
    $form['chart_container_' . $item_nid]['chart_item_' . $item_nid] = $cache->data;
  }
  else {

    $form['chart_container_' . $item_nid]['chart_item_' . $item_nid]['content'] = array(
      '#type' => 'markup',
      '#markup' => $item->title,
    );

    // Creating hook to alter contents of the item.
    $item_contents = module_invoke_all('sc_item_alter', $item_nid, $form['chart_container_' . $item_nid]['chart_item_' . $item_nid]);
    if ($item_contents) {
      $form['chart_container_' . $item_nid]['chart_item_' . $item_nid] = $item_contents;
    }
    // Save to cache.
    cache_set('smart_chart_item_' . $item_nid, $form['chart_container_' . $item_nid]['chart_item_' . $item_nid],
      'cache', 60 * 60);
  }

  // Refresh button, hidden.
  $form['chart_container_' . $item_nid]['actions']['chart_button_refresh_' . $item_nid] = array(
    '#type' => 'button',
    '#value' => t('Refresh'),
    '#name' => 'chart_button_refresh_' . $item_nid,
    '#ajax' => array(
      'callback' => '_smart_chart_refresh_item',
      'wrapper' => 'chart_container_' . $item_nid,
      'event' => 'click',
      'method' => 'replace',
    ),
    '#attributes' => array(
      'class' => array(
        'chart-button',
        'refresh',
        'refresh_' . $item_nid,
        'hidden',
      ),
    ),
  );

  // New button. Check user has permission.
  if (user_access('new smart chart item', $user)) {
    $href = 'chart/new/nojs/' . $chart_nid . '/' . $item_nid;
    $form['chart_container_' . $item_nid]['actions']['chart_button_new_' . $item_nid]
      = _smart_chart_create_button('new', t('New'), t('New'), $href, 'hidden', $chart_nid, $item_nid);
  }

  // Add button. Check user has permission.
  if (user_access('add smart chart item', $user)) {
    $href = 'chart/select/nojs/' . $chart_nid . '/' . $item_nid;
    $form['chart_container_' . $item_nid]['actions']['chart_button_add_' . $item_nid]
      = _smart_chart_create_button('add', t('Add'), t('Add'), $href, 'hidden', $chart_nid, $item_nid);
  }

  // Break button. Check user has permission.
  if (user_access('break smart chart item', $user)) {
    $href = 'chart/confirm/nojs/break/' . $chart_nid . '/' . $item_nid;
    $form['chart_container_' . $item_nid]['actions']['chart_button_break_' . $item_nid]
      = _smart_chart_create_button('break', t('Break'), t('Break'), $href, 'hidden', $chart_nid, $item_nid);
  }

  // Delete button. Check user has permission.
  if (user_access('delete smart chart item', $user)) {
    $href = 'chart/confirm/nojs/delete/' . $chart_nid . '/' . $item_nid;
    $form['chart_container_' . $item_nid]['actions']['chart_button_delete_' . $item_nid]
      = _smart_chart_create_button('delete', t('Delete'), t('Delete'), $href, 'hidden', $chart_nid, $item_nid);
  }

  // Creating hook to alter actions.
  $item_actions = module_invoke_all('sc_actions_alter', $item_nid, $form['chart_container_' . $item_nid]['actions']);
  if ($item_actions) {
    $form['chart_container_' . $item_nid]['actions'] = $item_actions;
  }

  // Display the bellow only if item has children.
  if (sc_item_has_children($item_nid)) {
    $form['chart_container_' . $item_nid]['actions']['chart_button_expand_' . $item_nid] = array(
      '#type' => 'button',
      '#value' => '...',
      '#name' => 'chart_button_expand_' . $item_nid,
      '#ajax' => array(
        'callback' => '_smart_chart_expand_item',
        'wrapper' => 'chart_item_children_' . $item_nid,
        'event' => 'click',
        'method' => 'replace',
      ),
      '#attributes' => array(
        'class' => array('chart-button', 'expand', 'left'),
      ),
      '#weight' => 0,
    );

    $form['chart_container_' . $item_nid]['chart_children_' . $item_nid] = array(
      '#type' => 'container',
      '#attributes' => array(
        'id' => 'chart_item_children_' . $item_nid,
        'class' => array('clearfix', 'chart-children'),
      ),
    );

    if (in_array($item_nid, $form_state['opened_items'])) {
      $form['chart_container_' . $item_nid]['chart_children_' . $item_nid]
        = _smart_chart_children($form['chart_container_' . $item_nid]['chart_children_' . $item_nid],
             $form_state, $item_nid, $chart_nid, $level + 1);
    }
  }

  return $form;
}

/**
 * Get Children.
 *
 * @param $form
 * @param $form_state
 * @param $parent_nid
 *   Parent nid.
 * @param $chart_nid
 *   Smart Chart nid.
 * @param $level
 *   Current level of item.
 *
 * @return array
 *   Form items.
 */
function _smart_chart_children($form, &$form_state, $parent_nid, $chart_nid, $level) {

  // Find children of current item.
  $query = db_select('node', 'n')
    ->fields('n', array('nid'));
  $query->join('field_data_field_sc_parent', 'parent', 'parent.field_sc_parent_target_id = ' . $parent_nid);
  $query->join('field_data_field_sc_child', 'child', 'n.nid = child.field_sc_child_target_id and child.entity_id = parent.entity_id');

  // Creating hook to alter query.
  $query_altered = module_invoke_all('sc_children_query_alter', $parent_nid, $query);
  if ($query_altered) {
    $query = $query_altered;
  }

  $result = $query->execute()->fetchAll();

  if (count($result)) {
    foreach ($result as $item) {
      $form = _smart_chart_get($form, $form_state, $item->nid, $chart_nid, $level);
    }
    return $form;
  }
}

/**
 * Open children of container.
 *
 * @param $form
 * @param $form_state
 *
 * @return array
 *   Form item that changed.
 */
function _smart_chart_expand_item($form, &$form_state) {

  $temp_form = &$form;

  // Get array path.
  if (isset($form_state['clicked_button']['#array_parents'])) {
    $parents_count = count($form_state['clicked_button']['#array_parents']);
    $step = 1;
    foreach ($form_state['clicked_button']['#array_parents'] as $array_parent) {
      if ($step > $parents_count - 2) {
        break;

      }
      $temp_form = &$temp_form[$array_parent];
      $step += 1;
    }

    // Get id.
    $current_id = str_replace('chart_button_expand_', '', $form_state['clicked_button']['#name']);
    return $temp_form['chart_children_' . $current_id];
  }
}

/**
 * Create button.
 *
 * @param $action
 *   Action that button does.
 * @param $title
 *   Value to display on button
 * @param $tooltip
 *   Tooltip on hover.
 * @param href
 *   Link.
 * @param class
 *   Extra css classes.
 * @param $chart_nid
 *   Current chart nid.
 * @param $item_nid
 *   Current item nid.
 *
 * @return array
 *   Button.
 */
function _smart_chart_create_button($action, $title, $tooltip, $href, $class, $chart_nid, $item_nid) {
  $button = ctools_modal_text_button($title, $href, $tooltip, 'ctools-modal-common-modal-style chart-button ' . $action . ' ' . $class);

  $button = array(
    '#type' => 'item',
    '#markup' => $button,
  );
  return $button;
}

/**
 * Refreshes current container.
 *
 * @param $form
 * @param $form_state
 *
 * @return array
 *   Form item that changed.
 */
function _smart_chart_refresh_item($form, &$form_state) {

  $temp_form = &$form;

  // Get array path.
  if (isset($form_state['clicked_button']['#array_parents'])) {
    // Get id.
    $current_id = str_replace('chart_button_refresh_', '', $form_state['clicked_button']['#name']);

    $parents_count = count($form_state['clicked_button']['#array_parents']);
    $step = 1;
    foreach ($form_state['clicked_button']['#array_parents'] as $array_parent) {
      if ($step > $parents_count - 3) {
        break;

      }
      $temp_form = &$temp_form[$array_parent];
      $step += 1;
    }

    return $temp_form['chart_container_' . $current_id];
  }
}

/**
 * Clear cache for specified item and children.
 *
 * @param $nid
 *   Smart Chart nid to clear cache for.
 * @param $children
 *   TRUE if you want to clear children cache too(Not working yet).
 */
function _smart_chart_clear_item_cache($nid, $children = FALSE) {
  cache_clear_all('smart_chart_item_' . $nid, 'cache');
}
