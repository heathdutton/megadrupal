<?php

/**
 * @file
 * Define a new StateMachine for the node
 */

/**
 * Implements hook_state_flow_plugins().
 *
 * Define the ctools plugin to add a new state machine type for the node workflow.
 * In this example we are Add a "reviewed" state to the StateFlow class.
 */

/**
 * Implements hook_state_flow_plugins().
 */
function hook_state_flow_plugins() {
  $info = array();
  $path = drupal_get_path('module', 'state_flow') . '/plugins';
  $info['state_flow_test'] = array(
    'handler' => array(
      'class' => 'StateFlowTest',
      'file' => 'state_flow.inc',
      'path' => $path,
      'parent' => 'state_flow'
    ),
  );
  return $info;
}

/**
 * Implements hook_state_flow_machine_type_alter()
 *
 * @param string $machine_type
 * @param object $node
 */
function hook_state_flow_machine_type_alter(&$machine_type, $node) {
  $machine_type = 'state_flow_test';
}

/**
 * Implements hook_state_flow_events_page_alter().
 *
 * Allows altering the render array for the workflow tab prior to being output.
 *
 * @param $data
 * The render array for the page content.
 *
 * @param $node
 * The node that the workflow page is being rendered for.
 *
 * @param $events
 * The available events/actions for the current workflow state.
 */
function hook_state_flow_events_page_alter(&$data, $node, $events) {
  // Remove the version number
  unset($data['content']['current_revision']['current_revision_vid']);
}

/**
 * Implements hook_state_flow_draft_table_alter().
 *
 * Allows altering the content of the Drafts table on the workflow tab
 * prior to rendering.
 *
 * @param $table
 * The render array for the drafts table.
 *
 * @param $node
 * The node that the workflow page is being rendered for.
 *
 * @param $states
 * An array of all of the revisions and workflow states for the node.
 */
function hook_state_flow_draft_table_alter(&$table, $node, $states) {
  // Remove the VID column header.
  unset($table['header'][0]);

  // Remove the vid column.
  foreach ($table['rows'] as $row_num => $row) {
    unset($table['rows'][$row_num][0]);
  }
}

/**
 * Implements hook_state_flow_others_table_alter().
 *
 * Allows altering the content of the Other Revisions table on the workflow tab
 * prior to rendering.
 *
 * @param $table
 * The render array for the others table.
 *
 * @param $node
 * The node that the workflow page is being rendered for.
 *
 * @param $states
 * An array of all of the revisions and workflow states for the node.
 */
function hook_state_flow_others_table_alter(&$table, $node, $states) {
  // Remove the vid column header.
  unset($table['header'][0]);

  // Remove the vid column.
  foreach ($table['rows'] as $row_num => $row) {
    unset($table['rows'][$row_num][0]);
  }
}

/**
 * Implements hook_state_flow_others_table_alter().
 *
 * Allows altering the content of the workflow history table on the workflow tab
 * prior to rendering.
 *
 * @param $table
 * The render array for the history table.
 *
 * @param $node
 * The node that the workflow page is being rendered for.
 */
function hook_state_flow_history_table_alter(&$table, $node) {

}

/**
 * Define a new workflow for a node type
 */
class StateFlowTest extends StateFlow {
	/**
	 * Override the init method to set the new states
	 *
	 * Add a to review state and "Review" event
	 */
	public function init() {
	  self::init();
		$this->add_state('review');

    $this->create_event('review', array(
      'origin' => 'draft',
      'target' => 'review',
    ));

	  // Initialize events
	  $this->create_event('publish', array(
	    'origin' => 'review',
	    'target' => 'published',
	  ));
	}

	/**
	 * Override workflow from working on certain content types
	 *
	 * @return bool
	 */
	public function ignore() {
    if ($this->object->type == 'ctype') {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Override what event is used to skip directly to publish
   *
   */
  public function skip_to_publish() {
    return 'immediate publish';
  }
}
