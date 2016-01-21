<?php
/**
 * @file
 * Hot TV Rooms block functionality for the zeebox Everywhere module.
 */

class ZeeboxHotTVRoomsBlockBean extends BeanPlugin {
  /**
   * Declares default block settings.
   */
  public function values() {
    return array(
      'settings' => array(
        'tv_room_link' => FALSE,
        'limit' => 5,
        'user_id' => FALSE,
      ),
    );
  }

  /**
   * Builds extra settings for the block edit form.
   */
  public function form($bean, $form, &$form_state) {
    $form['settings'] = array(
      '#tree' => TRUE,
    );
    $form['settings']['tv_room_link'] = array(
      '#type' => 'textfield', // MH: dunno how to ensure URL validation rules are applied...
      '#title' => t('Your TV Room Page URL'),
      '#description' => t('Enter the URL of the page that you wish the user to be navigated to when they click the Hot TV Rooms widget. Leaving blank will navigate to zeebox.com/tv/rooms/room-id.'),
      '#default_value' => $bean->settings['tv_room_link'],
      '#required' => FALSE,
    );
    $form['settings']['limit'] = array(
      '#type' => 'textfield',
      // '#size' => 5,
      '#title' => t('Limit'),
      '#description' => t('Maximum number of Hot TV Room results to display in the widget.'),
      '#default_value' => $bean->settings['limit'],
      '#required' => TRUE,
    );
    $form['settings']['user_id'] = array(
      '#type' => 'textfield',
      '#title' => t('zeebox User ID'),
      '#description' => t('Pop in the ID of the zeebox user that you wish to drawn a list of TV Rooms from.'),
      '#default_value' => $bean->settings['user_id'],
      '#required' => TRUE,
    );
    return $form;
  }

  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    return array(
      '#theme' => 'zeebox_hot_tv_rooms',
      '#settings' => $bean->settings,
    );
  }
}
