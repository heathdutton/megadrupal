<?php
// Concatenate the city row.
$city = !empty($event->Zip) ?  $event->Zip . ', ' : '';
$city .= !empty($event->City) ?  $event->City . ', ' : '';
$city .= !empty($event->State) ?  $event->State . ', ' : '';
$links = l(t('Event Details'), 'salsa/event/' . $event->event_KEY);

// Display cancel link conditionally.
if ($event->This_Event_Costs_Money == 'false' && $cancel_link) {
  $links .= '&nbsp;' . l(t('Cancel Registration'), 'salsa/event/' . $event->event_KEY . '/cancel');
}
?>
<div class="salsa-event-display-wrapper">
  <?php if (!empty($event->Start)): ?>
    <div class="salsa-event-start"><?php print format_date(strtotime($event->Start)); ?></div>
  <?php endif; ?>
  <?php if (!empty($event->Event_Name)): ?>
    <div class="salsa-event-name"><?php print l($event->Event_Name, 'salsa/event/' . $event->event_KEY); ?></div>
  <?php endif; ?>
  <?php if (!empty($event->Description)): ?>
    <div class="salsa-event-description"><?php print $event->Description; ?></div>
  <?php endif; ?>
  <?php if (!empty($event->Address)): ?>
    <div class="salsa-event-address"><?php print $event->Address; ?></div>
  <?php endif; ?>
  <?php if (!empty($city)): ?>
    <div class="salsa-event-city"><?php print $city; ?></div>
  <?php endif; ?>
  <div class="salsa-event-links"><?php print $links; ?></div>
</div>
<br />
