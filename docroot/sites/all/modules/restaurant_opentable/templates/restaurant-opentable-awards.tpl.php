<?php
/**
 * @file
 * Template file for Restaurant Opentable reservation form.
 */
?>
<?php if (count($awards)): ?>
  <div id="DinersChoiceBadgeContainer" class="DinersChoiceBadges">
    <ul class="list clearfix">
      <?php foreach ($awards as $award): ?>
        <li class="item">
          <div class="details">
            <span class="award-label"><?php print $award['label']; ?></span>
            <span class="award-name"><?php print $award['name']; ?></span>
          </div>
        </li>
      <?php endforeach; ?>
    </ul> 
  </div>
<?php endif; ?>
