-- SUMMARY --

VirusTotal API

This module provides the VirusTotal API functionality to your Drupal site.
It does not do anything on its own!

Virustotal is a service that analyzes suspicious files and URLs and facilitates
the quick detection of viruses, worms, trojans, and all kinds of malware
detected by antivirus engines.

More information at https://www.virustotal.com/about/


-- REQUIREMENTS --

No other modules are required but a valid VirusTotal account is needed.


-- How to use it? --

Install as usual, see http://drupal.org/node/895232 for further information.

Configuration
  Goto admin/config/system/virustotal and paste in a valid personal API key.
  If you are registered to the VirusTotal community you will find you key at
  your profile on the "API key" tab.

Sitebuilding
  If you just need the basic API functionality you should have a look at the
  VirusTotal Rules submodule. With Rules 2.x and the VirusTotal API Rules
  integration you can handle most of the API functionality without writing
  any line of code.

Development
  If you want to use or extend the VirusTotal API you should have a look at the
  VirusTotal Examples submodule. It will show you how to use the API in detail.


-- API Functionality --

Functions of the VirusTotal API Class
  scanFile()
    Sends a File to VirusTotal service to queue it for scanning.
  getFileReport()
    Tries to retrieve a file scan report.
  scanUrl()
    Sends an URL to VirusTotal service and queue it for scanning.
  getUrlReport()
    Tries to retrieve a URL scan report.
  makeComment()
    Creates a comment on a file or URL report.

Hooks
  hook_virustotal_query_alter
    Modules may make changes to the query data before it is send to VirusTotal.
  hook_virustotal_result_alter
    Modules may make changes to the response data before it is returned.
