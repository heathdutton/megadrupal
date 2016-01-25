Visitors Voice
--------------

This module provides a secure integration between Visitors Voice SaaS and Search
API. Visitors Voice SaaS free version makes it possible for site search managers
to customize the search engine result page (SERP) as they want with unique drag
and drop capabilities.

Try the live demo at http://www.visitorsvoice.com/live-demo.html

Sign up for a free account at http://www.visitorsvoice.com/sign-up-free.html

Security
--------

To be able to change search results on your site, Visitors Voice needs to get
access to your site. This permission is granted by entering your private
Visitors Voice access key (received after creating an account) in this module's
settings page. This module will authenticate all requests to its interface with
this key using modern and proven authentication mechanisms, so as long as you
keep the key safe no malicious access to your site will be possible via this
interface.

Furthermore, access via this interface is restricted to changing the search
results on your site (and only with content on your site), so no arbitrary
changes of the site are possible and no information other than normal search
results can be disclosed via this interface.

However, communication between Visitors Voice and your site will only be
authenticated, not encrypted. Therefore, unless your site uses HTTPS/SSL, an
attacker with a favored position in the network would still be able to read the
contents of this communication – i.e., the changes you make to search results on
your site and the searches you issue via the Visitors Voice dashboard (along
with their results).

Supported search engines
------------------------

Currently only Search API Solr Search is supported as the backend. If you are
interested in using Visitors Voice SaaS with other search engines, please
contact us at info@visitorsvoice.com.
Please also contact us if you are a developer and want to include support for
Visitors Voice in your module.

Installation
------------

In order to get started with Visitors Voice you need to have Search API and
Search API Solr search module up and running. Then you install this module and
sign up for a free account at Visitorsvoice.com. In the Account settings of
Visitors Voice dashboard you will find an Access Key that you need to copy and
paste into this module's configuration interface.

You then need to set the URL of your Drupal installation in the settings of the
Visitors Voice dashboard, in order to make the integration work properly.

Current restrictions
--------------------

Currently, only one search index per site is supported (configurable in the
settings). Also, only the results that can be viewed by anonymous users are
displayed in searches in the Visitors Voice dashboard.

If one of these restrictions prevents you from using Visitors Voice on your
site, please contact us in the issue queue and we will try to fix it in one of
the next versions of the module.

Premium version
---------------

There will be a premium version that helps site search managers prioritize their
work based on users' site search behavior. If you are interested in helping us
beta test, please contact us at info@visitorsvoice.com.

Support
-------

For general support, please use the project's issue queue:
https://drupal.org/project/issues/visitorsvoice

In cases where you'd have to disclose private information, you can also contact
our Support team directly:
http://www.visitorsvoice.com/support.html
