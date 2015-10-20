<?php
/**
 * @file
 * Prints a themeable Node registration. This file exists to be overridden!
 *
 * Available variables:
 * - object $node -- the 'owner' of this registration
 * - object $registration -- the registration details
 * - array  $content -- the renderable display mode result
 * - array  $node_content -- the renderable content for the $node
 * - array  $cancel -- a renderable link to the cancel page
 *
 * All of the shite that was here before, has moved to
 * NodeRegistrationEntityClass::buildContent() into $content below.
 */
?>

<div class="<?php print $classes; ?>" <?php print $attributes; ?>>

  <?php print render($content); ?>

</div>
