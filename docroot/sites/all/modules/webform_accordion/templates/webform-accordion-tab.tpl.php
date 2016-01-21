<?php
/**
 * @file
 * The template file for an Accordion Tab.
 *
 * Available variables:
 * $title: The title for the accordion tab.
 *
 * children: The rendered child fields of the tab.
 *
 * $element: The raw render array for the tab.
 */

?>
<h3 class="accordion-tab-title"><a href="#"><?php print $title; ?></a></h3>
<div class="accordion-tab-content"><?php print $children; ?></div>
