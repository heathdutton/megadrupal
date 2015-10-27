<?php
/**
 * @file
 * Fruit Blossom template file.
 */

/**
 * Styling for Node Submitted.
 */
function fruit_blossom_node_submitted($node) {
  return t('<div class="date_box">
              <div class="date_box_month">@month</div>
              <div class="date_box_day">@day</div>
            </div>',
    array(
      '@month' => format_date($node->created, 'custom', 'M'),
      '@day' => format_date($node->created, 'custom', 'd'),
    ));
}
