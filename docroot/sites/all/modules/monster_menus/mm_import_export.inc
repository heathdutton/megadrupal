<?php
/**
 * Functions to import/export a section of the MM tree.
 */

require_once('mm_create_path.inc');

define('MM_IMPORT_VERSION', '1.0');
define('MM_IMPORT_ADD',     'add');
define('MM_IMPORT_UPDATE',  'update');
define('MM_IMPORT_DELETE',  'delete');

function mm_export_form($form, &$form_state) {
  if (module_exists('node_export')) {
    $form['include_nodes'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include page contents (nodes)'),
      '#default_value' => isset($form_state['values']['include_nodes']) ? $form_state['values']['include_nodes'] : FALSE,
    );
  }
  else {
    $form['include_nodes'] = array(
      '#markup' => t('<p>To export page contents (nodes), the !link module is required. Only pages will be exported.</p>', array('!link' => l('node_export', 'https://drupal.org/project/node_export'))),
    );
  }

  $form['mmtid'] = array(
    '#type' => 'mm_catlist',
    '#mm_list_min' => 1,
    '#mm_list_max' => 1,
    '#mm_list_selectable' => MM_PERMS_READ,
    '#title' => t('Start at:'),
    '#required' => TRUE,
    '#description' => t('Export the tree starting at this location.'),
    '#default_value' => isset($form_state['values']['mmtid']) ? $form_state['values']['mmtid'] : NULL,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 1,
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Export')
    )
  );

  if (isset($form_state['mm_export_result'])) {
    $lines = min(substr_count($form_state['mm_export_result'], "\n"), 100);
    $form['code'] = array(
      '#type' => 'textarea',
      '#weight' => 2,
      '#title' => t('Export'),
      '#default_value' => $form_state['mm_export_result'],
      '#rows' => $lines,
    );
  }

  return $form;
}

function mm_export_form_validate($form, &$form_state) {
  if (!mm_content_user_can($form_state['values']['mmtid'], MM_PERMS_READ)) {
    form_set_error('mmtid', t('You do not have permission to read the starting location.'));
  }
  else {
    try {
      reset($form_state['values']['mmtid']);
      $form_state['mm_export_result'] = mm_export(key($form_state['values']['mmtid']), !empty($form_state['values']['include_nodes']));
    }
    catch (MMImportExportException $e) {
      drupal_set_message(t('An error occurred: @error', array('@error' => $e->getTheMessage())), 'error');
    }
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * @class Exception class used to throw error if import/export fails. Includes
 * translation with t().
 */
class MMImportExportException extends Exception {
  private $vars;

  public function __construct($message, $vars = array()) {
    $this->vars = $vars;
    parent::__construct($message);
  }

  public function __toString() {
    return t(parent::__toString(), $this->vars);
  }

  public function getTheMessage() {
    return t(parent::getMessage(), $this->vars);
  }
}


/**
 * @class Class used to hold page data for import with mm_create_path().
 */
class MMExportCat extends MMCreatePathCat {
  public static function __set_state($array) {
    _mm_export_prepend_groups_mmtid($array);
    return new self($array);
  }
}

/**
 * @class Class used to hold group data for import with mm_create_path().
 */
class MMExportGroup extends MMCreatePathGroup {
  public static function __set_state($array) {
    _mm_export_prepend_groups_mmtid($array);
    return new self($array);
  }
}

function _mm_export_prepend_groups_mmtid(&$array) {
  // Prepend the correct groups tree root to any permission groups.
  static $groups_root;
  if (!isset($groups_root)) {
    $groups_root = mm_content_get(mm_content_groups_mmtid());
  }

  foreach (array(MM_PERMS_WRITE, MM_PERMS_SUB, MM_PERMS_APPLY, MM_PERMS_READ) as $m) {
    if (isset($array['perms'][$m]['groups']) && is_array($array['perms'][$m]['groups'])) {
      foreach ($array['perms'][$m]['groups'] as &$g) {
        if (is_array($g)) {
          foreach ($g as $key => $gid) {
            $g[$key] = &$array['pool'][$gid];
          }
          array_unshift($g, $groups_root);
        }
      }
    }
  }
  // Keep mm_content_insert_or_update() happy.
  unset($array['pool']);
}

/**
 * Export a section of the MM tree.
 *
 * @param $mmtid
 *   Starting location.
 * @param $include_nodes
 *   If TRUE, include node contents.
 * @throws MMImportExportException
 *   Any error condition.
 * @return string
 *   PHP source code for the exported data.
 */
function mm_export($mmtid, $include_nodes = FALSE) {
  if ($include_nodes && !module_exists('node_export')) {
    throw new MMImportExportException('The node_export module is required to export nodes.');
  }

  if (!class_exists('MMExportIter')) {
    class MMExportIter extends getTreeIterator {
      public $nodes, $pages, $subrequest, $pool, $users, $parents, $aliases;
      private $include_nodes, $page_nodes;

      public function __construct($include_nodes) {
        $this->pages = $this->nodes = $this->parents = $this->aliases = $this->subrequest = $this->pool = $this->users = $this->page_nodes = array();
        $this->include_nodes = $include_nodes;
      }

      public function iterate($item) {
        // Skip recycle bins
        if ($item->perms[MM_PERMS_IS_RECYCLE_BIN]) {
          return -1;
        }
        if (!$item->perms[MM_PERMS_READ]) {
          throw new MMImportExportException('You do not have permission to read the page/group with mmtid=@mmtid.', array('@mmtid' => $item->mmtid));
        }
        // Create a new object and filter out unused fields.
        $compare = $item->perms[MM_PERMS_IS_GROUP] ? new MMExportGroup(array()) : new MMExportCat(array());
        $i = (object)array_intersect_key((array)$item, (array)$compare);
        unset($i->mmtid);

        if (!empty($item->bid)) {
          $i->menu_start = $item->bid;
        }

        $i->perms = array();
        // Groups
        $select = db_select('mm_tree', 't');
        $select->join('mm_tree_access', 'a', 'a.mmtid = t.mmtid');
        $select->leftJoin('mm_tree', 't2', 'a.gid = t2.mmtid');
        $result = $select->fields('t2', array('mmtid', 'name'))
          ->fields('a', array('mode'))
          ->condition('t2.mmtid', '0', '>=')
          ->condition('a.mmtid', $item->mmtid)
          ->execute();
        foreach ($result as $r) {
          if ($r->mmtid == $item->mmtid) {
            $i->perms[$r->mode]['groups'][] = 'self';
          }
          else {
            $i->perms[$r->mode]['groups'][$r->mmtid] = $this->add_group($r->mmtid, $item->mmtid);
          }
        }

        // Individual users
        $select = db_select('mm_tree', 't');
        $select->join('mm_tree_access', 'a', 'a.mmtid = t.mmtid');
        $result = $select->fields('a', array('mode', 'gid'))
          ->condition('a.gid', '0', '<')
          ->condition('a.mmtid', $item->mmtid)
          ->execute();
        foreach ($result as $r) {
          $i->perms[$r->mode]['users'] = array();
          foreach (mm_content_get_uids_in_group($r->gid) as $uid) {
            $i->perms[$r->mode]['users'][] = $this->uid2username($uid);
          }
        }

        // Owner
        $i->uid = $this->uid2username($i->uid);

        if ($item->perms[MM_PERMS_IS_GROUP]) {
          if ($i->vgroup = mm_content_is_vgroup($item->mmtid)) {
            // Virtual group
            $vquery = db_query('SELECT qfrom, field FROM {mm_group} g INNER JOIN {mm_vgroup_query} v ON g.vgid = v.vgid WHERE g.gid = :mmtid', array(':mmtid' => $item->mmtid))->fetchObject();
            if ($vquery) {
              $i->qfrom = $vquery->qfrom;
              $i->qfield = $vquery->field;
            }
          }
          else {
            // Regular group
            $i->members = array();
            foreach (mm_content_get_uids_in_group($item->mmtid) as $uid) {
              $i->members[] = $this->uid2username($uid);
            }
          }
        }
        else {
          // Cascaded settings
          $result = db_query('SELECT * FROM {mm_cascaded_settings} WHERE mmtid = :mmtid', array(':mmtid' => $item->mmtid));
          foreach ($result as $r) {
            if ($r->multiple) {
              if (!isset($i->cascaded) || !is_array($i->cascaded[$r->name])) {
                $i->cascaded[$r->name] = array();
              }
              $i->cascaded[$r->name][] = $r->data;
            }
            else {
              $i->cascaded[$r->name] = $r->data;
            }
          }

          // Nodes
          if ($this->include_nodes) {
            if ($nids = mm_content_get_nids_by_mmtid($item->mmtid)) {
              foreach (array_diff($nids, array_keys($this->nodes)) as $new_nid) {
                $this->nodes[$new_nid] = TRUE;
              }
              $this->page_nodes[$item->mmtid] = $nids;
            }
          }
        }

        $this->pool[$item->mmtid] = $item->perms[MM_PERMS_IS_GROUP] ? new MMExportGroup((array) $i) : new MMExportCat((array) $i);
        if (!$this->subrequest) {
          array_splice($this->aliases, $item->level);
          $this->aliases[] = $item->alias;
          array_splice($this->parents, $item->level);
          $this->parents[] = $item->mmtid;
          $this->pages[join('/', $this->aliases)] = $this->parents;
        }

        return 1;
      }

      public function dump() {
        $out = '$version = ' . $this->export_var(MM_IMPORT_VERSION) . ";\n";
        $out .= '$pool = array();' . "\n";
        foreach ($this->pool as $mmtid => $item) {
          $out .= '$pool[' . $mmtid . '] = ' . preg_replace('{,\n\)\)$}', ",\n  'pool' => &\$pool,\n))", $this->export_var($item)) . ";\n";
        }
        if ($this->nodes) {
          $out .= '$nodes = array(' . "\n";
          foreach (array_keys($this->nodes) as $nid) {
            if ($node = node_load($nid)) {
              $node->uid = $this->uid2username($node->uid);
              unset($node->mm_catlist);

              if (isset($node->users_w)) {
                $new_users = array();
                foreach (array_keys($node->users_w) as $uid) {
                  $new_users[$this->uid2username($uid)] = '';
                }
                $node->users_w = $new_users;
              }

              if (isset($node->groups_w)) {
                $groups_w = array();
                foreach (array_keys($node->groups_w) as $gid) {
                  $groups_w[$gid] = $this->add_group($gid);
                }
                $node->groups_w = $groups_w;
              }

              $node_export = node_export($node, 'drupal');
              if ($node_export['success']) {
                // Remove leading "array(\n  " and trailing ")".
                $out .= "    $nid => " . substr($node_export['output'], 9, -1);
              }
            }
          }
          $out .= ");\n";
          $out .= '$page_nodes = array(' . "\n";
          foreach ($this->pool as $mmtid => $item) {
            if (isset($this->page_nodes[$mmtid])) {
              $out .= "  $mmtid => array(&\$nodes[" . implode('], &$nodes[', $this->page_nodes[$mmtid]) . "]),\n";
            }
          }
          $out .= ");\n";
        }
        else {
          $out .= '$nodes = $page_nodes = array();' . "\n";
        }
        $out .= '$pages = array(' . "\n";
        foreach ($this->pages as $path => $items) {
          $out .= "  array(  // $path\n";
          $out .= '    &$pool[' . join($items, "],\n" . '    &$pool[') . "],\n  ),\n";
        }
        $out .= ");\n";
        return $out;
      }

      /**
       * Export a field.
       *
       * This is a replacement for var_export(), allowing us to more nicely
       * format exports. It will recurse down into arrays and will try to
       * properly export bools when it can, though PHP has a hard time with this
       * since they often end up as strings or ints.
       *
       * This function is adapted from ctools_var_export() in the ctools module.
       */
      private function export_var($var, $prefix = '') {
        if (is_array($var)) {
          if (empty($var)) {
            return 'array()';
          }
          $output = "array(\n";
          foreach ($var as $key => $value) {
            $output .= $prefix . "  " . $this->export_var($key) . " => " . $this->export_var($value, $prefix . '  ') . ",\n";
          }
          return $output . $prefix . ')';
        }
        if (is_object($var) && get_class($var) === 'stdClass') {
          // var_export() will export stdClass objects using an undefined
          // magic method __set_state() leaving the export broken. This
          // workaround avoids this by casting the object as an array for
          // export and casting it back to an object when evaluated.
          return '(object) ' . $this->export_var((array) $var, $prefix);
        }
        if (is_bool($var)) {
          return $var ? 'TRUE' : 'FALSE';
        }
        if (is_object($var)) {
          return get_class($var) . '::__set_state(' . $this->export_var((array) $var, $prefix) . ')';
        }
        return var_export($var, TRUE);
      }

      protected function uid2username($uid) {
        if (!isset($this->users[$uid])) {
          $loaded = user_load($uid);
          if (!$loaded) {
            throw new MMImportExportException('Unknown user with uid=@uid.', array('@uid' => $uid));
          }
          $this->users[$uid] = $loaded->name;
        }
        return $this->users[$uid];
      }

      private function add_group($gid, $orig_mmtid = NULL) {
        $groups_mmtid = mm_content_groups_mmtid();
        $out = array();
        foreach (mm_content_get_parents_with_self($gid) as $mmtid) {
          if ($mmtid != 1 && $mmtid != $groups_mmtid) {
            if (!isset($this->pool[$mmtid])) {
              if ($orig_mmtid && in_array($mmtid, $this->subrequest)) {
                throw new MMImportExportException('The groups with mmtid=@mmtid1 and mmtid=@mmtid2 have a circular reference which cannot be exported.', array(
                    '@mmtid1' => $orig_mmtid,
                    '@mmtid2' => $mmtid
                  ));
              }
              $temp_item = mm_content_get($mmtid, array(MM_GET_FLAGS));
              $temp_item->perms = mm_content_user_can($mmtid);
              $this->subrequest[] = $mmtid;
              $this->iterate($temp_item);
              array_pop($this->subrequest);
            }
            $out[] = (int) $mmtid;
          }
        }
        return $out;
      }
    }
  }

  $params = array(
    MM_GET_TREE_RETURN_BLOCK =>      TRUE,
    MM_GET_TREE_RETURN_FLAGS =>      TRUE,
    MM_GET_TREE_RETURN_NODE_COUNT => TRUE,
    MM_GET_TREE_RETURN_PERMS =>      TRUE,
    MM_GET_TREE_ITERATOR =>          new MMExportIter($include_nodes),
  );
  mm_content_get_tree($mmtid, $params);

  return $params[MM_GET_TREE_ITERATOR]->dump();
}

function mm_import_form($form, &$form_state) {
  if (module_exists('node_export')) {
    $form['include_nodes'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include page contents (nodes)'),
      '#default_value' => isset($form_state['values']['include_nodes']) ? $form_state['values']['include_nodes'] : FALSE,
    );
  }
  else {
    $form['include_nodes'] = array(
      '#markup' => t('<p>To import page contents (nodes), the !link module is required. Only pages will be imported.</p>', array('!link' => l('node_export', 'https://drupal.org/project/node_export'))),
    );
  }
  $form['mode'] = array(
    '#type' => 'radios',
    '#options' => array(
      MM_IMPORT_ADD    => t('Add-only: Don\'t change existing nodes or pages, just add anything new'),
      MM_IMPORT_UPDATE => t('Update: Overwrite existing nodes and pages, if different; does not modify any pre-existing nodes/pages not part of the import'),
      MM_IMPORT_DELETE => t('Delete first: Move any existing nodes and pages to a recycle bin before importing')
    ),
    '#default_value' => 'add',
  );

  $form['mmtid'] = array(
    '#type' => 'mm_catlist',
    '#mm_list_min' => 1,
    '#mm_list_max' => 1,
    '#mm_list_selectable' => MM_PERMS_WRITE,
    '#title' => t('Start at:'),
    '#required' => TRUE,
    '#description' => t('Import the tree as a child of this location.'),
    '#default_value' => isset($form_state['values']['mmtid']) ? $form_state['values']['mmtid'] : NULL,
  );

  $form['test'] = array(
    '#type' => 'checkbox',
    '#title' => t('Test only'),
    '#description' => t('If checked, test the import but do not actually make any changes.'),
    '#default_value' => FALSE,
  );

  if (isset($form_state['mm_import_result'])) {
    $form['result'] = array(
      '#type' => 'fieldset',
      '#title' => t('Import Results'),
      '#collapsible' => TRUE,
    );
    $results = $form_state['mm_import_result'];
    foreach (array('errors' => t('Errors (@count)'), 'pages' => t('Pages (@count)'), 'nodes' => t('Nodes (@count)'), 'groups' => t('Groups (@count)')) as $type => $desc) {
      if (isset($results[$type]) && $results[$type]) {
        $rows = _mm_import_item_list($results[$type]);
        $form['result'][$type] = array(
          '#type' => 'fieldset',
          '#title' => t($desc, array('@count' => count($rows))),
          '#collapsible' => TRUE,
          '#collapsed' => $type != 'errors',
          array(
            '#theme' => 'item_list',
            '#items' => $rows,
          ),
        );
      }
    }
    if (!element_children($form['result'])) {
      $form['result'][] = array(
        '#markup' => $form_state['values']['test'] ? t('No changes would have occurred.') : t('No changes occurred.'),
      );
    }
  }

  $form['code'] = array(
    '#type' => 'textarea',
    '#title' => t('Code to Import'),
    '#required' => TRUE,
    '#rows' => 10,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Import')
    )
  );

  return $form;
}

function _mm_import_item_list($array) {
  $out = array();
  foreach ($array as $key => $msg) {
    if (isset($msg['message'])) {
      $out[] = t($msg['message'], $msg['vars']);
    }
    else {  // Nested array, for groups/pages
      $out = array_merge($out, _mm_import_item_list($msg));
    }
  }
  return $out;
}

function mm_import_form_validate($form, &$form_state) {
  $vals = $form_state['values'];
  if (!mm_content_user_can($vals['mmtid'], MM_PERMS_WRITE)) {
    form_set_error('mmtid', t('You do not have permission to write to the starting location.'));
  }
  else if ($vals['mode'] == MM_IMPORT_DELETE && $vals['test']) {
    form_set_error('mode', t('The "Test only" option cannot be used with "Delete first" mode.'));
  }
  else {
    try {
      $stats = array('suppress_errors' => TRUE);
      mm_import($vals['code'], !empty($vals['mmtid']) ? key($vals['mmtid']) : NULL, $vals['mode'], $vals['test'], !empty($vals['include_nodes']), $stats);
      $form_state['mm_import_result'] = $stats;
    }
    catch (MMImportExportException $e) {
      drupal_set_message(t('An error occurred: @error', array('@error' => $e->getTheMessage())), 'error');
    }
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Import a section of the MM tree.
 *
 * @param $code
 *   The code block, from mm_export().
 * @param $start_mmtid
 *   The MM Tree ID of the location at which to import. The imported section is
 *   added (or updated) as a child of this location.
 * @param $mode
 *   One of the constants:
 *   - MM_IMPORT_ADD: Add-only: Don't change existing nodes or pages, just add
 *       anything new.
 *   - MM_IMPORT_UPDATE: Update: Overwrite existing nodes and pages, if
 *       different; does not modify any pre-existing nodes/pages not part of the
 *       import.
 *   - MM_IMPORT_DELETE: Delete first: Move any existing nodes and pages to a
 *       recycle bin before importing.
 * @param $test
 *   If TRUE, go through the motions, but do not make any changes.
 * @param $include_nodes
 *   If TRUE, insert or update any nodes contained in the $code.
 * @param $stats
 *   (optional) Array with which to populate statistics:
 *   - nodes:
 *     An array indexed by nid, containing sub-arrays with the elements
 *     "message" and "vars", which describe the nodes that were acted upon.
 *   - pages:
 *     An array indexed by mmtid, containing an array of sub-arrays each with
 *     the elements "message" and "vars", which describe the pages that were
 *     acted upon.
 *   - groups:
 *     An array indexed by mmtid, containing an array of sub-arrays each with
 *     the elements "message" and "vars", which describe the groups that were
 *     acted upon.
 *   - errors:
 *     An array containing sub-arrays with the elements "message" and "vars",
 *     which describe any errors that occurred.
 *   A count of the number of pages/nodes acted upon can be derived using the
 *   count() function.
 * @throws MMImportExportException
 *   Any error condition.
 */
function mm_import($code, $start_mmtid, $mode = MM_IMPORT_ADD, $test = FALSE, $include_nodes = FALSE, &$stats = 'undef') {
  eval($code);
  if (!isset($version) || !is_array($nodes) || !is_array($page_nodes) || !is_array($pool) | !is_array($pages)) {
    throw new MMImportExportException('mm_import: The code does not include all necessary variables.');
  }
  if (version_compare($version, '1.0', '!=')) {
    throw new MMImportExportException('mm_import: Incompatible version.');
  }
  if ($include_nodes && !module_exists('node_export')) {
    throw new MMImportExportException('mm_import: The node_export module is required to import nodes.');
  }
  if ($mode == MM_IMPORT_DELETE && $test) {
    throw new MMImportExportException('mm_import: The test option cannot be used with "delete first" mode.');
  }

  $add_only = $mode == MM_IMPORT_ADD;
  foreach ($pages as $index => $page) {
    if (!$index) {
      $start = mm_content_get($start_mmtid);
      if ($mode == MM_IMPORT_DELETE) {
        $existing = mm_content_get(array('parent' => $start_mmtid, 'alias' => $page[0]->alias));
        if ($existing) {
          mm_content_move_to_bin($existing[0]->mmtid);
          mm_content_update_sort_queue();
        }
      }
      array_unshift($page, $start);
    }
    mm_create_path($page, FALSE, $test, $add_only, $stats);
  }

  if ($include_nodes && $page_nodes) {
    if (is_array($stats) && isset($stats['errors']) && $stats['errors']) {
      _mm_report_error('Nodes were not imported, due to errors when processing pages/groups.', array(), $stats);
    }
    else {
      $groups_root = mm_content_get(mm_content_groups_mmtid());
      foreach ($page_nodes as $mmtid => $nodes2) {
        $new_mmtid = $pool[$mmtid]->mmtid;
        foreach ($nodes2 as $node) {
          if (!isset($node->mm_catlist)) {
            // Create any necessary groups.
            $new_groups_w = array();
            foreach ($node->groups_w as $g) {
              $path = array($groups_root);
              foreach ($g as $gid) {
                $path[] = &$pool[$gid];
              }
              mm_create_path($path, FALSE, $test, $add_only, $stats);
              $new_groups_w[$path[count($path) - 1]->mmtid] = '';
            }
            $node->groups_w = $new_groups_w;

            $node->mm_catlist = $node->_mm_import_catlist = array();
          }
          $node->mm_catlist[$new_mmtid] = '';         // Used by MM
          $node->_mm_import_catlist[] = $new_mmtid;   // Internal use
        }
      }

      // Now create/update the nodes.
      require_once DRUPAL_ROOT . '/includes/utility.inc';
      if (is_array($stats)) {
        $stats['nodes'] = array();
      }
      foreach ($nodes as $node) {
        $code_string = drupal_var_export(array($node));
        // Never let node_export_import() save here, just test.
        $result = node_export_import($code_string, 't', FALSE);
        if (is_array($stats) && !$result['success']) {
          foreach ($result['output'] as $message) {
            $stats['errors'][] = array('message' => $message, 'vars' => array());
          }
        }
        if ($result['success']) {
          $imported_node = $result['nodes'][0];
          if ($mode == MM_IMPORT_DELETE) {
            $imported_node->is_new = $is_new = TRUE;
            $imported_node->uuid = uuid_generate();
          }
          else {
            $exists = db_query('SELECT n2.nid FROM {mm_node2tree} n2 INNER JOIN {node} n ON n.nid = n2.nid WHERE n2.mmtid IN(:mmtid) AND n.type = :type AND (n.uuid = :uuid OR n.nid = :nid) ORDER BY n.uuid = :uuid DESC LIMIT 1', array(
              ':mmtid' => $node->_mm_import_catlist,
              ':type' => $node->type,
              ':uuid' => $node->uuid,
              ':nid' => $node->nid,
            ))->fetchField();

            if ($add_only && $exists) {
              // Don't save or update.
              continue;
            }

            if ($exists) {
              // Update with a new revision.
              $imported_node->nid = $exists;
              $imported_node->uuid = $node->uuid;
              $imported_node->is_new = $is_new = FALSE;
              $imported_node->revision = 1;
            }
            else {
              $imported_node->is_new = $is_new = TRUE;
              $imported_node->uuid = uuid_generate();
            }
          }

          if (!$test) {
            node_export_save($imported_node);
          }
          if (is_array($stats)) {
            if ($test) {
              $msg = $is_new ? 'Would have created !link' : 'Would have updated !link';
            }
            else {
              $msg = $is_new ? 'Created !link' : 'Updated !link';
            }
            $stats['nodes'][] = array(
              'message' => $msg,
              'vars' => array('!link' => l(mm_ui_fix_node_title($imported_node->title), 'node/' . $imported_node->nid))
            );
          }
        }
      }
    }
  }
}
