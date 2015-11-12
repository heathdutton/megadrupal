<?php
/**
 * @file
 * This is template file to render the reach us services.
 */

/**
 * Services data available as $services array.
 *
 * Settings data available in $settings array.
 */
 ?>	
<?php if($settings['display'] == 'text'): ?>
<ul class = "reachus-wrap reachus-text reachus-<?php echo $settings['visibility']; ?> ">
  <?php foreach($services as $key => $value):
  print '<li class = "reachus-item reachus-item-' . $key . '">';
  print '<a href = "' . $value['href'] . '" title = "' . $value['text'] . '" alt = "' . $value['text'] . '" >' . $value['text'] . '</a>';
  print '</li>'; ?>
<?php endforeach; ?>
</ul>
<?php elseif ($settings['display'] == 'icon'): ?>
<ul class = "reachus-wrap reachus-icon reachus-<?php echo $settings['visibility']; ?> reachus-<?php echo $settings['iconsize']; ?>">
	<?php foreach($services as $key => $value):
    print '<li class = "reachus-item reachus-item-' . $key . '">';
    print '<a href = "' . $value['href'] . '" title = "' . $value['text'] . '" alt = "' . $value['text'] . '" class = "reachus-icon-' . $settings['iconsize'] . '"><span class = "reachus-img"></span> </a>';
    print '</li>'; ?>
    <?php endforeach; ?>
</ul>
<?php  else: ?>
<ul class = "reachus-wrap reachus-both reachus-<?php echo $settings['visibility']; ?> reachus-<?php echo $settings['iconsize']; ?>">
    <?php foreach($services as $key => $value):
    print '<li class = "reachus-item reachus-item-' . $key . '">';
    print '<a href = "' . $value['href'] . '" title = "' . $value['text'] . '" alt = "' . $value['text'] . '" class = "reachus-icon-' . $settings['iconsize'] . '"><span class = "reachus-img"></span><span class = "reachus-txt">' . $value['text'] . '</span></a>';
    print '</li>'; ?>
    <?php endforeach; ?>

</ul>
<?php endif; ?>
