<?php
// $Id$
/**
 * @file
 * 
 * This file display a 'copy' of items of the wow armory character page.
 * 
 * Available variables:
 *   $profilemain - The large image of the character used as a backdrop.
 *   $toon - The character being displayed. 
 *   $set - The particular set (items and stats) being displayed.
 *   $details - (true|false).  Show item details?  Toggled by user.
 *   
 * @see template_preprocess_wowtoon_items()
 */
?>

<table border="0" class="summary-inventory" height="550" style="height: 550px; background:url(<?php echo $toonbackground; ?>) center 0 scroll;">
<tr>
  <td class='left-item'><?php echo theme('wowtoon_left_item',array('item' => $set->items[0], 'details' => $details)); ?></td>
  <td class='right-item'><?php echo theme('wowtoon_right_item',array('item' => $set->items[9], 'details' => $details)); ?></td>
</tr>
<tr>
  <td class='left-item'><?php echo theme('wowtoon_left_item',array('item' => $set->items[1], 'details' => $details)); ?></td>
  <td class='right-item'><?php echo theme('wowtoon_right_item',array('item' => $set->items[5], 'details' => $details)); ?></td>
</tr>
<tr>
  <td class='left-item'><?php echo theme('wowtoon_left_item',array('item' => $set->items[2], 'details' => $details)); ?></td>
  <td class='right-item'><?php echo theme('wowtoon_right_item',array('item' => $set->items[6], 'details' => $details)); ?></td>
</tr>
<tr>
  <td class='left-item'><?php echo theme('wowtoon_left_item',array('item' => $set->items[14], 'details' => $details)); ?></td>
  <td class='right-item'><?php echo theme('wowtoon_right_item',array('item' => $set->items[7], 'details' => $details)); ?></td>
</tr>
<tr>
  <td class='left-item'><?php echo theme('wowtoon_left_item',array('item' => $set->items[4], 'details' => $details)); ?></td>
  <td class='right-item'><?php echo theme('wowtoon_right_item',array('item' => $set->items[10], 'details' => $details)); ?></td>
</tr>
<tr>
  <td class='left-item'><?php echo theme('wowtoon_left_item',array('item' => $set->items[3], 'details' => $details)); ?></td>
  <td class='right-item'><?php echo theme('wowtoon_right_item',array('item' => $set->items[11], 'details' => $details)); ?></td>
</tr>
<tr>
  <td class='left-item'><?php echo theme('wowtoon_left_item',array('item' => $set->items[18], 'details' => $details)); ?></td>
  <td class='right-item'><?php echo theme('wowtoon_right_item',array('item' => $set->items[12], 'details' => $details)); ?></td>
</tr>
<tr>
  <td class='left-item'><?php echo theme('wowtoon_left_item',array('item' => $set->items[8], 'details' => $details)); ?></td>
  <td class='right-item'><?php echo theme('wowtoon_right_item',array('item' => $set->items[13], 'details' => $details)); ?></td>
</tr>
</table>
<table border="0" class="summary-inventory">
<tr>
  <td width="50%" class='right-item'><?php echo theme('wowtoon_right_item',array('item' => $set->items[15], 'details' => $details)); ?></td>
  <td width="50%" class='left-item'><?php echo theme('wowtoon_left_item',array('item' => $set->items[16], 'details' => $details)); ?></td>
</tr>
</table>
