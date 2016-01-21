<?php
/**
 * @file
 * Shared TV Room ID settings collection for BEAN plugins.
 */

abstract class ZeeboxTVRoomIDBeanPlugin extends BeanPlugin {
  /**
   * Declares default block settings.
   */
  public function values() {
    return array(
      'settings' => array(
        'tv_room_id' => FALSE,
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
    $form['settings']['tv_room_id'] = array(
      '#type' => 'textfield',
      '#title' => t('zeebox TV Room ID'),
      '#description' => t('Pop the ID of your desired zeebox TV Room in here.
        The ID should look something like <strong>ad627b45-cc45-11e2-1066-20c615b252f0</strong>, and can be found in the
        URL of the TV room on zeebox.com, or in Mission Control. E.g. <a href="@link">@link</a>',
        array('@link' => url('http://zeebox.com/tv/rooms/ad627b45-cc45-11e2-1066-20c615b252f0', array('external' => TRUE)))
      ),
      '#default_value' => $bean->settings['tv_room_id'],
      '#required' => TRUE,
    );
    return $form;
  }

  /**
   * The plugin validation callback.
   */
  public function validate($values, &$form_state) {
    if (!preg_match("/[a-f0-9]+-[a-f0-9]+-[a-f0-9]+-[a-f0-9]+-[a-f0-9]+/i", $values['settings']['tv_room_id'])) {
      form_set_error('settings][tv_room_id', t('TV Show ID input is invalid.'));
    }
  }
}
