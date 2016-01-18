<?php

/**
 * Entity view callback.
 */
function openid_sso_provider_rps_view($entity) {
  drupal_set_title(entity_label('openid_sso_provider_rps', $entity));
  return entity_view('openid_sso_provider_rps', array(entity_id('openid_sso_provider_rps', $entity) => $entity), 'full');
}
