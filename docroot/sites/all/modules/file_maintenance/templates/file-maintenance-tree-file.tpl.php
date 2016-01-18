<?php
/**
 * @file
 * Tree file template.
 */

// $Id$

?>
<div style="clear: both;">
	<?php for ($i=0; $i<=$level; $i++): ?>
		<div style="border: 1px solid; float: left; width: 2em; height: 2em">
		</div>
	<?php endfor; ?>
	<b><?php print $filename ?></b>
</div>
