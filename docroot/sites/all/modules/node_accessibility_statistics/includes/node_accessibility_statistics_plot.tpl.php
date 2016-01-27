<?php

/**
 * @file
 * Default theme implementation.
 *
 * Displays a single node accessibility statistics plot.
 *
 * Available Variables:
 *
 * Content Variables:
 * - $plot_machine_name: A machine-friendly name for the plot.
 * - $plot_human_name: A human-friendly name for the plot.
 * - $image_url: The url path to the image.
 * - $plot_rows: An array of rows, each array item contains the following
 *   fields:
 *   - machine_name: Machine-friendly name of the row.
 *   - human_name: Human-friendly name of the row.
 *   - link: (optional) A url in which this row will link to.
 *   - value: The statistical value in which the row is associated with.
 * - $total_human_name: (optional) Human-friendly name of what the total
 *   represents.
 * - $total_value: (optional) The statistical value in which the total is
 *   associated with.
 *
 * Styling Variables:
 * - $base_class: A generated, generic, css class.
 * - $specific_class: A generated, somewhat unique, css class.
 * - $title_block: The type of block in which the title is.
 * - $list_block: The type of bloick in which the list is (such as: ul or ol).
 */
?>
<div class="<?php print($base_class . '-wrapper ' . $specific_class . '-wrapper');?>">
  <?php if (!empty($title_block)) { ?>
    <<?php print($title_block); ?> class="<?php print($base_class); ?>-title <?php print($specific_class); ?>-title"><?php print($plot_human_name); ?></<?php print($title_block); ?>>
  <?php } ?>
  <?php if (!empty($image_url)) { ?> <img alt="<?php print($plot_human_name); ?>" title="<?php print($plot_human_name); ?>" src="<?php print($image_url); ?>" class="<?php print($base_class . '-image ' . $specific_class . '-image');?>"><?php } ?>
  <<?php print($list_block); ?> class="<?php print($base_class . '-list ' . $specific_class . '-list'); ?>">
    <?php foreach ($plot_rows as $plot_row => &$plot_row_data) { ?>
    <li class="<?php print($base_class . '-row ' . $specific_class . '-row'); ?>" <?php print($specific_class); ?>-title" title="<?php print($plot_row_data['human_name']);?>">
      <?php if (empty($plot_row_data['link'])) { ?>
        <span class="<?php print($base_class . '-row-name ' . $specific_class . '-row-name'); ?>"><?php print($plot_row_data['machine_name']); ?></span>
      <?php } else { ?>
        <a href="<?php print($plot_row_data['link']); ?>" class="<?php print($base_class . '-row-name ' . $specific_class . '-row-name'); ?>"><?php print($plot_row_data['show_human_name'] ? $plot_row_data['human_name'] : $plot_row_data['machine_name']); ?></a>
      <?php } ?>
      <span class="<?php print($base_class . '-row-value ' . $specific_class . '-row-value'); ?>"><?php print($plot_row_data['value']); ?></span>
    </li>
    <?php } ?>
  </<?php print($list_block); ?>>
  <?php if (!empty($total_human_name) && !empty($total_value)) { ?>
    <div class="<?php print($base_class . '-total ' . $specific_class . '-total');?>">
      <span class="<?php print($base_class . '-total-name ' . $specific_class . '-total-name'); ?>"><?php print($total_human_name); ?></span>
      <span class="<?php print($base_class . '-total-value ' . $specific_class . '-total-value'); ?>"><?php print($total_value); ?></span>
    </div>
  <?php } ?>
</div>
