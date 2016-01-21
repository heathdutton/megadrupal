<?php

/**
 * @file
 * Theme implementation to show forum legend.
 *
 */
?>

<div class="row well">
  <div class="col-md-4">
    <ul class="list-unstyled">
      <li>
        <span class="glyphicon glyphicon-star"></span> <?php print t('New posts'); ?>
      </li>
      <li>
        <span class="glyphicon glyphicon-star-empty"></span> <?php print t('No new posts'); ?>
      </li>
    </ul>
  </div>

  <div class="col-md-4">
    <ul class="list-unstyled">
      <li>
        <span class="glyphicon glyphicon-fire" style="color: #ff0000"></span> <?php print t('Hot topic with new posts'); ?>
      </li>
      <li>
        <span class="glyphicon glyphicon-fire"></span> <?php print t('Hot topic without new posts'); ?>
      </li>
    </ul>
  </div>

  <div class="col-md-4">
    <ul class="list-unstyled">
      <li>
        <span class="glyphicon glyphicon-pushpin"></span> <?php print t('Sticky topic'); ?>
      </li>
      <li>
        <span class="glyphicon glyphicon-ban-circle"></span> <?php print t('Locked topic'); ?>
      </li>
    </ul>
  </div>
</div>
