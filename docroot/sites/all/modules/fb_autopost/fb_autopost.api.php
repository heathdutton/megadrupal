<?php
/**
 * @file
 * Hook API examples for the Facebook Autopost module.
 */

/**
 * Implements hook_fb_autopost_publication_publish().
 *
 * @param array $publication
 *   The publication that was just published.
 *
 * @param array $result
 *   The result that was returned from Facebook.
 */
function hook_fb_autopost_publication_publish($publication, $result) {
  if (!empty($publication) && isset($publication['type']) && isset($publication['params'])) {
    drupal_set_message(
      t('The following %type has been posted to Facebook: !params',
        array(
          '%type'   => $publication['type'],
          '!params' => '<pre>' . print_r($publication['params'], TRUE) . '</pre>',
        )));
  }
}

/**
 * Implements hook_fb_autopost_publication_entity_publish().
 *
 * @param FacebookPublicationEntity $publication
 *   The publication entity that was just published.
 *
 * @param array $result
 *   The result that was returned from Facebook.
 */
function hook_fb_autopost_publication_entity_publish($publication, $result) {
  if (!empty($result) && isset($result['id'])) {
    $publication->facebook_id = $result['id'];

    // Update the publication with the ID returned from Facebook
    facebook_publication_save($publication);
  }
}
