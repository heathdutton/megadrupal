<?php
// $Id$


/**
 * @file
 * Checklist handler functions.
 */

/**
 * Callback for 'node' item type to give options.
 */
function checklist_checklist_item_node_admin_form_form(&$form, &$form_state, $cl, $item = NULL) {
  $form['node-type'] = array(
    '#type' => 'select',
    '#title' => t('Node type to create'),
    '#options' => node_type_get_names(),
    '#default_value' => $item && $item->data && isset($item->data['node-type']) ? $item->data['node-type'] : NULL,
    '#description' => t('Make sure that whichever node type you select, the user has permission to create the type.'),
    '#required' => TRUE,
  );
  $form['node-published'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require published node'),
    '#default_value' => $item && $item->data && isset($item->data['node-published']) ? $item->data['node-published'] : NULL,
    '#description' => t('Require the node to be published before marking the item off. Note that this will only work if the user that has the item on their checklist publishes the node.'),
  );
  return $form;
}

/**
 * Callback for 'node' item type to process the admin form.
 */
function checklist_checklist_item_node_admin_form_submit(&$form, &$form_state, $cl, $item = NULL) {
  return array('node-type' => $form_state['values']['node-type'], 'node-published' => $form_state['values']['node-published']);
}

/**
 * Callback for 'node' item type to provide allowed_paths.
 */
function checklist_checklist_item_node_allowed_paths($item, $account) {
  $paths = array('node/add/' . $item->data['node-type']);
  if (!empty($item->data['node-published'])) {
    $nodes = _checklist_checklist_item_node_load_nodes($account->uid, $item->data['node-type']);
    foreach ($nodes as $node) {
      // Allow a user to visit the node as well as to edit the node.
      $paths[] = 'node/' . $node->nid;
      $paths[] = 'node/' . $node->nid . '/edit';
    }
  }
  return $paths;
}

/**
 * Callback for 'node' item type to provide to modify how the item is displayed.
 */
function checklist_checklist_item_node_display_options($item, $account) {
  $info = array('path' => 'node/add/' . $item->data['node-type']);
  if (!empty($item->data['node-published'])) {
    $nodes = _checklist_checklist_item_node_load_nodes($account->uid, $item->data['node-type']);
    foreach ($nodes as $node) {
      // Send the user to the edit page since they probably need to publish the node.
      $info['path'] = 'node/' . $node->nid . '/edit';
      break;
    }
  }
  return $info;
}

/**
 * Helper function to load the nodes that have yet to be published by a user.
 */
function _checklist_checklist_item_node_load_nodes($uid, $node_type) {
  $nodes = node_load_multiple(array(), array('type' => $node_type, 'status' => 0, 'uid' => $uid));
  return $nodes;
}

/**
 * Callback for 'node' item type to process it.
 */
function checklist_checklist_item_node_process_item($item, $account = NULL) {
  static $items = array();
  // If item is the node type, return the item.
  if (!is_object($item)) {
    return isset($items[$item]) ? $items[$item] : NULL;
  }
  // Store the item so hook_nodeapi() can call this function later.
  $items[$item->data['node-type']] = $item;
  return FALSE;
}



/**
 * Callback for 'user' item type to provide allowed_paths.
 */
function checklist_checklist_item_user_allowed_paths($item, $account) {
  return array('checklist/check/' . $item->cliid);
}

/**
 * Callback for 'user' item type to provide values to modify how the item is displayed.
 */
function checklist_checklist_item_user_display_options($item, $account) {
  // Don't display anything if the item can't be unchecked.
  if (empty($item->checked_on) || $item->allow_uncheck) {
    $i = array(
      'path' => 'checklist/check/' . $item->cliid,
    );
    // Only add the javascript if this item is supposed to be linked.
    if ($item->link) {
      $i += array(
        'attributes' => array('onclick' => 'return checklist_check(' . $item->cliid . ', "' . url('checklist/check', array('absolute' => TRUE)) . '/");', 'title' => $item->checked_on ? t('Click to uncheck') : t('Click to check')),
        'div_class' => array('clUser'),
      );
      checklist_item_add_js($account);
    }
    return $i;
  }
}

/**
 * Callback for 'user' item type to process it.
 */
function checklist_checklist_item_user_process_item($item, $account) {
  $result = 1;
  // User already checked the item.
  if ($item->checked_on) {
    if ($item->allow_uncheck) {
      db_delete('checklist_user_checks')
        ->condition('uid', $account->uid)
        ->condition('cliid', $item->cliid)
        ->execute();
      $result = 0;
    }
  }
  $result &= !$item->checked_on;
  if (isset($_REQUEST['ajax'])) {
    // Call this since this is an ajax call.
    if ($result) {
      _checklist_complete_user_item($item, $account, TRUE, FALSE);
    }
    print(drupal_json_encode(array('cliid' => $item->cliid, 'checked' => $result, 'uncheck' => $item->allow_uncheck)));
    exit();
  }
  // This will complete the checklist item (if TRUE) if it makes it here.
  return $result;
}




/**
 * Callback for 'user' item type to provide values to modify how the item is displayed.
 */
function checklist_checklist_item_admin_display_options($item, $account) {
  // Don't display anything if the item can't be unchecked.
  if (user_access('manage user checklist') && (empty($item->checked_on) || $item->allow_uncheck)) {
    checklist_item_add_js($account, 'admin');
    return array(
      'path' => 'checklist/admin/check/' . $account->uid . '/' . $item->cliid,
      'attributes' => array('onclick' => 'return checklist_check(' . $item->cliid . ', "' . url('checklist/admin/check/' . $account->uid, array('absolute' => TRUE)) . '/");', 'title' => $item->checked_on ? t('Click to uncheck') : t('Click to check')),
      'div_class' => array('clAdmin'),
    );
  }
}

/**
 * Helper function to add the javascript needed to a checklist page.
 */
function checklist_item_add_js($account, $type = 'user') {
  static $js_types_added = array();
  if (empty($js_types_added)) {
    drupal_add_js('(function ($) {
  Drupal.checklist_check = function(cliid, url) {
    $("#checklistItem"+cliid).addClass(\'clWaiting\');
    $.ajax({url: url+cliid, success: Drupal.checklist_receive, data: {ajax: 1}, dataType: "json"});
    return false;
  }
  Drupal.checklist_receive = function(data) {
    if (data["cliid"]) {
      var box = $("#checklistItem"+data["cliid"]);
      if (data["checked"]) {
        box.addClass("clChecked");
        if (data["uncheck"] == 1) {
          box.siblings("a").attr("title", "' . t('Click to uncheck') . '");
        }
        else {
          box.siblings("a").attr("title", "");
        }
      }
      else {
        box.removeClass("clChecked");
        box.siblings("a").attr("title", "' . t('Click to check') . '");
      }
      box.removeClass("clWaiting");
    }
  }
}(jQuery));', array('type' => 'inline', 'scope' => JS_DEFAULT));
  }
  if (!isset($js_types_added[$type])) {
    $js_types_added[$type] = TRUE;
    if ($type == 'admin') {
      drupal_add_js('(function ($) {
    Drupal.behaviors.ajaxChecklistAdminCheck = {
      attach: function(context) {
        $(".clAdmin").click(function() {
          var cliid = $(this).attr("id").substr(13);
          return Drupal.checklist_check(cliid, "' . url('checklist/admin/check/' . $account->uid, array('absolute' => TRUE)) . '/");
        });
      }
    };
  }(jQuery));', array('type' => 'inline', 'scope' => JS_DEFAULT));
    }
    elseif ($type == 'user') {
      drupal_add_js('(function ($) {
    Drupal.behaviors.ajaxChecklistUserCheck = {
      attach: function(context) {
        $(".clUser").click(function() {
          var cliid = $(this).attr("id").substr(13);
          return Drupal.checklist_check(cliid, "' . url('checklist/check', array('absolute' => TRUE)) . '/");
        });
      }
    };
  }(jQuery));', array('type' => 'inline', 'scope' => JS_DEFAULT));
    }
  }
}




/**
 * Callback for 'page_view' item type to give options.
 */
function checklist_checklist_item_page_view_admin_form_form(&$form, &$form_state, $cl, $item = NULL) {
  $form['page-path'] = array(
    '#type' => 'textfield',
    '#title' => t('Page to visit'),
    '#default_value' => $item && $item->data && isset($item->data['page-path']) ? $item->data['page-path'] : '',
    '#description' => t('Enter the url of the internal drupal page the user must visit.'),
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Callback for 'page_view' item type to give options.
 */
function checklist_checklist_item_page_view_admin_form_validate(&$form, &$form_state, $cl, $item = NULL) {
  $path = trim($form_state['values']['page-path']);

  // Use the source path becuase an alias can change without notice.
  $normal_path = drupal_lookup_path('source', $path);
  if ($normal_path) {
    $path = $normal_path;
    drupal_set_message(t('The path entered is an alias. All paths are stored as the base path.'));
  }

  // Make sure the menu item exists.
  if (!($item = menu_get_item($path))) {
    form_set_error('page-path', t('The path must be a recognized internal drupal path.'));
  }
  else {
    $form_state['values']['page-path'] = $path;
  }
}

/**
 * Callback for 'page_view' item type to process the admin form.
 */
function checklist_checklist_item_page_view_admin_form_submit(&$form, &$form_state, $cl, $item = NULL) {
  return array('page-path' => $form_state['values']['page-path']);
}

/**
 * Callback for 'page_view' item type to provide allowed_paths.
 */
function checklist_checklist_item_page_view_allowed_paths($item, $account) {
  return array($item->data['page-path']);
}

/**
 * Callback for 'page_view' item type to provide to modify how the item is displayed.
 */
function checklist_checklist_item_page_view_display_options($item, $account) {
  return array('path' => $item->data['page-path']);
}

/**
 * Callback for 'page_view' item type to process it.
 */
function checklist_checklist_item_page_view_process_item($item, $account = NULL) {
  // By visiting the page, the item is checked off.
  return TRUE;
}




/**
 * Callback for 'node_agree' item type to give options.
 */
function checklist_checklist_item_node_agree_admin_form_form(&$form, &$form_state, $cl, $item = NULL) {
  $default = '';
  if ($item && $item->data && isset($item->data['nid']) && ($node = node_load($item->data['nid']))) {
    $default = "$node->title [nid:$node->nid]";
  }
  $form['nid'] = array(
    '#type' => 'textfield',
    '#title' => t('Agreement node'),
    '#default_value' => $default,
    '#description' => t('Begin entering the title of the node that a user must agree to. Note, you must make sure the user\'s to complete this checklist have the correct permissions to access the node.'),
    '#autocomplete_path' => 'autocomplete/checklist/node_agree',
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Callback for 'node_agree' item type to give options.
 */
function checklist_checklist_item_node_agree_admin_form_validate(&$form, &$form_state, $cl, $item = NULL) {
  $entry = trim($form_state['values']['nid']);
  // Add 5 for '[nid:'.
  $pos_open = strrpos($entry, '[') + 5;
  $pos_close = strrpos($entry, ']');
  $nid = drupal_substr($entry, $pos_open, $pos_close -($pos_open));
  if (!is_numeric($nid) || !($node = node_load($nid))) {
    form_set_error('nid', t('Please enter a valid node titel and select the corrseponding entry.'));
  }
  else {
    $form_state['values']['nid'] = $nid;
  }
}

/**
 * Callback for 'node_agree' item type to process the admin form.
 */
function checklist_checklist_item_node_agree_admin_form_submit(&$form, &$form_state, $cl, $item = NULL) {
  return array('nid' => $form_state['values']['nid']);
}

/**
 * Callback for 'node_agree' item type to provide allowed_paths.
 */
function checklist_checklist_item_node_agree_allowed_paths($item, $account) {
  return array('node/' . $item->data['nid']);
}

/**
 * Callback for 'node_agree' item type to provide to modify how the item is displayed.
 */
function checklist_checklist_item_node_agree_display_options($item, $account) {
  return array('path' => 'node/' . $item->data['nid']);
}

/**
 * Callback for 'node_agree' item type to process it.
 */
function checklist_checklist_item_node_agree_process_item($item, $account = NULL) {
  static $items = array();

  // If item is the nid of the node, return the item.
  if (!is_object($item)) {
    return isset($items[$item]) ? $items[$item] : NULL;
  }

  // Store the item so hook_nodeapi() can call this function later.
  $items[$item->data['nid']] = $item;
  return FALSE;
}

/**
 * Autocomplete callback for node_agree item type.
 */
function checklist_item_autocomplete_node($string = '') {
  $options = array();
  if ($string) {
    $sql = "SELECT nid, title FROM {node} WHERE status=1 AND lower(title) LIKE  ORDER BY title ASC";
    $results = db_select('node', 'n')
      ->fields('n', array('nid', 'title'))
      ->condition('status', 1)
      ->where('LOWER(title) LIKE LOWER(:lookup_string)', array(':lookup_string' => '%%'. db_like($string) .'%%'))
      ->orderBy('title', 'ASC')
      ->addTag('node_access')
      ->execute();
    foreach ($results as $row) {
      $title = check_plain($row->title);
      $options[$title . ' [nid:' . $row->nid . ']'] = $title;
    }
  }
  print drupal_json_encode($options);
  exit();
}

/**
 * Form to Agree to a Node
 */
function checklist_node_agree_form($form, &$form_state, $item) {
  $form['#checklist_item'] = $item;
  $form['checklist_submit'] = array(
    '#type' => 'submit',
    '#value' => t('I Agree'),
    '#weight' => 10,
  );
  return $form;
}

/**
 * Submit function for the checklist_agree_form form
 * Checks the Checklist item box off
 */
function checklist_node_agree_form_submit($form, &$form_state) {
  _checklist_complete_user_item($form['#checklist_item']);
}
