<?php
/**
 * @file
 * Template file for display pager.
 */

?>
<div class="verticrawl-search-pager item-list">
  <ul class="pager">
  <?php foreach ($pages as $page_text => $page_data) : ?>
    <li class="<?php print (implode(" ", $page_data['classes'])); ?>">
    <?php if(in_array('pager-current', $page_data['classes'])) : ?>
      <?php print (t($page_text)); ?>
    <?php else : ?>
      <a href="<?php print ($base_url . $page_data['query_string']); ?>">
        <?php print (t($page_text)); ?>
      </a>
    <?php endif; ?>
  <?php endforeach; ?>
</div>
