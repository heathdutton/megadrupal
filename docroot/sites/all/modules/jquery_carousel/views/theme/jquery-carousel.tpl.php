<?php
/**
 * @file
 * Creates common class names between parents and children.
 *
 * Hides non active children. Creates class names for containers.
 *
 * @ingroup views_templates
 */
?>
<div <?php print $selector_class;?> >
  <ul>
    <?php foreach($rows as $row): ?>
    <li>
      <?php print $row;?>
    </li>
    <?php endforeach; ?>
  </ul>
</div>
