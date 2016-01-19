<?php

/**
 * @file
 * Contains Views controllers for the Shared Content entities.
 */

/**
 * Views controller class for the consignment.
 */
class SharedContentIndexViewsController extends EntityDefaultViewsController {

  /**
   * Overrides EntityDefaultViewsController::views_data().
   */
  public function views_data() {
    $data = parent::views_data();
    $data['sharedcontent_index']['table']['base']['access query tag'] = 'sharedcontent_index_access';
    return $data;
  }

}

/**
 * Views controller class for the Shared Content assignment.
 */
class SharedContentassignmentViewsController extends EntityDefaultViewsController {

  /**
   * Overrides EntityDefaultViewsController::views_data().
   */
  public function views_data() {
    $data = parent::views_data();
    $data['sharedcontent_assignment']['table']['base']['access query tag'] = 'sharedcontent_assignment_access';
    return $data;
  }

}
