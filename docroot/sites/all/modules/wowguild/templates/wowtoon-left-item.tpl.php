<?php
// $Id$
/**
 * @file
 *
 * Displays left side items from the template wowtoon_items.
 *
 * Available variables:
 * - $item: item array.
 * - $gems: array of html image and links
 * - $icon: http link to icon
 * - $url: link to item on wowhead
 * - $rel: additional info (gems and enchants send to wowhead to append
 *
 * @see template_preprocess_wowtoon_left_item().
 *
 */

?>
<?php if (isset($item['name'])):?>
  <div class="slot slot-<?php echo $item['slotname']; ?> item-quality-<?php echo $item['quality'];
  if (isset($item['equipme'])) { echo ' equip-me'; }
  if (isset($item['unequipme'])) { echo ' unequip-me'; }
  ?>" style="background:url('<?php echo $icon; ?>') no-repeat scroll 0 0 transparent; width:49px; height: 49px;">
  <a href="<?php echo $url; ?>" class="item" rel="<?php echo $rel; ?>"><span class="frame"></span></a>
  </div>
  <?php if ($details): ?>
    <a href="<?php echo $url; ?>" class="shadow item color-q<?php echo $item['quality']; ?>" rel="<?php echo $rel; ?>"><?php echo $item['name']; ?></a><br />
    <?php if (isset($item['enchant-name'])): ?>
    <span class="enchant color-q2 shadow"><?php echo $item['enchant-name'];?></span><br />
    <?php endif ?>
    <span class="level"><?php //echo $item['level']; ?></span>
    <?php //if ($gems): echo implode(' ', $gems); endif; ?>
  <?php endif; ?>
<?php else: ?>
<div class="slot slot-<?php echo $item['slotname'];
if (isset($item['equipme'])) {
  echo ' equip-me';
}
if (isset($item['unequipme'])) {
  echo ' unequip-me';
}
?>"><a href="javascript:;" class="empty"><span class="frame"></span></a></div>
<?php endif; ?>