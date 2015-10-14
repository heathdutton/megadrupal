$Id:

Olark

The Olark module for Drupal  
by Jeff Robbins / Lullabot
Drupal 7 port by Ilari Mäkelä / Mearra
***************************

The Olark module provides some simple integration between the Olark live chat 
service and Drupal. The module also provides a 
[Context](http://drupal.org/project/context) reaction to allow you to add the 
code using more complex Context-based rules, such as adding the code at 
specific paths, for specific roles, or other criteria. For instance, you could 
only show the Olark chat on the home page to anonymous users.

In addition to adding the Olark code to your page, the module also tells the 
operator whether the user is logged in and if so, what their username, email 
address, and user page are.

Installation
============
1. Create and configure an account at [Olark](http://www.olark.com).
2. Place this module in your modules directory and enable it.
3. Visit http://www.olark.com/install and copy the code chunk
4. Visit admin/settings/olark on your Drupal site and paste the code chunk 
into the textarea. Submit the settings form.

Optional: If using Context module, enable the "use Context" setting on the 
admin/settings/olark page. Then visit admin/build/context and add the "Olark" 
reaction to one of your context items.
