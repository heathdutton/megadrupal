<?php
  /**
   * @file
   * Template to display the current lendee of an item.  This template is only
   * ever reached if there is a valid checkout for an item, otherwise we never
   * get here, so assume $user and $reservation are filled out.
   *
   * $queue - the request queue
   * $node - the node in question
   * $user - the current user, so we can see if they've got requests
   *
   */
?>
<p>
  The following is a list of announcements that were sent out based on this annoucement record.
</p>
<div class="node-announce-log">
  <table><tr><th>Node</th><th>When</th></tr>
  <?php $class = ""; ?>
  <?php foreach($records as $item): ?>

  <?php $class = ($class == "node-announce-odd") ? "node-announce-even" : "node-announce-odd"; ?>

  <tr class="<?php print $class; ?>" >
    <td><?php print l($item->node->title, $item->url); ?></td>
    <td><?php print $item->when; ?></td>
  </tr>

  <?php endforeach; ?>
  </table>
</div>
