<?php
/**
 * @file
 * This template handles the layout of the Weathercomau widget block.
 *
 * Variables available:
 * - $wrapper_classes: (string) Classes to be used on the wrapper element.
 * - $title_classes: (string) Classes to be used on the title element.
 * - $title: (string) Actual title.
 * - $content_classes: (String) Classes to be used on the items container element.
 * - $current_conditions_classes: (array) Classes to be used on each individual current condition.
 * - $current_conditions: (array) Each current condition contains:
 * - $current_condition->title: (string) Current condition title.
 * - $$current_condition->value: (string) Current condition value containing its suffix when applicable.
 *
 * @ingroup weathercomau_templates
 */
?>
<div class="<?php print $wrapper_classes; ?>">

  <h3 class="<?php print $title_classes; ?>"><?php print $title; ?></h3>

  <div class="<?php print $content_classes; ?>">
    <?php foreach ($current_conditions as $id => $current_condition): ?>
      <div class="<?php print $current_conditions_classes[$id]; ?>">
        <strong><?php print $current_condition->title ?></strong> <?php print $current_condition->value; ?>
      </div>
    <?php endforeach; ?>
  </div>

</div>
