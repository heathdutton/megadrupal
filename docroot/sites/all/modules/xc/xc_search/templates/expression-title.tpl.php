<?php
/**
 * @file
 * Title template for expressions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
?>
<?php if ($frbr['type'] == 'e-m'): ?>
  <?php if ($frbr['manifestations'][0]['element']['title']): ?>
    <div class="xc-title">
      <?php
        $title = $frbr['manifestations'][0]['element']['title'];
        if (is_array($title)) { $title = join(' &mdash; ', $title); }
      ?>
      <?php print l($title, $xc_record['full_display_link'], array('html' => TRUE)); ?>
    </div>
  <?php elseif ($element['title']): ?>
    <div class="xc-title">
      <?php print l($element['title'], $xc_record['full_display_link']); ?>
    </div>
  <?php endif; ?>
<?php endif; ?>

<?php if ($element['creator']): ?>
  <div class="xc-creator">
    by <?php print $element['creator']; ?>
  </div>
<?php elseif ($frbr['works'][0]['element']['creator']): ?>
  <div class="xc-creator">
    by <?php print $frbr['works'][0]['element']['creator']; ?>
  </div>
<?php endif; ?>

<?php if ($element['contributor']): ?>
  <div class="xc-contributor">
    <?php if (is_array($element['contributor'])): ?>
      Contributors: <?php print join(' &mdash; ', $element['contributor']); ?>
    <?php else: ?>
      Contributor: <?php print $element['contributor']; ?>
    <?php endif; ?>
  </div>
<?php endif; ?>

