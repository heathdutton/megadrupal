<?php

/**
 * @file
 * Defines the accessibilty page drush functions.
 */

/**
 * Implements hook_drush_command().
 */
function node_accessibility_drush_command() {
  $items = array();

  $items['acc_validate_node'] = array(
    'callback' => 'node_accessibility_drush_callback_validate',
    'description' => dt("Perform accessibility validation on nodes."),
    'arguments' => array(
      'nids' => dt("Optional list of space-separated node IDs to validate."),
    ),
    'options' => array(
      '--all' => dt("Perform validation on every single node in the system (this may take a while)."),
      '--type' => dt("Perform validation on every single node in the system of the specified content type (use the machine name)."),
      '--delete' => dt("Instead of performing validation, delete the validation information for a given node."),
      '--revisions' => dt("Include all revisions for each node (can make --all take a very long time)."),
      '--chunk' => dt("Set total number of nodes to validate in one pass; defaults to 256."),
    ),
    'examples' => array(
      'drush acc_validate_node 45 46 47' => dt("Validate nodes with IDs of 45, 46, and 47."),
      'drush acc_validate_node --all' => dt("Validate all nodes in the system."),
      'drush acc_validate_node --type page' => dt("Validate all nodes in the system of the content type 'page'."),
      'drush acc_validate_node --all --revisions --delete' => dt("Delete validation problems for all revisions for all nodes in the system."),
    ),
  );

  // Add aliases for usability.
  node_accessibility_drush_command_add_alias($items, 'acc_validate_node', 'acc_validate_node');

  return $items;
}

/**
 * A function to help alias commands as other commands.
 */
function node_accessibility_drush_command_add_alias(&$items, $command, $alias) {
  // Create a property on the command for adding aliases, if not there.
  if (!isset($items[$command]['node_accessibility command aliases'])) {
    $items[$command]['node_accessibility command aliases'] = array();
  }

  // Record the alias into that property.
  $items[$command]['node_accessibility command aliases'][] = $alias;
  // Create the alias as a new command.
  $items[$alias] = $items[$command];
  // Indicate what this new command is an alias for.
  $items[$alias]['node_accessibility alias for'] = $command;
}

/**
 * Implements hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function node_accessibility_drush_help($section) {
  // This is to prevent duplication of information from hook_drush_command().
  $commands = node_accessibility_drush_command();

  foreach ($commands as $command => $command_info) {
    if ($section == 'drush:' . $command) {
      $out = $command_info['description'];

      if (isset($command_info['node_accessibility alias for'])) {
        $output .= "\nThis command is an alias for ";
        $output .= $command_info['node_accessibility alias for'] . ".";
      }

      if (isset($command_info['node_accessibility command aliases'])) {
        if (count($command_info['node_accessibility command aliases']) == 1) {
          $output .= "\nThis command can be called by it's alias; ";
          $output .= $command_info['node_accessibility command aliases'] . ".";
        }
        else {
          $last_alias = array_pop($command_info['node_accessibility command aliases']);
          $output .= "\nThis command can be called by it's aliases; ";
          $output .= implode(", ", $command_info['node_accessibility command aliases']);
          $output .= ", or " . $last_alias . ".";
        }
      }

      $info = array();
      $info['arguments'] = "Arguments";
      $info['options'] = "Options";
      $info['examples'] = "Examples";

      foreach ($info as $key => $value) {
        $out .= "\n\n$value:";

        if (isset($command_info[$key])) {
          foreach ($command_info[$key] as $k => $v) {
            $out .= "\n  " . $k . " : " . $v;
          }
        }
      }

      return dt($out);
    }
  }
}

/**
 * Drush command callback.
 *
 * Import nodes from data.
 */
function node_accessibility_drush_callback_validate() {
  $uid = drush_get_option('uid');
  $nids = array();
  $rows = array();
  $delete = TRUE;
  $revisions = TRUE;
  $chunk_size = drush_get_option('chunk');

  $node_type_settings = node_accessibility_load_node_type_settings();
  $methods = quail_api_get_validation_methods(NULL);

  if (is_null(drush_get_option('delete'))) {
    $delete = FALSE;
  }

  if (is_null(drush_get_option('revisions'))) {
    $revisions = FALSE;
  }

  if (empty($chunk_size) || !is_numeric($chunk_size)) {
    $chunk_size = 256;
  }

  if ($uid != 0) {
    if (is_null($uid)) $uid = 1;

    global $user;
    $user = user_load($uid);
  }

  if (drush_get_option('all')) {
    $query = db_select('node', 'n');
    $query->fields('n', array('nid'));

    try {
      $rows = $query->execute()->fetchAllAssoc('nid');
    }
    catch (Exception $e) {
      if (module_exists('cf_error')) {
        cf_error::on_query_execution($e);
      }
    }
  }
  else {
    $node_type = drush_get_option('type');

    if (empty($node_type)) {
      $nids = array_filter(func_get_args(), 'is_numeric');
    }
    else {
      $query = db_select('node', 'n');
      $query->fields('n', array('nid'));
      $query->condition('type', check_plain($node_type), '=');

      try {
        $rows = $query->execute()->fetchAllAssoc('nid');
      }
      catch (Exception $e) {
        if (module_exists('cf_error')) {
          cf_error::on_query_execution($e);
        }
      }
    }
  }

  foreach ($rows as $row) {
    $nids[] = $row->nid;
  }

  unset($rows);

  // break the nids array into chunks of $chunk_size to reduce memory footprint when validating large quantities of nodes.
  $total_nodes = count($nids);
  $current_count = 0;

  if ($delete) {
    drush_print("Now deleting validation results for " . $total_nodes . " nodes in chunks of " . $chunk_size .  ".");
  }
  else {
    drush_print("Now validating " . $total_nodes . " nodes in chunks of " . $chunk_size .  ".");
  }

  while ($current_count < $total_nodes) {
    $current_nids = array();
    $counter = 0;

    if ($current_count + $chunk_size > $total_nodes) {
      $chunk_size = $total_nodes - $current_count;
    }

    while ($counter < $chunk_size) {
      $current_nids[] = array_pop($nids);
      $counter++;
    }

    $vid_query = db_select('node', 'n');
    $vid_query->fields('n', array('nid', 'vid', 'type'));
    $vid_query->condition('n.nid', $current_nids, 'IN');
    $vid_executed = $vid_query->execute();
    $nodes = $vid_executed->fetchAllAssoc('nid');

    if ($delete) {
      foreach ($current_nids as $nid) {
        if ($revisions) {
          $vid = NULL;
          $vid_string = "all";
        }
        else {
          $vid = $nodes[$node->nid]->vid;
          $vid_string = $vid;
        }

        $result = node_accessibility_delete_node_problems($nid, $vid);

        if ($result === FALSE) {
          drush_set_error('DRUSH_NOT_COMPLETED', "Failed to delete validation problems for node " . $nid . ":" . $vid_string . ".");
        }

        unset($result);
      }
    }
    else {
      $vids = array();

      if ($revisions) {
        $query = db_select('node_revision', 'nr');

        $query->fields('nr');
        $query->condition('nr.nid', $current_nids, 'IN');
        $executed = $query->execute();

        $results = (array) $executed->fetchAll();

        foreach ($results as $key => &$value) {
          if (!is_array($vids[$value->nid])) {
            $vids[$value->nid] = array();
          }

          if (!in_array($value->vid, $vids[$value->nid])) {
            $vids[$value->nid][] = $value->vid;
          }
        }
      }
      else {
        foreach ($nodes as $nid => $node) {
          $vids[$nid] = array($node->vid);
        }
      }

      $results = node_accessibility_perform_validation($current_nids, $vids, NULL, NULL);

      if ($result === FALSE) {
        drush_set_error('DRUSH_NOT_COMPLETED', "Failed to perform accessibility validation.");
      }
      else {
        foreach ($current_nids as $nid) {
          $vid = $nodes[$nid]->vid;
          $type = $nodes[$nid]->type;
          $reports = $results[$nid][$vid]['report'];
          $node_settings = $node_type_settings[$type];
          $database = isset($methods[$node_settings['method']]['database']) ? $methods[$node_settings['method']]['database'] : FALSE;

          if ($database && !empty($reports) && isset($results[$nid][$vid]['report'])) {
            $no_failures = TRUE;

            foreach ($reports as $severity => $severity_results) {
              if (isset($severity_results['total']) && $severity_results['total'] > 0) {
                $no_failures = FALSE;
                break;
              }
            }

            if ($no_failures) {
              node_accessibility_delete_node_problems($nid, $vid);
            }
            else {
              node_accessibility_save_node_problems($nid, $vid, $reports);
            }
          }
        }
      }

      unset($results);
    }

    drush_print("  [ " . ($current_count + count($current_nids)) . " / " . $total_nodes . " ]");
    $current_count += $chunk_size;
  }

  drush_print("Operation complete.");
}
