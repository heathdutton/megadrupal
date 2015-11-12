<?php

/**
 * @file
 * jQuery social stream bean plugin.
 */
class JquerySocialStreamBean extends BeanPlugin {
  /**
   * Declares default block settings.
   */
  public function values() {
    return array(
      'settings' => array(
        'conf' => array(),
      ),
    );
  }

  /**
   * Builds extra settings for the block edit form.
   */
  public function form($bean, $form, &$form_state) {
    $form = array();
    $form['settings'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Options'),
    );
    $form['settings']['conf'] = jquery_social_stream_settings_form(isset($bean->settings['conf']) ? $bean->settings['conf'] : array());
    return $form;
  }

  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    return jquery_social_stream_block_content(isset($bean->settings['conf']) ? $bean->settings['conf'] : array());
  }
}
