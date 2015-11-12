<?php

define('STAFFLIST_NONE', 0);
define('STAFFLIST_DEPARTMENT', 1);
define('STAFFLIST_LEVEL', 2);

/**
 * Generates a list of departments or levels in weighted order.
 */
function _stafflist_role_list($type) {
  $result = db_query("SELECT s.rid, r.name, s.weight FROM {staff_roles} s, {role} r WHERE s.rid = r.rid AND s.type = :type ORDER BY s.weight ASC", array(
    ':type' => $type
  ));
  $list = array();
  foreach ($result as $record) {
    $list[$record->rid] = array('name' => $record->name, 'weight' => $record->weight);
  }
  return $list;
}

/**
 * Collects all the information for the staff list and passes it off for theming.
 */
function _stafflist_view() {
  // Initializes an empty list of users.
  $departments = _stafflist_role_list(STAFFLIST_DEPARTMENT);
  $levels = _stafflist_role_list(STAFFLIST_LEVEL);
  $users = array();
  foreach ($departments as $sdid => $v) {
    $users[$sdid] = array();
    foreach($levels as $slid => $v) {
      $users[$sdid][$slid] = array();
    }
  }
  // Fills up the list and passes it off to theme.
  $subquery = "SELECT ur.uid, ur.rid FROM {users_roles} ur, {staff_roles} s WHERE ur.rid = s.rid AND s.type = ";
  $subquery1 = $subquery . ":dept";
  $subquery2 = $subquery . ":level";
  $query = "SELECT u.uid, sd.rid AS sdid, sr.rid AS srid FROM {users} u, ($subquery1) sd, ($subquery2) sr ";
  $query .= "WHERE u.uid = sd.uid AND u.uid = sr.uid ORDER BY u.name ASC";
  $result = db_query($query, array(
    ':dept' => STAFFLIST_DEPARTMENT,
    ':level' => STAFFLIST_LEVEL,
  ));
  foreach ($result as $record) {
    $users[$record->sdid][$record->srid][$record->uid] = $record->uid;
  }
  $page = theme('stafflist', $users, $departments, $levels);
  return $page;
}

/**
 * Default theme to display the staff list.
 */
function theme_stafflist($users, $departments, $levels) {
  $content = "";
  foreach ($users as $sdid => $dept_group) {
    $content .= "<h3>" . check_plain($departments[$sdid]['name']) . "</h3><ul>";
    foreach ($dept_group as $srid => $level_group) {
      $content .= "<li>" . check_plain($levels[$srid]['name']);
      if (count($level_group)) {
        $content .= "<ul>";
        foreach ($level_group as $uid) {
          $content .= "<li>". theme('username', user_load($uid)) ."</li>";
        }
        $content .= "</ul>";
      }
      else {
        $content .= "<ul><li><em>(None)</em></li></ul>";
      }
      $content .= "</li>";
    }
    $content .= "</ul><hr />";
  }
  return $content;	
}

/**
 * Generates form to associates user roles with departments and levels.
 */
function stafflist_admin_roles() {
  $roles = user_roles(TRUE);
  $departments = _stafflist_role_list(STAFFLIST_DEPARTMENT);
  $levels = _stafflist_role_list(STAFFLIST_LEVEL);
  
  $form = array();
  $form['#role_list'] = array_keys($roles);
  $options = array(STAFFLIST_NONE => 'None', STAFFLIST_DEPARTMENT => 'Department', STAFFLIST_LEVEL => 'Level');
  
  foreach ($roles as $rid => $name) {
    if (array_key_exists($rid, $departments)) {
      $type = STAFFLIST_DEPARTMENT;
      $weight = $departments[$rid]['weight'];
    }
    elseif (array_key_exists($rid, $levels)) {
      $type = STAFFLIST_LEVEL;
      $weight = $levels[$rid]['weight'];
    }
    else {
      $type = STAFFLIST_NONE;
      $weight = 0;
    }
    
    $form[$rid] = array(
      '#type' => 'fieldset',
      '#title' => $name,
    );
    $form[$rid][$rid .'_type'] = array(
      '#type' => 'radios',
      '#title' => 'Type',
      '#options' => $options,
      '#default_value' => $type,
    );
    $form[$rid][$rid .'_weight'] = array(
      '#type' => 'weight',
      '#title' => 'Weight',
      '#default_value' => $weight,
    );
  }
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

/**
 * Processes form to associates user roles with departments and levels.
 */
function stafflist_admin_roles_submit($form, &$form_state) {
  $values = $form_state['values'];
  db_delete('staff_roles')->execute();
  $query = db_insert('staff_roles')->fields(array('rid', 'type', 'weight'));
  foreach ($form['#role_list'] as $rid) {
    if ($rid . '_type') {
      $query->values(array('rid' => $rid, 'type' => $values[$rid . '_type'], 'weight' => $values[$rid . '_weight']));
    }
  }
  $query->execute();
}
