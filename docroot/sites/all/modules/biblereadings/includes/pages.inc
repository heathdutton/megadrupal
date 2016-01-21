<?php

function biblereadings_page_unsubscribe() {
  $email = $_GET['email'];
  $output = '';

  if ($email) {

    if (biblereadings_subscription_exists($email)) {
      biblereadings_unsubscribe($email);

      $output .= t('!email has been unsubscribed from daily Bible readings.', array('!email' => $email));
    }
    else {
      $output .= t('There is no subscription for !email.', array('!email' => $email));
    }
  }
  else {
    $output .= t('No email address was specified.');
  }


  return $output;
}