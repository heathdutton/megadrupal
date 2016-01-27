<?php

/**
 * @file
 * Default theme implementation for field collection items.
 *
 * Available variables:
 * - $content: An array of comment items. Use render($content) to print them all, or
 *   print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $title: The (sanitized) field collection item label.
 * - $url: Direct url of the current entity if specified.
 * - $page: Flag for the full page state.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. By default the following classes are available, where
 *   the parts enclosed by {} are replaced by the appropriate values:
 *   - entity-field-collection-item
 *   - field-collection-item-{field_name}
 *
 * Other variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * @see template_preprocess()
 * @see template_preprocess_entity()
 * @see template_process()
 */
?>
<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="content"<?php print $content_attributes; ?>>
    <div class="resume-photo">
      <?php
      if(!empty($content['field_resume_photo'])) {
        print render($content['field_resume_photo']);
      } else {
        print theme_image(array('path' => drupal_get_path('theme', 'cloudy') . '/images/recruiter/resume_default.jpg', 'attributes' => array()));
      }
      ?>
    </div>
    <div class="resume-info clearfix">
      <?php if (isset($content['full_name'])): ?>
      <div class="field field-name clearfix">
        <div class="field__label">Name:&nbsp;</div>
        <div class="field-items"><div class="field-item even"><?php print render($content['full_name']); ?></div></div>
      </div>
      <?php endif; ?>
      <?php
      print render($content);
      ?>
    </div>
  </div>
</div>

