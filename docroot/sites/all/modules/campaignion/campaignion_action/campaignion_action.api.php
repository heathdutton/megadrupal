<?php

/**
 * @file
 * Example implementations for all hooks that are
 * invoked by this module.
 */

use \Drupal\little_helpers\Webform\Submission;

/**
 * @return array
 *   class names indexed by (machine readable) content-type names
 */
function hook_campaignion_action_info() {
  $types['webform'] = array(
    'class' => 'Drupal\\campaignion_action\\FlexibleForm',
    'parameters' => array(
      'thank_you_page' => array(
        'type' => 'thank_you_page',
        'reference' => 'field_thank_you_pages',
      ),
    ),
  );
  return $types;
}

/**
 * This hook is triggered asynchronously after an action has been taken.
 *
 * Use this whenever you do something lengthy based on an action. For example:
 *  - Import of supporter data into your CRM.
 *  - Calling external APIs.
 *
 * @param $node The node object of the action.
 * @param $submissionObj The \Drupal\little_helpers\Webform\Submission object
 *   that can be used to obtain data from the submission.
 */
function hook_campaignion_action_taken($node, Submission $submissionObj) {
  $myCRM->import($node, $submissionObj);
}
