INLINEMANUAL API - PHP Library

Compatible with PHP 5.2+

Example usage:

<?php

require(dirname(__FILE__) . '/lib/InlineManual.php');

InlineManual::$site_api_key = 'YOUR_SITE_API_KEY';

try {
  $topics = InlineManual_Site::fetchAllTopics();
  foreach ($topics as $topic) {
    print "$topic->id: $topic->title\n";
  }
}
catch (InlineManual_Error $e) {
  print $e->getMessage();
}
