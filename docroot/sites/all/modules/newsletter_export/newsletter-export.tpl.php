<?php
/**
 * @file newsletter_export.tpl.php
 * Template to output a node as html for newsletters
 *
 * Node type specific templates can be created by copying to: newsletter-export--nodetype.tpl.php
 * Be sure to clear your theme registry for the new template file to be found
 * 
 * - $raw_markup: The basic html markup for all fields you would get from the content portion of a node.
 * - $fields: An array of all fields belonging to the node including body
 * - $node: The entire node object
 */
?>

<?php print $raw_markup ?>
