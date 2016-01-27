About Destination
------------------

This little module enables you to find the destination url of a short url. For example, if bit.ly/123 is a short url for youtube.com/12345678901234567890, you can do this: 

  $short_url = 'http://bit.ly/123';
  $destination_url = destination_url($short_url);

Now $destination_url's value will be http://youtube.com/12345678901234567890. For more robust useage call destionation($short_url) and check the returned http code for error handling.

This module does not expose new features or functionality to end-users. It's really only helpful for writing code.


Installation
-------------

Place destination in your modules directory, then go to admin/modules and enable.


Dependencies
-------------
cURL


Maintainer
----------
Bryan Hirsch, bryan [at] bryanhirsch [dot] com
