<?php
/**
 * @file
 * Medium Post bean plugin.
 */

class MediumPostBean extends BeanPlugin {
  /**
   * Declares default block settings.
   */
  public function values() {
    return array(
      'settings' => array(
        'placeholder_text' => '',
        'placeholder_url' => '',
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
      '#tree' => 1,
      '#title' => t('Options'),
    );


    $form['settings']['placeholder_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Placeholder text'),
      '#description' => t('Title of Medium Post.'),
      '#default_value' => isset($bean->settings['placeholder_text']) ? $bean->settings['placeholder_text'] : '',
    );

    $form['settings']['placeholder_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Placeholder url'),
      '#description' => t('Link of Medium Post.'),
      '#default_value' => isset($bean->settings['placeholder_url']) ? $bean->settings['placeholder_url'] : '',
    );

    return $form;
  }

  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    $content['medium_bean']['#markup'] = medium_post_bean_render($bean->settings['placeholder_text'], $bean->settings['placeholder_url']);
    return $content;
  }
}
