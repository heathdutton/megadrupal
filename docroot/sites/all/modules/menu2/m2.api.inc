<?php


$GLOBALS['M2_FIELDS']      = array('m2_name', 'title', 'language', 'description', 'owner_uid', 'date_created', 'date_removed', 'is_system', 'is_disabled', 'is_removed', 'is_expanded_active_trail');
$GLOBALS['M2_ITEM_FIELDS'] = array('item_id', 'm2_name', 'weight', 'depth', 'title', 'url', 'class', 'language', 'description', 'date_created', 'date_removed', 'is_expanded', 'is_disabled', 'is_disabled_grp', 'is_removed');


/**
 * Implements hook_m2_menu_insert_alter
 * Implements hook_m2_menu_insert
 * Implements hook_m2_menu_remove_alter
 * Implements hook_m2_menu_remove_to_garbage_alter
 * Implements hook_m2_menu_restore_from_garbage_alter
 * Implements hook_m2_menu_all_clear_alter
 * Implements hook_m2_menu_update_values_alter
 * Implements hook_m2_item_insert_alter
 * Implements hook_m2_item_insert
 * Implements hook_m2_item_insert_before_alter
 * Implements hook_m2_item_insert_after_alter
 * Implements hook_m2_item_prepend_alter
 * Implements hook_m2_item_append_alter
 * Implements hook_m2_item_update_values_alter
 * Implements hook_m2_item_update_values_bulk_alter
 * Implements hook_m2_item_remove_alter
 * Implements hook_m2_item_remove_to_garbage_alter
 * Implements hook_m2_item_restore_from_garbage_alter
 * Implements hook_m2_item_all_clear_alter
 */
 
function hook_m2_menu_insert_alter                 (&$m2_name, &$values)        {}
function hook_m2_menu_insert                       ( $m2_name)                  {}
function hook_m2_menu_remove_alter                 (&$m2_name)                  {}
function hook_m2_menu_remove_to_garbage_alter      (&$m2_name, &$with_children) {}
function hook_m2_menu_restore_from_garbage_alter   (&$m2_name, &$with_children) {}
function hook_m2_menu_all_clear_alter              (&$owner_uid)                {}
function hook_m2_menu_update_values_alter          (&$m2_name, &$changes)       {}
function hook_m2_item_insert_alter                 (&$m2_name, &$values)        {}
function hook_m2_item_insert                       ( $new_item_id)              {}
function hook_m2_item_insert_before_alter          (&$c_item,  &$parent)        {}
function hook_m2_item_insert_after_alter           (&$c_item,  &$parent)        {}
function hook_m2_item_prepend_alter                (&$c_item,  &$parent)        {}
function hook_m2_item_append_alter                 (&$c_item,  &$parent)        {}
function hook_m2_item_update_values_alter          (&$c_item,  &$changes)       {}
function hook_m2_item_update_weight_and_depth_bulk (&$m2_name, &$changes)       {}
function hook_m2_item_remove_alter                 (&$c_item,  &$with_children) {}
function hook_m2_item_remove_to_garbage_alter      (&$c_item,  &$with_children) {}
function hook_m2_item_restore_from_garbage_alter   (&$c_item,  &$with_children) {}
function hook_m2_item_all_clear_alter              (&$m2_name)                  {}


/**
 * m2_api_menu_select_universal                         | return array(N) or array() | return object(stdClass) or bool(false)
 * m2_api_menu_select_by_name                           | return object(stdClass) or bool(false)
 * m2_api_menu_select_all                               | return array(N) or array()
 * m2_api_menu_select_all_removed                       | return array(N) or array()
 * m2_api_menu_select_all_removed_or_with_removed_items | return array(N) or array()
 * m2_api_menu_insert                                   | return object(stdClass) or bool(false)
 * m2_api_menu_remove                                   | return bool(true) or bool(false)
 * m2_api_menu_update_values                            | return bool(true) or bool(false)
 * m2_api_menu_name_check_exists                        | return bool(true) or bool(false)
 * m2_api_menu_last_weight                              | return int
 */

function m2_api_menu_select_universal($conditions = array(), $single_row = false) {
  $query = db_select('m2', 'm');
  $query->leftJoin('users', 'u', 'm.owner_uid = u.uid'); /* leftJoin for ignore losted users */
  $query->leftJoin('m2_items', 'i', 'm.m2_name = i.m2_name'); /* leftJoin for menu without items */
  $query->fields('m', $GLOBALS['M2_FIELDS']);
  $query->addField('u', 'name', 'owner_name');
  $query->addExpression('ifnull(sum(i.is_removed), 0)', 'removed_items_num');
  $query->addExpression('count(i.item_id)', 'total_items_num');
  $query->groupBy('m.m2_name');
  $query->orderBy('title');
/* prepare conditions */
  foreach ($conditions as $condition) {
    if ($condition['field'] == 'removed_items_num') {
      $query->havingCondition($condition['field'],
                              $condition['value'],
                              isset($condition['condition']) ? $condition['condition'] : '=');
    } else {
      $query->condition($condition['field'],
                        $condition['value'],
                        isset($condition['condition']) ? $condition['condition'] : '=');
    }
  }
/* prepare and return result */
  if ($single_row) {
    return $query->execute()->fetchObject();
  } else {
    $items = array();
    foreach ($query->execute() as $row) {
      $items[$row->m2_name] = $row;
    }
    return $items;
  }
}


function m2_api_menu_select_by_name($m2_name) {
  return m2_api_menu_select_universal(array(array('field' => 'm.m2_name', 'value' => $m2_name)), true);
}


function m2_api_menu_select_all($owner_uid = null, $with_system = M2_WITH_SYSTEM_OFF, $with_removed = M2_WITH_REMOVED_OFF) {
  $conditions = array();
  if ($owner_uid)                           $conditions[]= array('field' => 'm.owner_uid',  'value' => $owner_uid);
  if ($with_removed == M2_WITH_REMOVED_OFF) $conditions[]= array('field' => 'm.is_removed', 'value' => 0);
  if ($with_system == M2_WITH_SYSTEM_OFF)   $conditions[]= array('field' => 'm.is_system',  'value' => 0);
  return m2_api_menu_select_universal($conditions);
}


function m2_api_menu_select_all_removed($owner_uid = null, $with_system = M2_WITH_SYSTEM_OFF) {
  $conditions = array(array('field' => 'm.is_removed', 'value' => 1));
  if ($owner_uid)                         $conditions[]= array('field' => 'm.owner_uid', 'value' => $owner_uid);
  if ($with_system == M2_WITH_SYSTEM_OFF) $conditions[]= array('field' => 'm.is_system', 'value' => 0);
  return m2_api_menu_select_universal($conditions);
}


function m2_api_menu_select_all_removed_or_with_removed_items($owner_uid = null, $with_system = M2_WITH_SYSTEM_OFF) {
  $conditions = array();
  if ($owner_uid)                         $conditions[]= array('field' => 'm.owner_uid', 'value' => $owner_uid);
  if ($with_system == M2_WITH_SYSTEM_OFF) $conditions[]= array('field' => 'm.is_system', 'value' => 0);
  $removed_by_menus = m2_api_menu_select_universal(array_merge($conditions, array(array('field' => 'm.is_removed', 'value' => 1))));
  $removed_by_items = m2_api_menu_select_universal(array_merge($conditions, array(array('field' => 'removed_items_num', 'value' => 0, 'condition' => '>'))));
  if (count($removed_by_menus) || count($removed_by_items)) {
    return m2_api_menu_select_universal(array(array('field' => 'm.m2_name', 'value' => array_keys($removed_by_menus + $removed_by_items), 'condition' => 'in')));
  } else {
    return array();
  }
}


function m2_api_menu_insert($m2_name, $values = array()) {
  drupal_alter('m2_menu_insert', $m2_name, $values);
  if ($m2_name && !m2_api_menu_name_check_exists($m2_name)) {
    db_insert('m2')->fields($values + array('m2_name' => $m2_name, 'title' => $m2_name, 'date_created' => time()))->execute();
    $menu = m2_api_menu_select_by_name($m2_name);
    if ($menu) module_invoke_all('m2_menu_insert', $m2_name);
    return is_object($menu);
  }
  return false;
}


function m2_api_menu_remove($m2_name) {
  drupal_alter('m2_menu_remove', $m2_name);
  $transaction = db_transaction();
  try {
    db_delete('m2_items')->condition('m2_name', $m2_name)->execute();
    $result = db_delete('m2')->condition('m2_name', $m2_name)->execute();
    return $result;
  }
  catch (Exception $e) {
    $transaction->rollback();
    return false;
  }
}


function m2_api_menu_update_values($m2_name, $changes = array()) {
  drupal_alter('m2_menu_update_values', $m2_name, $changes);
  try {
    if (count($changes)) {
      return (bool)db_update('m2')->fields($changes)->condition('m2_name', $m2_name)->execute();
    }
  }
  catch (Exception $e) {
    return false;
  }
}


function m2_api_menu_name_check_exists($m2_name, $with_removed = M2_WITH_REMOVED_ON) {
  $conditions = array(array('field' => 'm.m2_name', 'value' => $m2_name));
  if ($with_removed == M2_WITH_REMOVED_OFF) $conditions[]= array('field' => 'm.is_removed', 'value' => 0);
  return (bool)m2_api_menu_select_universal($conditions, true);
}


function m2_api_menu_last_weight($m2_name) {
  $query = db_select('m2_items', 'i')->condition('m2_name', $m2_name);
  $query->addExpression('max(weight)');
  return $query->execute()->fetchField();
}


/**
 * m2_api_item_select_universal             | return array(N) or array() | return object(stdClass) or bool(false)
 * m2_api_item_select                       | return object(stdClass) or bool(false)
 * m2_api_item_select_by_menu_all           | return array(N) or array()
 * m2_api_item_select_active                | return object(stdClass) or bool(false)
 * m2_api_item_select_children              | return array(N) or array()
 * m2_api_item_select_parent                | return object(stdClass) or bool(false)
 * m2_api_item_select_parent_all            | return array(N) or array()
 * m2_api_item_select_prev                  | return object(stdClass) or bool(false)
 * m2_api_item_select_next                  | return object(stdClass) or bool(false)
 * m2_api_item_select_prev_sibling          | return object(stdClass) or bool(false)
 * m2_api_item_select_next_sibling          | return object(stdClass) or bool(false)
 * m2_api_item_select_prev_sibling_absolute | return object(stdClass) or bool(false)
 * m2_api_item_select_next_sibling_absolute | return object(stdClass) or bool(false)
 * m2_api_item_insert                       | return int() or bool(false)
 * m2_api_item_update_values                | return bool(true) or bool(false)
 * m2_api_item_update_weight_and_depth_bulk | return array(N) or bool(false)
 * m2_api_item_insert_before                | return bool(true) or bool(false)
 * m2_api_item_insert_after                 | return bool(true) or bool(false)      !!! UNDER CONSTRUCTION !!!
 * m2_api_item_prepend                      | return bool(true) or bool(false)      !!! UNDER CONSTRUCTION !!!
 * m2_api_item_append                       | return bool(true) or bool(false)      !!! UNDER CONSTRUCTION !!!
 * m2_api_item_remove                       | return bool(true) or bool(false)
 */

function m2_api_item_select_universal($conditions = array(), $single_row = false, $order_by_weight = 'asc') {
  $query = db_select('m2_items', 'i');
  $query->fields('i', $GLOBALS['M2_ITEM_FIELDS']);
  $query->orderby('m2_name', 'asc');
  $query->orderby('weight', $order_by_weight);
/* prepare conditions */
  foreach ($conditions as $condition) {
    $query->condition($condition['field'],
                      $condition['value'],
                      isset($condition['condition']) ? $condition['condition'] : '=');
  }
/* prepare and return result */
  if ($single_row) {
    return $query->execute()->fetchObject();
  } else {
    $items = array();
    foreach ($query->execute() as $row) {
      $items[$row->item_id] = $row;
    }
    return $items;
  }
}


function m2_api_item_select($item_id) {
  return m2_api_item_select_universal(array(array('field' => 'item_id', 'value' => $item_id)), true);
}


function m2_api_item_select_by_menu_all($m2_name, $levels_num = 0, $depth_shift = 0, $with_removed = M2_WITH_REMOVED_OFF, $with_disabled = M2_WITH_DISABLED_OFF) {
  $conditions = array(array('field' => 'm2_name', 'value' => $m2_name));
  if ($levels_num)                            $conditions[]= array('field' => 'depth',           'value' => $levels_num, 'condition' => '<');
  if ($with_removed == M2_WITH_REMOVED_OFF)   $conditions[]= array('field' => 'is_removed',      'value' => 0);
  if ($with_disabled == M2_WITH_DISABLED_OFF) $conditions[]= array('field' => 'is_disabled_grp', 'value' => 0);
  $items = m2_api_item_select_universal($conditions);
  foreach ($items as $c_item) {
    $c_item->depth = ($c_item->depth - $depth_shift > 0 ? $c_item->depth - $depth_shift : 0);
  }
  return $items;
}


function m2_api_item_select_active($m2_name) {
  $conditions = array(
    array('field' => 'm2_name',    'value' => $m2_name),
    array('field' => 'url',        'value' => $_GET['q']),
    array('field' => 'is_removed', 'value' => 0),
  );
  return m2_api_item_select_universal($conditions, true);
}


function m2_api_item_select_children($parent, $levels_num = 0, $depth_shift = 0, $with_removed = M2_WITH_REMOVED_OFF, $with_disabled = M2_WITH_DISABLED_OFF) {
  $parent = _m2_api_item_prepare($parent);
  if ($parent) {
    $conditions = array(
      array('field' => 'm2_name', 'value' => $parent->m2_name),
      array('field' => 'weight',  'value' => $parent->weight, 'condition' => '>'),
      array('field' => 'depth',   'value' => $parent->depth,  'condition' => '>'),
    );
    $absolute_next = m2_api_item_select_next_sibling_absolute($parent, M2_WITH_REMOVED_ON, M2_WITH_DISABLED_ON);
    if ($with_removed == M2_WITH_REMOVED_OFF)   $conditions[]= array('field' => 'is_removed',      'value' => 0);
    if ($with_disabled == M2_WITH_DISABLED_OFF) $conditions[]= array('field' => 'is_disabled_grp', 'value' => 0);
    if ($absolute_next)                         $conditions[]= array('field' => 'weight',          'value' => $absolute_next->weight, 'condition' => '<');
    if ($levels_num)                            $conditions[]= array('field' => 'depth',           'value' => (isset($parent->depth) ? $parent->depth + 1 : 0) + $levels_num, 'condition' => '<');
    $items = m2_api_item_select_universal($conditions);
    foreach ($items as $c_item) {
      $c_item->depth = ($c_item->depth - $depth_shift > 0 ? $c_item->depth - $depth_shift : 0);
    }
  }
  return isset($items) ? $items : array();
}


function m2_api_item_select_parent($item_or_id, $height = 1) {
  $c_item = _m2_api_item_prepare($item_or_id);
  if ($c_item) {
    $conditions = array(
      array('field' => 'm2_name', 'value' => $c_item->m2_name),
      array('field' => 'weight',  'value' => $c_item->weight, 'condition' => '<'),
      array('field' => 'depth',   'value' => $c_item->depth - $height),
    );
    return m2_api_item_select_universal($conditions, true, 'desc');
  } else {
    return false;
  }
}


function m2_api_item_select_parent_all($item_or_id) {
  $c_item = _m2_api_item_prepare($item_or_id);
  $c_item_orig = $c_item;
  $cache = &drupal_static(__FUNCTION__, array());
  if (isset($cache[$c_item->item_id])) {
    return $cache[$c_item->item_id];
  } else {
    $parents = array();
    if ($c_item && $c_item->depth > 0) {
      while ($c_parent = m2_api_item_select_parent($c_item)) {
        $parents[$c_parent->item_id] = $c_parent;
        $c_item = $c_parent;
      }
    }
    $cache[$c_item_orig->item_id] = $parents;
    return $parents;
  }
}


function m2_api_item_select_prev($item_or_id, $with_removed = M2_WITH_REMOVED_OFF, $with_disabled = M2_WITH_DISABLED_OFF, $skip_without_url = M2_SKIP_WITHOUT_URL_OFF) {
  $c_item = _m2_api_item_prepare($item_or_id);
  if ($c_item) {
    $conditions = array(
      array('field' => 'm2_name', 'value' => $c_item->m2_name),
      array('field' => 'weight',  'value' => $c_item->weight, 'condition' => '<'),
    );
    if ($with_removed == M2_WITH_REMOVED_OFF)        $conditions[]= array('field' => 'is_removed',      'value' => 0);
    if ($with_disabled == M2_WITH_DISABLED_OFF)      $conditions[]= array('field' => 'is_disabled_grp', 'value' => 0);
    if ($skip_without_url == M2_SKIP_WITHOUT_URL_ON) $conditions[]= array('field' => 'url',             'value' => '', 'condition' => '<>');
    return m2_api_item_select_universal($conditions, true, 'desc');
  } else {
    return false;
  }
}


function m2_api_item_select_next($item_or_id, $with_removed = M2_WITH_REMOVED_OFF, $with_disabled = M2_WITH_DISABLED_OFF, $skip_without_url = M2_SKIP_WITHOUT_URL_OFF) {
  $c_item = _m2_api_item_prepare($item_or_id);
  if ($c_item) {
    $conditions = array(
      array('field' => 'm2_name', 'value' => $c_item->m2_name),
      array('field' => 'weight',  'value' => $c_item->weight, 'condition' => '>'),
    );
    if ($with_removed == M2_WITH_REMOVED_OFF)        $conditions[]= array('field' => 'is_removed',      'value' => 0);
    if ($with_disabled == M2_WITH_DISABLED_OFF)      $conditions[]= array('field' => 'is_disabled_grp', 'value' => 0);
    if ($skip_without_url == M2_SKIP_WITHOUT_URL_ON) $conditions[]= array('field' => 'url',             'value' => '', 'condition' => '<>');
    return m2_api_item_select_universal($conditions, true);
  } else {
    return false;
  }
}


function m2_api_item_select_prev_sibling($item_or_id, $with_removed = M2_WITH_REMOVED_OFF, $with_disabled = M2_WITH_DISABLED_OFF, $skip_without_url = M2_SKIP_WITHOUT_URL_OFF) {
  $c_item = _m2_api_item_prepare($item_or_id);
  if ($c_item) {
    $absolute_prev = m2_api_item_select_prev_sibling_absolute($c_item, $with_removed, $with_disabled, $skip_without_url);
    if ($absolute_prev) {
      $conditions = array(
        array('field' => 'm2_name', 'value' => $c_item->m2_name),
        array('field' => 'weight',  'value' => $c_item->weight,        'condition' => '<'),
        array('field' => 'weight',  'value' => $absolute_prev->weight, 'condition' => '>'),
        array('field' => 'depth',   'value' => $c_item->depth,         'condition' => '<'),
      );
      $is_overfly = (bool)m2_api_item_select_universal($conditions, true);
      return $is_overfly ? false : $absolute_prev;
    } else {
      return false;
    }
  } else {
    return false;
  }
}


function m2_api_item_select_next_sibling($item_or_id, $with_removed = M2_WITH_REMOVED_OFF, $with_disabled = M2_WITH_DISABLED_OFF, $skip_without_url = M2_SKIP_WITHOUT_URL_OFF) {
  $c_item = _m2_api_item_prepare($item_or_id);
  if ($c_item) {
    $absolute_next = m2_api_item_select_next_sibling_absolute($c_item, $with_removed, $with_disabled, $skip_without_url);
    if ($absolute_next) {
      $conditions = array(
        array('field' => 'm2_name', 'value' => $c_item->m2_name),
        array('field' => 'weight',  'value' => $c_item->weight,        'condition' => '>'),
        array('field' => 'weight',  'value' => $absolute_next->weight, 'condition' => '<'),
        array('field' => 'depth',   'value' => $c_item->depth,         'condition' => '<'),
      );
      $is_overfly = (bool)m2_api_item_select_universal($conditions, true);
      return $is_overfly ? false : $absolute_next;
    } else {
      return false;
    }
  } else {
    return false;
  }
}


function m2_api_item_select_prev_sibling_absolute($item_or_id, $with_removed = M2_WITH_REMOVED_OFF, $with_disabled = M2_WITH_DISABLED_OFF, $skip_without_url = M2_SKIP_WITHOUT_URL_OFF) {
  $c_item = _m2_api_item_prepare($item_or_id);
  if ($c_item) {
    $conditions = array(
      array('field' => 'm2_name', 'value' => $c_item->m2_name),
      array('field' => 'weight',  'value' => $c_item->weight, 'condition' => '<'),
      array('field' => 'depth',   'value' => $c_item->depth),
    );
    if ($with_removed == M2_WITH_REMOVED_OFF)        $conditions[]= array('field' => 'is_removed',      'value' => 0);
    if ($with_disabled == M2_WITH_DISABLED_OFF)      $conditions[]= array('field' => 'is_disabled_grp', 'value' => 0);
    if ($skip_without_url == M2_SKIP_WITHOUT_URL_ON) $conditions[]= array('field' => 'url',             'value' => '', 'condition' => '<>');
    return m2_api_item_select_universal($conditions, true, 'desc');
  } else {
    return false;
  }
}


function m2_api_item_select_next_sibling_absolute($item_or_id, $with_removed = M2_WITH_REMOVED_OFF, $with_disabled = M2_WITH_DISABLED_OFF, $skip_without_url = M2_SKIP_WITHOUT_URL_OFF) {
  $c_item = _m2_api_item_prepare($item_or_id);
  if ($c_item) {
    $conditions = array(
      array('field' => 'm2_name', 'value' => $c_item->m2_name),
      array('field' => 'weight',  'value' => $c_item->weight, 'condition' => '>'),
      array('field' => 'depth',   'value' => $c_item->depth),
    );  
    if ($with_removed == M2_WITH_REMOVED_OFF)        $conditions[]= array('field' => 'is_removed',      'value' => 0);
    if ($with_disabled == M2_WITH_DISABLED_OFF)      $conditions[]= array('field' => 'is_disabled_grp', 'value' => 0);
    if ($skip_without_url == M2_SKIP_WITHOUT_URL_ON) $conditions[]= array('field' => 'url',             'value' => '', 'condition' => '<>');
    return m2_api_item_select_universal($conditions, true);
  } else {
    return false;
  }
}


function m2_api_item_insert($m2_name, $values = array()) {
  drupal_alter('m2_item_insert', $m2_name, $values);
  try {
    $last_weight = m2_api_menu_last_weight($m2_name);
    $query = db_insert('m2_items');
    $query->fields(array('m2_name' => $m2_name, 'weight' => $last_weight + 1, 'date_created' => time()) + $values);
    $new_item_id = $query->execute();
    module_invoke_all('m2_item_insert', $new_item_id);
    _m2_api_rebuild_disabled_grp($m2_name);
    return $new_item_id;
  }
  catch (Exception $e) {
    return false;
  }
}


function m2_api_item_update_values($item_or_id, $changes = array()) {
  $c_item = _m2_api_item_prepare($item_or_id);
  drupal_alter('m2_item_update_values', $c_item, $changes);
  try {
    if (count($changes)) {
      $result = (bool)db_update('m2_items')->fields($changes)->condition('item_id', $c_item->item_id)->execute();
      _m2_api_rebuild_disabled_grp($c_item->m2_name);
      return $result;
    }
  }
  catch (Exception $e) {
    return false;
  }
}


function m2_api_item_update_weight_and_depth_bulk($m2_name, $changes) {
  drupal_alter('m2_item_update_weight_and_depth_bulk', $m2_name, $changes);
  $items_all = m2_api_item_select_universal(array(array('field' => 'm2_name', 'value' => $m2_name)));
  $items_to_change = array();
  $items_unchanged = array();
  $items_update_id = array();
  if (count($items_all) && count($changes)) {
    $transaction = db_transaction();
 /* finding id of parent for each element (for restore unchanged items in a future) */
    foreach ($items_all as $c_item) {
      if ($c_item->depth == 0) $c_item->parent_id = 0;
      if ($c_item->depth >= 1) $c_item->parent_id = ${'p'.($c_item->depth-1)}->item_id;
      ${"p$c_item->depth"} = $c_item;
    }
 /* create new collection of items by new changes */
    foreach ($changes as $item_id => $c_change) {
      $new_item = clone $items_all[$item_id];
      $new_item->new_weight = $c_change['weight'];
      $new_item->new_depth = $c_change['depth'];
      $items_to_change[$item_id] = $new_item;
    }
 /* restore unchanged items */
    $sub_weight = 0;
    $items_unchanged = array_diff_key($items_all, $changes);
    foreach ($items_unchanged as $item_id => $c_item) {
      $ext_item = clone $items_all[$item_id];
      $items_to_change[$item_id] = $ext_item;
      if ($ext_item->parent_id) {
        $sub_weight += .0001;
        $ext_item->new_weight = $items_to_change[$ext_item->parent_id]->new_weight + $sub_weight;
        $ext_item->new_depth = $items_to_change[$ext_item->parent_id]->new_depth + 1;
      } else {
        $ext_item->new_weight = count($items_to_change) + 10000;
        $ext_item->new_depth = 0;
      }
    }
 /* resort collection and convert weight to integer */
    uasort($items_to_change, 'm2_api_flat_tree_sort_by_new_weight');
    $c_weight = 1;
    foreach ($items_to_change as $c_item) {
      $c_item->new_weight = $c_weight++;
    }
 /* save new states */
    foreach($items_to_change as $c_item) {
      if ($c_item->new_weight != $c_item->weight || $c_item->new_depth != $c_item->depth) {
        if (m2_api_item_update_values($c_item->item_id, array('weight' => $c_item->new_weight, 'depth' => $c_item->new_depth))) {
          $items_update_id[]= $c_item->item_id;
        } else {
          $transaction->rollback();
          return false;
        }
      }
    }
  }
  _m2_api_rebuild_disabled_grp($m2_name);
  return $items_update_id;
}


function m2_api_item_insert_before($item_or_id, $parent_or_id) {
  $c_item = _m2_api_item_prepare($item_or_id);
  $parent = $parent_or_id ? _m2_api_item_prepare($parent_or_id) : null;
  drupal_alter('m2_item_insert_before', $c_item, $parent);
  if ($c_item && $parent) {
    $transaction = db_transaction();
    try {
      $children = m2_api_item_select_children($c_item, 0, 0, M2_WITH_REMOVED_ON, M2_WITH_DISABLED_ON);
      if (count($children)) {
        $has_hole = _m2_api_insert_hole($c_item->m2_name, $parent->weight - 1);
     /* code */
      } else {
        $has_hole = _m2_api_insert_hole($c_item->m2_name, $parent->weight - 1);
        m2_api_item_update_values($c_item->item_id, array('weight' => $parent->weight, 'depth' => $parent->depth));
      }
      _m2_api_rebuild_disabled_grp($c_item->m2_name);
      return true;
    }
    catch (Exception $e) {
      $transaction->rollback();
      return false;
    }
  } else {
    return false;
  }
}


function m2_api_item_insert_after($item_or_id, $parent_or_id) {
  // $c_item = _m2_api_item_prepare($item_or_id);
  // $parent = $parent_or_id ? _m2_api_item_prepare($parent_or_id) : null;
  // drupal_alter('m2_item_insert_after', $c_item, $parent);
  // if ($c_item && $parent) {
  //   $transaction = db_transaction();
  //   try {
  //  /* code */
  //     _m2_api_rebuild_disabled_grp($c_item->m2_name);
  //     return true;
  //   }
  //   catch (Exception $e) {
  //     $transaction->rollback();
  //     return false;
  //   }
  // } else {
  //   return false;
  // }
}


function m2_api_item_prepend($item_or_id, $parent_or_id = null) {
  // $c_item = _m2_api_item_prepare($item_or_id);
  // $parent = $parent_or_id ? _m2_api_item_prepare($parent_or_id) : null;
  // drupal_alter('m2_item_prepend', $c_item, $parent);
  // if ($c_item) {
  //   $transaction = db_transaction();
  //   try {
  //  /* code */
  //     _m2_api_rebuild_disabled_grp($c_item->m2_name);
  //     return true;
  //   }
  //   catch (Exception $e) {
  //     $transaction->rollback();
  //     return false;
  //   }
  // } else {
  //   return false;
  // }
}


function m2_api_item_append($item_or_id, $parent_or_id = null) {
  // $c_item = _m2_api_item_prepare($item_or_id);
  // $parent = $parent_or_id ? _m2_api_item_prepare($parent_or_id) : null;
  // drupal_alter('m2_item_append', $c_item, $parent);
  // if ($c_item) {
  //   $transaction = db_transaction();
  //   try {
  //  /* code */
  //     _m2_api_rebuild_disabled_grp($c_item->m2_name);
  //     return true;
  //   }
  //   catch (Exception $e) {
  //     $transaction->rollback();
  //     return false;
  //   }
  // } else {
  //   return false;
  // }
}


function m2_api_item_remove($item_or_id, $with_children = M2_WITH_CHILDREN_ON) {
  $c_item = _m2_api_item_prepare($item_or_id);
  drupal_alter('m2_item_remove', $c_item, $with_children);
  if ($c_item) {
    $transaction = db_transaction();
    try {
      $children = m2_api_item_select_children($c_item, 0, 0, M2_WITH_REMOVED_ON, M2_WITH_DISABLED_ON);
      if (count($children)) {
        if ($with_children == M2_WITH_CHILDREN_ON) db_delete('m2_items')->condition('item_id', array_keys($children), 'in')->execute();
        else {
       /* set remove date from parent to all child items at level +1 */
          db_update('m2_items')->fields(array('date_removed' => $c_item->date_removed))->condition('item_id', array_keys($children), 'in')->condition('depth', $c_item->depth + 1)->execute();
       /* move child items to level -1 */
          db_update('m2_items')->expression('depth', 'depth - 1')->condition('item_id', array_keys($children), 'in')->execute();
        }
      }
      db_delete('m2_items')->condition('item_id', $c_item->item_id)->execute();
      return true;
    }
    catch (Exception $e) {
      $transaction->rollback();
      return false;
    }
  } else {
    return false;
  }
}


/**
 * m2_api_menu_remove_to_garbage    | return bool(true) or bool(false)
 * m2_api_menu_restore_from_garbage | return bool(true) or bool(false)
 * m2_api_menu_all_clear            | return bool(true) or bool(false)
 * m2_api_item_remove_to_garbage    | return bool(true) or bool(false)
 * m2_api_item_restore_from_garbage | return bool(true) or bool(false)
 * m2_api_item_all_clear            | return bool(true) or bool(false)
 */

function m2_api_menu_remove_to_garbage($m2_name, $with_children = M2_WITH_CHILDREN_ON) {
  drupal_alter('m2_menu_remove_to_garbage', $m2_name, $with_children);
  $transaction = db_transaction();
  try {
    if ($with_children == M2_WITH_CHILDREN_ON) db_update('m2_items')->fields(array('is_removed' => 1))->condition('m2_name', $m2_name)->execute();
    db_update('m2')->fields(array('is_removed' => 1, 'date_removed' => time()))->condition('m2_name', $m2_name)->execute();
    return true;
  }
  catch (Exception $e) {
    $transaction->rollback();
    return false;
  }
}


function m2_api_menu_restore_from_garbage($m2_name, $with_children = M2_WITH_CHILDREN_ON) {
  drupal_alter('m2_menu_restore_from_garbage', $m2_name, $with_children);
  $transaction = db_transaction();
  try {
    db_update('m2')->fields(array('is_removed' => 0, 'date_removed' => 0))->condition('m2_name', $m2_name)->execute();
    if ($with_children == M2_WITH_CHILDREN_ON) db_update('m2_items')->fields(array('is_removed' => 0, 'date_removed' => 0))->condition('m2_name', $m2_name)->execute();
    return true;
  }
  catch (Exception $e) {
    $transaction->rollback();
    return false;
  }
}


function m2_api_menu_all_clear($owner_uid = null) {
  drupal_alter('m2_menu_all_clear', $owner_uid);
  $transaction = db_transaction();
  try {
    $menus = m2_api_menu_select_all_removed_or_with_removed_items($owner_uid, M2_WITH_SYSTEM_ON);
    if (count($menus)) {
      foreach ($menus as $c_menu) {
        if ($c_menu->is_removed) {
          db_delete('m2_items')->condition('m2_name', $c_menu->m2_name)->execute();
          db_delete('m2')->condition('m2_name', $c_menu->m2_name)->execute();
        } else {
          db_delete('m2_items')->condition('is_removed', 1)->condition('m2_name', $c_menu->m2_name)->execute();
        }
      }
    }
    return true;
  }
  catch (Exception $e) {
    $transaction->rollback();
    return false;
  }
}


function m2_api_item_remove_to_garbage($item_or_id, $with_children = M2_WITH_CHILDREN_ON) {
  $c_item = _m2_api_item_prepare($item_or_id);
  drupal_alter('m2_item_remove_to_garbage', $c_item, $with_children);
  if ($c_item) {
    $transaction = db_transaction();
    try {
      $children = m2_api_item_select_children($c_item, 0, 0, M2_WITH_REMOVED_ON, M2_WITH_DISABLED_ON);
      if (count($children)) {
        if ($with_children == M2_WITH_CHILDREN_ON) db_update('m2_items')->fields(array('is_removed' => 1, 'date_removed' => 0))->condition('item_id', array_keys($children), 'in')->execute();
        else db_update('m2_items')->expression('depth', 'depth - 1')->condition('item_id', array_keys($children), 'in')->execute();
      }
      db_update('m2_items')->fields(array('is_removed' => 1, 'date_removed' => time()))->condition('item_id', $c_item->item_id)->execute();
      return true;
    }
    catch (Exception $e) {
      $transaction->rollback();
      return false;
    }
  } else {
    return false;
  }
}


function m2_api_item_restore_from_garbage($item_or_id, $with_children = M2_WITH_CHILDREN_ON) {
  $c_item = _m2_api_item_prepare($item_or_id);
  drupal_alter('m2_item_restore_from_garbage', $c_item, $with_children);
  if ($c_item) {
    $transaction = db_transaction();
    try {
      $parent = m2_api_item_select_parent($c_item);
      if ($parent && $parent->is_removed == true) return false; /* can not restore items if parent was removed */
      $children = m2_api_item_select_children($c_item, 0, 0, M2_WITH_REMOVED_ON, M2_WITH_DISABLED_ON);
      if (count($children)) {
        if ($with_children == M2_WITH_CHILDREN_ON) db_update('m2_items')->fields(array('is_removed' => 0, 'date_removed' => 0))->condition('item_id', array_keys($children), 'in')->execute();
        else db_update('m2_items')->expression('depth', 'depth - 1')->condition('item_id', array_keys($children), 'in')->execute();
      }
      db_update('m2_items')->fields(array('is_removed' => 0, 'date_removed' => 0))->condition('item_id', $c_item->item_id)->execute();
      return true;
    }
    catch (Exception $e) {
      $transaction->rollback();
      return false;
    }
  } else {
    return false;
  }
}


function m2_api_item_all_clear($m2_name) {
  drupal_alter('m2_item_all_clear', $m2_name);
  try {
    db_delete('m2_items')->condition('is_removed', 1)->condition('m2_name', $m2_name)->execute();
    return true;
  }
  catch (Exception $e) {
    return false;
  }
}


/**
 * _m2_api_item_prepare         | return object(stdClass) or bool(false)
 * _m2_api_insert_hole          | return bool(true) or bool(false)
 * _m2_api_rebuild_disabled_grp | return null
 */

function _m2_api_item_prepare($c_item) {
  if (isset($c_item->item_id)) {
    return $c_item;
  } else if (is_numeric($c_item)) {
    return m2_api_item_select($c_item);
  } else {
    return false;
  }
}


function _m2_api_insert_hole($m2_name, $from_weight, $lenght = 1) {
  $query = db_update('m2_items');
  $query->expression('weight', "weight + $lenght");
  $query->condition('m2_name', $m2_name);
  $query->condition('weight', $from_weight, '>');
  return $query->execute();
}


function _m2_api_rebuild_disabled_grp($m2_name) {
  db_update('m2_items')->fields(array('is_disabled_grp' => 0))->condition('m2_name', $m2_name)->execute();
  $disabled_items = m2_api_item_select_universal(array(array('field' => 'm2_name', 'value' => $m2_name), array('field' => 'is_disabled', 'value' => 1)));
  foreach ($disabled_items as $c_item) {
    db_update('m2_items')->fields(array('is_disabled_grp' => 1))->condition('item_id', $c_item->item_id)->execute();
    $children = m2_api_item_select_children($c_item, 0, 0, M2_WITH_REMOVED_ON, M2_WITH_DISABLED_ON);
    if (count($children)) {
      db_update('m2_items')->fields(array('is_disabled_grp' => 1))->condition('item_id', array_keys($children), 'in')->execute();
    }
  }
}


/**
 * m2_api_mark_active_trail            | return array(N) or array()
 * m2_api_flat_to_tree                 | return array(N) or array()
 * m2_api_flat_tree_sort_by_weight     | return -1, 0, 1
 * m2_api_flat_tree_sort_by_new_weight | return -1, 0, 1
 */

function m2_api_mark_active_trail($flat_tree, $m2_name) {
  $active_item = m2_api_item_select_active($m2_name);
  if ($active_item) {
    $parents = m2_api_item_select_parent_all($active_item);
    foreach (array_keys($parents) as $parent_id) {
      if (isset($flat_tree[$parent_id])) {
        $flat_tree[$parent_id]->is_active_trail = true;
      }
    }
  }
  return $flat_tree;
}


function m2_api_flat_to_tree($flat_tree) {
  $tree = array();
  foreach ($flat_tree as $item) {
    $item = clone $item;
    $item->children = array();
    if ($item->depth == 0) {$tree[$item->item_id] = $item; $p0 = &$item->children;}
    if ($item->depth >= 1) {${'p'.($item->depth-1)}[$item->item_id] = $item; ${'p'.$item->depth} = &$item->children;}
  }
  return $tree;
}


function m2_api_flat_tree_sort_by_weight($a, $b) {
  if ($a->weight == $b->weight) {
    return 0;
  }
  return ($a->weight < $b->weight) ? -1 : 1;
}


function m2_api_flat_tree_sort_by_new_weight($a, $b) {
  if ($a->new_weight == $b->new_weight) {
    return 0;
  }
  return ($a->new_weight < $b->new_weight) ? -1 : 1;
}


/**
 * theme_m2_tree_simple           | return string
 * theme_m2_tree_levels_recursive | return string
 * theme_m2_tree                  | return string
 */

function theme_m2_tree_simple($vars) {
  extract($vars);
  $output = '';
  foreach ($flat_tree as $item_id => $c_item) {
    $output.= str_repeat(M2_HTM_EM_DASH, $c_item->depth + 1).' '.$c_item->title;
    if ($c_item != end($flat_tree)) {
      $output.= M2_HTM_TAG_NEW_LINE;
    }
  }
  return theme('m2_content_wrapper', array(
    'class'  => array('tree-simple'),
    'markup' => $output,
  ));
}


function theme_m2_tree_levels_recursive($vars) {
  extract($vars);
  $output = '';
  $rendered_num = 0;
  if (count($tree)) {
    $output.= '<ul>';
    foreach ($tree as $item) {
      $class = array();
      $class[] = $item->url ? 'type-link' : 'type-text';
      $link_options = isset($item->link_options) ? $item->link_options : array();
      if (isset($link_options['attributes']) == false) $link_options['attributes'] = array();
      if (isset($item->class) && strlen($item->class)) $class[] = $item->class; /* $item->class is the string */
      if (isset($item->is_expanded) && $item->is_expanded) $class[] = 'is-expanded';
      if (isset($item->is_disabled) && $item->is_disabled) $class[] = 'is-disabled';
      if (isset($item->is_removed) && $item->is_removed) $class[] = 'is-removed';
      if (count($item->children)) $class[] = 'has-children';
      if (isset($item->is_active_trail)) $class[] = 'active-trail';
      if ($_GET['q'] && $item->url == $_GET['q']) $class[] = 'active';
      if ($item == reset($tree)) $class[] = 'first';
      if ($item == end($tree)) $class[] = 'last';
      if (count($tree) == 1) $class[] = 'single';
      if ($item->description) $link_options['attributes'] = $link_options['attributes'] + array('title' => $item->description);
      $wrapper_1 = isset($item->wr_1) ? $item->wr_1 : '';
      $wrapper_2 = isset($item->wr_2) ? $item->wr_2 : '';
      $wrapper_3 = isset($item->wr_3) ? $item->wr_3 : '';
      $wrapper_4 = isset($item->wr_4) ? $item->wr_4 : '';
      $output.= '<li'.(count($class) ? ' class="'.implode(' ', $class).'"' : '').' rel="'.$item->item_id.'">'.
                  ($wrapper_1).
                  ($item->url ? l($wrapper_2.($item->title != '' ? t($item->title) : t(M2_TXT_NA)).$wrapper_3, $item->url, $link_options + array('html' => true)) :
                                  $wrapper_2.($item->title != '' ? t($item->title) : t(M2_TXT_NA)).$wrapper_3).
                  ($wrapper_4).
                  (count($item->children) ? theme_m2_tree_levels_recursive(array('tree' => $item->children, 'options' => $options)) : '').
                '</li>';
      $rendered_num++;
    }
    $output.= '</ul>';
  }
  return $rendered_num ? $output : '';
}


function theme_m2_tree($vars) {
  $m2_module_path = drupal_get_path('module', 'm2');
  drupal_add_css("$m2_module_path/css/m2_Tree.css");
  drupal_add_css("$m2_module_path/css/m2_Tree-ie-7.css", array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 7', '!IE' => false), 'preprocess' => false));
  drupal_add_library('system', 'jquery.cookie');
  drupal_add_js("$m2_module_path/js/m2_Shared.js");
  drupal_add_js("$m2_module_path/js/m2_Tree.js");
  extract($vars);
  if ($m2_name) {
    $flat_tree = m2_api_mark_active_trail($flat_tree, $m2_name);
  }
  foreach ($flat_tree as $item) {
    $item->wr_1 = '<div class="item-value">'.(isset($item->wr_1) ? $item->wr_1 : '');
    $item->wr_4 = (isset($item->wr_4) ? $item->wr_4 : '').'</div>';
  }
  return theme('m2_content_wrapper', array(
    'class'  => array('m2-tree', 'clearfix'),
    'rel'    => $m2_name,
    'markup' => theme('m2_tree_levels_recursive', array('tree' => m2_api_flat_to_tree($flat_tree))),
  ));
}

