<?php
/**
 * @file
 * Snippet template for OAI DC record
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
?>
<?php if (isset($xc_record['dc:description'])): ?>
  <?php print $xc_record['dc:description']; ?>
<?php endif; ?>
