<?php
/**
 * @file
 * Title template for OAI DC
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
?>
<?php if (isset($dc_title)): ?>
  <?php print l(xc_util_conditional_join(', ', $dc_title), $xc_record['full_display_url']); ?>
<?php else: ?>
  <?php print 'non title'; ?>
<?php endif; ?>
