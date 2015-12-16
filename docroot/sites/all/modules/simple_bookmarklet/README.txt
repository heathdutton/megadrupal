Simple Bookmarklet

A bookmarklet is a bookmark stored in a web browser that contains JavaScript
commands to extend the browser's functionality. For instance, a bookmarklet
might allow the user to select text on a page, click the bookmarklet, and be
presented with a search engine results page for the search term selected
(from Wikipedia).

This module is intended to be a simple but flexible and fully functional
bookmarklet module, as the name suggests. This way, a user who is browsing a
page can create content on a Drupal site from that page. There is just one
administration interface. Just enable the module and go to this interface
(admin/config/content/simple_bookmarklet).

Through this interface, you can:

* Choose a title for the bookmarklet
* Add extra CSS files
* Add extra JavaScript files
* Choose what content type can be created through the bookmarklet
* Choose which fields can be filled through the bookmarklet
* Define how fields can be pre-populated (page title, selected text or URL)
* Force HTTPS

After setting the preferences, a link will be generated, which is stored on a
variable, so you can display it easily on other places of your site. Users can
then drag and drop this link to their bookmarks bars.

Tested on Firefox, Google Chrome and Internet Explorer.

Check this less-than-one-minute video [1] to understand better how it works.

This module was fully sponsored by Meedan [2].

References:

1- http://ca.ios.ba/files/drupal/simplebookmarklet.ogv
2- http://meedan.org
