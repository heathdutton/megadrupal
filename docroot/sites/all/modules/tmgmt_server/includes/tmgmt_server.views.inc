<?php

/**
 * @file
 * Contains Views controllers for the Translation Management Server module.
 */

/**
 * Views controller class for the remote source entity.
 */
class TMGMTRemoteSourceViewsController extends EntityDefaultViewsController {

  /**
   * Overrides EntityDefaultViewsController::views_data().
   */
  public function views_data() {
    $data = parent::views_data();

    return $data;
  }

}

/**
 * Views controller class for the client entity.
 */
class TMGMTClientViewsController extends EntityDefaultViewsController {

  /**
   * Overrides EntityDefaultViewsController::views_data().
   */
  public function views_data() {
    $data = parent::views_data();

    $data['tmgmt_server_client']['operations'] = array(
      'title' => t('Operations'),
      'help' => t('Displays a list of options which are available for a remote client.'),
      'real field' => 'cid',
      'field' => array(
        'handler' => 'tmgmt_server_handler_field_client_operations',
      ),
    );

    return $data;
  }

}
