Sunlight APIs
-------------

This module makes it easy for any Drupal module to use the Sunlight APIs.

You can read more about the Sunlight data services at
https://sunlightfoundation.com/api/

To use the APIs, simply call the sunlight() function, passing it a provider
(such as "Congress") and a resource (such as "legislators_locate").  You can
find a list of supported providers and resources at
https://github.com/lobostome/FurryBear/wiki/Providers

Here's some sample code a Drupal module could use:

$tweet = 'Hey Congress...';
$legislators = sunlight('Congress', 'legislators_locate')->getByZip($zip);
foreach ($legislators->results as $legislator) {
  if ($legislator->chamber == 'house' && $legislator->twitter_id) {
    $tweet .= " @{$legislator->twitter_id}";
  }
}

To file a bug or feature request, please visit the project page for this Drupal
module at https://www.drupal.org/project/sunlight

Note, this module is not affiliated with Sunlight Foundation; it has been
developed independently.
