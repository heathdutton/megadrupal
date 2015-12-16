<?php
/**
 * @file
 * Leaflet theme implementation for Scald Map.
 *
 * - $vars['context']: scald context
 * - $vars['id']: atom entity id
 * - $vars['zoomlevel']: map zoom level, override if you want,
 *       integer between 0 and 18, default 8
 */
?>
<iframe width="<?php echo $vars['width']; ?>" height="<?php echo $vars['height']; ?>" frameborder="0" style="border:0"
        src="<?php echo url("scald/mapleaflet/render/{$vars['id']}/{$vars['context']}/{$vars['zoomlevel']}"); ?>"></iframe>
