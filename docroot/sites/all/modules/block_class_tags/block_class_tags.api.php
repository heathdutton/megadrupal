<?php
/**
 * @file
 * Defines the api functions of the module
 */

/**
 * Implements hook_block_class_tags_classes().
 *
 * Return array of classes to be available via autocomplete.
 */
function hook_block_class_tags_classes() {
	return array('margin-left', 'margin-right', 'margin-top', 'etc');
}
