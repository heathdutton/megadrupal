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

<div id="<?php print $widget->id; ?>-wrapper" class="<?php print implode(' ', $classes_array); ?>">
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

  <?php if (property_exists($widget, 'operator')): ?><div class="views-widget"><?php endif; ?>
    <?php print $widget->widget; ?>
  <?php if (property_exists($widget, 'operator')): ?></div><?php endif; ?>

  <?php if (!empty($widget->description)): ?>
    <div class="description">
      <?php print $widget->description; ?>
    </div>
  <?php endif; ?>
</div>
