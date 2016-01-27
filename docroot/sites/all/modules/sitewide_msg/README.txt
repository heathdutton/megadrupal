Description
-----------
This module provides functionality to display urgent message to all users.
Originally was created for the project with a lot of Ajax when user can stay
on the same page for significant period.

This is a simple tool which uses periodic polling from client side within small
dedicated <iframe>. Pulled file sitewide_msg.poller.php using limited Drupal
bootstrap to minimize performance overhead.

Usage
-----
Install module according to standard Drupal procedure and enable it. After that
you'll be able to configure displaying sitewide messages in two way:
  - by assigning dedicated block to desired region;
  - by using in your page.tpl.php new variable $sitewide_msg, sample usage from
    real project:

  <?php
  if ($sitewide_msg) {
    echo $sitewide_msg['placeholder'];
  }
  else {
    // optional - to keep spacing when module disabled.
    echo "<div style='padding-top: 1.5em'></div>";
  }
  ?>

To send message, as well as to configure polling interval and optional expiration
of the displayed message, use screen at admin/config/people/sitewide_msg .

Have fun!
