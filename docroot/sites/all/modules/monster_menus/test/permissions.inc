<?php

require 'permissions_baseline.inc';

function __mm_test_perms_cleanup() {
  $roots = array(mm_home_mmtid() => '[home]', mm_content_users_mmtid() => MM_ENTRY_NAME_USERS);
  $dels = array_keys($roots);
  $dels[] = mm_content_groups_mmtid();
  if ($existing = mm_content_get(array('parent' => mm_content_groups_mmtid(), 'name' => MM_ENTRY_NAME_VIRTUAL_GROUP))) {
    $dels[] = $existing[0]->mmtid;
  }
  foreach ($dels as $root) {
    if ($existing = mm_content_get(array('parent' => $root, 'name' => '~MM TEST'))) {
      mm_content_delete($existing[0]->mmtid);
      mm_content_update_sort_queue();
    }
  }
  // Reset auto increment counter on mmtree.mmtid
  db_query('ALTER TABLE {mm_tree} AUTO_INCREMENT = 1');
  db_query('ALTER TABLE {mm_tree_revisions} AUTO_INCREMENT = 1');
  db_query('ALTER TABLE {node} AUTO_INCREMENT = 1');
}

function __mm_test_export_var($var, $prefix = '') {
  static $key_xlate = array(
    MM_PERMS_ADMIN =>          'MM_PERMS_ADMIN',
    MM_PERMS_APPLY =>          'MM_PERMS_APPLY',
    MM_PERMS_IS_GROUP =>       'MM_PERMS_IS_GROUP',
    MM_PERMS_IS_RECYCLE_BIN => 'MM_PERMS_IS_RECYCLE_BIN',
    MM_PERMS_IS_RECYCLED =>    'MM_PERMS_IS_RECYCLED',
    MM_PERMS_IS_USER =>        'MM_PERMS_IS_USER',
    MM_PERMS_READ =>           'MM_PERMS_READ',
    MM_PERMS_SUB =>            'MM_PERMS_SUB',
    MM_PERMS_WRITE =>          'MM_PERMS_WRITE',
  );
  if (is_array($var)) {
    if (empty($var)) {
      return 'array()';
    }
    $output = "array(\n";
    foreach ($var as $key => $value) {
      $key = is_scalar($key) && isset($key_xlate[$key]) ? $key_xlate[$key] : __mm_test_export_var($key);
      $output .= $prefix . "  " . $key . " => " . __mm_test_export_var($value, $prefix . '  ') . ",\n";
    }
    return $output . $prefix . ')';
  }
  if (is_object($var) && get_class($var) === 'stdClass') {
    // var_export() will export stdClass objects using an undefined
    // magic method __set_state() leaving the export broken. This
    // workaround avoids this by casting the object as an array for
    // export and casting it back to an object when evaluated.
    return '(object) ' . __mm_test_export_var((array) $var, $prefix);
  }
  if (is_bool($var)) {
    return $var ? 'TRUE' : 'FALSE';
  }
  if (is_object($var)) {
    return get_class($var) . '::__set_state(' . __mm_test_export_var((array) $var, $prefix) . ')';
  }
  return var_export($var, TRUE);
}

class mmtucTestTreeGetTreeIterator extends getTreeIterator {
  public $match;
  private $path, $stats, $pindex;

  public function __construct($path, $stats) {
    $this->path = $path;
    $this->stats = $stats;
    $this->pindex = count($path) - 1;
  }

  public function iterate($item) {
    global $__mm_test_perms_baseline;
    // function must return 1 if no error, 0 if error, -1 if this node
    // and any of its children should be skipped
    $this->path = array_slice($this->path, 0, $this->pindex + $item->level);
    $this->path[] = $item->name;
    $print_path = implode('/', $this->path);

    if (isset($__mm_test_perms_baseline[$this->stats['label']][$print_path])) {
      foreach (array(MM_PERMS_WRITE, MM_PERMS_SUB, MM_PERMS_APPLY, MM_PERMS_READ, MM_PERMS_IS_USER, MM_PERMS_IS_GROUP, MM_PERMS_IS_RECYCLE_BIN, MM_PERMS_IS_RECYCLED) as $mode) {
        if ($mode != MM_PERMS_APPLY || $this->path[0] != MM_ENTRY_NAME_GROUPS) {
          $this->stats['count']++;
          if (!isset($__mm_test_perms_baseline[$this->stats['label']][$print_path]['page'][$mode])) {
            $this->stats['fail'][] = "Undefined baseline entry ['" . $this->stats['label'] . "']['$print_path']['page'][$mode]. Re-run this script using \$want_baseline=TRUE and use the output to rewrite permissions_baseline.inc.";
          }
          else {
            $old = $__mm_test_perms_baseline[$this->stats['label']][$print_path]['page'][$mode];
            if ($item->perms[$mode] !== $old) {
              $this->stats['fail'][] = ($old ? 'T' : 'F') . ": ($item->mmtid) $print_path: $mode";
            }
          }
        }
      }
    }

    return 1;   // continue
  } // iterate
}   // class

function __mm_test_perms($test_nodes = FALSE, $uids = NULL, $want_baseline = FALSE) {
  // keep the devel module from outputting way too many queries
  if (function_exists('devel_query_enabled') && devel_query_enabled()) {
    // The following will turn off devel query logging
    Database::getLog('devel');
  }

  $test_node = function($usr, $print_path, $node, &$stats, $pass, $curr, $secondary) use ($want_baseline) {
    static $key_xlate = array(
      MM_PERMS_ADMIN =>          'MM_PERMS_ADMIN',
      MM_PERMS_APPLY =>          'MM_PERMS_APPLY',
      MM_PERMS_IS_GROUP =>       'MM_PERMS_IS_GROUP',
      MM_PERMS_IS_RECYCLE_BIN => 'MM_PERMS_IS_RECYCLE_BIN',
      MM_PERMS_IS_RECYCLED =>    'MM_PERMS_IS_RECYCLED',
      MM_PERMS_IS_USER =>        'MM_PERMS_IS_USER',
      MM_PERMS_READ =>           'MM_PERMS_READ',
      MM_PERMS_SUB =>            'MM_PERMS_SUB',
      MM_PERMS_WRITE =>          'MM_PERMS_WRITE',
    );
    global $__mm_test_perms_baseline;

    $new = mm_content_user_can_node($node, '', $usr);
    foreach (array(MM_PERMS_READ, MM_PERMS_WRITE) as $mode) {
      $stats['count']++;
      if ($want_baseline) {
        $stats['baseline'][$print_path]['node'][$node->title][$mode][$pass] = !empty($new[$mode]);
      }
      else if (!isset($__mm_test_perms_baseline[$stats['label']][$print_path]['node'][$node->title][$mode][$pass])) {
        $stats['fail'][] = "Undefined baseline entry ['" . $stats['label'] . "']['$print_path']['node'][$node->title][$mode][$pass]. Re-run this script using \$want_baseline=TRUE and use the output to rewrite permissions_baseline.inc.";
      }
      else {
        $old = $__mm_test_perms_baseline[$stats['label']][$print_path]['node'][$node->title][$mode][$pass];
        if ($new[$mode] != $old) {
          $m = isset($key_xlate[$mode]) ? $key_xlate[$mode] : $mode;
          if (!$curr) {
            $stats['fail'][] = ($old ? 'T' : 'F') . ": $print_path: node '$node->title' pass $pass: $m";
          }
          else if ($pass) {
            $stats['fail'][] = ($old ? 'T' : 'F') . ": ($curr->mmtid) and ($secondary->mmtid) $print_path: node '$node->title': $m";
          }
          else {
            $stats['fail'][] = ($old ? 'T' : 'F') . ": ($curr->mmtid) $print_path: node '$node->title': $m";
          }
        }
      }
    }
  };

  $test_page = function ($usr, $path, $nodes, $secondaries, &$stats) use ($want_baseline, $test_node) {
    global $__mm_test_perms_baseline;

    $item = $path[count($path) - 1];
    $print_path = array();
    foreach ($path as $p) {
      $print_path[] = $p->name;
    }
    $print_path = join('/', $print_path);

    $new = mm_content_user_can($item->mmtid, '', $usr);
    foreach (array(MM_PERMS_WRITE, MM_PERMS_SUB, MM_PERMS_APPLY, MM_PERMS_READ, MM_PERMS_IS_USER, MM_PERMS_IS_GROUP, MM_PERMS_IS_RECYCLE_BIN, MM_PERMS_IS_RECYCLED) as $mode) {
      if ($mode != MM_PERMS_APPLY || $path[0]->name != MM_ENTRY_NAME_GROUPS) {
        $stats['count']++;
        if ($want_baseline) {
          $stats['baseline'][$print_path]['page'][$mode] = !empty($new[$mode]);
        }
        else if (!isset($__mm_test_perms_baseline[$stats['label']][$print_path]['page'][$mode])) {
          $stats['fail'][] = "Undefined baseline entry ['" . $stats['label'] . "']['$print_path']['page'][$mode]. Re-run this script using \$want_baseline=TRUE and use the output to rewrite permissions_baseline.inc.";
        }
        else {
          $old = $__mm_test_perms_baseline[$stats['label']][$print_path]['page'][$mode];
          if ($new[$mode] !== $old) {
            $stats['fail'][] = ($old ? 'T' : 'F') . ": ($item->mmtid) $print_path: $mode";
          }
        }
      }
    }

    foreach ($nodes as $node) {
      $node->mm_catlist = array($item->mmtid => '');
      node_save($node);
      $test_node($usr, $print_path, $node, $stats, 0, $item, NULL);
      $bin = mm_content_move_to_bin(NULL, $node->nid);
      mm_content_update_sort_queue();
      if (!is_numeric($bin)) {
        $stats['fail'][] = "Could not recycle the node '$node->title' at $print_path: $bin";
      }
      else {
        $pp = $print_path . '/[recycled]';
        $test_node($usr, $pp, $node, $stats, 0, NULL, NULL);
        $err = mm_content_move_from_bin(NULL, $node, $bin, FALSE);
        if (is_string($err)) {
          $stats['fail'][] = "Could not move '$node->title' out of recycle bin at $pp: $err";
        }
        else if ($secondaries) {
          foreach ($secondaries as $pass => $secondary) {
            $node->mm_catlist = array($item->mmtid => '', $secondary->mmtid => '');
            node_save($node);
            $test_node($usr, $print_path, $node, $stats, $pass + 1, $item, $secondary);
            $bin = mm_content_move_to_bin(NULL, array($node->nid => array($item->mmtid)));
            mm_content_update_sort_queue();
            if (!is_numeric($bin)) {
              $stats['fail'][] = "Could not recycle the node '$node->title' at $print_path: $bin";
            }
            else {
              $test_node($usr, $pp, $node, $stats, $pass + 1, NULL, NULL);
              $err = mm_content_move_from_bin(NULL, $node, $bin, FALSE);
              if (is_string($err)) {
                $stats['fail'][] = "Could not move '$node->title' out of recycle bin at $pp: $err";
              }
            }
          }
        }
      }
    }
    if ($nodes) {
      // Flush the cache now instead of waiting until there are tons of entries
      $mmtids = _mm_content_clear_access_cache();
      if (!empty($mmtids)) {
        _mm_content_get_access_cache_object()->clearNidsByMmtid($mmtids);
      }
    }
  };

  $test_tree = function ($usr, $path, &$stats) {
    $iter = new mmtucTestTreeGetTreeIterator(array($path[0]->name, $path[1]->name), $stats);
    $params = array(MM_GET_TREE_ITERATOR => $iter, MM_GET_TREE_FILTER_HIDDEN => TRUE, MM_GET_TREE_RETURN_PERMS => TRUE, MM_GET_TREE_USER => $usr);
    mm_content_get_tree($path[1]->mmtid, $params);
  };

  function __mm_test_perms_node_save($usr, $groups_w, $users_w, &$nodes, $title, $everyone = FALSE) {
    $node = (object)array(
      'type' => 'story',
      'uid' => $usr->uid,
      'name' => $usr->name,
      'title' => $title,
      'body' => 'body',
      'status' => 1,
      'comment' => 0,
      'mm_catlist_restricted' => array(),
      'mm_catlist' => array(),
      'owner' => $usr->uid,
      'groups_w' => $groups_w,
      'users_w' => $users_w,
      'others_w' => $everyone,
      'show_node_info' => 3,
      'revision' => FALSE,
      'mm_others_w_force' => TRUE,  // always allow "writable by everyone"
      'op' => t('Save'),
    );
    node_save($node);
    $errs = form_get_errors();
    if ($errs) {
      print_r($errs);
      return FALSE;
    }
    $nodes[$node->nid] = node_load($node->nid);
    if (!$nodes[$node->nid]) {
      print('Could not reload node');
      return FALSE;
    }
    return TRUE;
  }

  function __mm_test_perms_delete_if_exists($arr) {
    if ($existing = mm_content_get($arr)) {
      mm_content_delete($existing[0]->mmtid);
      mm_content_update_sort_queue();
      mm_create_path($dummy, TRUE);
    }
  }

  global $user;
  if ($user->uid != 1) {
    die("This program must be run as the admin user.");
  }
  $dummy_uid = 99999;   // to keep mm_create_path() from using uid=1 by default
  module_load_include('inc', 'monster_menus', 'mm_create_path');

  if (empty($uids) && $uids !== 0) {
    $uids = array('anonymous' => 0, 'admin' => 1);
    $access_modes = array('administer all menus', 'administer all users', 'administer all groups', 'view all menus');
    foreach ($access_modes as $access_mode) {
      $row = db_query('SELECT GROUP_CONCAT(rp.permission) AS perm, uid FROM {role_permission} rp INNER JOIN {users_roles} ur ON ur.rid = rp.rid WHERE rp.permission IN(:modes) AND uid > 1 GROUP BY uid HAVING perm = :mode', array(':modes' => $access_modes, ':mode' => $access_mode))->fetchObject();
      if (empty($row->uid)) {
        print "Warning: Could not find any users with only permission '$access_mode' to test. This test will be skipped.\n";
      }
      else {
        print "Using uid = $row->uid to test permission '$access_mode'.\n";
        $uids[$access_mode] = $row->uid;
      }
    }

    $uid = db_query('SELECT u.uid FROM {users} u LEFT JOIN {users_roles} ur ON ur.uid = u.uid WHERE u.uid > 1 AND u.status = 1 AND ur.uid IS NULL LIMIT 1')->fetchField();
    if (empty($uid)) {
      print "Warning: Could not find any users not in a role. This test will be skipped.\n";
    }
    else {
      print "Using uid = $uid for user not in any roles.\n";
      $uids['no roles'] = $uid;
    }

    print "\n";
  }

  __mm_test_perms_cleanup();

  $roots = array(mm_home_mmtid() => '[home]', mm_content_users_mmtid() => MM_ENTRY_NAME_USERS);
  $stats = array();
  if (!is_array($uids)) {
    $uids = array($uids);
  }
  foreach ($uids as $label => $test_uid) {
    $stats[$test_uid] = array('label' => $label, 'count' => 0, 'fail' => array(), 'baseline' => array());
    $usr = user_load($test_uid);

    $grp = array(
      new MMCreatePathGroup(array(
        'name' => MM_ENTRY_NAME_GROUPS,
        'mmtid' => mm_content_groups_mmtid(),
      )),
      new MMCreatePathGroup(array(
        'name' => '~MM TEST',
        'members' => $test_uid ? array($test_uid) : array(),
      )),
    );
    if (!mm_create_path($grp)) {
      print "Create group failed";
      return;
    }
    $gid = $grp[1]->mmtid;

    if ($test_uid) {
      $vgrp = array(
        &$grp[0],
        new MMCreatePathGroup(array(
          'name' => MM_ENTRY_NAME_VIRTUAL_GROUP
        )),
        new MMCreatePathGroup(array(
          'name' => '~MM TEST',
          'vgroup' => TRUE,
          'qfield' => $test_uid,
          'qfrom' => ''
        )),
      );
      if (!mm_create_path($vgrp)) {
        print "Create vgroup failed";
        return;
      }
      $vgid = db_query('SELECT vgid FROM {mm_group} WHERE gid = :gid', array(':gid' => $vgrp[2]->mmtid))->fetchField();
      db_delete('mm_virtual_group')->condition('vgid', $vgid)->execute();
      db_insert('mm_virtual_group')->fields(array('vgid' => $vgid, 'uid' => $test_uid, 'preview' => 0))->execute();
    }

    $nodes = array();
    if ($test_nodes) {
      // Create a bunch of nodes, initially not on any page.
      // This section requires $user->uid == 1.
      if (!__mm_test_perms_node_save($user, array(), array(), $nodes, 'owned by admin') ||
          !__mm_test_perms_node_save($usr, array(), array(), $nodes, 'owned by user') ||
          !__mm_test_perms_node_save($user, array(), array(), $nodes, 'writable by everyone', TRUE) ||
          $test_uid &&
          (!__mm_test_perms_node_save($user, array($gid => ''), array(), $nodes, 'writable by user in group') ||
            !__mm_test_perms_node_save($user, array(), array($test_uid => ''), $nodes, 'writable by user in ad hoc group') ||
            !__mm_test_perms_node_save($user, array($vgid = ''), array(), $nodes, 'writable by user in virtual group'))) {
        return;
      }
    }

    $parents = array(
      new MMCreatePathCat(array(
        'name' => 'unreadable parent',
        'alias' => 'xparent',
        'default_mode' => '',
        'uid' => $dummy_uid,
      )),
      new MMCreatePathCat(array(
        'name' => 'parent readable by everyone',
        'alias' => 'rparent',
        'uid' => $dummy_uid,
      )),
    );
    if ($test_uid) {
      $parents[] = new MMCreatePathCat(array(
        'name' => 'parent readable by regular group',
        'alias' => 'rreggroupparent',
        'perms' => array(MM_PERMS_READ => array('groups' => array(&$grp))),
        'uid' => $dummy_uid,
      ));
      $parents[] = new MMCreatePathCat(array(
        'name' => 'parent readable by ad hoc group',
        'alias' => 'radhocgroupparent',
        'perms' => array(MM_PERMS_READ => array('users' => array($test_uid))),
        'uid' => $dummy_uid,
      ));
      $parents[] = new MMCreatePathCat(array(
        'name' => 'parent readable by virtual group',
        'alias' => 'rvirtgroupparent',
        'perms' => array(MM_PERMS_READ => array('groups' => array(&$vgrp))),
        'uid' => $dummy_uid,
      ));
    }

    foreach ($roots as $root => $root_name) {
      __mm_test_perms_delete_if_exists(array('parent' => $root, 'alias' => '~mmtest'));
      foreach ($parents as $parent) {
        $path = array(
          new MMCreatePathCat(array(
            'mmtid' => $root,
            'name' => $root_name,
          )),
          new MMCreatePathCat(array(
            'name' => '~MM TEST',
            'alias' => '~mmtest',
            'uid' => $dummy_uid,
          )),
          clone($parent),
          new MMCreatePathCat(array(
            'name' => 'no read',
            'alias' => 'noread',
            'default_mode' => '',
            'uid' => $dummy_uid,
          )),
        );
        mm_create_path($path);
        // Test nodes first on the page by themselves, then also on a
        // world-readable page and an unreadable page.
        $secondaries = array($path[1], $path[3]);
        $test_page($usr, $path, $nodes, $secondaries, $stats[$test_uid]);

        $path[3] = new MMCreatePathCat(array(
          'name' => 'owns',
          'alias' => 'owns',
          'default_mode' => '',
          'uid' => $test_uid,
        ));
        mm_create_path($path);
        $test_page($usr, $path, $nodes, $secondaries, $stats[$test_uid]);

        $path[3] = new MMCreatePathCat(array(
          'name' => 'inaccessible',
          'alias' => 'inaccessible',
          'default_mode' => '',
          'uid' => $dummy_uid,
        ));
        mm_create_path($path);
        $test_page($usr, $path, $nodes, $secondaries, $stats[$test_uid]);

        foreach (array('read', 'write', 'add sub', 'use') as $long) {
          $short = $long[0];
          $path[3] = new MMCreatePathCat(array(
            'name' => $long . ' by everyone',
            'alias' => $short . 'everyone',
            'default_mode' => $short,
            'uid' => $dummy_uid,
          ));
          mm_create_path($path);
          $test_page($usr, $path, $nodes, $secondaries, $stats[$test_uid]);

          if ($test_uid) {
            $path[3] = new MMCreatePathCat(array(
              'name' => $long . ' by regular group',
              'alias' => $short . 'reggroup',
              'default_mode' => '',
              'perms' => array($short => array('groups' => array(&$grp))),
              'uid' => $dummy_uid,
            ));
            mm_create_path($path);
            $test_page($usr, $path, $nodes, $secondaries, $stats[$test_uid]);

            $path[3] = new MMCreatePathCat(array(
              'name' => $long . ' by ad hoc group',
              'alias' => $short . 'adhocgroup',
              'default_mode' => '',
              'perms' => array($short => array('users' => array($test_uid))),
              'uid' => $dummy_uid,
            ));
            mm_create_path($path);
            $test_page($usr, $path, $nodes, $secondaries, $stats[$test_uid]);

            $path[3] = new MMCreatePathCat(array(
              'name' => $long . ' by virtual group',
              'alias' => $short . 'virtgroup',
              'default_mode' => '',
              'perms' => array($short => array('groups' => array(&$vgrp))),
              'uid' => $dummy_uid,
            ));
            mm_create_path($path);
            $test_page($usr, $path, $nodes, $secondaries, $stats[$test_uid]);
          }
        }
      }

      // recycle bin
      $bin = mm_content_move_to_bin($path[1]->mmtid);
      mm_content_update_sort_queue();
      if (!is_numeric($bin)) {
        print "Error while moving '" . $path[1]->name . "' (" . $path[1]->mmtid . ") to recycle bin: $bin\n";
        return;
      }
      $path2 = $path;
      $path2[1] = new MMCreatePathCat(array(
        'name' => $path[1]->name . ' [recycled]',
        'mmtid' => $path[1]->mmtid,
      ));
      unset($path2[3]);
      $test_page($usr, $path2, array(), array(), $stats[$test_uid]);  // subpage
      unset($path2[2]);
      $test_page($usr, $path2, array(), array(), $stats[$test_uid]);  // page
      $test_page($usr, array(
        new MMCreatePathCat(array(
          'name' => $root_name . '/[recycle bin]',
          'mmtid' => $bin,
        ))
      ), array(), array(), $stats[$test_uid]);  // bin itself
      $err = mm_content_move_from_bin($path2[1]->mmtid, NULL, NULL, FALSE);
      mm_content_update_sort_queue();
      if (is_string($err)) {
        print "Error while moving '" . $path2[1]->name . "' (" . $path2[1]->mmtid . ") out of recycle bin: $err\n";
        return;
      }

      if (!$want_baseline) {
        $test_tree($usr, array($path[0], $path[1]), $stats[$test_uid]);
      }
    }

    // groups
    $tgrp = array(
      &$grp[0],
      new MMCreatePathGroup(array(
        'name' => '~MM TEST',
      )),
      new MMCreatePathGroup(array(
        'name' => 'owns',
        'alias' => 'owns',
        'default_mode' => '',
        'uid' => $test_uid,
      )),
    );
    mm_create_path($tgrp);
    $test_page($usr, $tgrp, array(), array(), $stats[$test_uid]);

    $tgrp[2] = new MMCreatePathGroup(array(
      'name' => 'inaccessible',
      'alias' => 'inaccessible',
      'default_mode' => MM_PERMS_APPLY,
      'uid' => $dummy_uid,
    ));
    mm_create_path($tgrp);
    $test_page($usr, $tgrp, array(), array(), $stats[$test_uid]);

    $tgrp[3] = new MMCreatePathGroup(array(
      'name' => 'read by everyone child of inaccessible',
      'default_mode' => 'u,r',
      'uid' => $dummy_uid,
    ));
    mm_create_path($tgrp);
    $test_page($usr, $tgrp, array(), array(), $stats[$test_uid]);
    unset($tgrp[3]);

    foreach (array('read', 'write', 'add sub') as $long) {
      $short = $long[0];
      $tgrp[2] = new MMCreatePathGroup(array(
        'name' => $long . ' by everyone',
        'alias' => $short . 'everyone',
        'default_mode' => "u,$short",
        'uid' => $dummy_uid,
      ));
      mm_create_path($tgrp);
      $test_page($usr, $tgrp, array(), array(), $stats[$test_uid]);

      if ($test_uid) {
        $tgrp[2] = new MMCreatePathGroup(array(
          'name' => $long . ' by regular group',
          'alias' => $short . 'reggroup',
          'default_mode' => MM_PERMS_APPLY,
          'perms' => array($short => array('groups' => array(&$grp))),
          'uid' => $dummy_uid,
        ));
        mm_create_path($tgrp);
        $test_page($usr, $tgrp, array(), array(), $stats[$test_uid]);

        $tgrp[2] = new MMCreatePathGroup(array(
          'name' => $long . ' by ad hoc group',
          'alias' => $short . 'adhocgroup',
          'default_mode' => MM_PERMS_APPLY,
          'perms' => array($short => array('users' => array($test_uid))),
          'uid' => $dummy_uid,
        ));
        mm_create_path($tgrp);
        $test_page($usr, $tgrp, array(), array(), $stats[$test_uid]);

        $tgrp[2] = new MMCreatePathGroup(array(
          'name' => $long . ' by virtual group',
          'alias' => $short . 'virtgroup',
          'default_mode' => MM_PERMS_APPLY,
          'perms' => array($short => array('groups' => array(&$vgrp))),
          'uid' => $dummy_uid,
        ));
        mm_create_path($tgrp);
        $test_page($usr, $tgrp, array(), array(), $stats[$test_uid]);
      }
    }

    if (!$want_baseline) {
      mm_content_update_sort_queue();
      $test_tree($usr, array($tgrp[0], $tgrp[1]), $stats[$test_uid]);
    }

    // various standard locations
    $list = array(
      1 => '[root]',
      mm_home_mmtid() => '[home]',
      mm_content_users_mmtid() => MM_ENTRY_NAME_USERS,
      mm_content_groups_mmtid() => MM_ENTRY_NAME_GROUPS,
      -65 => MM_ENTRY_NAME_USERS . '/A',
      1234567890 => '[non-existant]'
    );
    foreach ($list as $tid => $label) {
      $path = array(new MMCreatePathCat(array(
        'mmtid' => $tid,
        'name' => $label,
      )));
      $test_page($usr, $path, array(), array(), $stats[$test_uid]);
    }

    if ($nodes) {
      $node = (object)array('nid' => 1234567, 'title' => 'non-existant node');
      $test_node($usr, '[non-existant node]', $node, $stats[$test_uid], 0, NULL, NULL);
    }

    mm_create_path($path, TRUE);

    // delete nodes, so they get re-created for next user
    foreach ($nodes as $node) {
      node_delete($node->nid);
    }
  }

  if ($want_baseline) {
    print "<?php\n\nglobal \$__mm_test_perms_baseline;\n\$__mm_test_perms_baseline = array(\n";
    foreach ($stats as $uid => $stat) {
      print "  '" . $stat['label'] . "' => " . __mm_test_export_var($stat['baseline'], '  ') . ",\n";
    }
    print ");\n";
  }
  else {
    foreach ($stats as $uid => $stat) {
      printf("uid = %s (%s): %d of %d tests failed\n", $uid < 0 ? 'other' : $uid, $stat['label'], count($stat['fail']), $stat['count']);
      if ($stat['fail']) {
        print join("\n", $stat['fail']) . "\n";
      }
      print "\n";
    }
  }
}
