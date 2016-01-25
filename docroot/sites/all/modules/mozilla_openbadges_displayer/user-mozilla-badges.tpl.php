<?php
/**
 * @file
 * Template to display a mozilla group badges.
 */
?>
<div class="user-mozilla-badges">
  <?php foreach ($mozilla_badges as $key => $group): ?>
    <h1 class="user-mozilla-badges-groupname">
      <?php print $group['groupname']; ?>
    </h1>
    <ul class="user-mozilla-badges-item-list">
      <?php foreach ($group['badges'] as $id => $badge): ?>
        <li class="user-mozilla-badges-item">
          <img src='<?php print $badge['image']; ?>' border=0 />
        </li>
      <?php endforeach; ?>
    </ul>
  <?php endforeach; ?>
</div>
