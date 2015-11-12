<?php

/**
 * @file
 * Default theme implementation to display a space.
 *
 * Available variables:
 * - $label: the (sanitized) name of the space.
 * - $content: An array of items for the content of the space (fields and
 *   description). Use render($content) to print them all, or print a subset
 *   such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $url: Direct URL of the current space.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. Default values can be one or more of the following:
 *   - studyroom-space: The current template type, i.e., "theming hook".
 *   - location-[location-name]: The location to which the space belongs to.
 *     For example, if the space is a "Tag" it would result in "location-tag".
 *
 * Other variables:
 * - $space: Full space object. Contains data that may not be safe.
 * - $view_mode: View mode, e.g. 'full', 'teaser'...
 * - $page: Flag for the full page state.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the space. Increments each time it's output.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_studyroom_space()
 * @see template_process()
 *
 * @ingroup themeable
 */
?>
<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="content"<?php print $content_attributes; ?>>
    <div id='calendar'></div>
    <?php print render($content); ?>
  </div>
</div>
