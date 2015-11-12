<?php
/**
 * @file
 * This file is a script for filling the database with initial data for
 * statistics purposes. It empties the users and all ces tables and populates
 * it with ($numaccounts) users with their respective accounts in one exchange 
 * and applies ($numtransactions) random transactions between them.
 * Randomizes 'created' field for accounts and transactions,
 * between ($firsttime) and ($lasttime).
 */

/** Number of accounts to create */
$numaccounts = 20;
/** number of transactions to create */
$numtransactions = 100;
/** now, randomize along this time (one year= 31536000segs) */
$lasttime = time();$firsttime = $lasttime - 31536000;

ces_develop_clean();

// Create stuff.
$bank = new CesBank();

// Create users.
$usernames = array();
for ($i = 1; $i <= $numaccounts; $i++) {
  $usernames[$i] = 'user' . $i;
}
$users = array();
foreach ($usernames as $name) {
  $users[$name] = ces_develop_register_user($name);
}
// Create exchange.
$net1 = array(
  'code' => 'NET1',
  'shortname' => 'Net 1',
  'name' => 'Network 1 - Time bank',
  'website' => 'http://www.integralces.net',
  'country' => 'ES',
  'region' => 'Bages',
  'town' => 'Manresa',
  'map' => 'http://maps.google.com/?ll=41.723796,1.832142&spn=0.083663,0.145912&hnear=Manresa,+Province+of+Barcelona,+Catalonia,+Spain&t=m&z',
  'currencysymbol' => 'ℏ',
  'currencyname' => 'hour',
  'currenciesname' => 'hours',
  'currencyvalue' => '1.0',
  'currencyscale' => '2',
  'admin' => $users['user1']->uid,
  'data' => array(
    'registration_offers' => 1,
    'registration_wants' => 0,
  ),
);
$bank->createExchange($net1);
$bank->activateExchange($net1);

// Create accounts.
$accounts = array();
for ($i = 1; $i <= $numaccounts; $i++) {
  $name = $usernames[$i];
  $accounts[$name] = ces_develop_register_account($users[$name], $net1, $i);
}

// Create account for admin (uid=1)
  $bank = new CesBank();
  $limit = $bank->getDefaultLimitChain($net1['id']);
  $account = array(
    'exchange' => $net1['id'],
    'name' => $net1['code'] . 'ADMI',
    'limitchain' => $limit['id'],
    'kind' => CesBankLocalAccount::TYPE_INDIVIDUAL,
    'state' => CesBankLocalAccount::STATE_HIDDEN,
    'users' => array(
      array(
        'user' => 1,
        'role' => CesBankAccountUser::ROLE_ACCOUNT_ADMINISTRATOR,
      ),
    ),
  );
  $bank->createAccount($account);
  $bank->activateAccount($account);

// Randomize created date in accounts.
for ($i = 1; $i <= $numaccounts; $i++) {
  $rndcreated = rand($firsttime, $lasttime);
  db_update('ces_account')
    ->condition('name', 'NET1' . sprintf('%04d', $i))
    ->fields(array('created' => $rndcreated))
    ->execute();
}


// Create local transactions.
$rndtransaction = array();
$rndfromuser = 0;
$rndtouser = 0;
for ($i = 1; $i <= $numtransactions; $i++) {
  $rndfromuser = rand(1, $numaccounts);
  $rndtouser = rand(1, $numaccounts);
  while ($rndtouser == $rndfromuser) {
    $rndtouser = rand(1, $numaccounts);
  }
  $rndvalue = rand(1, 1000) / 100;
  $rndtransaction[$i] = array(
    'NET1' . sprintf('%04d', $rndfromuser),
    'NET1' . sprintf('%04d', $rndtouser),
    $rndvalue,
    'concept' . $i,
    $users['user' . $rndtouser]->uid,
  );
}

foreach ($rndtransaction as $t) {
  $trans = array(
    'fromaccountname' => $t[0],
    'toaccountname' => $t[1],
    'amount' => $t[2],
    'concept' => $t[3],
    'user' => $t[4],
  );
  $bank->createTransaction($trans);
  $bank->applyTransaction($trans['id']);
}

// Randomize created date in transactions.
for ($i = 1; $i <= $numtransactions; $i++) {
  $rndcreated = rand($firsttime, $lasttime);
  db_update('ces_transaction')
    ->condition('concept', 'concept' . $i)
    ->fields(array('created' => $rndcreated))
    ->execute();
}

// OFFERWANTS.
// Add categories.
$names = array('Food', 'Hygiene', 'Professional services', 'Reparation',
  'Education');
$exchanges = array($net1);
$categories = array();
foreach ($exchanges as $e) {
  $categories[$e['id']] = array();
  foreach ($names as $c) {
    $cat = array(
      'parent' => 0,
      'title' => $c,
      'description' => $c,
      'exchange' => $e['id'],
      'context' => 1,
    );
    $categories[$e['id']][$c] = ces_category_save((object) $cat);
  }
}
// Add some offers.
$offers = array(array(
  'type' => 'offer',
  'user' => $users['user1']->uid,
  'title' => 'Cow\'s milk',
  'body' => 'Natural cow\'s milk. Probably the best you\'ve ever tasted.',
  'category' => $categories[$net1['id']]['Food']->id,
  'keywords' => '',
  'state' => 1,
  'created' => time(),
  'modified' => time(),
  'expire' => time() + 3600 * 24 * 365,
  'rate' => '0.2',
  ),
  array(
    'type' => 'offer',
    'user' => $users['user2']->uid,
    'title' => 'Bicycle mechanic',
    'body' => 'I fix or setup your bike in less than an hour.',
    'category' => $categories[$net1['id']]['Reparation']->id,
    'keywords' => '',
    'state' => 1,
    'created' => time(),
    'modified' => time(),
    'expire' => time() + 3600 * 24 * 365,
    'rate' => '1h/hour',
  ),
  array(
    'type' => 'offer',
    'user' => $users['user3']->uid,
    'title' => 'Natural soap',
    'body' => 'Natural soap with smell of Alpine flowers. Very good for your skin.',
    'category' => $categories[$net1['id']]['Hygiene']->id,
    'keywords' => '',
    'state' => 1,
    'created' => time(),
    'modified' => time(),
    'expire' => time() + 3600 * 24 * 365,
    'rate' => '0.40',
  ),
  array(
    'type' => 'offer',
    'user' => $users['user4']->uid,
    'title' => 'Cow\'s milk',
    'body' => 'Natural sheep\'s milk. Probably the best you\'ve ever tasted.',
    'category' => $categories[$net1['id']]['Food']->id,
    'keywords' => '',
    'state' => 1,
    'created' => time(),
    'modified' => time(),
    'expire' => time() + 3600 * 24 * 365,
    'rate' => '2.5',
  ),
  array(
    'type' => 'offer',
    'user' => $users['user5']->uid,
    'title' => 'Car mechanic',
    'body' => 'I fix or setup your car in less than an hour.',
    'category' => $categories[$net1['id']]['Reparation']->id,
    'keywords' => '',
    'state' => 1,
    'created' => time(),
    'modified' => time(),
    'expire' => time() + 3600 * 24 * 365,
    'rate' => 'it depends',
  ),
  array(
    'type' => 'offer',
    'user' => $users['user6']->uid,
    'title' => 'Natural shampoo',
    'body' => 'Natural shampoo with smell of Pyrinee flowers. Very good for your hair.',
    'category' => $categories[$net1['id']]['Hygiene']->id,
    'keywords' => '',
    'state' => 1,
    'created' => time(),
    'modified' => time(),
    'expire' => time() + 3600 * 24 * 365,
    'rate' => '6ECO each',
  ));
foreach ($offers as $offer) {
  $o = (object) $offer;
  $o->ces_offer_rate = array(LANGUAGE_NONE => array(array('value' => $offer['rate'])));
  unset($o->rate);
  ces_offerwant_save($o);
}
// Blog posts.
ces_develop_post_blog('Demo post', 'This is a demonstration blog post.

This space is intended for the administrator or a group of editors to publish relevant information about the trading community, such as markets, events and news.

Happy testing!', $net1, $users['user1']);

ces_develop_post_blog('Lorem ipsum', 'Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.', $net1, $users['user2']);
ces_develop_post_blog('May exchange newsletter', 'Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.', $net1, $users['user3']);
ces_develop_post_blog('Another post', 'Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.', $net1, $users['user4']);
