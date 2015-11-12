<?php
/**
 * @file
 * Default theme implementation for Scald Map Render.
 */
?>
<iframe width="<?php echo $vars['width']; ?>" height="<?php echo $vars['height']; ?>" frameborder="0" style="border:0"
        src="<?php echo url("scald/map/render/{$vars['id']}/{$vars['context']}/{$vars['zoomlevel']}"); ?>"></iframe>
