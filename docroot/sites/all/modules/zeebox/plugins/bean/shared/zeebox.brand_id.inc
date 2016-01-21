<?php
/**
 * @file
 * Shared Brand ID settings collection for BEAN plugins.
 */

abstract class ZeeboxBrandIDBeanPlugin extends BeanPlugin {
  /**
   * Declares default block settings.
   */
  public function values() {
    return array(
      'settings' => array(
        'brand_id' => FALSE,
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
    $form['settings']['brand_id'] = array(
      '#type' => 'textfield',
      '#title' => t('zeebox Brand ID'),
      '#description' => t('Pop the zeebox ID of the Brand you want users to be able to "Follow" in here.
        A brand id for a show can be found by searching for the Brand name in Mission Control.
        If you need assistance, please email <a href="mailto:MissionControl@zeebox.com">MissionControl@zeebox.com</a>'
      ),
      '#default_value' => $bean->settings['brand_id'],
      '#required' => TRUE,
    );
    return $form;
  }

  /**
   * The plugin validation callback.
   */
  public function validate($values, &$form_state) {
    if (!is_numeric($values['settings']['brand_id'])) {
      form_set_error('settings][brand_id', t('Brand ID can be numeric only.'));
    }
  }
}
