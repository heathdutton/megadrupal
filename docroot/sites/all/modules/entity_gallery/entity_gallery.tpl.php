<?php

/**
 * @file
 * Default theme implementation to display an entity gallery.
 *
 * Available variables:
 * - $title: the (sanitized) title of the entity gallery.
 * - $content: An array of entity gallery items. Use render($content) to print
 *   them all, or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The entity gallery's author's picture from
 *   user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of entity gallery's author output from
 *   theme_username().
 * - $entity_gallery_url: Direct URL of the current entity gallery.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_entity_gallery().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - entity_gallery: The current template type; for example, "theming hook".
 *   - entity-gallery-[type]: The current entity gallery type. For example, if
 *     the entity gallery contains "File entities" it would result in
 *     "entity-gallery-file". Note that the machine name will often be in a
 *     short form of the human readable label.
 *   - entity-gallery-teaser: Entity galleries in teaser form.
 *   - entity-gallery-preview: Entity galleries in preview mode.
 *   The following are controlled through the entity gallery publishing options.
 *   - entity-gallery-sticky: Entity galleries ordered above other non-sticky
 *     entity galleries in teaser listings.
 *   - entity-gallery-unpublished: Unpublished entity galleries visible only to
 *     administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $entity_gallery: Full entity gallery object. Contains data that may not be
 *   safe.
 * - $type: Entity gallery type; for example, node, file, user, etc.
 * - $uid: User ID of the entity gallery author.
 * - $created: Time the entity gallery was published formatted in Unix
 *   timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the entity gallery. Increments each time it's output.
 *
 * Entity gallery status variables:
 * - $view_mode: View mode; for example, "full", "teaser".
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $readmore: Flags true if the teaser content of the entity gallery cannot
 *   hold the main description content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the entity gallery a
 * corresponding variable is defined; for example, $entity_gallery->description
 * becomes $description. When needing to access a field's raw values,
 * developers/themers are strongly encouraged to use these variables. Otherwise
 * they will have to explicitly specify the desired field language; for example,
 * $entity_gallery->description['en'], thus overriding any language negotiation
 * rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_entity_gallery()
 * @see template_process()
 *
 * @ingroup themeable
 */
?>
<div id="entity-gallery-<?php print $entity_gallery->egid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php print $user_picture; ?>

  <?php print render($title_prefix); ?>
  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $entity_gallery_url; ?>"><?php print $title; ?></a></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <?php if ($display_submitted): ?>
    <div class="submitted">
      <?php print $submitted; ?>
    </div>
  <?php endif; ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the links now so that we can render them later.
      hide($content['links']);
      print render($content);
    ?>
  </div>

  <?php print render($content['links']); ?>

</div>
