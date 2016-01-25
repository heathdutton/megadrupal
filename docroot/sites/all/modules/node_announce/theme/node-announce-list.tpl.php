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
<div class="node-announce-list">
  <table><tr><th>Name</th><th>Days Before</th><th>Date Field</th></tr>


  <?php $class = ""; ?>
  <?php foreach($announces as $item): ?>

  <?php $class = ($class == "node-announce-odd") ? "node-announce-even" : "node-announce-odd"; ?>

  <tr class="<?php print $class; ?>" >
    <td><?php print l($item->name, 'admin/config/system/node_announce/' . $item->id); ?></td>
    <td><?php print $item->days_before; ?></td>
    <td><?php print node_announce_date_field_pretty($item->date_field); ?></td>
  </tr>
  <tr class="<?php print $class; ?>">
    <td class="node-announce-subject" colspan="4">Subject: <?php print $item->subject; ?></td>
  </tr>

  <?php endforeach; ?>
  </table>
</div>
