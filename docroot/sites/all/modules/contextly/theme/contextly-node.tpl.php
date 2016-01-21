<?php
/**
 * @file
 * Node template override for Contextly view mode.
 *
 * See node.tpl.php for available variables.
 */

hide($content['comments']);
hide($content['links']);
print render($content);
