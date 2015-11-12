<?php
/**
 * @file
 * Twitter Pull bean plugin.
 */

class TwitterPullBean extends BeanPlugin {
  /**
   * Declares default block settings.
   */
  public function values() {
    return array(
      'settings' => array(
        'twitkey' => FALSE,
        'num_items' => 3,
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

    $form['settings']['twitkey'] = array(
      '#type' => 'textfield',
      '#title' => t('Twitter Key'),
      '#description' => t('Can be a username [prepended with @] a hashtag [prepended with #] or a search keyword [no prefix].'),
      '#size' => 40,
      '#maxlength' => 255,
      '#default_value' => isset($bean->settings['twitkey']) ? $bean->settings['twitkey'] : '',
    );

    $form['settings']['num_items'] = array(
      '#type' => 'select',
      '#title' => t('Number of items'),
      '#description' => t('Maximum number of tweets to pull from the Twitter result-set.'),
      '#options' => drupal_map_assoc(range(1, 20)),
      '#default_value' => isset($bean->settings['num_items']) ? $bean->settings['num_items'] : 3,
    );

    return $form;
  }

  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    $content['twitter_results']['#markup'] = twitter_pull_render($bean->settings['twitkey'], FALSE, $bean->settings['num_items']);
    return $content;
  }
}
