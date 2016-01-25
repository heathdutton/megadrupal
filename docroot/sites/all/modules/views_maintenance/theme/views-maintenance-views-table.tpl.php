<?php
/**
 * @file
 * Displays table with views and display status information.
 *
 * Available variables:
 * - $thead: HTML of primary table header.
 * - $views: array containing views info:
 *   - $views[NAME]['table']: table with displays use cases info.
 *   - $views[NAME]['description']: view description.
 *   - $views[NAME]['name']: view name.
 *   - $views[NAME]['type']: view type, e.g. "Node" or "User".
 *   - $views[NAME]['storage']: storage type.
 *   - $views[NAME]['status']: status of view based on displays use cases.
 *   - $views[NAME]['links']: administrative links for view.
 *   - $views[NAME]['rows']: array of rows used to build displays table.
 *   - $views[NAME]['displays']: array containing displays info:
 *     - $views[NAME]['displays'][ID]['name']: human-readable display title (RAW).
 *     - $views[NAME]['displays'][ID]['type']: human-readable display type (RAW).
 *     - $views[NAME]['displays'][ID]['use_cases']: array of display use cases.
 * - $views_header: array with primary table headers suitable for theme_table()
 *   or tablesort_init().
 * - $displays_header: array containing displays table header.
 *
 * @see template_preprocess_views_maintenance_views_table()
 */
?>
<div class="views-mnt-views">
  <table class="views-mnt-views-table" cellspacing="0" cellpadding="0">
    <?php print $thead ?>
    <tbody>
      <?php if (!empty($views)) : ?>
        <?php foreach ($views as $view_id => $view_info) : ?>
          <tr class="views-mnt-view-info">
            <td class="views-mnt-view-toggle">&nbsp;</td>
            <td class="views-mnt-view-name"><?php print $view_info['name'] ?></td>
            <td class="views-mnt-view-type"><?php print $view_info['type'] ?></td>
            <td class="views-mnt-view-storage"><?php print $view_info['storage'] ?></td>
            <td class="views-mnt-view-status"><?php print $view_info['status'] ?></td>
            <td class="views-mnt-view-links"><?php print $view_info['links'] ?></td>
          </tr>
          <tr class="views-mnt-view-displays">
            <td colspan="<?php print count($views_header) ?>" class="views-mnt-view-displays">
              <?php if ($view_info['description']) : ?>
                <div class="views-mnt-description" title="<?php print t('View description') ?>"><?php print $view_info['description'] ?></div>
              <?php endif; ?>
              <div class="views-mnt-displays-caption"><?php print t('Displays:') ?></div>
              <?php print $view_info['table'] ?>
            </td>
          </tr>
        <?php endforeach; ?>
      <?php else : ?>
        <tr class="views-mnt-view-info">
          <td colspan="<?php print count($views_header) ?>" class="views-mnt-empty">
            <?php print t('Create or enable some views first.') ?>
          </td>
        </tr>
      <?php endif ?>
    </tbody>
  </table>
</div>
