
Google Adwords Tracking module for Drupal 7.x.
This module adds tracking information (JS) to nodes that you want to track for 
advanced ROI reports with Google Adwords

REQUIREMENTS
------------
* Google Adwords Acccount
* An existing converstion tracking in your Google Adwords account (you will 
  need the information from this to complete setup)

INSTALLATION INSTRUCTIONS
-------------------------
1.  Copy the files included in the tarball into a directory named 
    "google_adwords" in your Drupal sites/all/modules/ directory. (Or use 
    drush "drush dl google_adwords")
2.  Login as site administrator.
3.  Enable the Google Adwords module on the Administer -> Modules page 
    (Under the "Statistics" category).
4.  Fill in required and desired settings on the Administer -> Configuration -> 
    System -> Google Adwords page. This includes specifying which role(s) the 
    Adwords code should be displayed for.
5.  To complete the installation you will now have to add the Google Adwords 
    field to a content type.  Under Structure -> Content Types -> 
    <Choose Your Content Type> -> Manage Fields -> Add new field 
    (Field type: Google Adwords).
6.  If you have setup all the information in #4 with your Google Adwords 
    tracking information you will now see the information from step 4 as the 
    default value for the field. Save content type.
7.  One last step before tracking will function. Conversion labels have to be 
    setup on a per node basis!  
    Now edit a node that you want to track.  Find the Google Adwords field and 
    supply the "Converstion label" that Google Adwords gave you when you setup 
    tracking look for "var google_conversion_label = "SOMELONGSTRINGOFSTUFF";"
8. Once you have created a node with the Adwords tracking code, view the source
   code of the finished node and locate the tracking code to verify it is there.
   Make sure you view the page as a user with one of the roles assigned in 
   step 4. If you chose only the Anonymous role, make sure to log out before 
   viewing the page.
9. Once you start getting conversions via the page, verify in Google Adwords 
   that the conversions are showing up in your Adwords reports.
10.Enjoy better ROI tracking on your PPC campaigns.
