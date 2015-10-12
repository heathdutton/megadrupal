<?php

require_once 'm2.constants.inc';

/**
 * m2_admin_garbage_tab
 -- forms ---------------------------------
 * m2_admin_garbage_menu_manage_form
 * m2_admin_garbage_item_all_manage_form
 * m2_admin_garbage_menu_all_clear_form
 -- forms confirm -------------------------
 * m2_admin_garbage_menu_remove_confirm_form
 * m2_admin_garbage_menu_restore_confirm_form
 * m2_admin_garbage_item_remove_confirm_form
 * m2_admin_garbage_item_restore_confirm_form
 * m2_admin_garbage_clear_menus_confirm_form
 * m2_admin_garbage_clear_items_confirm_form
 */


/**
 * m2_admin_garbage_tab
 */

function m2_admin_garbage_tab($m2_name = '') {
  $menu_list = m2_api_menu_select_all_removed_or_with_removed_items(user_access('administer menu2') || user_access('manage all menu2') ? null : $GLOBALS['user']->uid, variable_get('m2_show_system_menus_for_manage', false));
  if ($m2_name == '' && count($menu_list)) {
    drupal_goto(M2_PATH_ADM_MENU2_TAB_GARBAGE.'/'.key($menu_list));
  }
  drupal_set_title(t('Administer Menu2'));
  $output = (count($menu_list) ? m2_form_render('m2_admin_garbage_menu_all_clear_form', $m2_name) : '').
            m2_form_theme_menu_list($menu_list, M2_PATH_ADM_MENU2_TAB_GARBAGE);
  if ($m2_name == '') {
    $output.= theme('m2_content_wrapper', array('class' => array('m2-menu-garbage-manage'), 'markup' =>
      theme('m2_content_wrapper', array('class' => array('clearfix'))).
      theme('m2_content_wrapper', array('class' => array('message-no-items-available'), 'markup' => t('garbage is empty')))
    ));
  } else if ($m2_name && isset($menu_list[$m2_name])) {
    $m2_info = m2_api_menu_select_by_name($m2_name);
    $output.= theme('m2_content_wrapper', array('class' => array('m2-menu-garbage-manage'), 'markup' =>
      theme('m2_title', array('type' => 'h2', 'class' => array('m2-menu-title'), 'title' => $m2_info->title, 'markup' => t('Menu: %name', array('%name' => truncate_utf8($m2_info->title, 50, false, true))))).
      theme('m2_meta_info', array('owner_name' => $m2_info->owner_name, 'date_created' => $m2_info->date_created)).
      theme('m2_title', array('type' => 'h3', 'class' => array('m2-menu-items-title'), 'title' => t('Items'), 'markup' => t('Items'))).
      m2_form_render('m2_admin_garbage_item_all_manage_form', $m2_name).
      m2_form_render('m2_admin_garbage_menu_manage_form', $m2_name)
    ));
  } else {
    drupal_goto(M2_PATH_ADM_MENU2_TAB_GARBAGE);
  }
  return theme('m2_content_wrapper', array('class' => array('m2-tab', 'm2-garbage-tab', 'clearfix'), 'markup' => $output));
}


/**
 * m2_admin_garbage_menu_manage_form
 * m2_admin_garbage_menu_manage_form_submit
 */

function m2_admin_garbage_menu_manage_form($form, &$form_state, $m2_name) {
  $module_path = drupal_get_path('module', 'm2');
  drupal_add_css("$module_path/css/m2_Tree-manage.css");
  $m2_info = m2_api_menu_select_by_name($m2_name);
  if ($m2_info->is_removed) {
    $form['button_menu_restore'] = array(
      '#type' => 'submit',
      '#value' => t('Restore menu'),
    );
    $form['button_menu_remove'] = array(
      '#type' => 'submit',
      '#value' => t('Remove menu'),
    );
  } else if ($m2_info->removed_items_num > 0) {
    $form['button_menu_clear'] = array(
      '#type' => 'submit',
      '#value' => t('Clear Menu'),
      '#attributes' => array('title' => t('Clear all removed Items in this Menu')),
    );
  }
  $form['#m2_name'] = $m2_name;
  drupal_add_js("$module_path/js/m2_Shared.js");
  return $form;
}


function m2_admin_garbage_menu_manage_form_submit($form, &$form_state) {
  $m2_name = $form['#m2_name'];
  if ($form_state['clicked_button']['#id'] == 'edit-button-menu-restore') {
    drupal_goto(M2_PATH_ADM_MENU2_RESTORE_FROM_GARBAGE."/$m2_name");
  }
  if ($form_state['clicked_button']['#id'] == 'edit-button-menu-remove') {
    drupal_goto(M2_PATH_ADM_MENU2_REMOVE."/$m2_name");
  }
  if ($form_state['clicked_button']['#id'] == 'edit-button-menu-clear') {
    drupal_goto(M2_PATH_ADM_MENU2_CLEAR_ITEMS."/$m2_name");
  }
}


/**
 * m2_admin_garbage_item_all_manage_form
 */

function m2_admin_garbage_item_all_manage_form($form, &$form_state, $m2_name, $params = array()) {
  $destination = isset($params['destination']) ? $params['destination'] : '';
  $m2_info = m2_api_menu_select_by_name($m2_name);
  $flat_tree = m2_api_item_select_by_menu_all($m2_name, 0, 0, M2_WITH_REMOVED_ON, M2_WITH_DISABLED_ON);
  if (count($flat_tree)) {
    if ($m2_info->is_removed == false) {
      $c_rest_depth = -1;
      foreach ($flat_tree as $item) {
        $links = array();
        if ($item->is_removed == 0) $c_rest_depth = -1;
        if ($item->is_removed) {
          if ($c_rest_depth == -1 || $item->depth <= $c_rest_depth) {
            $c_rest_depth = $item->depth;
            $links[] = l(t('restore'), M2_PATH_ADM_MENU2_ITEM_RESTORE_FROM_GARBAGE."/$item->item_id", array('query' => $destination, 'attributes' => array('class' => array('m2-link-restore'))));
          }
        }
        if ($item->is_removed) $links[] = l(t('remove'), M2_PATH_ADM_MENU2_ITEM_REMOVE."/$item->item_id", array('query' => $destination, 'attributes' => array('class' => array('m2-link-remove'))));
        $states = array(t('type = !type', array('!type' => $item->url ? t('document') : t('folder'))));
        if ($item->is_disabled) $states[]= t('disabled');
        if ($item->is_removed) $states[]= t('removed');
        $item->wr_1 = '<div class="item item-type-'.($item->url ? 'link' : 'text').'">';
        $item->wr_1.= theme('m2_content_wrapper', array('class' => array('item-links'), 'markup' => implode('', $links)));
        $item->wr_1.= '<div class="icon" '.(count($states) ? ' title="'.implode(' | ', $states).'"' : '').'></div>';
        $item->wr_1.= '<div class="item-value">';
        $item->wr_4 = '</div>';
        $item->wr_4.= '</div>';
        $item->link_options = array('attributes' => array('target' => '_blank'));
      }
      $form['m2_tree'] = array(
        '#markup' =>
          theme('m2_content_wrapper', array('class' => array('m2-items-manage-header'), 'markup' => '&nbsp;')).
          theme('m2_content_wrapper', array('class' => array('m2-items-manage', 'm2-items-manage-disabled'), 'markup' => theme('m2_tree_levels_recursive', array('tree' => m2_api_flat_to_tree($flat_tree))))),
      );
    } else {
      foreach ($flat_tree as $item) {
        $item->wr_1 = '<div class="item item-type-'.($item->url ? 'link' : 'text').'">';
        $item->wr_1.= '<div class="icon"></div>';
        $item->wr_1.= '<div class="item-value">';
        $item->wr_4 = '</div>';
        $item->wr_4.= '</div>';
        $item->link_options = array('attributes' => array('target' => '_blank'));
      }
      $form['m2_tree'] = array(
        '#markup' =>
          theme('m2_content_wrapper', array('class' => array('m2-items-manage-header'), 'markup' => '&nbsp;')).
          theme('m2_content_wrapper', array('class' => array('m2-items-manage', 'm2-items-manage-removed'), 'markup' => theme('m2_tree_levels_recursive', array('tree' => m2_api_flat_to_tree($flat_tree))))),
      );
    }
  } else {
    $form['m2_tree'] = array(
      '#markup' => theme('m2_content_wrapper', array('class' => array('message-no-items-available'), 'markup' => t('no removed items'))),
    );
  }
  return $form;
}


/**
 * m2_admin_garbage_menu_all_clear_form
 * m2_admin_garbage_menu_all_clear_form_submit
 */

function m2_admin_garbage_menu_all_clear_form($form, &$form_state, $m2_name) {
  $form['button_menu_all_clear'] = array(
    '#type' => 'submit',
    '#value' => t('Clear Garbage'),
    '#attributes' => array('title' => t('Clear all removed Menu and all removed Items')),
  );
  $form['#m2_name'] = $m2_name;
  return $form;
}


function m2_admin_garbage_menu_all_clear_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-button-menu-all-clear') {
    $m2_name = $form['#m2_name'];
    drupal_goto(M2_PATH_ADM_MENU2_CLEAR_MENUS."/$m2_name");
  }
}


/**
 * m2_admin_garbage_menu_remove_confirm_form
 * m2_admin_garbage_menu_remove_confirm_form_submit
 */

function m2_admin_garbage_menu_remove_confirm_form($form, &$form_state, $m2_name) {
  $m2_info = m2_api_menu_select_by_name($m2_name);
  if ($m2_info && isset($m2_info->m2_name)) {
    $form['#m2_name'] = $m2_name;
    $form = m2_form_confirm($form, array(
      'title' => t('Remove Menu with name = %m2_name to Garbage?', array('%m2_name' => truncate_utf8($m2_name, 50, false, true))),
      'path_ok' => M2_PATH_ADM_MENU2_TAB_MANAGE,
      'path_cancel' => M2_PATH_ADM_MENU2_TAB_MANAGE."/$m2_name",
      'text_ok' => t('Remove to Garbage'),
      'description_ok' => '',
    ));
    return $form;
  } else {
    drupal_not_found();
    exit();
  }
}


function m2_admin_garbage_menu_remove_confirm_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-button-ok') {
    $m2_name = $form['#m2_name'];
    $result = m2_api_menu_remove_to_garbage($m2_name, M2_WITH_CHILDREN_OFF);
    if ($result) {
      m2_report('Menu2 was removed to Garbage.!nl'.
                'Menu2 name = %m2_name', array('%m2_name' => $m2_name), 'm2_admin');
    } else {
      m2_report('Can not remove Menu2 to Garbage!!nl'.
                'Menu2 name = %m2_name', array('%m2_name' => $m2_name), 'm2_admin', 'error', WATCHDOG_ERROR);
    }
  }
}


/**
 * m2_admin_garbage_menu_restore_confirm_form
 * m2_admin_garbage_menu_restore_confirm_form_submit
 */

function m2_admin_garbage_menu_restore_confirm_form($form, &$form_state, $m2_name) {
  $m2_info = m2_api_menu_select_by_name($m2_name);
  if ($m2_info && isset($m2_info->m2_name)) {
    $form['#m2_name'] = $m2_name;
    $form = m2_form_confirm($form, array(
      'title' => t('Restore Menu with name = %m2_name from Garbage?', array('%m2_name' => truncate_utf8($m2_name, 50, false, true))),
      'path_ok' => M2_PATH_ADM_MENU2_TAB_GARBAGE."/$m2_name",
      'path_cancel' => M2_PATH_ADM_MENU2_TAB_GARBAGE."/$m2_name",
      'text_ok' => t('Restore from Garbage'),
      'description_ok' => '',
    ));
    return $form;
  } else {
    drupal_not_found();
    exit();
  }
}


function m2_admin_garbage_menu_restore_confirm_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-button-ok') {
    $m2_name = $form['#m2_name'];
    $result = m2_api_menu_restore_from_garbage($m2_name, M2_WITH_CHILDREN_OFF);
    if ($result) {
      m2_report('Menu2 was restored from Garbage.!nl'.
                'Menu2 name = %m2_name', array('%m2_name' => $m2_name), 'm2_admin');
    } else {
      m2_report('Can not restore Menu2 from Garbage!!nl'.
                'Menu2 name = %m2_name', array('%m2_name' => $m2_name), 'm2_admin', 'error', WATCHDOG_ERROR);
    }
  }
}


/**
 * m2_admin_garbage_item_remove_confirm_form
 * m2_admin_garbage_item_remove_confirm_form_submit
 */

function m2_admin_garbage_item_remove_confirm_form($form, &$form_state, $item_id) {
  $c_item = m2_api_item_select($item_id);
  if ($c_item && isset($c_item->item_id)) {
    $children = m2_api_item_select_children($c_item->item_id, null, $c_item->depth + 1, M2_WITH_REMOVED_OFF, M2_WITH_DISABLED_ON);
    $form['with_children'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove related Items?'),
      '#default_value' => true,
      '#access' => (bool)count($children),
      '#description' => theme('m2_tree_simple', array('flat_tree' => $children)),
    );
    $form['#c_item'] = $c_item;
    $form['#children'] = $children;
    $form = m2_form_confirm($form, array(
      'title' => t('Remove Item with ID = %item_id to Garbage?', array('%item_id' => $c_item->item_id)),
      'path_ok' => M2_PATH_ADM_MENU2_TAB_MANAGE."/$c_item->m2_name",
      'path_cancel' => M2_PATH_ADM_MENU2_TAB_MANAGE."/$c_item->m2_name",
      'text_ok' => t('Remove to Garbage'),
      'description_ok' => '',
    ));
    return $form;
  } else {
    drupal_not_found();
    exit();
  }
}


function m2_admin_garbage_item_remove_confirm_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-button-ok') {
    $c_item = $form['#c_item'];
    $children = $form['#children'];
    $with_children = $form_state['values']['with_children'];
    $result = m2_api_item_remove_to_garbage($c_item->item_id, $with_children);
    if ($result) {
      $children_id = $children && $with_children ? implode(', ', array_keys($children)) : t(M2_TXT_NA);
      m2_report('Item was removed to Garbage.!nl'.
                'Menu2 name = %m2_name!nl'.
                'Item ID = %item_id!nl'.
                'Children items ID = %children_id', array('%m2_name' => $c_item->m2_name, '%item_id' => $c_item->item_id, '%children_id' => $children_id), 'm2_admin');
    } else {
      m2_report('Can not remove Item to Garbage!!nl'.
                'Menu2 name = %m2_name!nl'.
                'Item ID = %item_id', array('%m2_name' => $c_item->m2_name, '%item_id' => t(M2_TXT_NA)), 'm2_admin', 'error', WATCHDOG_ERROR);
    }
  }
}


/**
 * m2_admin_garbage_item_restore_confirm_form
 * m2_admin_garbage_item_restore_confirm_form_submit
 */

function m2_admin_garbage_item_restore_confirm_form($form, &$form_state, $item_id) {
  $c_item = m2_api_item_select($item_id);
  if ($c_item && isset($c_item->item_id)) {
    $children = m2_api_item_select_children($c_item->item_id, null, $c_item->depth + 1, M2_WITH_REMOVED_ON, M2_WITH_DISABLED_ON);
    $form['with_children'] = array(
      '#type' => 'checkbox',
      '#title' => t('Restore related Items?'),
      '#default_value' => true,
      '#access' => (bool)count($children),
      '#description' => theme('m2_tree_simple', array('flat_tree' => $children)),
    );
    $form['#c_item'] = $c_item;
    $form['#children'] = $children;
    $form = m2_form_confirm($form, array(
      'title' => t('Restore Item with ID = %item_id from Garbage?', array('%item_id' => $c_item->item_id)),
      'path_ok' => M2_PATH_ADM_MENU2_TAB_GARBAGE."/$c_item->m2_name",
      'path_cancel' => M2_PATH_ADM_MENU2_TAB_GARBAGE."/$c_item->m2_name",
      'text_ok' => t('Restore from Garbage'),
      'description_ok' => '',
    ));
    return $form;
  } else {
    drupal_not_found();
    exit();
  }
}


function m2_admin_garbage_item_restore_confirm_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-button-ok') {
    $c_item = $form['#c_item'];
    $children = $form['#children'];
    $with_children = $form_state['values']['with_children'];
    $result = m2_api_item_restore_from_garbage($c_item->item_id, $with_children);
    if ($result) {
      $children_id = $children && $with_children ? implode(', ', array_keys($children)) : t(M2_TXT_NA);
      m2_report('Item was restored from Garbage.!nl'.
                'Menu2 name = %m2_name!nl'.
                'Item ID = %item_id!nl'.
                'Children items ID = %children_id', array('%m2_name' => $c_item->m2_name, '%item_id' => $c_item->item_id, '%children_id' => $children_id), 'm2_admin');
    } else {
      m2_report('Can not restore Item from Garbage!!nl'.
                'Menu2 name = %m2_name!nl'.
                'Item ID = %item_id', array('%m2_name' => $c_item->m2_name, '%item_id' => t(M2_TXT_NA)), 'm2_admin', 'error', WATCHDOG_ERROR);
    }
  }
}


/**
 * m2_admin_garbage_clear_menus_confirm_form
 * m2_admin_garbage_clear_menus_confirm_form_submit
 */
 
function m2_admin_garbage_clear_menus_confirm_form($form, &$form_state, $m2_name) {
  return m2_form_confirm($form, array(
    'title' => t('Clear Garbage?'),
    'path_ok' => M2_PATH_ADM_MENU2_TAB_GARBAGE,
    'path_cancel' => M2_PATH_ADM_MENU2_TAB_GARBAGE."/$m2_name",
    'text_ok' => t('Clear Garbage'),
  ));
}


function m2_admin_garbage_clear_menus_confirm_form_submit($form, &$form_state) {
  $result = m2_api_menu_all_clear(user_access('administer menu2') ||
                                  user_access('manage all menu2') ? null : $GLOBALS['user']->uid);
  if ($result) {
    m2_report('Garbage cleared.', null, 'm2_admin');
  } else {
    m2_report('Can not clear Garbage!', null, 'm2_admin', 'error', WATCHDOG_ERROR);
  }
}


/**
 * m2_admin_garbage_clear_items_confirm_form
 * m2_admin_garbage_clear_items_confirm_form_submit
 */

function m2_admin_garbage_clear_items_confirm_form($form, &$form_state, $m2_name) {
  $m2_info = m2_api_menu_select_by_name($m2_name);
  if ($m2_info && isset($m2_info->m2_name)) {
    $form['#m2_name'] = $m2_name;
    $form = m2_form_confirm($form, array(
      'title' => t('Clear removed Items in Menu with name = %m2_name?', array('%m2_name' => $m2_name)),
      'path_ok' => M2_PATH_ADM_MENU2_TAB_GARBAGE."/$m2_name",
      'path_cancel' => M2_PATH_ADM_MENU2_TAB_GARBAGE."/$m2_name",
      'text_ok' => t('Clear removed Items'),
    ));
    return $form;
  } else {
    drupal_not_found();
    exit();
  }
}


function m2_admin_garbage_clear_items_confirm_form_submit($form, &$form_state) {
  $m2_name = $form['#m2_name'];
  $result = m2_api_item_all_clear($m2_name);
  if ($result) {
    m2_report('Menu2 was cleared.!nl'.
              'Menu2 name = %m2_name', array('%m2_name' => $m2_name), 'm2_admin');
  } else {
    m2_report('Can not clear Menu2!!nl'.
              'Menu2 name = %m2_name', array('%m2_name' => $m2_name), 'm2_admin', 'error', WATCHDOG_ERROR);
  }
}


