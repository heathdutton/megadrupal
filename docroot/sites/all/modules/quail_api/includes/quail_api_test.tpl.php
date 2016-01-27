<?php

/**
 * @file
 * Default theme implementation to display the validation results.
 *
 * This displays the validation results of a single validation test.
 *
 * Available Variables:
 *
 * Content Variables:
 * - $test_title: The human friendly title of the test.
 * - $test_description: A description of the test.
 * - $test_problems: An array of problems found that are associated with the
 *   test.
 *
 * Styling Variables:
 * - $base_class: A generated, generic, css class.
 * - $specific_class: A generated, somewhat unique, css class.
 * - $markup_format: The filter format to use for check_markup calls.
 */
?>
<div class="<?php print($specific_class); ?>-wrapper <?php print($base_class); ?>-wrapper">
  <fieldset id="<?php print($specific_class); ?>" class="<?php print($base_class); ?> collapsible collapsed">
    <legend><span class="fieldset-legend"><?php print($test_title); ?></span></legend>
    <div class="fieldset-wrapper">
      <div class="<?php print($base_class); ?>-description">
        <?php print($test_description); ?>
      </div>
      <div class="<?php print($base_class); ?>-problems">
        <?php foreach ($test_problems as $problem_id => $problem_data) {
          print(theme('quail_api_problem', array('problem_id' => $problem_id, 'problem_data' => $problem_data, 'markup_format' => $markup_format)));
        } ?>
      </div>
    </div>
  </fieldset>
</div>
