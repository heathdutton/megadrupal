<?php

/**
 * @file
 * Class implementation for FBAutopostPost
 */

/**
 * Special case for publication type Post
 */
class FBAutopostPost extends FBAutopost {
  /**
   * Prepares the parameters to publish to Facebook, this means setting any
   * field or destination dependent configuration.
   */
  protected function publishParameterPrepare(&$publication) {
    parent::publishParameterPrepare($publication);
    // It is mandatory to have action links for posts. Provide them if empty.
    $name = t('Visit');
    $link = $publication['params']['link'] ?: $GLOBALS['base_url'];
    // Actions is encoded in drupal as name|link. This should be prepared as an
    // array.
    if (!empty($publication['params']['actions'])) {
      list($name, $link) = explode('|', $publication['params']['actions']);
    }
    $publication['params']['actions'] = array(array(
      'name' => $name,
      'link' => $link,
    ));
  }
}
