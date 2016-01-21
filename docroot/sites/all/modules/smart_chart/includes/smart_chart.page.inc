<?php

/**
 * @file
 * Chart display for Smart Chart.
 */

/**
 * @defgroup smart_chart_constants.
 * @{
 */
const SMART_CHART_CACHE_TIME = 86400; // 24 hours.
/**
 * @} End of "defgroup smart_chart_constants".
 */

/**
 * Show the chart as form.
 * @param int $item
 *   Chart or Smart Chart item to open.
 * @param int $open_nid
 *   Smart Chart Item to open to. The whole tree until this item.
 */
function smart_chart_form($form, &$form_state, $item, $open_nid = NULL) {
  global $user;

  // Load functions.
  module_load_include('inc', 'smart_chart', 'includes/smart_chart.functions');

  // Get available Smart Chart Item types to check if there are any.
  $smart_chart_item_types = sc_get_chart_item_types();
  if (count($smart_chart_item_types) == 0) {
    drupal_set_message(t('No Smart Chart Item types are selected.'), 'warning');
    return array();
  }

  // Array to keep all viewable items.
  if (!isset($form_state['all_items'])) {
    $form_state['all_items'] = array();
  }

  // Add ctools ajax libraries.
  ctools_include('modal');
  ctools_include('ajax');
  ctools_modal_add_js();

  // Attach custom js.
  $form['#attached']['js'][] = array(
    'data' => drupal_get_path('module', 'smart_chart') . '/js/smart_chart.js',
    'type' => 'file',
  );

  // Add css.
  $form['#attached']['css'][] = array(
    'data' => drupal_get_path('module', 'smart_chart') . '/css/smart_chart.css',
  );

  // Set page title.
  drupal_set_title($item->title);

  // Chart nid.
  $chart_nid = $item->nid;

  // Chart node id as value to be used by other modules.
  $form['chart_nid'] = array(
    '#type' => 'value',
    '#value' => $chart_nid,
  );

  // Get Smart Chart type.
  $chart_type = sc_get_chart_type($chart_nid);

  $form['#attributes']['id'] = 'chart_item_form';
  $form['#attributes']['class'][] = 'chart-item-form';

  // If refresh button is clicked, clear cache for whole chart.
  if (isset($form_state['clicked_button']['#name'])
    && strpos($form_state['clicked_button']['#name'], 'chart_button_refresh_' . $chart_nid) !== FALSE
  ) {
    // Clear cache for this nid.
    _smart_chart_clear_chart_cache();
  }

  // Main refresh button, hidden.
  $form['chart_container_' . $chart_nid] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'chart_container_' . $chart_nid,
      'class' => array('clearfix'),
    ),
  );

  // Wrap item actions and level, container.
  $form['chart_container_' . $chart_nid]['chart_item_wrapper'] = array(
    '#type' => 'html_tag',
    '#tag' => 'div',
    '#attributes' => array(
      'class' => array('clearfix', 'chart-item-wrapper'),
    ),
  );

  $form['chart_container_' . $chart_nid]['chart_item_wrapper']['actions'] = array(
    '#type' => 'html_tag',
    '#tag' => 'div',
    '#attributes' => array(
      'class' => array('chart-actions', 'chart-actions-main'),
    ),
  );

  // Refresh button, hidden.
  $form['chart_container_' . $chart_nid]['chart_item_wrapper']['actions']['chart_button_refresh_' . $chart_nid] = array(
    '#type' => 'button',
    '#value' => t('Refresh'),
    '#name' => 'chart_button_refresh_' . $chart_nid,
    '#ajax' => array(
      'callback' => '_smart_chart_refresh_chart',
      'wrapper' => 'chart_item_form',
      'event' => 'click',
      'method' => 'replace',
    ),
    '#attributes' => array(
      'class' => array(
        'chart-button',
        'refresh',
        'refresh_' . $chart_nid,
        'hidden',
      ),
    ),
  );

  // Refresh button, hidden, silent. For remote refreshes via node.js.
  $form['chart_container_' . $chart_nid]['chart_item_wrapper']['actions']['chart_button_refresh_silent_' . $chart_nid] = array(
    '#type' => 'button',
    '#value' => t('Refresh'),
    '#name' => 'chart_button_refresh_silent_' . $chart_nid,
    '#ajax' => array(
      'callback' => '_smart_chart_refresh_chart',
      'wrapper' => 'chart_container_' . $chart_nid,
      'event' => 'click',
      'method' => 'replace',
      'progress' => array('type' => 'none'),
    ),
    '#attributes' => array(
      'class' => array(
        'chart-button',
        'refresh_silent',
        'refresh_silent_' . $chart_nid,
        'hidden',
      ),
    ),
  );

  // Set actions according to children.
  if (_sc_item_has_children($chart_nid)) {
    // No actions at the moment.
  }
  else {
    // New button. Check user has permission.
    if (user_access('new smart chart item', $user)) {
      $href = 'chart/new/nojs/' . $chart_nid . '/' . $chart_nid;
      $form['chart_container_' . $chart_nid]['chart_item_wrapper']['actions']['chart_button_new_' . $chart_nid]
        = _smart_chart_create_button('new', t('New'), t('Create a new node below the current node.'), $href, '', $chart_nid, $chart_nid);
    }

    // Add button. Check user has permission.
    if (user_access('add smart chart item', $user)) {
      $href = 'chart/select/nojs/' . $chart_nid . '/' . $chart_nid;
      $form['chart_container_' . $chart_nid]['chart_item_wrapper']['actions']['chart_button_add_' . $chart_nid]
        = _smart_chart_create_button('add', t('Add'), t('Add a new node below the current node.'), $href, '', $chart_nid, $chart_nid);
    }
  }

  // Get initial item.
  $first_nid = _smart_chart_get_first_item($chart_nid, $user);

  // Get if there is item to build tree for.
  // The URL format to open a chart until a specific item is
  // "chart/[chart_nid]/[item_nid]#chart_item_[item_nid]".
  if ($open_nid != NULL) {
    // Get intermediate items, if there are any.
    $intermediate_items = sc_get_intermediate_items($first_nid, $open_nid);
    if ($intermediate_items
      && !isset($form_state['expanded_items'])
    ) {
      array_shift($intermediate_items);
      $form_state['expanded_items'] = $intermediate_items;
    }
  }

  $form['first_nid'] = array(
    '#type' => 'value',
    '#value' => $first_nid,
  );

  if ($first_nid) {
    // Create opened items array if it does not exist.
    $form_state['expanded_items'] = !isset($form_state['expanded_items']) ?
      array($first_nid) : $form_state['expanded_items'];

    if (isset($form_state['triggering_element']['#name'])
      && strpos($form_state['triggering_element']['#name'], 'chart_button_expand_') !== FALSE
    ) {
      $expanded_item = $form_state['triggering_element']['#name'];
      $expanded_item = str_replace('chart_button_expand_', '', $expanded_item);

      // Check that value exists in array and remove it.
      if (in_array($expanded_item, $form_state['expanded_items'])) {
        $form_state['expanded_items'] = array_diff($form_state['expanded_items'], array($expanded_item));

      }
      else {
        // Get intermediate items, if there are any.
        $intermediate_items = sc_get_intermediate_items($first_nid, $expanded_item);
        $form_state['expanded_items'] = $intermediate_items;
      }
    }
    // Get chart item for first step.
    $form = _smart_chart_get($form, $form_state, $first_nid, $chart_nid, $chart_type, $first_nid);
  }

  return $form;
}

/**
 * Get first item.
 *
 * @param $chart_nid
 *   Initial Chart nid.
 * @param object $user
 *   The user object.
 *
 * @return int
 *   Item nid.
 */
function _smart_chart_get_first_item($chart_nid, $user) {

  $first_item_var_name = '_smart_chart_get_first_item_'
    . (string) $chart_nid . '_' . (string) $user->uid;
  $first_item_static = &drupal_static($first_item_var_name);

  if (!isset($first_item_static)) {

    // Find children of current item.
    $query = db_select('node', 'n')
      ->fields('n', array('nid'));
    $query->join('field_data_field_sc_parent', 'parent', 'parent.field_sc_parent_target_id = ' . $chart_nid);
    $query->join('field_data_field_sc_child', 'child', 'n.nid = child.field_sc_child_target_id and child.entity_id = parent.entity_id');

    $data = array(
      'cid' => $chart_nid,
      'user' => $user,
    );
    drupal_alter('sc_first_item_query', $query, $data);

    $result = $query->execute()->fetchAssoc();

    $first_item_static = isset($result['nid']) ? $result['nid'] : FALSE;
  }

  return $first_item_static;
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
 * @param $chart_type
 *   Chart type to be available to hooks.
 * @param $first_nid
 *   The nid of the first item.
 * @param $level
 *   Level of item.
 *
 * @return array
 *   Form.
 */
function _smart_chart_get($form, &$form_state, $item_nid, $chart_nid, $chart_type, $first_nid, $level = 1) {

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

  // Wrap item actions and level, container.
  $form['chart_container_' . $item_nid]['chart_item_wrapper'] = array(
    '#type' => 'html_tag',
    '#tag' => 'div',
    '#attributes' => array(
      'class' => array('clearfix', 'chart-item-wrapper'),
    ),
  );

  // Need a way to get new items added.
  if (!in_array($item_nid, $form_state['all_items'])) {
    // Add new class.
    $form['chart_container_' . $item_nid]['#attributes']['class'][] = 'refreshed';
    $form_state['all_items'][] = $item_nid;
    $form_state['all_items'] = array_unique($form_state['all_items']);
  }

  // Set class according to level.
  if ($level == 1) {
    $form['chart_container_' . $item_nid]['#attributes']['class'][] = 'first';
  }
  elseif ($level > 1) {
    $form['chart_container_' . $item_nid]['#attributes']['class'][] = 'not-first';
  }

  // Level container.
  $form['chart_container_' . $item_nid]['chart_item_wrapper']['level'] = array(
    '#type' => 'html_tag',
    '#tag' => 'div',
    '#attributes' => array(
      'class' => array('chart-level'),
    ),
  );
  $form['chart_container_' . $item_nid]['chart_item_wrapper']['level']['text'] = array(
    '#type' => 'markup',
    '#markup' => $level,
  );

  // Content containers.
  $form['chart_container_' . $item_nid]['chart_item_wrapper']['chart_item_' . $item_nid] = array(
    '#type' => 'html_tag',
    '#tag' => 'div',
    '#attributes' => array(
      'class' => array('chart-content'),
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
    $form['chart_container_' . $item_nid]['chart_item_wrapper']['chart_item_' . $item_nid] = $cache->data;
  }
  else {

    $form['chart_container_' . $item_nid]['chart_item_wrapper']['chart_item_' . $item_nid]['content'] = array(
      '#type' => 'markup',
      '#markup' => $item->title,
    );

    // Creating hook to alter contents of the item.
    $context = array(
      'item_nid' => $item_nid,
    );
    drupal_alter('sc_item', $form['chart_container_' . $item_nid]['chart_item_wrapper']['chart_item_' . $item_nid], $context);

    // Save to cache.
    cache_set('smart_chart_item_' . $item_nid, $form['chart_container_' . $item_nid]['chart_item_wrapper']['chart_item_' . $item_nid],
      'cache', SMART_CHART_CACHE_TIME);
  }

  // Before recreating actions, check cache.
  if ($cache = cache_get('smart_chart_item_actions_' . $item_nid . '_' . $user->uid)) {
    $form['chart_container_' . $item_nid]['chart_item_wrapper']['actions'] = $cache->data;
  }
  else {
    // Other buttons container.
    $form['chart_container_' . $item_nid]['chart_item_wrapper']['actions'] = array(
      '#type' => 'html_tag',
      '#tag' => 'div',
      '#attributes' => array(
        'class' => array('chart-actions'),
      ),
    );

    // Refresh button, hidden.
    $form['chart_container_' . $item_nid]['chart_item_wrapper']['actions']['chart_button_refresh_' . $item_nid] = array(
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

    // Refresh button, hidden, silent. For remote refreshes via node.js.
    $form['chart_container_' . $item_nid]['chart_item_wrapper']['actions']['chart_button_refresh_silent_' . $item_nid] = array(
      '#type' => 'button',
      '#value' => t('Refresh'),
      '#name' => 'chart_button_refresh_silent_' . $item_nid,
      '#ajax' => array(
        'callback' => '_smart_chart_refresh_item',
        'wrapper' => 'chart_container_' . $item_nid,
        'event' => 'click',
        'method' => 'replace',
        'progress' => array('type' => 'none'),
      ),
      '#attributes' => array(
        'class' => array(
          'chart-button',
          'refresh_silent',
          'refresh_silent_' . $item_nid,
          'hidden',
        ),
      ),
    );

    // New button. Check user has permission.
    if (user_access('new smart chart item', $user)) {
      $href = 'chart/new/nojs/' . $chart_nid . '/' . $item_nid;
      $form['chart_container_' . $item_nid]['chart_item_wrapper']['actions']['chart_button_new_' . $item_nid]
        = _smart_chart_create_button('new', t('New'), t('Create new node below the current node.'), $href, 'hidden', $chart_nid, $item_nid);
    }

    // Add button. Check user has permission.
    if (user_access('add smart chart item', $user)) {
      $href = 'chart/select/nojs/' . $chart_nid . '/' . $item_nid;
      $form['chart_container_' . $item_nid]['chart_item_wrapper']['actions']['chart_button_add_' . $item_nid]
        = _smart_chart_create_button('add', t('Add'), t('Add an existing node below the current node.'), $href, 'hidden', $chart_nid, $item_nid);
    }

    // Break button. Check user has permission.
    if (user_access('break smart chart item', $user)) {
      $href = 'chart/confirm/nojs/break/' . $chart_nid . '/' . $item_nid;
      $form['chart_container_' . $item_nid]['chart_item_wrapper']['actions']['chart_button_break_' . $item_nid]
        = _smart_chart_create_button('break', t('Break'), t('Break current node from chart. All nodes below the current node will be drawn with their connections. You can later use the "Add" button, to restore these connections under a different node.'), $href, 'hidden', $chart_nid, $item_nid);
    }

    // Delete button. Check user has permission.
    if (user_access('delete smart chart item', $user)) {
      $href = 'chart/confirm/nojs/delete/' . $chart_nid . '/' . $item_nid;
      $form['chart_container_' . $item_nid]['chart_item_wrapper']['actions']['chart_button_delete_' . $item_nid]
        = _smart_chart_create_button('delete', t('Delete'), t('Delete'), $href, 'hidden', $chart_nid, $item_nid);
    }

    // Show/Hide actions button.
    $form['chart_container_' . $item_nid]['chart_item_wrapper']['actions']['chart_button_actions_toggle_' . $item_nid]
      = array(
      '#type' => 'html_tag',
      '#tag' => 'span',
      '#attributes' => array(
        'class' => array(
          'form-item',
        )
      ),
      '#weight' => -50,
      '#value' => l('', '', array(
          'attributes' => array(
            'class' => array(
              'toggle-actions',
              'chart-button',
              'no-hide',
              'fa',
              'fa-cogs',
            ),
            'title' => t('Actions'),
          ),
          'fragment' => 'toggle-actions',
          'external' => TRUE,
        )
      ),
    );

    // Creating hook to alter actions.
    $context = array(
      'item_nid' => $item_nid,
      'chart_nid' => $chart_nid,
      'first_nid' => $first_nid,
      'chart_type' => $chart_type,
    );
    drupal_alter('sc_actions', $form['chart_container_' . $item_nid]['chart_item_wrapper']['actions'], $context);

    // Save to cache.
    cache_set('smart_chart_item_actions_' . $item_nid . '_' . $user->uid, $form['chart_container_' . $item_nid]['chart_item_wrapper']['actions'],
      'cache', SMART_CHART_CACHE_TIME);
  }

  // Display the bellow only if item has children.
  if (_sc_item_has_children($item_nid)) {
    // Expand button.
    $form['chart_container_' . $item_nid]['chart_item_wrapper']['actions']['chart_button_expand_' . $item_nid] = array(
      '#type' => 'button',
      '#value' => '',
      '#name' => 'chart_button_expand_' . $item_nid,
      '#ajax' => array(
        'callback' => '_smart_chart_expand_item',
        'wrapper' => 'chart_container_' . $first_nid,
        'event' => 'click',
        'method' => 'replace',
      ),
      '#attributes' => array(
        'class' => array('chart-button', 'expand', 'left'),
        'title' => t('Expand'),
      ),
      '#weight' => 0,
    );

    // Class "noid" is passed to completely remove id attribute later on theme.
    $form['chart_container_' . $item_nid]['chart_children_' . $item_nid] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'clearfix',
          'chart-children',
          'noid',
        ),
      ),
    );

    if (in_array($item_nid, $form_state['expanded_items'])) {
      $form['chart_container_' . $item_nid]['chart_children_' . $item_nid]
        = _smart_chart_children($form['chart_container_' . $item_nid]['chart_children_' . $item_nid],
        $form_state, $item_nid, $chart_nid, $chart_type, $first_nid, $level + 1);

      // Arrow down.
      $form['chart_container_' . $item_nid]['chart_children_' . $item_nid]['arrow'] = array(
        '#type' => 'html_tag',
        '#tag' => 'span',
        '#attributes' => array('class' => array('arrow')),
      );
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
 * @param $first_nid
 *   Nid of the first item.
 * @param $level
 *   Current level of item.
 *
 * @return array
 *   Form items.
 */
function _smart_chart_children($form, &$form_state, $parent_nid, $chart_nid, $chart_type, $first_nid, $level) {

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
      $form = _smart_chart_get($form, $form_state, $item->nid, $chart_nid, $chart_type, $first_nid, $level);
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

  // Get clicked button.
  if (isset($form_state['clicked_button']['#array_parents'])) {

    $current_id = str_replace('chart_button_expand_', '', $form_state['clicked_button']['#name']);

    // Create form to return plus ajax call to scroll to item.
    $html = drupal_render($form['chart_container_' . $form['first_nid']['#value']]);
    $commands[] = ajax_command_insert(NULL, $html);
    $commands[] = ajax_command_invoke(NULL, 'scScrollToItemBackEnd', array('chart-button-expand-' . $current_id));
    $commands[] = ajax_command_invoke(NULL, 'scRegisterEvents');

    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
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
  $button_value = ctools_modal_text_button($title, $href, $tooltip, 'ctools-modal-common-modal-style chart-button ' . $action . ' ' . $class);

  $button = array(
    '#type' => 'html_tag',
    '#tag' => 'span',
    '#attributes' => array(
      'class' => array(
        'form-item',
      ),
    ),
    '#value' => $button_value,
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
    // Replace in case of silent or normal button.
    $replace_array = array(
      'chart_button_refresh_',
      'chart_button_refresh_silent_',
      'silent_',
    );
    $current_id = str_replace($replace_array, '', $form_state['clicked_button']['#name']);

    $parents_count = count($form_state['clicked_button']['#array_parents']);

    $step = 1;
    foreach ($form_state['clicked_button']['#array_parents'] as $array_parent) {
      if ($step > $parents_count - 4) {
        break;

      }
      $temp_form = &$temp_form[$array_parent];
      $step += 1;
    }

    // Create form to return plus register js events.
    $html = drupal_render($temp_form['chart_container_' . $current_id]);
    $commands[] = ajax_command_insert(NULL, $html);
    $commands[] = ajax_command_invoke(NULL, 'scRegisterEvents');

    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  }
}

/**
 * Refreshes whole form/chart.
 */
function _smart_chart_refresh_chart($form, &$form_state) {

  // Create form to return plus register js events.
  $html = drupal_render($form);
  $commands[] = ajax_command_insert(NULL, $html);
  $commands[] = ajax_command_invoke(NULL, 'scRegisterEvents');

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
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
  cache_clear_all('smart_chart_item_actions_' . $nid, 'cache', TRUE);
  cache_clear_all('smart_chart_item_has_children_' . $nid, 'cache');
}

/**
 * Clear cache for whole chart.
 */
function _smart_chart_clear_chart_cache() {
  cache_clear_all('smart_chart_item_', 'cache', TRUE);
  cache_clear_all('smart_chart_item_actions_', 'cache', TRUE);
  cache_clear_all('smart_chart_item_has_children_', 'cache', TRUE);
}

/**
 * Get if node has children. Cached response.
 *
 * @param $parent_nid
 *   Parent nid to check for.
 *
 * @return bool
 *   Has children or not.
 */
function _sc_item_has_children($parent_nid) {

  // Before getting if it has children, check cache.
  if ($cache = cache_get('smart_chart_item_has_children_' . $parent_nid)) {
    $has_children = $cache->data;
  }
  else {
    // Load functions.
    module_load_include('inc', 'smart_chart', 'includes/smart_chart.functions');
    $has_children = sc_item_has_children($parent_nid);

    // Save to cache.
    cache_set('smart_chart_item_has_children_' . $parent_nid, $has_children,
      'cache', SMART_CHART_CACHE_TIME);
  }

  return $has_children;
}
