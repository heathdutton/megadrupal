<?php
/**
 * @file
 * Helper class to set up the Disqus formatter.
 */

namespace Drupal\disqus_widgets;

/**
 * Helper class to work around the poorly packaged Disqus API.
 *
 * Because the Disqus API is all in one file adding a namespace to it results
 * in the DisqusAPI class looking for Disqus\dsq_json_decode when trying to
 * process API results. It can't find the function, so the results are empty.
 *
 * When the Disqus API is packaged as separate files this helper can be removed.
 */
class DisqusAPI extends \Disqus\DisqusAPI {

  public function __construct($key = NULL, $format = 'json', $version = '3.0') {

    // Replace the default json format with a namespaced one.
    $this->formats['json'] = 'Disqus\dsq_json_decode';

    parent::__construct($key, $format, $version);
  }
}
