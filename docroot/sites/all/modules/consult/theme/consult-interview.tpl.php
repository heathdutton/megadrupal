<?php
/**
 * @file
 * Consult main interview template.
 *
 * Do not change js- prefixed classes as they are required for the Backbone
 * views to attach correctly.
 */
?>
<div <?php print $attributes; ?>>
  <div class="js-questions consult--questions">
    <?php foreach ($question_groups as $group_name) : ?>
      <div class="js-questions-group" data-group-name="<?php print $group_name; ?>">
      </div>
    <?php endforeach; ?>
  </div>
  <div class="js-results consult--results">
    <?php foreach ($result_groups as $group_name) : ?>
      <div class="js-results-group" data-group-name="<?php print $group_name; ?>">
      </div>
    <?php endforeach; ?>
  </div>
</div>
