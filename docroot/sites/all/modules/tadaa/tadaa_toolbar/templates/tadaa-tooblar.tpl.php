<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * Tadaa toolbar variables:
 * - $environment_name: The label of the current active enviornment.
 *
 * @see hook_page_alter
 */
?>

<div id="tadaa-status"><?php print $environment_name; ?></div>
