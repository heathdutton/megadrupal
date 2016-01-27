<?php

require_once 'm2.constants.inc';

/**
 * m2_admin_manage_tab
 -- forms ---------------------------------
 * m2_admin_menu_manage_form
 * m2_admin_menu_insert_form
 * m2_admin_item_all_manage_form
 * m2_admin_item_manage_form
 * m2_admin_item_insert_form
 * m2_admin_item_on_off_page
 -- forms confirm -------------------------
 * m2_admin_menu_remove_confirm_form
 * m2_admin_item_remove_confirm_form
 */


/**
 * m2_admin_manage_tab
 */

function m2_admin_manage_tab($m2_name = '') {
  $menu_list = m2_api_menu_select_all(user_access('administer menu2') || user_access('manage all menu2') ? null : $GLOBALS['user']->uid, variable_get('m2_show_system_menus_for_manage', true));
  if ($m2_name == '' && count($menu_list)) {
    drupal_goto(M2_PATH_ADM_MENU2_TAB_MANAGE.'/'.key($menu_list));
  }
  drupal_set_title(t('Administer Menu2'));
  $output = m2_form_render('m2_admin_menu_insert_form').
            m2_form_theme_menu_list($menu_list, M2_PATH_ADM_MENU2_TAB_MANAGE);
  if ($m2_name == '') {
    $output.= theme('m2_content_wrapper', array('class' => array('m2-menu-manage'), 'markup' =>
      theme('m2_content_wrapper', array('class' => array('clearfix'))).
      theme('m2_content_wrapper', array('class' => array('message-no-items-available'), 'markup' => t('no menus! add new one')))
    ));
  } else if ($m2_name && isset($menu_list[$m2_name])) {
      $m2_info = m2_api_menu_select_by_name($m2_name);
      $output.= theme('m2_content_wrapper', array('class' => array('m2-menu-manage'), 'markup' =>
        theme('m2_content_wrapper', array('class' => array('clearfix'))).
        theme('m2_title', array('type' => 'h2', 'class' => array('m2-menu-title'), 'title' => $m2_info->title, 'markup' => t('Menu: %name', array('%name' => truncate_utf8($m2_info->title, 50, false, true))))).
        theme('m2_content_wrapper', array('type' => 'span', 'class' => array('m2-menu-manage-links'), 'markup' =>
          l(t('edit'), M2_PATH_ADM_MENU2_MANAGE."/$m2_name", array('attributes' => array('class' => array('link-menu-manage')))).
          l(t('remove to garbage'), M2_PATH_ADM_MENU2_REMOVE_TO_GARBAGE."/$m2_name", array('attributes' => array('class' => array('link-menu-remove'))))
        )).
        theme('m2_meta_info', array('owner_name' => $m2_info->owner_name, 'date_created' => $m2_info->date_created)).
        theme('m2_title', array('type' => 'h3', 'class' => array('m2-menu-items-title'), 'title' => t('Items'), 'markup' => t('Items'))).
        m2_form_render('m2_admin_item_all_manage_form', $m2_name).
        m2_form_render('m2_admin_item_insert_form', $m2_name)
      ));
  } else {
    drupal_goto(M2_PATH_ADM_MENU2_TAB_MANAGE);
  }
  drupal_add_js('var m2_name = '.($m2_name ? "'$m2_name'" : 'null').';', 'inline');
  drupal_add_library('system', 'jquery.cookie');
  return theme('m2_content_wrapper', array('class' => array('m2-tab', 'm2-manage-tab', 'clearfix'), 'markup' => $output));
}


/**
 * m2_admin_menu_manage_form
 * m2_admin_menu_manage_form_submit
 */

function m2_admin_menu_manage_form($form, &$form_state, $m2_name) {
  require_once 'm2.admin.tab.security.inc';
  $m2_info = m2_api_menu_select_by_name($m2_name);
  if ($m2_info) {
    $root_info = user_load(1);
    $super_users = array(1 => $root_info->name) + _m2_sec_get_users_by_roles(array_keys(_m2_sec_get_roles_by_permissions(array_keys(m2_permission()))));
    $form['cur_m2_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Menu Title'),
      '#default_value' => $m2_info->title,
      '#maxlength' => M2_MENU_TITLE_MAX_LENGTH,
      '#description' => t('Maximum !num characters.', array('!num' => M2_MENU_TITLE_MAX_LENGTH)),
      '#required' => true,
    );
    $form['owner_uid'] = array(
      '#type' => 'select',
      '#title' => t('Owner'),
      '#options' => $super_users,
      '#default_value' => $m2_info->owner_uid,
      '#description' => t('Shown only users with admin or manager roles.'),
      '#required' => true,
    );
    $form['language'] = array(
      '#type' => 'select',
      '#title' => t('Language'),
      '#options' => array(LANGUAGE_NONE => t('Neutral')) + (function_exists('locale_language_list') ? locale_language_list('name', true) : array()),
      '#default_value' => $m2_info->language,
      '#description' => t(M2_TXT_UNDER_CONSTRUCTION),
      '#required' => true,
      '#disabled' => true,
    );
    $form['is_disabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('is Disabled?'),
      '#default_value' => $m2_info->is_disabled,
    );
    $form['button_settings_save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    $form['button_cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#validate' => array('m2_admin_menu_manage_form_validate_cancel'),
    );
    $form['#m2_info'] = $m2_info;
    $form['#path_ok'] = M2_PATH_ADM_MENU2_TAB_MANAGE."/$m2_name";
    $form['#path_cancel'] = M2_PATH_ADM_MENU2_TAB_MANAGE."/$m2_name";
    return $form;
  } else {
    drupal_not_found();
    exit();
  }
}


function m2_admin_menu_manage_form_validate_cancel($form, &$form_state) {
 /* unset error message when clicked to cancel button with empty required fileds */
    unset($_SESSION['messages']['error']);
 /* go to destination in url or #path_cancel by default */
    drupal_goto($form['#path_cancel']);
}


function m2_admin_menu_manage_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-button-settings-save') {
    $m2_info = $form['#m2_info'];
    $changes = array();
    if ($form_state['values']['cur_m2_title'] != $m2_info->title)       $changes['title']       = $form_state['values']['cur_m2_title'];
    if ($form_state['values']['owner_uid']    != $m2_info->owner_uid)   $changes['owner_uid']   = $form_state['values']['owner_uid'];
    if ($form_state['values']['language']     != $m2_info->language)    $changes['language']    = $form_state['values']['language'];
    if ($form_state['values']['is_disabled']  != $m2_info->is_disabled) $changes['is_disabled'] = $form_state['values']['is_disabled'];
    if (count($changes)) {
      $result = m2_api_menu_update_values($m2_info->m2_name, $changes);
      if ($result) {
        m2_report('Menu2 was updated.!nl'.
                  'Menu2 name = %m2_name!nl'.
                  '!changes', array('%m2_name' => $m2_info->m2_name, '!changes' => m2_report_changes_render($changes)), 'm2_admin');
      } else {
        m2_report('Can not update Menu2!!nl'.
                  'Menu2 name = %m2_name!nl'.
                  '!changes', array('%m2_name' => $m2_info->m2_name, '!changes' => m2_report_changes_render($changes)), 'm2_admin', 'error', WATCHDOG_ERROR);
      }
    }
    drupal_goto($form['#path_ok']);
  }
}


/**
 * m2_admin_menu_insert_form
 * m2_admin_menu_insert_form_submit
 */

function m2_admin_menu_insert_form($form, &$form_state) {
  $form['#attributes']['class'] = array('form-collapsed');
  $form['form_title'] = array(
    '#markup' => theme('m2_title', array('type' => 'div', 'class' => array('collapsed-title'), 'markup' => t('add new menu'))),
  );
  $form['collapsed_data'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('collapsed-data', 'm2-machine-name')),
  );
  $form['collapsed_data']['new_m2_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu Title'),
    '#maxlength' => M2_MENU_TITLE_MAX_LENGTH,
    '#description' => t('Maximum !num characters.', array('!num' => M2_MENU_TITLE_MAX_LENGTH)),
    '#attributes' => array('class' => array('m2-machine-name-src')),
    '#required' => true,
  );
  $form['collapsed_data']['m2_name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine-readable name'),
    '#required' => true,
    '#maxlength' => M2_MENU_NAME_MAX_LENGTH,
    '#machine_name' => array('exists' => 'm2_api_menu_name_check_exists'),
    '#attributes' => array('class' => array('m2-machine-name-dst')),
    '#description' => t('A unique machine-readable name. Can only contain lowercase letters, numbers, and underscores.').' '.
                      t('Maximum !num characters.', array('!num' => M2_MENU_NAME_MAX_LENGTH)),
  );
  $form['collapsed_data']['button_menu_add'] = array(
    '#type' => 'submit',
    '#value' => t('Add new Menu'),
  );
  return $form;
}


function m2_admin_menu_insert_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-button-menu-add') {
    $m2_title = $form_state['values']['new_m2_title'];
    $m2_name = $form_state['values']['m2_name'];
    $result = m2_api_menu_insert($m2_name, array('title' => $m2_title, 'owner_uid' => $GLOBALS['user']->uid));
    if ($result) {
      m2_report('Menu2 was added.!nl'.
                'Menu2 name = %m2_name', array('%m2_name' => $m2_name), 'm2_admin');
      drupal_goto(M2_PATH_ADM_MENU2_TAB_MANAGE."/$m2_name");
    } else {
      m2_report('Can not add Menu2!!nl'.
                'Menu2 name = %m2_name', array('%m2_name' => $m2_name), 'm2_admin', 'error', WATCHDOG_ERROR);
    }
  }
}


/**
 * m2_admin_item_all_manage_form
 * m2_admin_item_all_manage_form_submit
 */

function m2_admin_item_all_manage_form($form, &$form_state, $m2_name, $params = array()) {
  $destination = isset($params['destination']) ? $params['destination'] : '';
  $module_path = drupal_get_path('module', 'm2');
  drupal_add_css("$module_path/css/m2_Tree-manage.css");
  $flat_tree = m2_api_item_select_by_menu_all($m2_name, 0, 0, M2_WITH_REMOVED_OFF, M2_WITH_DISABLED_ON);
  if (count($flat_tree)) {
    foreach ($flat_tree as $item) {
      $links = array();
      $links[] = l($item->is_disabled ? t('on') : t('off'), M2_PATH_ADM_MENU2_ITEM_ON_OFF."/$item->item_id", array('query' => $destination, 'attributes' => array('class' => array('m2-link-on-off', 'm2-link-on-off-state-'.($item->is_disabled ? 'on' : 'off')))));
      $links[] = l(t('edit'), M2_PATH_ADM_MENU2_ITEM_MANAGE."/$item->item_id",                               array('query' => $destination, 'attributes' => array('class' => array('m2-link-edit'))));
      $links[] = l(t('remove to garbage'), M2_PATH_ADM_MENU2_ITEM_REMOVE_TO_GARBAGE."/$item->item_id",       array('query' => $destination, 'attributes' => array('class' => array('m2-link-remove'))));
      $states = array(t('type = !type', array('!type' => $item->url ? t('document') : t('folder'))));
      if ($item->is_disabled) $states[]= t('disabled');
      if ($item->is_removed) $states[]= t('removed');
      $item->wr_1 = '<div class="item item-type-'.($item->url ? 'link' : 'text').'">';
      $item->wr_1.= theme('m2_content_wrapper', array('class' => array('item-links'), 'markup' => implode('', $links)));
      $item->wr_1.= '<div class="icon" '.(count($states) ? ' title="'.implode(' | ', $states).'"' : '').'></div>';
      $item->wr_1.= '<div rel="'.$item->item_id.'" class="item-value">';
      $item->wr_4 = '</div>';
      $item->wr_4.= '</div>';
      $item->link_options = array('attributes' => array('target' => '_blank'));
    }
    $form['#attributes']['class'] = array('m2-items-manage-form');
    $form['m2_tree']['items'] = array(
      '#markup' => 
        theme('m2_content_wrapper', array('class' => array('m2-items-manage-header'), 'markup' => '&nbsp;')).
        theme('m2_content_wrapper', array('class' => array('m2-items-manage'), 'markup' => theme('m2_tree_levels_recursive', array('tree' => m2_api_flat_to_tree($flat_tree))))),
    );
    $form['m2_tree']['return_wr'] = array(
      '#type' => 'container',
      '#attributes' => array('style' => 'display:none'),
    );
    $form['m2_tree']['return_wr']['return_tree_state'] = array(
      '#type' => 'textarea',
      '#resizable' => false,
      '#default_value' => '',
    );
    $form['m2_tree']['button_save_tree_changes'] = array(
      '#type' => 'submit',
      '#value' => t('Save Tree changes'),
      '#attributes' => array('style' => 'display:none'),
    );
    $form['#m2_name'] = $m2_name;
    $form['#orig_tree'] = $flat_tree;
    $form['#submit'][]= 'm2_admin_item_all_manage_form_submit';
    drupal_add_js("$module_path/js/m2_Shared.js");
    drupal_add_js("$module_path/js/m2_DragAndDrop.js");
    drupal_add_js("$module_path/js/m2_Tree-manage.js");
  } else {
    $form['m2_tree']['items'] = array(
      '#markup' => theme('m2_content_wrapper', array('class' => array('message-no-items-available'), 'markup' => t('no items'))),
    );
  }
  return $form;
}


function m2_admin_item_all_manage_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-button-save-tree-changes') {
    $m2_name = $form['#m2_name'];
    $orig_tree = $form['#orig_tree'];
    $changes = array();
    foreach (explode('|', $form_state['values']['return_tree_state']) as $c_change_row) {
      list($item_id, $weight, $depth) = explode(',', $c_change_row);
      if (($orig_tree[$item_id]) &&
          (is_numeric($weight) && $weight >= 0) &&
          (is_numeric($depth) && $depth >= 0)) {
        $changes[$item_id] = array('item_id' => $item_id, 'weight' => $weight, 'depth' => $depth);
      }
    }
    if (count($changes)) {
      $updated_items = m2_api_item_update_weight_and_depth_bulk($m2_name, $changes);
      if (count($updated_items)) {
        m2_report('Items was updated.!nl'.
                  'Menu2 name = %m2_name!nl'.
                  'Items ID = %items_id', array('%m2_name' => $m2_name, '%items_id' => implode(', ', $updated_items)), 'm2_admin');
      } else {
        m2_report('Can not update Items!!nl'.
                  'Menu2 name = %m2_name!nl'.
                  'Items ID = %items_id', array('%m2_name' => $m2_name, '%items_id' => implode(', ', $updated_items)), 'm2_admin', 'error', WATCHDOG_ERROR);
      }
    }
  }
}


/**
 * m2_admin_item_manage_form
 * m2_admin_item_manage_form_submit
 */

function m2_admin_item_manage_form($form, &$form_state, $item_id) {
  $c_item = m2_api_item_select($item_id);
  if ($c_item) {
    $form['parent'] = array(
      '#type' => 'select',
      '#title' => t('Parent'),
      '#options' => m2_form_get_root_options(),
      '#default_value' => null,
      '#description' => t(M2_TXT_UNDER_CONSTRUCTION),
      '#required' => true,
      '#disabled' => true,
    );
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Item title'),
      '#default_value' => $c_item->title,
      '#maxlength' => M2_ITEM_TITLE_MAX_LENGTH,
      '#description' => t('HTML is allowed.').M2_HTM_TAG_NEW_LINE.
                        t('Maximum !num characters.', array('!num' => M2_ITEM_TITLE_MAX_LENGTH)),
      '#required' => true,
    );
    $form['url'] = array(
      '#type' => 'textfield',
      '#title' => t('URL'),
      '#default_value' => $c_item->url,
      '#maxlength' => M2_ITEM_URL_MAX_LENGTH,
      '#description' => t('Maximum !num characters.', array('!num' => M2_ITEM_URL_MAX_LENGTH)),
    );
    $form['class'] = array(
      '#type' => 'textfield',
      '#title' => t('CSS Class'),
      '#default_value' => $c_item->class,
      '#maxlength' => M2_ITEM_CLASS_MAX_LENGTH,
      '#description' => t('This class will be using in LI element.').M2_HTM_TAG_NEW_LINE.
                        t('Maximum !num characters.', array('!num' => M2_ITEM_CLASS_MAX_LENGTH)),
    );
    $form['language'] = array(
      '#type' => 'select',
      '#title' => t('Language'),
      '#options' => array(LANGUAGE_NONE => t('Neutral')) + (function_exists('locale_language_list') ? locale_language_list('name', true) : array()),
      '#default_value' => $c_item->language,
      '#description' => t(M2_TXT_UNDER_CONSTRUCTION),
      '#required' => true,
      '#disabled' => true,
    );
    $form['is_expanded'] = array(
      '#type' => 'checkbox',
      '#title' => t('Expanded?'),
      '#default_value' => $c_item->is_expanded,
    );
    $form['is_disabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('is Disabled?'),
      '#default_value' => $c_item->is_disabled,
    );
    $form['description'] = array(
      '#title' => t('Item Description'),
      '#type' => 'textarea',
      '#default_value' => $c_item->description,
      '#maxlength' => M2_ITEM_DESCRIPTION_MAX_LENGTH,
      '#description' => t('This description used in link TITLE attribute (if this item have URL).').M2_HTM_TAG_NEW_LINE.
                        t('Maximum !num characters.', array('!num' => M2_ITEM_DESCRIPTION_MAX_LENGTH)),
    );
    $form['button_save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    $form['button_cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#validate' => array('m2_admin_item_manage_form_validate_cancel'),
    );
    $form['#c_item'] = $c_item;
    $form['#path_ok'] = M2_PATH_ADM_MENU2_TAB_MANAGE."/$c_item->m2_name";
    $form['#path_cancel'] = M2_PATH_ADM_MENU2_TAB_MANAGE."/$c_item->m2_name";
    return $form;
  } else {
    drupal_not_found();
    exit();
  }
}


function m2_admin_item_manage_form_validate_cancel($form, &$form_state) {
 /* unset error message when clicked to cancel button with empty required fileds */
    unset($_SESSION['messages']['error']);
 /* go to destination in url or #path_cancel by default */
    drupal_goto($form['#path_cancel']);
}


function m2_admin_item_manage_form_submit($form, &$form_state) {
  $c_item = $form['#c_item'];
  if ($form_state['clicked_button']['#id'] == 'edit-button-save') {
    $changes = array();
    if ($form_state['values']['title']       != $c_item->title)       $changes['title']       = $form_state['values']['title'];
    if ($form_state['values']['url']         != $c_item->url)         $changes['url']         = $form_state['values']['url'];
    if ($form_state['values']['class']       != $c_item->class)       $changes['class']       = $form_state['values']['class'];
    if ($form_state['values']['language']    != $c_item->language)    $changes['language']    = $form_state['values']['language'];
    if ($form_state['values']['is_expanded'] != $c_item->is_expanded) $changes['is_expanded'] = $form_state['values']['is_expanded'];
    if ($form_state['values']['is_disabled'] != $c_item->is_disabled) $changes['is_disabled'] = $form_state['values']['is_disabled'];
    if ($form_state['values']['description'] != $c_item->description) $changes['description'] = $form_state['values']['description'];
    if (count($changes)) {
      $result = m2_api_item_update_values($c_item->item_id, $changes);
      if ($result) {
        m2_report('Item was updated.!nl'.
                  'Menu2 name = %m2_name!nl'.
                  'Item ID = %item_id!nl'.
                  '!changes', array('%m2_name' => $c_item->m2_name, '%item_id' => $c_item->item_id, '!changes' => m2_report_changes_render($changes)), 'm2_admin');
      } else {
        m2_report('Can not update Item!!nl'.
                  'Menu2 name = %m2_name!nl'.
                  'Item ID = %item_id!nl'.
                  '!changes', array('%m2_name' => $c_item->m2_name, '%item_id' => $c_item->item_id, '!changes' => m2_report_changes_render($changes)), 'm2_admin', 'error', WATCHDOG_ERROR);
      }
    }
 /* go to destination in url or #path_ok by default */
    drupal_goto($form['#path_ok']);
  }
}


/**
 * m2_admin_item_insert_form
 * m2_admin_item_insert_form_submit
 */

function m2_admin_item_insert_form($form, &$form_state, $m2_name) {
  $form['#attributes']['class'] = array('form-collapsed');
  $form['form_title'] = array(
    '#markup' => theme('m2_title', array('type' => 'div', 'class' => array('collapsed-title'), 'markup' => t('add new item'))),
  );
  $form['collapsed_data'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('collapsed-data')),
  );
  $form['collapsed_data']['new_item_title'] = array(
    '#type' => 'textfield',
    '#title' => t('New Item title'),
    '#maxlength' => M2_ITEM_TITLE_MAX_LENGTH,
    '#description' => t('Maximum !num characters.', array('!num' => M2_ITEM_TITLE_MAX_LENGTH)),
    '#required' => true,
  );
  $form['collapsed_data']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#maxlength' => M2_ITEM_URL_MAX_LENGTH,
    '#description' => t('Maximum !num characters.', array('!num' => M2_ITEM_URL_MAX_LENGTH)),
  );
  $form['collapsed_data']['button_add_item'] = array(
    '#type' => 'submit',
    '#value' => t('Add new Item'),
  );
  $form['#m2_name'] = $m2_name;
  return $form;
}


function m2_admin_item_insert_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-button-add-item') {
    $m2_name = $form['#m2_name'];
    $item_title = $form_state['values']['new_item_title'];
    $item_url = $form_state['values']['url'];
    $item_id = m2_api_item_insert($m2_name, array('title' => $item_title, 'url' => $item_url));
    if ($item_id) {
      m2_report('Item was added.!nl'.
                'Menu2 name = %m2_name!nl'.
                'Item ID = %item_id!nl'.
                'Item title = %item_title', array('%m2_name' => $m2_name, '%item_id' => $item_id, '%item_title' => $item_title), 'm2_admin');
    } else {
      m2_report('Can not add Item!!nl'.
                'Menu2 name = %m2_name!nl'.
                'Item ID = %item_id!nl'.
                'Item title = %item_title', array('%m2_name' => $m2_name, '%item_id' => t(M2_TXT_NA), '%item_title' => $item_title), 'm2_admin', 'error', WATCHDOG_ERROR);
    }
  }
}


/**
 * m2_admin_item_on_off_page
 */

function m2_admin_item_on_off_page($item_id) {
  $c_item = m2_api_item_select($item_id);
  if ($c_item) {
    $result = m2_api_item_update_values($c_item->item_id, array('is_disabled' => (int)!$c_item->is_disabled));
    if ($result) {
      m2_report('New state of Item = %state.!nl'.
                'Menu2 name = %m2_name!nl'.
                'Item ID = %items_id', array('%state' => $c_item->is_disabled ? t('On') : t('Off'), '%m2_name' => $c_item->m2_name, '%items_id' => $c_item->item_id), 'm2_admin');
    } else {
      m2_report('Can not change state of Item!!nl'.
                'Menu2 name = %m2_name!nl'.
                'Item ID = %items_id', array('%m2_name' => $c_item->m2_name, '%items_id' => $c_item->item_id), 'm2_admin', 'error', WATCHDOG_ERROR);
    }
 /* go to destination in url or tab manage by default */
    drupal_goto(M2_PATH_ADM_MENU2_TAB_MANAGE.'/'.$c_item->m2_name);
  } else {
    drupal_not_found();
    exit();
  }
}


/**
 * m2_admin_menu_remove_confirm_form
 * m2_admin_menu_remove_confirm_form_submit
 */

function m2_admin_menu_remove_confirm_form($form, &$form_state, $m2_name) {
  $m2_info = m2_api_menu_select_by_name($m2_name);
  if ($m2_info && isset($m2_info->m2_name)) {
    $form['#m2_name'] = $m2_name;
    $form = m2_form_confirm($form, array(
      'title' => t('Remove Menu with name = %m2_name?', array('%m2_name' => truncate_utf8($m2_name, 50, false, true))),
      'path_ok' => M2_PATH_ADM_MENU2_TAB_GARBAGE,
      'path_cancel' => M2_PATH_ADM_MENU2_TAB_GARBAGE."/$m2_name",
      'text_ok' => t('Remove'),
    ));
    return $form;
  } else {
    drupal_not_found();
    exit();
  }
}


function m2_admin_menu_remove_confirm_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-button-ok') {
    $m2_name = $form['#m2_name'];
    $result = m2_api_menu_remove($m2_name);
    if ($result) {
      m2_report('Menu2 was removed.!nl'.
                'Menu2 name = %m2_name', array('%m2_name' => $m2_name), 'm2_admin');
    } else {
      m2_report('Can not remove Menu2!!nl'.
                'Menu2 name = %m2_name', array('%m2_name' => $m2_name), 'm2_admin', 'error', WATCHDOG_ERROR);
    }
  }
}


/**
 * m2_admin_item_remove_confirm_form
 * m2_admin_item_remove_confirm_form_submit
 */
 
function m2_admin_item_remove_confirm_form($form, &$form_state, $item_id) {
  $c_item = m2_api_item_select($item_id);
  if ($c_item && isset($c_item->item_id)) {
    $children = m2_api_item_select_children($c_item->item_id, null, $c_item->depth + 1, M2_WITH_REMOVED_ON, M2_WITH_DISABLED_ON);
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
      'title' => t('Remove Item with ID = %item_id?', array('%item_id' => $c_item->item_id)),
      'path_ok' => M2_PATH_ADM_MENU2_TAB_GARBAGE."/$c_item->m2_name",
      'path_cancel' => M2_PATH_ADM_MENU2_TAB_GARBAGE."/$c_item->m2_name",
      'text_ok' => t('Remove'),
    ));
    return $form;
  } else {
    drupal_not_found();
    exit();
  }
}


function m2_admin_item_remove_confirm_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-button-ok') {
    $c_item = $form['#c_item'];
    $children = $form['#children'];
    $with_children = $form_state['values']['with_children'];
    $result = m2_api_item_remove($c_item->item_id, $with_children);
    if ($result) {
      $children_id = $children && $with_children ? implode(', ', array_keys($children)) : t(M2_TXT_NA);
      m2_report('Item was removed.!nl'.
                'Menu2 name = %m2_name!nl'.
                'Item ID = %item_id!nl'.
                'Children items ID = %children_id', array('%m2_name' => $c_item->m2_name, '%item_id' => $c_item->item_id, '%children_id' => $children_id), 'm2_admin');
    } else {
      m2_report('Can not remove Item!!nl'.
                'Menu2 name = %m2_name!nl'.
                'Item ID = %item_id', array('%m2_name' => $c_item->m2_name, '%item_id' => t(M2_TXT_NA)), 'm2_admin', 'error', WATCHDOG_ERROR);
    }
  }
}


