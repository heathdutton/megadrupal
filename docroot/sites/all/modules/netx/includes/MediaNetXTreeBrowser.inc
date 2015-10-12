<?php

/**
 * @file
 * Contains \MediaNetXTreeBrowser.
 */

/**
 * Media browser plugin for displaying a specific tree view and display.
 */
class MediaNetXTreeBrowser extends MediaBrowserPlugin {

  /**
   * {@inheritdoc}
   */
  public function access($account = NULL) {
    return file_entity_access('create', $account);
  }

  /**
   * {@inheritdoc}
   */
  public function view() {
    // Check if the user is able to add remote media.
    if (user_access('add media from remote sources')) {
      return array(
        'form' => drupal_get_form('netx_cfb_form', $this->params['types'], $this->params['multiselect']),
        // @todo Uncomment when media browser start support callbacks to load
        //   tabs on demand.
        // '#callback' => 'netx',
      );
    }
    return array(
      '#markup' => t('No access to remote sources'),
    );
  }

}
