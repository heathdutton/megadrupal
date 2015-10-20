<?php

/**
 * @file
 *
 * Default theme implementation to display a listing of validation results.
 *
 * This displays validation results for a single display level.
 *
 * Available Variables:
 *
 * Content Variables:
 * - $severity_id: The error id of the display level whose results this
 *   represents.
 * - $severity_machine_name: The error id of the display level whose results
 *   this represents.
 * - $severity_human_name: The error id of the display level whose results this
 *   represents.
 * - $severity_description: The description of the display level whose results
 *   this represents.
 * - $severity_results: An array containing the results for this display level.
 *
 * Styling Variables:
 * - $base_class: A generated, generic, css class.
 * - $specific_class: A generated, somewhat unique, css class.
 * - $markup_format: The filter format to use for check_markup calls.
 * - $title_block: The type of block in which the title is.
 * - $display_title: Define whether or not to display the title block.
 * - $display_description: Define whether or not to display the title block.
 */
?>
<div class="<?php print($specific_class); ?>-wrapper <?php print($base_class); ?>-wrapper">
  <?php if ($display_title) { ?>
    <<?php print($title_block); ?> class="<?php print($base_class); ?>-title"><?php print($severity_human_name); ?></<?php print($title_block); ?>>
  <?php } ?>
  <?php if ($display_description) { ?>
    <div class="<?php print($base_class); ?>-description description">
      <?php print($severity_description); ?>
    </div>
  <?php } ?>
  <?php if ($severity_results['total'] > 0) { ?>
    <div class="<?php print($base_class); ?>-reports">
      <?php foreach ($severity_results as $test_name => $test_results) {
        if ($test_name === 'total') continue;
        print(theme('quail_api_test', array('test_name' => $test_name, 'test_results' => $test_results, 'markup_format' => $markup_format)));
      } ?>
    </div>
  <?php }
  else { ?>
    <div class="<?php print($base_class); ?>-no_reports">
      <?php print(t("There is nothing to report.")); ?>
    </div>
  <?php } ?>
</div>
