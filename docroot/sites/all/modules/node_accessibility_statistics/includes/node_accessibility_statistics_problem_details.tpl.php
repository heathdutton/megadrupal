<?php

/**
 * @file
 * Default theme implementation.
 *
 * Displays details about a node accessibility statistics problem.
 *
 * Available Variables:
 *
 * Content Variables:
 * - $problem_machine_name: The machine-friendly name of the problem.
 * - $problem_human_name: The human-friendly name of the problem.
 * - $severity_human_name: The human-friendly severity name.
 * - $total: The total number of issues detected that match this problem.
 * - $explanation: A detailed explanation of the problem.
 * - $statistics_type_path: A path appended onto the problem link to restrict
 *   the displayed problems to a specific type.
 *
 * Styling Variables:
 * - $base_class: A generated, generic, css class.
 * - $specific_class: A generated, somewhat unique, css class.
 * - $id: The unique id representing the problem.
 * - $title_block: The type of block in which the title is.
 * - $listing_link: A boolean that designates whether or not to show/hide the
 *   listing link.
 */
?>
<div class="<?php print($base_class . '-wrapper ' . $specific_class . '-wrapper');?>">
  <<?php print($title_block); ?> id="<?php print($id); ?>" class="<?php print($base_class); ?>-title <?php print($specific_class); ?>-title" title="<?php print(t("!problem report", array('!problem' => $problem_machine_name)));?>"><?php print($problem_human_name); ?></<?php print($title_block); ?>>
  <ul>
    <li class="<?php print($base_class . '-severity ' . $specific_class . '-severity'); ?>">
      <span class="<?php print($base_class . '-severity-text ' . $specific_class . '-severity-text'); ?>"><?php print(t("Severity of this report is"));?></span>
      <span class="<?php print($base_class . '-severity-name ' . $specific_class . '-severity-name'); ?>"><?php print($severity_human_name); ?></span>
    </li>
    <li class="<?php print($base_class . '-total ' . $specific_class . '-total'); ?>">
      <span class="<?php print($base_class . '-total-pre_text ' . $specific_class . '-total-pre_text'); ?>"><?php print(t("There are a total of"));?></span>
      <span class="<?php print($base_class . '-total-problems ' . $specific_class . '-total-problems'); ?>"><?php print($total); ?></span>
      <span class="<?php print($base_class . '-total-post_text ' . $specific_class . '-post_text'); ?>"><?php print(t("issues for this report."));?></span>
    </li>
    <li class="<?php print($base_class . 'explanation ' . $specific_class . '-explanation'); ?>">
      <div class="<?php print($base_class . '-explanation-title ' . $specific_class . '-explanation-title'); ?>"><?php print(t("Report Explanation"));?></div>
      <div class="<?php print($base_class . '-explanation-markup ' . $specific_class . '-explanation-markup'); ?>">
        <?php print($explanation); ?>
      </div>
    </li>
    <?php if ($listing_link) { ?>
      <li class="<?php print($base_class . 'listing ' . $specific_class . '-listing'); ?>">
        <a class="<?php print($base_class . 'listing-link ' . $specific_class . '-listing-link'); ?>" href="/admin/content/accessibility/node_statistics/problem/<?php print($problem_machine_name . $statistics_type_path); ?>"><?php print(t("View listing of all issues for the %problem report.", array('%problem' => $problem_machine_name))); ?></a>
      </li>
    <?php } ?>
  </ul>
</div>
