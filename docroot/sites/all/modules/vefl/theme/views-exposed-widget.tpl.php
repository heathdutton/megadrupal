<?php

/**
 * @file
 * This template handles the layout of the views exposed filter form.
 *
 * Variables available:
 * - $widget->label: The visible label to print. May be optional.
 * - $widget->operator: The operator for the widget. May be optional.
 * - $widget->widget: The widget itself.
 *
 * @ingroup views_templates
 */
?>

<div class="views-exposed-widget views-widget-<?php print $widget->id; ?>">
  <?php if (!empty($widget->label)): ?>
    <label for="<?php print $widget->id; ?>">
      <?php print $widget->label; ?>
    </label>
  <?php endif; ?>
  <?php if (!empty($widget->operator)): ?>
    <div class="views-operator">
      <?php print $widget->operator; ?>
    </div>
  <?php endif; ?>
  <div class="views-widget">
    <?php print $widget->widget; ?>
  </div>
  <?php if (!empty($widget->description)): ?>
    <div class="description">
      <?php print $widget->description; ?>
    </div>
  <?php endif; ?>
</div>
