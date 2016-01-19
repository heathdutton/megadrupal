<?php
/**
 * @file
 * This is a template file for use with Codit: Blocks.
 *
 * Variables returned by the _callback are passed in as
 * $blockdata_{BLOCK_DELTA}
 * so the extract below should make theme directly available to this template.
 */

extract($blockdata_{BLOCK_DELTA});
?>
<?php print codit_tto(__FILE__); ?>

<p>This is the output of Codit Block: {BLOCK_DELTA}</p>

<?php print codit_ttc(__FILE__); ?>
