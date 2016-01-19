<?php
/**
 * @file
 * Contains all hooks that are exposed by the module.
 */

/**
 * Hook for checking if an user has access to the masked content.
 *
 * @param string $entity_type
 *   The type of entity the content is in.
 * @param object $entity
 *   The entity object.
 * @param object $account
 *   The user object to perform the access check on.
 * @param array $attributes
 *   An array of attributes that were found on the parsed shortcode.
 *
 * @return string
 *   - NODE_ACCESS_ALLOW: if the user has access.
 *   - NODE_ACCESS_DENY: if the user doesn't has access.
 */
function hook_content_mask_access($account, array $attributes) {

}

/**
 * Hook for modifying the attributes parsed from the shortcode.
 *
 * @param array $attributes
 *   An array of attributes parsed from the shortcode.
 */
function hook_content_mask_shortcode_attributes_alter(array &$attributes) {

}
