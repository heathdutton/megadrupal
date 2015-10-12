<?php
/**
 * @file
 * character_sheet.features.inc
 */

/**
 * Implements hook_workflow_default_workflows().
 */
function character_sheet_workflow_default_workflows() {
  $workflows = array();

  // Exported workflow: Character evaluation
  $workflows['Character evaluation'] = array(
    'name' => 'Character evaluation',
    'tab_roles' => 'author,3,6,4',
    'options' => 'a:3:{s:16:"comment_log_node";i:1;s:15:"comment_log_tab";i:1;s:13:"name_as_title";i:1;}',
    'states' => array(
      0 => array(
        'state' => '(creation)',
        'weight' => '-50',
        'sysid' => '1',
        'status' => '1',
      ),
      1 => array(
        'state' => 'draft',
        'weight' => '0',
        'sysid' => '0',
        'status' => '1',
      ),
      2 => array(
        'state' => 'submitted',
        'weight' => '0',
        'sysid' => '0',
        'status' => '1',
      ),
      3 => array(
        'state' => 'approved',
        'weight' => '0',
        'sysid' => '0',
        'status' => '1',
      ),
      4 => array(
        'state' => 'disapproved',
        'weight' => '0',
        'sysid' => '0',
        'status' => '1',
      ),
      5 => array(
        'state' => 'removed from game',
        'weight' => '0',
        'sysid' => '0',
        'status' => '1',
      ),
    ),
    'transitions' => array(
      0 => array(
        'roles' => 'workflow_features_author_name',
        'state' => '(creation)',
        'target_state' => 'draft',
      ),
      1 => array(
        'roles' => 'workflow_features_author_name,administrator',
        'state' => 'draft',
        'target_state' => 'submitted',
      ),
      2 => array(
        'roles' => 'administrator',
        'state' => 'draft',
        'target_state' => 'approved',
      ),
      3 => array(
        'roles' => 'administrator',
        'state' => 'draft',
        'target_state' => 'disapproved',
      ),
      4 => array(
        'roles' => 'administrator',
        'state' => 'draft',
        'target_state' => 'removed from game',
      ),
      5 => array(
        'roles' => 'workflow_features_author_name,administrator',
        'state' => 'submitted',
        'target_state' => 'draft',
      ),
      6 => array(
        'roles' => 'administrator',
        'state' => 'submitted',
        'target_state' => 'approved',
      ),
      7 => array(
        'roles' => 'administrator',
        'state' => 'submitted',
        'target_state' => 'disapproved',
      ),
      8 => array(
        'roles' => 'administrator',
        'state' => 'submitted',
        'target_state' => 'removed from game',
      ),
      9 => array(
        'roles' => 'administrator',
        'state' => 'approved',
        'target_state' => 'submitted',
      ),
      10 => array(
        'roles' => 'administrator',
        'state' => 'approved',
        'target_state' => 'removed from game',
      ),
      11 => array(
        'roles' => 'administrator',
        'state' => 'approved',
        'target_state' => 'draft',
      ),
      12 => array(
        'roles' => 'administrator',
        'state' => 'approved',
        'target_state' => 'disapproved',
      ),
      13 => array(
        'roles' => 'administrator',
        'state' => 'disapproved',
        'target_state' => 'submitted',
      ),
      14 => array(
        'roles' => 'administrator',
        'state' => 'disapproved',
        'target_state' => 'draft',
      ),
      15 => array(
        'roles' => 'administrator',
        'state' => 'disapproved',
        'target_state' => 'approved',
      ),
      16 => array(
        'roles' => 'administrator',
        'state' => 'disapproved',
        'target_state' => 'removed from game',
      ),
      17 => array(
        'roles' => 'administrator',
        'state' => 'removed from game',
        'target_state' => 'submitted',
      ),
      18 => array(
        'roles' => 'administrator',
        'state' => 'removed from game',
        'target_state' => 'draft',
      ),
      19 => array(
        'roles' => 'administrator',
        'state' => 'removed from game',
        'target_state' => 'approved',
      ),
      20 => array(
        'roles' => 'administrator',
        'state' => 'removed from game',
        'target_state' => 'disapproved',
      ),
    ),
    'node_types' => array(
      0 => 'character_sheet',
    ),
  );

  return $workflows;
}
