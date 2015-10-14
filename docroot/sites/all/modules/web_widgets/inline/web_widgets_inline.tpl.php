<?php
/**
 * @file
 * Template for the code for what to embed in external sites
 */
?>
<script type="text/javascript">
widgetContext_<?php print $id_suffix ?> = <?php print $js_variables ?>;
</script>
<script src="<?php print $script_url ?>"></script>
<div id="<?php print $wid ?>"></div>
