<?php

/**
 * @file
 * Template for spinint.
 *
 * Available variables are:
 * $instance: @see hook_field_widget_form().
 * $value: The default value.
 */
?>

<label for="<?php print $instance['field_name']; ?>">
  <?php print check_plain($instance['label']); ?>
</label>
<div class="spinint-wrapper">
<?php if (strlen($instance['settings']['prefix']) > 0): ?>
  <div class="prefix">
    <?php print check_plain($instance['settings']['prefix']); ?>
  </div>
<?php endif; ?>

  <div class="spinint-scroller">
    <a href="#" class="spinint-up">Up</a>

    <span class="spinint"><?php print $value; ?></span>

    <a href="#" class="spinint-down">Down</a>
  </div>

<?php if (strlen($instance['settings']['suffix']) > 0): ?>
  <div class="suffix">
    <?php print check_plain($instance['settings']['suffix']); ?>
  </div>
<?php endif; ?>

</div>
