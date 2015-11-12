<?php

/**
 * @file
 * Default theme implementation for an Activity Stream item.
 *
 * Available variables:
 * - $node: The fully-loaded node object.
 * - $service: An array of information about the service.
 * - $statement: A renderable statement of the activity. By default, we
     split the statement into 'actor', 'verb', and 'object', but there's
     no guarantee these three parts will make it to the theme layer (modules
     might preprocess their own non-S/P/O-formatted statement, etc.). The
     'actor' is a renderable linked Drupal username, the 'verb' is a
     renderable action performed by the actor, and 'object' is a renderable
     thingy thing upon which the action was performed. Themes should always
     render $statement and not the individual parts.
 * - $service_icon: A renderable icon representing the service.
 * - $permalink_icon: A renderable icon for the local item link.
 * - $external_icon: A renderable icon for the external item link.
 * - $time_ago: A renderable string for when the item was posted.
 */
?>
<div class="activitystream-item">
  <div class="activitystream-item-service">
    <?php print render($service_icon); ?>
  </div>
  <ul class="activitystream-item-meta meta">
    <li><?php print render($time_ago); ?></li>
    <li><?php print render($permalink_icon); ?></li>
    <li><?php print render($external_icon); ?></li>
  </ul>
  <div class="activitystream-item-statement">
    <?php print render($statement); ?>
  </div>
</div>
