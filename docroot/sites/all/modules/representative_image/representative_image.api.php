<?php

/**
 * @file
 * Describe hooks provided by the Representative Image module.
 */

/**
 * Alter the image path of the representative image to use.
 *
 * You will likely use file_create_url() to help build that URL. An example
 * function name might be mymodule_representative_image_node_image_alter(); for
 * actual usage please see the code at
 * tests/representative_image_test/representative_image_test.module.
 *
 * @param  string $image
 *   The full URL of the representative image.
 *
 * @param  object $entity
 *   The current entity. This is provided for context.
 *
 * @param  string $bundle_name
 *   The name of the bundle we are dealing with. This is provided for context.
 */
function hook_representative_image_ENTITY_TYPE_image_alter(&$image, $entity, $bundle_name) {

}
