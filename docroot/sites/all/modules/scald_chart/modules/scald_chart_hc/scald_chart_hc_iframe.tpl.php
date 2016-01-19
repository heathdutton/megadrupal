<?php
/**
 * @file
 * Default theme implementation for Scald Highchart insert code.
 */
?>
<iframe width="<?php print isset($options['width']) ? $options['width'] : '100%'; ?>" height="<?php print isset($options['height']) ? $options['height'] : '400'; ?>" frameborder="0" style="border:0"
        src="<?php print base_path(); ?>scald_chart_hc/<?php print $atom->sid . '/' . $context . '/' . $mode; ?>"></iframe>
