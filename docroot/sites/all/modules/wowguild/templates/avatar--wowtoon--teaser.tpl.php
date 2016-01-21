<?php

/**
 * @file
 * A basic template for swortoon entities
 *
 * Available variables:
 * - $content: An array of comment items. Use render($content) to print them all, or
 *   print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $title: The name of the swortoon
 * - $url: The standard URL for viewing a swortoon entity
 * - $page: TRUE if this is the main view page $url points too.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. By default the following classes are available, where
 *   the parts enclosed by {} are replaced by the appropriate values:
 *   - entity-profile
 *   - swortoon-{TYPE}
 *
 * Other variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * @see template_preprocess()
 * @see template_preprocess_entity()
 * @see template_process()
 */

// We'll be showing our own realm field.  Hide the generic one.
hide($content['field_wowtoon_realm']);
?>
<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>>
        <a href="<?php print $url; ?>"><?php print $title; ?></a>
    </h2>
  <?php endif; ?>

  <div class="content"<?php print $content_attributes; ?>>

<img src="<?php echo $render['avatar']; ?>" width="48" height="48" class="avatar select-avatar-block-select-avatar" />

    <div class="level-class-race shadow color-c<?php echo $classId; ?>">
    <strong><?php echo $level; ?></strong>
      <?php echo $race; ?>
      <?php echo $class; ?>,
      <?php echo $realm; ?>
    </div>
    <?php if ($guild_name) :?><div class="guild-name"><?php echo $guild_name; ?></div><?php endif; ?>
    
    <div class="achievements-spec-ilevel">
      <span class="achievements"><?php echo $achievement_points; ?></span>
      <?php if (!empty($spec)) :?><span class="spec"><?php echo $spec; ?></span><?php endif; ?>
      <span class="ilevel" title="Equiped Average iLevel: <?php echo $avg_ilevel; ?>">avg ilevel: <?php echo $avg_ilevel_best; ?></span>
      <?php if (!empty($validated_img)): ?>
      <span class="validated-image"><?php echo $validated_img; ?></span>
      <?php endif; ?>
    </div>
    <?php
      print render($content);
    ?>
  </div>
</div>
