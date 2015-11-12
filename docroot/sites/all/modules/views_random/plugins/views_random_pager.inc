<?php

/**
 * @file
 * Definition of views_random_pager.
 */

/**
 * Plugin for views without pagers.
 *
 * @ingroup views_pager_plugins
 */
class views_random_pager extends views_plugin_pager {
  function summary_title() {
    return format_plural($this->options['items'], '@count item', '@count items', array('@count' => $this->options['items']));
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['items'] = array('default' => 0);

    return $options;
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['items'] = array(
      '#title' => t('Items'),
      '#type' => 'textfield',
      '#description' => t('The number of items to display. Enter 0 for no limit.'),
      '#default_value' => $this->options['items'],
    );
  }

  function use_pager() {
    return FALSE;
  }

  function use_count_query() {
    return FALSE;
  }
}
