---READ ME---

This module comes handy if you do not want to keep checking your approval queue by logging in your site again and again and if 
you dont want to receive a notification for every comment posted on the site. 
The basic idea originated from a real world case where a site with comment approval implemented received random number of comments in a day.
Either the admin's mail would get buckets of notification or else he had to periodically keep on checking the queue.
This module just sends the number of comments pending for approval in your mail at user defined intervals.



---DEPENDENCY---

Elysia Cron (http://drupal.org/elysia_cron)


---INSTALLATION---

 > Download the module
 > Extract the contents in your modules directory
 > Go to /admin/build/modules and enable Comment Count Notify and Elysia Cron
 > Go to admin/settings/comment_count_notify and configure the module settings
 

For more flexibility, you can even configure the cron run for "Send periodic comment count notifications" function at admin/build/cron


---THEY MAY ALSO INTEREST YOU---

A comparison of notification modules:
http://groups.drupal.org/node/15928

