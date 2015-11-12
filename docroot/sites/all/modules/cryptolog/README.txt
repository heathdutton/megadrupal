CRYPTOLOG
---------

Cryptolog enhances user privacy by logging ephemeral identifiers in
IPv6 notation instead of client IP addresses in Drupal's database
tables and syslog.

Once enabled in a site's settings.php file, Cryptolog replaces the PHP 
global variable $_SERVER['REMOTE_ADDR'] with an HMAC of the client IP 
address using a salt that is stored in memory and regenerated each day.

Because Cryptolog uses the same unique identifier per IP address for a 
24-hour period, it is still possible to do some statistical analysis of 
the logs such as counting unique visitors per day.

Note: As long as the salt can still be retrieved, brute force can be 
used to generate a rainbow table and reverse engineer the client IP 
addresses. However, once the salt has expired and a new salt 
regenerated, or the web server has been shutdown or restarted, it 
should not be feasible to determine client IP addresses.

CREDITS
-------

This module was inspired by the Cryptolog log filter script:
https://git.eff.org/?p=cryptolog.git;a=summary
