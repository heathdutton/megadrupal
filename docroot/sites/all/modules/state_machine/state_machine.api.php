<?php

/**
 * State Machine Developer Documentation
 *
 * The State Machine module is an API only.  You must extend the StateMachine class to use this module.
 * Extend the StateMachine class to get started
 *
 * @see the state_flow module for a complete example.
 */
class TestMachine extends StateMachine {
  /**
   * Called from StateMachine::__construct to initialize the states and events.
   *
	 * Defines States Draft, Published and Unpublished.
	 * Define Events Publish, Unpublish and To Draft
   */
  public function init() {
    // Initialize states
		// @see StateMachine::create_state

		// Add Draft State
    $this->create_state('draft');
		// Add Published State
		// We Use the the "on_enter and on_exit functionality."
    $this->create_state('published', array(
	 		# When the state is set to "Published" run the mehtod "on_enter_published" on this object.
      'on_enter' => array($this, 'on_enter_published'),
			# When leaving the state "Published" then run the method "on_exit_published" on this object.
      'on_exit' => array($this, 'on_exit_published'),
    ));
		// Add the state "unpublished"
    $this->create_state('unpublished');

    // Initialize events
		// @see StateMachine::create_event

		// Add the pusblish event to  move the state from "draft" to "published"
    $this->create_event('publish', array(
      'origin' => 'draft',
      'target' => 'published',
    ));

		// Add the unpusblish event to  move the state from "published" to "unpublished"
    $this->create_event('unpublish', array(
      'origin' => 'published',
      'target' => 'unpublished',
    ));

		// Add the "to draft" event to move the state from unpublished to draft
    $this->create_event('to draft', array(
      'origin' => 'unpublished',
      'target' => 'draft',
    ));
  }

  public function on_enter_published() {
    // Do some stuff
  }

  public function on_exit_published() {
    // Do some stuff
  }
}