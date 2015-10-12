<?php
/**
 * @file
 * This file is a script for filling the database with initial data for
 * demo purposes. It empties the users and all ces tables and populates
 * it with 10 users with their respective accounts in two exchanges and
 * applies random transactions between them.
 */

ces_develop_clean();

// Create stuff.
$bank = new CesBank();

// Create users.
$usernames = array('Riemann', 'Euclides', 'Gauss' , 'Noether', 'Fermat');
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
  'admin' => $users['Riemann']->uid,
  'data' => array(
    'registration_offers' => 1,
    'registration_wants' => 0,
  ),
);
$bank->createExchange($net1);
$bank->activateExchange($net1);
$exchanges_demo[] = $net1;
$net2 = array(
  'code' => 'NET2',
  'shortname' => 'Net 2',
  'name' => 'Network 2 - Euro based',
  'website' => 'http://www.integralces.net',
  'country' => 'ES',
  'region' => 'Barcelonès',
  'town' => 'Barcelona',
  'map' => 'http://maps.google.com/barcelona',
  'currencysymbol' => 'ECO',
  'currencyname' => 'eco',
  'currenciesname' => 'ecos',
  'currencyvalue' => '0.1',
  'currencyscale' => '2',
  'admin' => $users['Fermat']->uid,
  'data' => array(
    'registration_offers' => 0,
    'registration_wants' => 0,
  ),
);
$bank->createExchange($net2);
$bank->activateExchange($net2);
$exchanges_demo[] = $net2;
// Create accounts.
$accounts = array();
for ($i = 0; $i < 3; $i++) {
  $name = $usernames[$i];
  $accounts[$name] = ces_develop_register_account($users[$name], $net1, $i + 1);
}
for ($i = 0; $i < 3; $i++) {
  $name = $usernames[$i + 2];
  $accounts[$name] = ces_develop_register_account($users[$name], $net2, $i + 1);
}
// Create local transactions.
$transactions = array(
  array('NET10001', 'NET10002', 1.2, '3kg of potatoes.',
    $users['Euclides']->uid),
  array('NET10002', 'NET10003', 0.8, 'Standard haircut.', $users['Gauss']->uid),
  array('NET10003', 'NET10001', 2.1, 'Yearly website mantainment',
    $users['Riemann']->uid),
  array('NET20001', 'NET20002', 25, 'Bike revision.', $users['Noether']->uid),
  array('NET20002', 'NET20003', 6, 'Natural soap.', $users['Fermat']->uid),
  array('NET20003', 'NET20001', 5.5, 'Ecologic carrots', $users['Gauss']->uid),
);
foreach ($transactions as $t) {
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
// Create one interexchange transaction.
$trans = array(
  'fromaccountname' => 'NET10001',
  'toaccountname' => 'NET20001',
  'amount' => '10',
  'concept' => 'Some old math books from Germany.',
  'user' => $users['Gauss']->uid,
);
$bank->createTransaction($trans);
$bank->applyTransaction($trans['id']);
// Activate virtual accounts.
$account = $bank->getAccountByName('NET1NET2');
$bank->activateAccount($account);
$account = $bank->getAccountByName('NET2NET1');
$bank->activateAccount($account);
// Re-trigger interexchange transaction.
$bank->applyTransaction($trans['id']);

// OFFERWANTS.
// Add categories.
$names = array('Food', 'Hygiene', 'Professional services', 'Reparation',
  'Education');
$exchanges = array($net1, $net2);
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
  'user' => $users['Riemann']->uid,
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
    'user' => $users['Euclides']->uid,
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
    'user' => $users['Gauss']->uid,
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
    'user' => $users['Gauss']->uid,
    'title' => 'Cow\'s milk',
    'body' => 'Natural sheep\'s milk. Probably the best you\'ve ever tasted.',
    'category' => $categories[$net2['id']]['Food']->id,
    'keywords' => '',
    'state' => 1,
    'created' => time(),
    'modified' => time(),
    'expire' => time() + 3600 * 24 * 365,
    'rate' => '2.5',
  ),
  array(
    'type' => 'offer',
    'user' => $users['Noether']->uid,
    'title' => 'Car mechanic',
    'body' => 'I fix or setup your car in less than an hour.',
    'category' => $categories[$net2['id']]['Reparation']->id,
    'keywords' => '',
    'state' => 1,
    'created' => time(),
    'modified' => time(),
    'expire' => time() + 3600 * 24 * 365,
    'rate' => 'it depends',
  ),
  array(
    'type' => 'offer',
    'user' => $users['Fermat']->uid,
    'title' => 'Natural shampoo',
    'body' => 'Natural shampoo with smell of Pyrinee flowers. Very good for your hair.',
    'category' => $categories[$net2['id']]['Hygiene']->id,
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
  $offers_demo[] = $o;
}
// Blog posts.
ces_develop_post_blog('Demo post', 'This is a demonstration blog post.

This space is intended for the administrator or a group of editors to publish relevant information about the trading community, such as markets, events and news.

Happy testing!', $net1, $users['Riemann']);

ces_develop_post_blog('Lorem ipsum', 'Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.', $net1, $users['Riemann']);
ces_develop_post_blog('May exchange newsletter', 'Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.', $net2, $users['Fermat']);
ces_develop_post_blog('Another post', 'Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.', $net2, $users['Fermat']);
