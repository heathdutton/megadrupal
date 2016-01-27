<?php
/**
 * @file
 * Locations template for manifestation's full record display
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
// FIXME: May be better in a theme function
?>
<table id="xc-search-full-locations">
  <thead>
  <tr>
    <th><?php print t('Location'); ?></th>
    <th><?php print t('Call number'); ?></th>
    <th><?php print t('Textual Holdings'); ?></th>
    <th><?php print t('Availability'); ?></th>
  </tr>
  </thead>
  <tbody>

  <?php foreach ($locations as $location) : ?>
    <tr>
      <td><?php print $location['location']; ?></td>
      <td><?php print $location['call_number']; ?></td>
      <td><?php print $location['textual_holdings']; ?></td>
      <td id="avail-<?php print $location['id']; ?>">
        <img src="<?php print base_path() . drupal_get_path('module', 'xc_search') . '/images/ajax-loader.gif'; ?>"
          alt="<?php print t('Loading availability information'); ?>"
          title="<?php print t('Loading availability information'); ?>" />
      </td>
    </tr>
  <?php endforeach; ?>

  </tbody>
</table>
