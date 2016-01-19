<?php
/**
 * @file
 * Renders the big listing
 */
 ?>
 <div id="personalized-content-list">
  <ul>
    <?php echo $nodes; ?>
  </ul>
  <?php echo theme('pager', array('tags' => array())); ?>
</div>
