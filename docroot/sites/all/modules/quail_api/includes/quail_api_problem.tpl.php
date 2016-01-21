<?php

/**
 * @file
 * Default theme implementation to display the validation results.
 *
 * This displays validation results of a single validation problem
 * or suggestion.
 *
 * Available Variables:
 *
 * Content Variables:
 * - $problem_line: The line number in which the problem was found.
 * - $problem_description: A description of the problem.
 *
 * Styling Variables:
 * - $base_class: A generated, generic, css class.
 * - $specific_class: A generated, somewhat unique, css class.
 * - $markup_format: The filter format to use for check_markup calls.
 */
?>
<div class="<?php print($specific_class); ?>-wrapper <?php print($base_class); ?>-wrapper">
  <div class="<?php print($base_class); ?>-line">
    <?php print(t("Code Snippet:")); ?>
  </div>
  <div class="<?php print($base_class); ?>-description">
    <?php print($problem_description); ?>
  </div>
</div>
