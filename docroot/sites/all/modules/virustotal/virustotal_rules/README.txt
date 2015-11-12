-- SUMMARY --

VirusTotal API Rules

This module provides the VirusTotal API Rules integration.


-- REQUIREMENTS --

VirusTotal API main module with a valid API key.
Rules 2.x


-- How to use it? --

Install as usual, see http://drupal.org/node/895232 for further information.

Following will be provided to rules:
  Actions
    Send a File
      Sends a File to VirusTotal service to queue it for scanning.
    Send an URL
      Sends an URL to VirusTotal service and queue it for scanning.
    Retrieve a file report
      Tries to retrieve a file scan report.
    Retrieve an URL report
      Tries to retrieve a URL scan report.
    Comment a report
      Creates a comment on a file or URL report.
  Events
    Before sending a request
      Event that triggers before an API request is send.
    After sending a request
      Event that triggers after an API request was send.
