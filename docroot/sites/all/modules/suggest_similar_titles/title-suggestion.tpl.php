<?php
/**
 * @file
 * Default theme implementation to display a suggested similar titles.
 *
 * Available variables:
 * - $node: array of similar node objects.
 */
?>
<div class="similar_titles_wraper">
  <b><?php echo t('Following similar nodes already exist in the record');?></b><span class="ajax-title-div-close"><a href="javascript:void(0);">X</a></span>
  <ul>
    <?php foreach ($node as $n):?>
      <li><?php echo $n->link; ?></li>
    <?php endforeach; ?>
  </ul>
</div>
