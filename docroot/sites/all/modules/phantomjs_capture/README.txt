The module comes with an integration to PhantomJS which allows you to take
screenshots of homepages as images and use them on your site.

The module comes with an administration interface to allows you to configure the
location of PhantomJS and a default destination folder. It also has a test
function to, well test if your configuration works. PhantomJS works by giving
phantom an JavaScript file that tells phantom what to do and this module comes
with a default JavaScript file that renders full HD size screenshots.

For more information about PhantomJS possibilities you should look at the wiki
pages at https://github.com/ariya/phantomjs/wiki and the examples that comes
with PhantomJS.

# Usages
The module comes with a simple field that allows users to enter a absolute URL
to a page they want to capture and the field provides a display of the captured
site which can be displayed using image style and the image may link to the site
captured.

To use this module in your code the phantomjs_capture_screen can be called after
you have saved its configuration.

The $url is the page to capture and the $dest is an URI to where the image
should be stored and the $filename to store it under.

<?php
  phantomjs_capture_screen($url, $dest, $filename)
?>

# Requirements
The module requires that PhantomJS is installed on your server. See the PhatomJS
download page at http://phantomjs.org/download.html.

# Installation
Simply download and enable the module(s) as any other modules goto your content
type (or entity) and add the PhantomJS field to your entity.