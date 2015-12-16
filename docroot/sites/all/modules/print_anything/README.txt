== Introduction ==
This module generates a print friendly page by using the javascript window.opener.document object to display the content of specified CSS IDs. It can display anything irregardless of how it was generated in Drupal.

== Usage ==
After installing in the usual fashion, place the "print link" block in the location of your choosing.

Then add and configure some urls at admin/config/print-anything/paths. Each path has it's own list of CSS ids, which control what is included in the printer page for that url. Paths can be as specific or as general as you like, wildcards are allowed. If no match is found the default settings are used.

There's an additional settings page where you can control a few other features. The "Header" and "footer" on the printer page and the text of "print link" that appears throughout the site.