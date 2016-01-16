<?php
/**
 * @file
 * Snippet template for OAI DC record
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
?>
<table>
<?php foreach ($dc as $field => $values): ?>
  <?php if (!empty($values)): ?>
    <tr>
      <td><span><?php print $field; ?>: </span></td>
      <td><?php print join(' &mdash; ', $values); ?></td>
    </tr>
  <?php endif; ?>
<?php endforeach; ?>
</table>