<?php
/**
 * @file
 * Title template for OAI DC
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
?>
<?php if (isset($xc_record['dc:title'])): ?>
  <?php print l(xc_util_conditional_join(', ', $xc_record['dc:title']), $xc_record['full_display_url']); ?>
<?php else: ?>
  <?php print 'non title'; ?>
<?php endif; ?>
