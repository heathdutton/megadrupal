<?php
/**
 * @file
 * Default theme implementation for nodes displayed via region_view_modes.
 *
 * Removes unwanted content like title, comments, links, and the wrapper div.
 */

hide($content['comments']);
hide($content['links']);
print render($content);
