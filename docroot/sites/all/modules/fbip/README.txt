FBIp - Form Block Ip is that lightweight IP Blocking module that you always 
wanted on Drupal to block spammers.

LIGHTWEIGHT because it add NO additional tables. This module uses your core 
IP Blocking tables and Drupal's inbuilt Flood Control Mechanism/API to 
keep a track of IP accesses.

It keeps an eye on all forms (or the form ids you specify).

When a configured threshold number of submissions is reached, the IP is 
blocked and the user can no longer access the site.

Optionally, the IP Ban can be configured to be reset on cron.

While modules like captcha and its likes do a good job in preventing spam  
submissions, this module will come in handy when the spammer is consuming your 
site resources by generating the form thousands of times and 
making your cache_form uncontrollably large by huge number of attempts 
to submit your form.

Beware, you might lock yourself out! 
If that happens, delete the entry in blocked_ips table with your IP address.

Feature Overview:
* Allows banning IP when number of form submissions in specified time reaches threashold
* Choose between tracking all forms, or specific form IDs  
* Users with a special permission can be allowed to bypass this restriction
* Allows IP whitelisting
 