Description
-----------
The JobFaucet module works in conjunction with a web service (JobFaucet.com) to
take job openings that you post on your Drupal website and share them with job
boards like Indeed, Simply Hired, and others. You create and update job
postings in the one place that makes the most sense - on your own website -
and JobFaucet does the rest.

To use this module, you need a free JobFaucet.com account.  This module adds a
jobfaucet content type to your Drupal site.

Requirements
------------
Drupal 7.x
Oauth 7.x-3.x
Token 7.x-1.x
Text 7.x
List 7.x

Installation
------------
1. Copy the jobfaucet directory to the Drupal sites/all/modules directory.

2. Login as an administrator. Enable the module in the "Administer" ->
   "Modules"

3. Edit the settings at 'Administer' -> 'Configuration' -> 'Web services' ->
   'JobFaucet'.  You'll need to create a free account at jobfaucet.com if you
   don't already have one and enter your API keys on this page.  Set
   syndication to 'on'.

4. Create and save a job posting at node/add/jobfaucet-job.  

Advanced usage and customization
--------------------------------
Please see the jobfaucet.com website for more detailed documentation.

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/1972978
