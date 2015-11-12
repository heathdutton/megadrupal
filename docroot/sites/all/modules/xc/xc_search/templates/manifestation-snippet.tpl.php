<?php
/**
 * @file
 * Snippet template for manifestation
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

// FIXME: This should be pre-processed with variables since many of the labels
//        are not translatable. Could also be partitioned or divided into sections
//        and turned into smaller template files.
?>
<table class="xc-snippet">
<?php if (!empty($format) || !empty($extent)) : ?>
<tr class="xc-format">
  <td class="snippet-label"><?php print t('Format'); ?>:</td>
  <td<?php if (isset($format_classes)): ?> class="<?php print $format_classes; ?>"<?php endif; ?>>
    <?php if (!empty($format)) : ?>
      <?php print $format; ?>
    <?php endif; ?>
    <?php if (!empty($extent)) : ?>
      &mdash; <?php print $extent; ?>
    <?php endif; ?>
  </td>
</tr>
<?php endif ?>

<?php if ($get_availability) : ?>
<tr class="xc-availability">
  <td class="snippet-label"><?php print t('Location'); ?>:</td>
  <td id="xc-availability-<?php print $xc_record['node_id']; ?>"
    class="xc-availability <?php if ($is_journal) : ?>xc_journal<?php endif; ?>"><?php print t('loading&hellip;')?></td>
</tr>
<tr id="xc-call-number-row-<?php print $xc_record['node_id']; ?>" class="xc-call-number">
  <td class="snippet-label"><?php print t('Call number'); ?>:</td>
  <td id="xc-call-number-<?php print $xc_record['node_id']; ?>"
    class="xc-availability <?php if ($is_journal) : ?>xc_journal<?php endif; ?>"><?php print t('loading&hellip;')?></td>
</tr>
<?php else :?>
  <?php if (!empty($online_links)) : ?>
<tr class="xc-online">
  <td class="snippet-label"><?php print t('Location'); ?>:</td>
  <td><?php print join(', ', $online_links); ?></td>
</tr>
  <?php endif; ?>
<?php endif; ?>

<?php if (!empty($date)) : ?>
<tr class="xc-date">
  <td class="snippet-label"><?php print t('Date'); ?>:</td>
  <td><?php print $date; ?></td>
</tr>
<?php endif ?>

<?php if (!empty($publisher) || !empty($placeOfProduction)) : ?>
<tr class="xc-date">
  <td class="snippet-label"><?php print t('Published'); ?>:</td>
  <td>
    <?php if (!empty($placeOfProduction)) : ?>
      <?php print $placeOfProduction; ?>
    <?php endif; ?>
    <?php if (!empty($publisher)) : ?>
      <?php print $publisher; ?>
    <?php endif; ?>
  </td>
</tr>
<?php endif ?>

<?php if (!empty($citation)) : ?>
<tr class="xc-citation">
  <td class="snippet-label"><?php print t('Citation'); ?>:</td>
  <td><?php print $citation; ?></td>
</tr>
<?php endif ?>
</table>

<?php if (isset($xc_record['admin']) && $xc_record['admin']['debug_mode'] == TRUE) : ?>
  <div class="xc-debug-info">
    FRBR level: manifestation
     - [<?php print $xc_record['full_display_link']; ?>]
     - [<?php print $xc_record['schema_record_link']; ?>]
     - [<?php print $xc_record['remote_xml_link']; ?>]
     - [<?php print $xc_record['solr_link']; ?>]
     - [<?php print $xc_record['mlt_link']; ?>]
    <?php if (!empty($xc_record['sibling_link'])) : ?>
     - [<?php print $xc_record['sibling_link']?>]
    <?php endif; ?>
    <?php if (!empty($xc_record['oclc_urls'])) : ?> -
      <?php foreach ($xc_record['oclc_urls'] as $url) : ?>
        [<a href="<?php print $url; ?>">oclc</a>]
      <?php endforeach; ?>
    <?php endif; ?>
</div>
<?php endif; // debug ?>