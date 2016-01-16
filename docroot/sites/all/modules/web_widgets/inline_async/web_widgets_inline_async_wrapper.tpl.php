<?php
/**
 * @file
 * Inline embedding means that we include the views output into a javascript file
 */
?>
var id = '<?php print $wid; ?>';
document.getElementById(id).innerHTML = <?php print $js_string ?>;

