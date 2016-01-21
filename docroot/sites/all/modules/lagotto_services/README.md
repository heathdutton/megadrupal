# Overview

This module provides a PHP wrapper for the web services supplied by the
article metrics server Lagotto[1], used by journal publishing companies to track
metrics of journal use such as page views, PubMed and Scopus references,
Facebook likes, and Mendeley article 'saves'.

The module encapsulates the basic "fetch information", complete with access
control, and provides an admin page where the base url and api keys can be
managed. The complete results from the remote server are made available to the
client.

The submodule lagotto_services_submit enables the add, modify and delete
operations for articles on the server.

# Requirements

 - Runs on PHP 5.3.x, 5.5.x
 - Drupal 7.x (x > 27)
 - Accessible Lagotto server and appropriate user api keys

# Restrictions

Although Lagotto servers do support the use of PubMed and Mendeley IDs to
discover articles, this module does not currently attempt to support lookup
using them; only DOIs are supported. PubMed and Mendeley IDs are still included
in the returned results.

# Installation

  1. Extract the module to sites/all/modules or sites/{*}/modules depending on
     the site.

  2. Enable the module at admin/modules or use drush, e.g.:

     drush pm-enable lagotto_services

  3. You may also wish to enable the example block, to check whether the module
     is working properly. Enable it at admin/modules or use drush, e.g.:

     drush pm-enable lagotto_services_example

# Configuration

The module's administration page is at admin/config/services/lagotto_services
with additional role-based access permissions on the admin/people/permissions
page.

The example block (called 'Lagotto services block' in the Blocks GUI) has a
configuration item which is the DOI of an article to fetch which you should
set to the DOI of an article that is available on your Lagotto server when you
add the block to your site.

## Server

The module requires an external server running the Lagotto software. The project
is on github and with a project website at <http://articlemetrics.github.io>.
The installation documentation can be found at:

  http://articlemetrics.github.io/docs/installation

You should install and configure the server on a host that is stable; Lagotto
needs time to acquire the metrics. It doesn't care whether it has a DNS name,
but it does need to have internet access. It also needs to be told which
articles it should be looking for: you can use a number of methods, the simplest
(but not best) of which is to provide a text file in the form:

    DOI Pubdate Title
    DOI Pubdate Title
    DOI Pubdate Title

where DOI is the http://dx.doi.org/ of the article, pubdate is the day of
first publication, and Title is the title. This file can then be loaded into
the server using "rake db:articles:load". See the Lagotto docs for details.

## Server URL

Provide the base URL for this server on the configuration form. For example
if you have installed it on a machine with IP 192.168.98.98, you should enter:

    http://192.168.98.98

in the 'Lagotto server URL' field of the admin form. To use a public server,
such as that run by plos.org, use that server's name in the normal way:

    http://alm.plos.org

## API Key

To make requests of the server, you need to provide an API key. Lagotto by
default allows users to create accounts that can read API data from the server
and you should do this. (If you have installed a local instance you make an
admin account as part of that process: you can use the API key for that account
if you wish).

Once you have an account, the API key you need is available by:

 - visiting the lagotto server home page;
 - logging in;
 - clicking on your username at the top right of the screen and selecting
   the 'Your account' menu.
 - Note down the key string that appears beside 'API Key'.

That string -- e.g. SQxutHvztKp6+PhtzVzT -- should be copied into the
module 'User API Key' field. There is no need to enter your username.


# Usage

There are no out-of-the-box visible elements provided in this module,
although there is a sub-module lagotto_services_example that defines a demo
Drupal block containing metrics for one article. You can use that example as the
basis for your own work.

Software must call one of the php API functions, of which the main ones are:

    lagotto_services_fetch_work
    lagotto_services_work_exists
    lagotto_services_service_host
    lagotto_services_service_key
    lagotto_services_service_url

Refer to the function comments for additional information on these calls.

# Authors

  - rivimey - https://www.drupal.org/u/rivimey
  - nlisgo - https://www.drupal.org/u/nlisgo - support

[1] Lagotto development is sponsored by CrossRef and the Public Library of
Science:

  - http://www.crossref.org
  - http://www.plos.org
  - http://articlemetrics.github.io/docs
