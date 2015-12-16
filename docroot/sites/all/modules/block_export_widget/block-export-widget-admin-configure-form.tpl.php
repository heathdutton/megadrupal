<?php

/**
 * @file
 * Default theme implementation to configure Block export widget.
 *
 * Available variables:
 * - $block_modules: An array of modules. Keyed by name with the title as value.
 * - $block_listing: An array of blocks keyed by module and then delta.
 * - $form_submit: Form submit button.
 *
 * Each $block_listing[$module] contains an array of blocks for that module.
 *
 * Each $data in $block_listing[$module] contains:
 * - $data->block_title: Block title.
 * - $data->export_checkbox: Checkbox for enabling export.
 * - $data->format_select: Drop-down menu for setting export format.
 * - $data->export_link: Block export link.
 *
 * @see template_preprocess_block_export_widget_admin_configure_form()
 *
 * @ingroup themeable
 */
?>
<?php
  drupal_add_js('misc/tableheader.js');
  drupal_add_css(drupal_get_path('module', 'block_export_widget') . '/block-export-widget.css');
?>
<table id="blocks" class="sticky-enabled">
  <thead>
    <tr>
      <th class="col-export"><?php print t('Export'); ?></th>
      <th><?php print t('Block'); ?></th>
      <th><?php print t('Format'); ?></th>
      <th><?php print t('Operations'); ?></th>
    </tr>
  </thead>
  <tbody>
    <?php $row = 0; ?>
    <?php foreach ($block_modules as $module => $title): ?>
      <tr class="module-title module-title-<?php print $module; ?>">
        <td colspan="4"><?php print $title; ?></td>
      </tr>
      <?php foreach ($block_listing[$module] as $delta => $data): ?>
      <tr class="<?php print $row % 2 == 0 ? 'odd' : 'even'; ?>">
        <td class="checkbox"><?php print $data->export_checkbox; ?></td>
        <td class="block"><?php print $data->block_title; ?></td>
        <td><?php print $data->format_select; ?></td>
        <td><?php print $data->export_link; ?></td>
      </tr>
      <?php $row++; ?>
      <?php endforeach; ?>
    <?php endforeach; ?>
  </tbody>
</table>

<?php print $form_submit; ?>
