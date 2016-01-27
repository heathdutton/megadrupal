<?php

// based on https://drupal.org/node/1770582

/**
 * Implements hook_drush_command().
 */
function dt_send_test_mail_drush_command() {
  $items = array();

  $items['dt-send-test-mail'] = array(
    'description' => 'Send a test e-mail through Drupal.',
    'callback' => 'drush_dt_send_test_mail',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'arguments' => array(
      'to-address' => 'Address to which the test e-mail should be sent (comma separate multiple addresses).',
      'from-address' => '(Optional) address from which the test e-mail should be sent.',
      'subject' => '(Optional) custom subject for the test e-mail.',
    ),
    'options' => array(
      'lipsum' => 'Add some randomish words to the body of the e-mail.',
      'vogon-poetry' => 'Add an example of the third worst poety in the universe to the body of the e-mail.',
    ),
    'required-arguments' => 1, // only to-address is required
    'examples' => array(
      'drush dtstm dev@example.com' => 'Send a test e-mail to dev@example.com',
      'drush dtstm dev@example.com,pm@example.com' => 'Send a test e-mail to dev@example.com and pm@example.com',
      "drush dtstm dev@example.com website@example.com 'testing 1 2 3'" => "Send a test e-mail to dev@example.com from website@example.com with the subject 'testing 1 2 3'",
      'drush dtstm dev@example.com --lipsum' => 'Send a test e-mail to dev@example.com containing some randomish words in the body.',
    ),
    'aliases' => array('dtstm'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function dt_send_test_mail_drush_help($section) {
  switch ($section) {
    case 'drush:dt-send-test-mail':
      return dt("Send a test e-mail through Drupal.");
    // This is necessary to tie drush_debug_tools commands from
    // different commandfiles together in the output of drush help.
    case 'meta:drush_debug_tools:title':
      return dt("Drush debug tools commands");
  }
}

function drush_dt_send_test_mail($to_address, $from_address = NULL, $subject = NULL) {

  $to = $to_address; // todo: validate at least this?
  $from = empty($from_address) ? $from_address = variable_get('site_mail', ini_get('sendmail_from')) : $from_address;
  $headers = array();
  $headers['From'] = $headers['Sender'] = $headers['Return-Path'] = $headers['Errors-To'] = $from;
  $subject = empty($subject) ? 'Test e-mail from ' . variable_get('site_name', 'Drupal') : $subject;
  if (drush_get_option('lipsum')) {
    $body = drush_stm_lipsum();
  }
  elseif (drush_get_option('vogon-poetry')) {
    $body = drush_stm_vogon_poetry();
  }
  else {
    $body = $subject;
  }

  if (function_exists('drupal_mail_system')) {
    // D7 implementation

    // Prepare the message.
    $message = drupal_mail('drush', 'key', $to, NULL, array(), $from, FALSE);

    $message['subject'] = $subject;
    $message['body'] = array();
    $message['body'][] = $body;
    $message['headers'] = $headers;

    // Retrieve the responsible implementation for this message.
    $system = drupal_mail_system('drush', 'key');
    // Format the message body.
    $message = $system->format($message);
    // Send e-mail.
    $message['result'] = $system->mail($message);
    $result = $message['result'];

  }
  elseif (function_exists(drupal_mail_send)) {
    // D6 implementation
    $message = array(
      'to' => $to,
      'subject' => $subject,
      'body' => $body,
      'headers' => $headers,
    );
    $result = drupal_mail_send($message);
  }
  else {
    drush_set_error(dt('No usable Drupal mail system found.'));
    exit;
  }

  // Return result.
  if ($result) {
    drush_print(dt('E-mail message sent to <!to>', array('!to' => $to)));
  }
  else {
    drush_set_error(dt('An error occurred while sending the e-mail message.'));
  }
}

function drush_stm_lipsum() {
  $lipsum = array();
  $lipsum[] = 'Et non cursus praesent et placerat.';
  $lipsum[] = 'Amet urna facilisis sed sed tempor.';
  $lipsum[] = 'Molestie dui eros.';
  $lipsum[] = 'Enim at tincidunt integer magna vitae sit elit ultrices.';
  $lipsum[] = 'Sit diam eget amet posuere pellentesque.';
  $lipsum[] = 'Urna venenatis eu.';
  $lipsum[] = 'Potenti arcu nullam.';
  $lipsum[] = 'Lacus ullamcorper adipiscing ac et nullam a vitae pede eleifend.';
  $lipsum[] = 'Exercitation nec at et dignissim id saepe ante elit ligula dolor.';
  $lipsum[] = 'Neque amet vivamus proin officia maecenas ad sit magna.';
  $lipsum[] = 'A pulvinar dapibus ipsum orci tempor.';
  $lipsum[] = 'Conubia nec in vivamus rhoncus sed.';
  $lipsum[] = 'Vel sed posuere.';
  $lipsum[] = 'Nulla phasellus vulputate posuere tincidunt interdum.';
  $lipsum[] = 'Fusce vulputate dolor.';
  $lipsum[] = 'Amet nibh suspendisse rutrum lacus rhoncus.';
  $lipsum[] = 'Nulla ante risus nam nunc ornare.';
  $lipsum[] = 'Diam tempor aliquam ipsum etiam a.';
  $lipsum[] = 'Vel bibendum mauris erat nullam sollicitudin.';
  $lipsum[] = 'Eget diam penatibus.';
  $lipsum[] = 'Phasellus pellentesque pede orci wisi auctor.';
  $lipsum[] = 'Eu in dolor nunc quis euismod massa congue et.';
  $lipsum[] = 'Cras ipsum morbi nisl sed volutpat.';
  $lipsum[] = 'Justo egestas suspendisse.';
  $lipsum[] = 'Dolor id nec.';
  $lipsum[] = 'Faucibus cursus suspendisse et a ante.';
  $lipsum[] = 'Id adipiscing maecenas.';
  $lipsum[] = 'Eros laoreet nascetur in fusce vel morbi non donec cras et eros ullamcorper id vel.';
  $lipsum[] = 'Sed amet tincidunt.';
  $lipsum[] = 'Sit etiam nulla in justo a.';
  $lipsum[] = 'Dignissim lacinia morbi.';
  $lipsum[] = 'Porttitor tristique facilisis volutpat mus magna in massa lacus.';
  $lipsum[] = 'Lectus ac aliquet.';
  $lipsum[] = 'Quam id porttitor consectetuer donec a quis urna vestibulum.';
  $lipsum[] = 'Rutrum imperdiet et vel taciti fringilla.';
  $lipsum[] = 'Integer tortor mauris.';
  $lipsum[] = 'Congue imperdiet arcu.';
  $lipsum[] = 'Et scelerisque consequat.';
  $lipsum[] = 'Ac metus pellentesque placerat orci aliquam.';
  $lipsum[] = 'Congue suspendisse duis integer pellentesque lectus.';
  $lipsum[] = 'Pellentesque at pellentesque.';
  $lipsum[] = 'Varius ut faucibus.';
  $lipsum[] = 'Dui magnis nullam ante tincidunt tellus.';
  $lipsum[] = 'Vehicula massa vel nullam ac sagittis quis ullamcorper eu eros quisque etiam ut elit porttitor.';
  $lipsum[] = 'Pede praesent venenatis.';
  $lipsum[] = 'Ultricies nisl laoreet a dis integer.';
  $lipsum[] = 'Non vestibulum neque.';
  $lipsum[] = 'Posuere nibh aenean quae et nec egestas nulla diam praesent suspendisse.';
  $lipusm[] = 'Tellus habitant justo risus nibh semper scelerisque pharetra aptent aliquet.';
  $lipsum[] = 'Per nulla sit.';
  $lipsum[] = 'Tellus nec ut dictum eleifend eu.';
  $lipsum[] = 'Accumsan venenatis arcu.';

  $return = '';
  for ($i = 0; $i < rand(5, 30); $i++) {
    $return .= $lipsum[array_rand($lipsum)];
    if ($i % rand(2, 3) == 0) {
      $return .= "\n";
    }
    else {
      $return .= ' ';
    }
  }
  return trim($return);
}

  // alternatively
  function drush_stm_vogon_poetry() {
  $vogon_poetry = <<<EOVP
Oh freddled gruntbuggly,
Thy micturitions are to me,
As plurdled gabbleblotchits,
On a lurgid bee,
That mordiously hath blurted out,
Its earted jurtles,
Into a rancid festering confectious organ squealer.
Now the jurpling slayjid agrocrustles,
Are slurping hagrilly up the axlegrurts,
And living glupules frart and slipulate,
Like jowling meated liverslime,
Groop, I implore thee, my foonting turling dromes,
And hooptiously drangle me,
With crinkly bindlewurdles,
Or else I shall rend thee in the gobberwarts with my blurglecruncheon,
See if I don't.
EOVP;
  return $vogon_poetry;
}
