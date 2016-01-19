GoogleNews
----------
This module generates a Google News sitemap feed based on the content created
within the last 48 hours.


Features
-------------------------------------------------------------------------------
* Google News -compatible sitemap XML output.

* Selection of content types to be output is configurable.

* Output XML file can be cached to reduce server load with a configurable timer.

* Additional tags may be added and controlled using tokens.
  * access - Indicate whether the content is public or has access restrictions.
  * genres - Indicate the categorization of the content.
  * geo_locations - Indicate an address relevant to the content.
  * keywords - Aids with content categorization.
  * stock_tickers - Google Finance-derrived company identifiers.


Configuration / Usage
-------------------------------------------------------------------------------
 1. Without any configuration, the module should generate a Google News sitemap
    feed for all nodes from the past 48 hours at the following URL:
	    http://example.com/googlenews.xml

 2. All configuration is handled via the main settings page:
      admin/config/services/googlenews

 3. The following items may be controlled:

    * The publication name defaults to the site's name, this may be overridden.

    * By default all content types will be used in the sitemap, this may be
      changed as needed; as Google News expects only *news* articles, choose
      the content types wisely.

    * The publication name on the sitemap file defaults to the site name.

    * Only content created within the past 48 hours will be displayed, this can
      be changed though the default is recommended.

    * To aid with site performance, the file's output will be cached for 15
      minutes; this can be increased as necessary.

    * A number of optional tags can be added to the output for each node.

 4. Visit the sitemap file to confirm it contains the desired content:
      http://example.com/googlenews.xml

 5. Use the Google Webmaster Tools to submit the sitemap file per the official
    documentation:
      http://support.google.com/webmasters/bin/answer.py?hl=en&answer=74289


Troubleshooting
------------------------------------------------------------------------------
* If no content shows in the sitemap file then check that there is content
  matching the filters and within the time range.


Credits / Contact
------------------------------------------------------------------------------
Currently maintained by Damien McKenna [1]. Dave Reid [2] co-maintained the
module from 2009 through 2011. All initial development was by Adam Boyse [3].

Ongoing development is sponsored by Mediacurrent [4]. Past maintenance was
sponsored by Palantir.net [5]. All initial development was sponsored by
Webopius.com [6].

The best way to contact the authors is to submit an issue, be it a support
request, a feature request or a bug report, in the project issue queue:
  http://drupal.org/project/issues/googlenews


References
------------------------------------------------------------------------------
1: http://drupal.org/user/108450
2: http://drupal.org/user/53892
3: http://drupal.org/user/442408
4: http://www.mediacurrent.com/
5: http://www.palantir.net/
6: http://www.webopius.com/
