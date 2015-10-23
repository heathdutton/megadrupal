<?php

/**
 * @file
 * Template for a single ticket.
 */

?>

<p class="ticket-recieved">The following ticket was received at <?php print date("jS F Y", $date); ?> at <?php print date("H:ia", $date) ?>:</p>

<table id="ticket-info">

<tr>
	<td class="label">From</td>
	<td class="value"><?php print $customer['name'] ?> &lt;<?php print $email ?>&gt;</td>
</tr>

<tr>
	<td class="label">Subject</td>
	<td class="value"><?php print $subject ?></td>
</tr>

<tr>
	<td class="label">Message:</td>
	<td><?php print $message; ?></td>
</tr>

</table>
<hr>
<h2>Responses to your ticket:</h2>
<?php if (count($updates) <= 1) : ?>
	<p>No responses to this ticket yet.</p>
<?php else : ?>

<?php foreach ($updates as $key => $update) : ?>
    <?php if($key == 0) :
    continue;
    endif; ?>

<table id="ticket-info">

<tr>
	<td class="label">From</td>
	<td class="value"><?php print $update['from_name'] ?> &lt;<?php print $update['from_address'] ?>&gt;</td>
</tr>

<tr>
	<td class="label">Subject</td>
	<td class="value"><?php print $update['subject'] ?></td>
</tr>

<tr>
	<td class="label">Message:</td>
	<td><?php print $update['message']; ?></td>
</tr>
</table>

<?php endforeach; ?>
<?php endif; ?>
<?php if ($status['status_type'] != 1): ?>
<?php print $form ?>
<?php endif; ?>
