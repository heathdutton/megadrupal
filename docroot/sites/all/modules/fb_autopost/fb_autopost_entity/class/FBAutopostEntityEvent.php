<?php

/**
 * @file
 * Class implementation for FBAutopostEntityEvent
 */

/**
 * Special case for FacebookPublicationType Event
 */
class FBAutopostEntityEvent extends FBAutopostEntity {
  /**
   * Prepares the parameters to publish to Facebook, this means settings any
   * field or destination dependent configuration.
   */
  protected function publishParameterPrepare(&$publication) {
    parent::publishParameterPrepare($publication);
    if (is_numeric($publication['params']['start_time'])) {
      $start = new DateTime('@' . $publication['params']['start_time']);
      $publication['params']['start_time'] = $start->format(DateTime::ISO8601);
    }
    if (!empty($publication['params']['end_time']) && is_numeric($publication['params']['end_time'])) {
      $end = new DateTime('@' . $publication['params']['end_time']);
      $publication['params']['end_time'] = $end->format(DateTime::ISO8601);
    }
  }

  /**
   * Edits a publication in facebook from a stored entity. Events in Facebook
   * can actually be updated, this means that there is no deletion needed.
   *
   * @param FacebookPublicationEntity $publication
   *   The fully loaded Facebook publication entity
   *
   * @throws FBAutopostException
   * @see FBAutopost::remoteEdit()
   */
  public function remoteEntityEdit(FacebookPublicationEntity $publication_entity) {
    // For an event we should update the Entity, instead of deleting an
    // recreating it.
    $wrapper = entity_metadata_wrapper('facebook_publication', $publication_entity);
    $remote_id = $wrapper->facebook_id->value();

    $publication = array(
      'type' => $publication_entity->type,
      'params' => fb_autopost_entity_get_properties($publication_entity),
    );

    $this->publishParameterPrepare($publication);
    // Call api method from ancestor.
    return $this->api(
      // Post to destination on the selected endpoint.
      '/' . $remote_id,
      // This is fixed.
      'POST',
      // Add access token to the params.
      $publication['params']
    );
  }
}
