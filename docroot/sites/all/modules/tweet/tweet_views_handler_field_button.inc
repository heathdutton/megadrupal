<?php

/**
 * @file
 *   Provides a Tweet button field for Views.
 */

class tweet_views_handler_field_button extends views_handler_field {
  function render($values) {
    if (variable_get('tweet_teaser', 'none') != 'none') {
      $links = array();
      foreach (tweet_sites(TRUE) as $site => $info) {
        $links['tweet_' . $site] = array(
          'title' => tweet_to_twitter($site, '', '', $values->{$this->field_alias}),
          'html' => TRUE,
        );
      }
      if (!empty($links)) {
        return theme('links', array(
          'links' => $links,
          'attributes' => array('class' => array('links', 'inline')),
        ));
      }
    }
  }
}
