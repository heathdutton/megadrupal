<?php
/**
 * @file
 * Snippet template for expressions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
?>
<?php if (isset($element['citation'])): ?>
  <div class="xc-citation">
    Citation: <?php print $element['citation']; ?>
  </div>
<?php endif ?>

<?php if (isset($element['date'])): ?>
  <div class="xc-date">
    Date: <?php print $element['date']; ?>
  </div>
<?php endif ?>

<?php if ($frbr['type'] == 'e-m+'): ?>
  <?php if (isset($element['format'])): ?>
    <div class="xc-format">
        Format: <?php print $element['format']; ?>
    </div>
  <?php elseif (isset($frbr['manifestations'][0]['element']['format'])): ?>
    <div class="xc-format">
        Format: <?php print $frbr['manifestations'][0]['element']['format']; ?>
    </div>
  <?php endif; ?>
<?php endif; ?>

<?php if (isset($element['abstract'])): ?>
  <div class="xc-abstract">
    Abstract: <?php print $element['abstract']; ?>
  </div>
<?php endif ?>

<div class="xc-frbr-navigation">
  See the expressed
  <?php print l(format_plural(count($frbr['works']), 'work', '@count works'),
    'xc/search/id:("' . join('" OR "', $xc_record['xc:workExpressed']) . '")',
    array('absolute' => TRUE, 'query' => 'search_type=uplink'));?>
  , and its
  <?php print l(format_plural(count($frbr['manifestations']), 'manifestation', '@count manifestations'),
    'xc/search/xc__expressionManifested_t:"' . $xc_record['id'] . '"',
    array('absolute' => TRUE, 'query' => 'search_type=uplink'));?>
</div>

<div class="debugInfo">
  FRBR level: expression
   - [<?php print $xc_record['schema_record_link']; ?>]
   - [<?php print $xc_record['remote_xml_link']; ?>]

  <?php
    if (!empty($xc_record['oclc_urls'])) {
      print ' - [<a href="' . join('">oclc</a>] [<a href="', $xc_record['oclc_urls']) . '">oclc</a>]';
    }
  ?>
</div>
