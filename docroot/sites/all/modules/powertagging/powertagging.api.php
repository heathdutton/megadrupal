<?php

/**
 * @file
 * API documentation for PowerTagging.
 */

/**
 * Alter the PowerTagging tag list.
 *
 * Replace the default HTML output of the field formatter "Tag list" of a
 * "PowerTagging Tags" field with your custom HTML output.
 *
 * @param string $markup
 *   The markup to alter. This parameter will always be NULL at the beginning,
 *   set it to whatever you want the HTML output to look like.
 * @param array $context
 *   An array of context variables possibly required to alter the tag list.
 *   Available array keys are:
 *   - items "items" => parameter "items" of hook_field_formatter_view()
 *   - items "entity" => parameter "entity" of hook_field_formatter_view()
 *   - string "langcode" => parameter "langcode" of hook_field_formatter_view()
 *   - int "instance" => parameter "instance" of hook_field_formatter_view()
 *
 * @see powertagging_field_formatter_view()
 * @see hook_field_formatter_view()
 */
function hook_powertagging_tag_list_alter(&$markup, $context) {
}
