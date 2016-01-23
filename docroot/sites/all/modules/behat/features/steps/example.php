<?php
$steps->Given('/^a user "([^"]*)" with password "([^"]*)"$/', function($world, $name, $pass) {
  $world->d->drupalCreateNamedUser($name, $pass);  
});

$steps->When('/^I login as "([^"]*)" with password "([^"]*)"$/', function($world, $name, $pass) {  
  $world->d->drupalPost('user', array('name' => $name, 'pass' => $pass), t('Log in'));
});

$steps->Given('/^I visit "([^"]*)"$/', function($world, $path) {
  $world->d->drupalGet($path);
});

$steps->Then('/^it should display "([^"]*)"$/', function($world, $text) {
  $world->d->assertText($text);
});

$steps->Then('/^I should see the login form$/', function($world) {
  // Assert username input field
  $world->d->assertFieldByName('name');
});