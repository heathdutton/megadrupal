
The Icecast YP directory listing specification allows an Icecast server 
to update a central directory with information about its available 
public streams.

The YP module provides a Drupal-based Icecast YP directory, including 
both the CGI script to receive YP posts from Icecast servers and a 
sortable directory display page.

After installing the module, you can submit your Icecast stream(s) 
to the YP directory by adding the following to your icecast.xml 
configuration file, where "http://example.org/" is the URL of your 
Drupal site:
  <directory>
    <yp-url-timeout>15</yp-url-timeout>
    <yp-url>http://example.org/yp/cgi</yp-url>
  </directory>

More features can and should be added.  Please submit your patches 
to http://drupal.org/project/icecast -- Thanks!

Icecast YP specification: http://www.icecast.org/spec.php
Reference implementation: https://trac.xiph.org/browser/trunk/icecast2yp
More about Icecast: http://www.icecast.org/
