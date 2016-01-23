<?php

/**
 * Return the embed codes for a given node limited by Ooyala object type.
 *
 * Implement this hook to allow your module to inform the Ooyala module of
 * associated embed codes for a node when performing actions such as saving
 * a node.
 *
 * @param $node
 *   The node to fetch embed codes for.
 * @param $type
 *   The types of Ooyala objects to fetch associated embed codes for. 'all'
 *   indicates that all object embed codes for the node should be returned.
 *
 * @return
 *   An array of embed_codes associated with the node.
 */
function hook_ooyala_node_embed_codes($node, $type) {
  // From the ooyala_channels module.
  $embed_codes = array();
  if ($type == 'all' || $type == 'channel') {
    $embed_code = ooyala_channels_node_embed_code($node->nid);
    if ($embed_code) {
      $embed_codes[] = $embed_code;
    }
  }
  return $embed_codes;
}

/**
 * Return an array of additional columns to include with Ooyala fields.
 *
 * Implement this hook to add a column for additional information or metadata
 * to be associated with the field. For example, this could be used to store
 * data saved with the custom metadata API from the Ooyala backlot.
 *
 * @return
 *   An array of columns matching the Schema API column specification. As well,
 *   an additional 'index' key can be set to TRUE to indicate that the column
 *   should have an index associated with it if the field storage backend
 *   supports it.
 */
function hook_ooyala_columns() {
  return array(
    'episode_id' => array(
      'type' => 'varchar',
      'length' => 6,
      'index' => TRUE,
    ),
  );
}

/**
 * Alter the PATCH API call sent for an asset which is being updated
 * in Backlot.
 *
 * Use this hook to piggy back on the API call already made when updating
 * an asset's name in Backlot to match it's entity label. This gives other
 * modules an opportunity to update properties of an asset in Backlot without
 * needing to make additional API calls.
 *
 * This is hook is invoked after an entity is inserted or updated.
 *
 * @param array $params
 *   Will contain at least the keys 'embed_code' and 'name', mapping to those
 *   properties for an Ooyala Asset.
 *
 * @param array $context
 *   An array with the following structure:
 *   @code
 *   array(
 *     'entity_type' => 'node', // May be any entity type.
 *     'entity' => $node, // The entity the ooyala field is attached to.
 *     'field' => $field, // The field via which the updated asset is attached.
 *     'instance' => $instance, // The instance of the field.
 *     'langcode' => 'en', // The langcode for the field value.
 *     'items' => array(), // All the values associated with the field.
 *   );
 *   @endcode
 *
 *   For more information about these params see hook_field_insert().
 *
 *  @see ooyala_field_asset_patch()
 */
function hook_ooyala_field_asset_patch_alter(&$params, $context) {
  if ($context['entity_type'] == 'node') {
    $node = $context['entity'];

    // If the asset is attached to a node, then set the Backlot asset's
    // description to the value of the first item in the body field that
    // has the same $langcode.
    $description = '';
    if (!empty($node->body[$langcode][0]['value'])) {
      $description = $node->body[$langcode][0]['value'];
    }

    $params['description'] = $description;
  }
}
