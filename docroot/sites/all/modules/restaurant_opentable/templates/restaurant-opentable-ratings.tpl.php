<?php
/**
 * @file
 * Template file for Restaurant Opentable reservation form.
 */
?>
<?php if (count($ratings)): ?>
  <div class="opentable-ratings">
    <ul class="list-group">
      <?php foreach ($ratings as $rating): ?>
        <li class="list-group-item">
          <strong class="rating-label"><?php print $rating['label']; ?></strong>
          <span class="rating-value pull-right"><?php print render($rating); ?></span>
        </li>
      <?php endforeach; ?>
    </ul> 
  </div>
<?php endif; ?>
