<?php

/**
 * @file
 * Contains FeedsCopypasteFetcher.
 */

/**
 * Fetches data from a textarea.
 */
class FeedsCopypasteFetcher extends FeedsFetcher {

  /**
   * Implements FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source) {
    $source_config = $source->getConfigFor($this);
    return new FeedsFetcherResult($source_config['content']);
  }

  /**
   * Source form.
   */
  public function sourceForm($source_config) {
    $form['clear'] = array(
      '#type' => 'submit',
      '#value' => t('Clear'),
      '#submit' => array(array('FeedsCopypasteFetcher', 'clearSourceForm')),
      '#limit_validation_errors' => array(),
    );
    $form['content'] = array(
      '#type' => 'textarea',
      '#rows' => 25,
      '#title' => t('Content'),
      '#description' => t('Paste here the content to be imported'),
      '#default_value' => empty($source_config['content']) ? '' : $source_config['content'],
      '#required' => TRUE,
    );
    return $form;
  }

  /**
   * Clear source form.
   */
  public static function clearSourceForm($form, &$form_state) {
    $source = feeds_source($form['#importer_id']);
    $source->config[__CLASS__]['content'] = '';
    $source->save();
  }

}
