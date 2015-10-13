<?php
/**
 * @file
 * Helper functions for working with Disqus and widget implementations.
 */

/**
 * Helper function to log or display DisqusAPIErrors.
 *
 *
 * @param Disqus\DisqusAPIError $error
 *   A DisqusAPIError object.
 *
 * @param int $code
 *   The exception code.
 *   @see http://disqus.com/api/docs/errors/ for error codes.
 *
 * @param string $message
 *   The message to display or log.
 *
 * @param boolean $watchdog
 *   TRUE to log to watchdog, FALSE to display on screen.
 */
function disqus_widgets_handle_disqusapierror(Disqus\DisqusAPIError $error, $code, $message, $watchdog = TRUE) {

  if ($error->getCode() == $code) {

    if ($watchdog) {

      watchdog('disqus_widgets', $message, array(), WATCHDOG_ERROR);
    }
    else {

      drupal_set_message($message, 'error', FALSE);
    }
  }
}

/**
 * Get comments from Disqus.
 *
 * @param integer $limit
 *   The number of comments to retrieve.
 *
 * @param string $page
 *   The page to filter by.
 *
 * @param string $ident
 *   The ident to filter by.
 *
 * @return array|bool
 *   An array of comments or FALSE on failure.
 */
function disqus_widgets_get_comments($limit = 3, $page = NULL, $ident = NULL) {

  $comments = FALSE;

  $settings = disqus_widgets_settings();

  $args = array(
    'forum' => $settings['shortname'],
    'limit' => intval($limit),
  );

  if (!empty($page)) {

    $args['thread'] = 'link:' . $page;
  }
  else if (!empty($ident)) {

    $args['thread'] = 'ident:' . $ident;
  }

  $cid = __FUNCTION__ . ':' . serialize($args);

  if (!($cache = cache_get($cid))) {

    try {

      $disqus = disqus_widgets_disqus();
      if (!empty($disqus)) {

        $comments = $disqus->threads->listPosts($args);
        cache_set($cid, $comments, 'cache', CACHE_TEMPORARY);
      }
    }
    catch (Disqus\DisqusAPIError $x) {

      // 2 will be returned if there are no comments for a page.
      if (2 != $x->getCode()) {

        disqus_widgets_handle_disqusapierror($x, $x->getCode(), $x->getMessage());
        return FALSE;
      }
    }
  }
  else {

    $comments = $cache->data;
  }

  return $comments;
}

/**
 * Retrieve a Disqus object.
 *
 * Helper to maintain a single copy of the Disqus object instead of a new one
 * for each request.
 *
 * @return DisqusAPI
 *   A DisqusAPI object.
 */
function disqus_widgets_disqus() {

  $disqus = &drupal_static(__FUNCTION__);

  $settings = disqus_widgets_settings();
  if (empty($disqus) && !empty($settings['api_secret'])) {

    /*
      Use our DisqusAPI implementation to work around a problem with the
      default Disqus library as currently packaged.
     */
    $disqus = new \Drupal\disqus_widgets\DisqusAPI($settings['api_secret']);
  }

  return $disqus;
}

/**
 * Get the settings to use for Disqus.
 *
 * @return array
 *   An array of settings.
 */
function disqus_widgets_settings() {

  $settings = &drupal_static(__FUNCTION__, array());

  if (empty($settings)) {

    $settings = array(
      'identifier' => current_path(),
      'shortname' => variable_get('disqus_forum'),
      'api_secret' => variable_get('disqus_api_secret'),
    );

    drupal_alter(__FUNCTION__, $settings);
  }

  return $settings;
}
