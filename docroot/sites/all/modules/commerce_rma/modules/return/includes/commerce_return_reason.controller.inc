<?php

/**
 * @file
 * The controller for the return reason entity containing the CRUD operations.
 */
class CommerceReturnReasonEntityController extends EntityAPIController {
  /**
   * Unserializes the data property of loaded returns.
   */
  public function attachLoad(&$queried_returns, $revision_id = FALSE) {
    foreach ($queried_returns as $return_reason_id => &$return_reason) {
      $return_reason->data = is_string($return_reason->data) ? unserialize($return_reason->data) : FALSE;
    }

    // Call the default attachLoad() method.
    parent::attachLoad($queried_returns, $revision_id);
  }
}
