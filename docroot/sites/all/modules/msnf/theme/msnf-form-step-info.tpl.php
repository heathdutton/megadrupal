<?php
/**
 * @file msnf-form-step-info.tpl.php
 *
 * Displays information about the current form step.
 *
 * Available variables:
 * - $step_classes_array
 *   Array with classes for info wrapper.
 * - $step_classes:
 *   Classes for info wrapper, separated by space.
 * - $show_step_title
 *   Whether to display the current step title or not.
 * - $step_description
 *   Description of current step.
 * - $title_element
 *   HTML element name of step title.
 * - $step_position
 *   Number of the current step.
 * - $step_count
 *   Number of total available steps.
 */
?>
<div class="<?php print $step_classes; ?>">
  <?php if ($show_step_title): ?>
    <<?php print $title_element; ?>><?php print $step_title; ?></<?php print $title_element; ?>>
  <?php endif; ?>
  <?php if (drupal_strlen($step_description) > 0): ?>
    <div class="description">
      <?php print $step_description; ?>
    </div>
  <?php endif; ?>
</div>
