<?php

/**
 * @file
 * Default theme implementation for configuring social links block.
 *
 * @ingroup themeable
 */

  // Add table javascript.
  drupal_add_js('misc/tableheader.js');
  drupal_add_tabledrag('social-links', 'order', 'sibling', 'weight-select', NULL);
?>
<table id="social-links" class="sticky-enabled">
  <thead>
    <tr>
      <th><?php print t('Item'); ?></th>
      <th><?php print t('Weight'); ?></th>
    </tr>
  </thead>
  <tbody>
    <?php $row = 0; ?>
      <?php foreach ($link_listing as $delta => $data): ?>
      <tr class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?>">
        <td class="social-link"><?php print $data->link; ?></td>
        <td><?php print $data->weight_select; ?></td>
      </tr>
      <?php $row++; ?>
    <?php endforeach; ?>
  </tbody>
</table>

