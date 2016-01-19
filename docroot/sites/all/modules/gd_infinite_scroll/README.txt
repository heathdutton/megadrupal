1) Overview

Provide an administration to use the infinite scroll jQuery plugin : auto-pager on custom pages using drupal pager.


2) Features

 - Create settings by page, you can use it with every list that use the default theme pager.
 - Test on your local environment and deploy your settings with <a href="http://drupal.org/project/features">features </a> when your ready.
 - You can use pattern in URL to active similar settings on multiple page. (like category/*)
 - Call directly gd_infinite_scroll_add_unique_settings('my_settings'); in your module to arbitrary add a settings on your pages.
 - Can display a load more button instead of autoload on scroll. 


3) Requirements

 You must download the jQuery plugin auto pager 
 http://jquery-autopager.googlecode.com/files/jquery.autopager-1.0.0.js
 and move it in the libraries path 'autopager' or directly in the js directory of this module. 
 The file must be named jquery.autopager-1.0.0.js


4) Tutorials

 - Create a list of x item by page with a default pager (can be a views with the complete pager, 
   or the result of db_query, etc ...
 - Display the list on a drupal url, for example : "my-items"
 - Go to admin/config/user-interface/gd-infinite-scroll
 - add a new infinite scroll settings via the link
 - set the url to 'my-items'
 - adjust the selector for your markup (default selector is for views markup)
 - save, visit 'my-items' page.
 - it doesn't works : post your markup, the selector used and the errors you get (if you have) in the issue queue.
 - it works : create a feature, select the gd infinite scroll settings, deploy.


5) Credits

Thanks to the developers of jquery plugin auto pager.
Thanks to studio.gd to support the development.


6) Similar projects

https://www.drupal.org/project/views_infinite_scroll did the same work but for views only.
