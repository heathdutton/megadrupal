<?php

/**
 * @file
 * Wrapper for the directory tree.
 *
 * There should not be any whitespace at the beginning or the end of the div.
 * Otherwise jQuery's replaceWith will add an additional <div> wrapper each time.
 */

// $Id$

?><div id="file-list-wrapper">
	<?php print render($list['list']) ?>
</div>