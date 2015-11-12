<?php 
/**
 * @file
 * Template file to render rating widget.
 */

?>

<div
	class='rating-widget<?php print (!empty($full_node_class) ? "-" . $full_node_class : '');?>'
	data-id='node-<?php print $nid;?>'
	<?php isset($rating_value) ? print " data-average='" . $rating_value . "'" : '' ?>></div>
