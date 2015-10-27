Introduction
================

The Domain 301 Redirect module allows sites to 301 redirect to a domain that is
marked as the main domain. This means you can have all subdomains and other
domains 301 redirect to a domain that you choose as the main domain.
This provides great SEO benefit.

For all bugs, feature requests or support requests please use the Domain 301
Redirect issue queue at http://drupal.org/project/issues/domain_301_redirect


Key Functionality
=====================

1. Allows you to 301 redirect all other domains to a primary domain.

2. Allows validation that the primary domain points to the site.

3. Allows for periodic checking that the primary domain still points to the site.



Installation
===============

1. Upload and install the Domain 301 Redirect module.

2. Go to Administer -> Config -> Search and metadata -> Domain 301 Redirect.

3. Enter the primary domain into the textfield, change the status to enabled and
    enter the checking period you wish to have.


Examples
========

With the main domain set as http://www.example.com the following redirections
would take place:

1. http://example.com/user/1 -> http://www.example.com/user/1

2. http://mydomain.com/content/2 -> http://www.example.com/content/2

3. http://www.mydomain.com/somepage -> http://www.example.com/somepage

As you can see from the examples, the path remains in tact but the host portion
of the URL changes.
