<?php
/**
 * @file
 * The Review Pane Template
 *
 * This replaces the review pane in the checkout process with divs instead of
 * tables for better control of appearance.
 *
 * @param $pane_id
 *  The unique identifier for the review pane.
 * @param $data
 *  An array that holds the information of the review pane. This usually
 *  consists of other panes from the checkout process.
 * @param $data['title']
 *  The title of the panel pane.
 * @param $data['data']
 *  The content of the panel pane.
 */

?>
<div class="review-panes clearfix">
  <?php foreach ($variables['form']['#data'] as $pane_id => $data):?>
    <div class="review-pane <?php print $pane_id;?>">
      <h3 class="pane-title"><?php print $data['title'];?></h3>
      <?php print $data['data'];?>
    </div>
  <?php endforeach;?>
</div>
