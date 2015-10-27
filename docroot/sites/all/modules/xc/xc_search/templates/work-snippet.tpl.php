<?php
/**
 * @file
 * Snippet template for work
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
?>
<?php if (isset($element['citation'])): ?>
  <div class="xc-citation">
    Citation: <?php print $element['citation'] ?>
  </div>
<?php endif ?>

<?php if (isset($element['date'])): ?>
  <div class="xc-date">
    Date: <?php print $element['date'] ?>
  </div>
<?php endif ?>

<?php if (isset($element['format'])): ?>
  <div class="xc-format">
    Format: <?php print $element['format'] ?>
  </div>
<?php endif ?>

<?php if (isset($element['abstract'])): ?>
  <div class="xc-abstract">
    Abstract: <?php print $element['abstract'] ?>
  </div>
<?php endif ?>

<div class="xc-frbr-navigation">
  See it's <?php print
    l(format_plural(count($frbr['expressions']), 'expression', '@count expressions'),
      'xc/search/xc__workExpressed_t:"' . $xc_record['id'] . '"',
      array('absolute' => TRUE, 'query' => 'search_type=uplink')); ?>
</div>

<div class="xc-debug-info">
  FRBR level: work
   - [<?php print $xc_record['schema_record_link']; ?>]
   - [<?php print $xc_record['remote_xml_link']; ?>]
  <?php if (!empty($xc_record['oclc_urls'])) {
    print ' - [<a href="' . join('">oclc</a>] [<a href="', $xc_record['oclc_urls']) . '">oclc</a>]';
  } ?>
</div>
