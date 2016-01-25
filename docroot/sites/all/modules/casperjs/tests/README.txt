These files are sample tests for you to use as a template.

You should create a tests directory out of your project's docroot and place
test files there. Then, run tests with the following command:

$ drush casperjs --test-root=path-to-tests

Alternatively, you can create a Drush settings file at sites/default/drushrc.php
with the following contents:

<?php

$command_specific['casperjs'] = array('test-root' => 'path-to-tests');
