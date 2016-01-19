<?php 

/**
 * @file
 * Theme file.
 *
 * Template for rendering year links in the TARDIS block.
 * If there's no link, it's rendered as plain text.
 */

?>
<?php ($link) ? $tardis_year = l($title, $link) : $tardis_year = $title; ?>
<h3><?php print $tardis_year; ?></h3>
