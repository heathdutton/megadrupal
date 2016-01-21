<?php
$fee_type = $subscribed->event_fee_KEY ? entity_load_single('salsa_event_fee', $subscribed->event_fee_KEY)->Category : FALSE;
$amount = $subscribed->donation_KEY ? entity_load_single('salsa_donation', $subscribed->donation_KEY)->amount : FALSE;
$status = !empty($statuses) ? $statuses[$subscribed->_Status] : '';

echo '<div><strong>' . t('It appears you have already signed up for this event as @email', array('@email' => $supporter->Email)) . '</strong></div>';
if (user_access('view salsa my event page') && module_exists('salsa_profile') && salsa_profile_get_supporter()) {
  global $user;
  echo '<div>' . t('If u want to cancel your registration visit your !profile', array('!profile' => l(t('profile'), 'user/' . $user->uid . '/salsa_my_events'))) . '</div>';
}
else {
  echo '<div>' . t('If this is not you, sign up under a different email address !link', array('!link' => l(t('here'), 'salsa/supporter/reset', array('query' => array('destination' => current_path()))))) . '</div>';
}
echo '<div><strong>' . t('Your Registration Information for @event', array('@event' => $event->Event_Name)) . '</strong></div>';

echo '<table>';
echo '<tr>';
echo '<th>&nbsp;</th>';
echo '<th>' . t('Name') . '</th>';
echo '<th>' . t('Email') . '</th>';
echo '<th>' . t('Status') . '</th>';
if ($fee_type) echo '<th>' . t('Fee Type') . '</th>';
if ($amount) echo '<th>' . t('Amount') . '</th>';
echo '</tr>';
echo '<tr>';
echo '<td><strong>' . t('Your Information') . '</strong></td>';
echo '<td>' . $supporter->First_Name . ' ' . $supporter->Last_Name . '</td>';
echo '<td>' . $supporter->Email . '</td>';
echo '<td>' . $status . '</td>';
if ($fee_type) echo '<td>' . $fee_type . '</td>';
if ($amount) echo '<td>' . $amount . '</td>';
echo '</tr>';
if ($amount) {
  echo '<tr>';
  echo '<td colspan="5"><strong>' . t('Total') . '</strong></td>';
  echo '<td>' . $amount . '</td>';
  echo '</tr>';
}
echo '</table>';
