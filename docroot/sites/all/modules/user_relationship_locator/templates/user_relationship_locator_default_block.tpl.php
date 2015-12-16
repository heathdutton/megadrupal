<?php
/**
 * @file
 * Theme wrapper html for relationship list.
 *
 *
 * Variables available:
 * - $user
 * - $requester
 * - $requestee
 * - $relationship
 * - $items
 */
?>
<ul class="user-relationship-locator-list">
  <?php foreach ($items as $item): ?>
    <li>
      <?php print $item ?>
    </li>
  <?php endforeach ?>
</ul>

<?php if ($load_buttons) print $load_buttons ?>
