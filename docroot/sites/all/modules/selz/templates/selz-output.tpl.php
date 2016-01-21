<?php
/**
 * @file
 * Displays the Selz tag.
 *
 * Available variables:
 * - $type
 * - $position
 * - $buttton_color
 * - $button_color_text
 * - $header_color_order
 * - $header_color_order_text
 * - $button_text
 * - $logos
 * - $overlay
 * - $fixed_width
 * - $url_value.
 *
 * @ingroup themeable
 */
?>

<?php if (!empty($fixed_width) && $type == 'widget') : ?>
	<div style="max-width:<?php  print (int) $fixed_width?>px; margin: 0 auto;">
<?php endif; ?>

<?php
$t = substr($type, 0, 1);
$logos_data = empty($logos) ? '' : 'data-selz-lg="true"';
$overlay_data = empty($overlay) ? '' : 'data-selz-a="modal"';
$url = urldecode($url_value);

if ($type == 'button') {
  if ($position == 'right') {
    $d_pos = 'data-selz-t="_selz-btn-default"';
  }
  elseif ($position == 'above') {
    $d_pos = 'data-selz-t="_selz-btn-above"';
  }
  else {
    $d_pos = 'data-selz-t="_selz-btn-fluid"';
  }
}
else {
  $d_pos = '';
}
?>

<script <?php print $d_pos?> <?php print $overlay_data?> data-selz-cb="<?php print $button_color?>" data-selz-ct="<?php print $button_color_text?>" data-selz-chbg="<?php print $header_color_order?>" data-selz-chtg="<?php print $header_color_order_text?>" data-selz-<?php print $t?>="<?php print $url?>" <?php print $logos_data?> data-text="<?php print $button_text?>">
if (typeof _$elz === "undefined") { var _$elz = {}; }
if (typeof _$elz.<?php print $t?> === "undefined") { 
_$elz.<?php print $t?> = { e: document.createElement("script") }; 
_$elz.<?php print $t?>.e.src = "https://selz.com/embed/<?php print $type ?>"; 
document.body.appendChild(_$elz.<?php print $t?>.e); }
</script>
<noscript><a href="<?php print $url?>" target="_blank"><?php print $button_text?></a></noscript>

<?php if (!empty($fixed_width) && $type == 'widget') : ?>
	</div>
<?php endif; ?>
