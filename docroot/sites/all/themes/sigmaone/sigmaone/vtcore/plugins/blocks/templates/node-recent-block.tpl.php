<?php
/**
 * Template for recent node block
 *
 * Available object to use :
 *
 * $contents - is an array of node object keyed with node id
 * $contents[$cid][picture] - pre build node user picture
 * $contents[$cid][title_linked] - pre themed content title linked to node
 * $contents[$cid][posted_data_themed] - pre themed node date and user
 * $contents[$cid][node] - full raw node object
 * $contents[$cid][user] = full raw node object
 */
?>
<div class="content-block-wrapper">
  <?php if (isset($contents) && is_array($contents)) : ?>
  <?php foreach ($contents as $id => $value) : ?>
  <div class="content-block clearfix">
    <?php if (!empty($value['picture'])) print $value['picture'];?>
    <div class="content-comment <?php if (empty($value['picture'])) print 'no-picture' ;?>">
      <h6><?php print html_entity_decode($value['title_link']);?></h6>
      <p><?php print $value['posted_data_themed'];?></p>
    </div>
  </div>
  <?php endforeach;?>
  <?php endif; ?>
</div>