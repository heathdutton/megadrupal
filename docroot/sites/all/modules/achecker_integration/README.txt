The achecker integration module integrates with the AChecker through its API.

AChecker reviews single HTML pages for conformance with accessibility 
standards to ensure the content can be accessed by everyone.

Two types AChecker web service API are provided:
- Accessibility validation review.
- Save or reverse decisions made on accessibility checks that a human must 
  make.

This module allows the submission of nodes to the ACHecker validation API via
a node operation on the content page (/admin/content).  Note: if you have
overridden this page (such as using the admistration_views module) you will 
need to replicate the actions of hook_node_operations appropriate to your 
override.

The results of all AChecker scans are listed on the reports page 
(/admin/reports/achecker), with the option to inspect each of the scans 
individually.

Within an individual scan will be listed errors that do not require human
judgement.  Also listed are likely and potential problems that do require the
discretion of a human.  Criteria for deciding whether a problem is present is
supplied in the form of a radio group, which can be submitted back the server
to update the individual scan. Problems judged as non-issued can be reversed
back to either fail or no decision.

The accessability standards applied can be configured at the settings page 
(/admin/config/content/achecker), as well as suppling your AChecker webservice
ID (which can be obtained by registering with AChecker 
http://achecker.ca/register.php). You can also configure the module to use you
own copy of the AChecker validation service.

There is also a submodule, 'Achecker integration - WYSIWYG' which allows
content from an individual textarea be vallidated using a button that can be
added to the TinyMCE editor.
