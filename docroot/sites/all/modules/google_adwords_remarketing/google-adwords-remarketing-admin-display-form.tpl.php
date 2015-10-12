<?php

/**
 * @file google-adwords-remarketing-admin-display-form.tpl.php
 * Default theme implementation to configure GARCs.
 *
 * Available variables:
 * - $garc_regions: An array of regions. Keyed by name with the title as value.
 * - $garc_listing: An array of garcs keyed by region and then delta.
 * - $form_submit: Form submit button.
 * - $throttle: TRUE or FALSE depending on throttle module being enabled.
 *
 * Each $garc_listing[$region] contains an array of garcs for that region.
 *
 * Each $data in $garc_listing[$region] contains:
 * - $data->region_title: Region title for the listed garc. < Enabled or Disabled>
 * - $data->garc_title: Campaign title.
 * - $data->region_select: Drop-down menu for assigning a region aka status.
 * - $data->weight_select: Drop-down menu for setting weights.
 * - $data->configure_link: Campaign configuration link.
 * - $data->delete_link: For deleting added campaigns.
 *
 * @see template_preprocess_garc_admin_display_form()
 * @see theme_garc_admin_display()
 */
?>
<?php
  // Add table javascript.
  drupal_add_js('misc/tableheader.js');
  drupal_add_js(drupal_get_path('module', 'google_adwords_remarketing') .'/garc.js');
  foreach ($garc_regions as $region => $title) {
    drupal_add_tabledrag('garcs', 'match', 'sibling', 'garc-region-select', 'garc-region-'. $region, NULL, FALSE);
    drupal_add_tabledrag('garcs', 'order', 'sibling', 'garc-weight', 'garc-weight-'. $region);
  }
?>
<table id="garcs" class="sticky-enabled">
  <thead>
    <tr>
      <th><?php print t('Campaign'); ?></th>
      <th><?php print t('Status'); ?></th>
      <th><?php print t('Weight'); ?></th>
      <th colspan="2"><?php print t('Operations'); ?></th>
    </tr>
  </thead>
  <tbody>
    <?php $row = 0; ?>
    <?php foreach ($garc_regions as $region => $title): ?>
      <tr class="region region-<?php print $region?>">
        <td colspan="6" class="region"><strong><?php print $title; ?></strong></td>
      </tr>
      <?php if (empty($garc_listing[$region])) : ?>
      <tr class="region-message region-<?php print $region?>-message <?php print empty($garc_listing[$region]) ? 'region-empty' : 'region-populated'; ?>">
        <td colspan="6"><em><?php print t('No Campaigns having this status'); ?></em></td>
      </tr>
      <?php endif; ?>
      <?php foreach ($garc_listing[$region] as $delta => $data): ?>
      <tr class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?><?php print $data->row_class ? ' '. $data->row_class : ''; ?>">
        <td class="garc"><?php print $data->garc_title; ?></td>
        <td><?php print $data->region_select; ?></td>
        <td><?php print $data->weight_select; ?></td>
        <td><?php print $data->configure_link; ?></td>
        <td><?php print $data->delete_link; ?></td>
      </tr>
      <?php $row++; ?>
      <?php endforeach; ?>
    <?php endforeach; ?>
  </tbody>
</table>

<?php print $form_submit; ?>
