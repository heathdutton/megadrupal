<?php

/**
 * UI Controller for user notifications states.
 */
class UserNotificationsStatesUIController extends EntityDefaultUIController {

  public function overviewForm($form, &$form_state) {
    return drupal_get_form('user_notifications_state_overview_form');
  }

}
