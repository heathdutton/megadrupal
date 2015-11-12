This module is an integration with Zip Alerts and the Recruiter Distribution.

ZipAlerts

If you are using the Recruiter Distribution for a posting jobs website and you
are looking for drive more job seeker traffic to your website Zip Alters can
be a great service to market your jobs.

How the integration works ?

You need to contact Zip Alerts and become a partner with them. After you
become a partner they will ask you for a a XML file that contains all your jobs.
By using the Views & Views Data Export, this module will generate the 
XML with all Zip Alerts Requirements. Currently, this module only currently
works for "Job Per Template" content type (that is in the Recruiter
Distribution). If you are not using the Recruiter Distribution you can either
used the recruiter_features or you can used the submodule Zip Alerts for
Non Recruiter.

* Requirements
  - Views (https://www.drupal.org/project/views)
  - Views Data Export Module  (https://www.drupal.org/project/views_data_export)

* Installation
 - Place the module this directory in sites/all/modules/ for more question
   visit https://drupal.org/documentation/install/modules-themes

* Configuration
  - You need to go the Zip Alerts Views and insert your domain and website name.
     admin/structure/views/view/zip_alerts
  - To insert domain click on the third field that is called
    "Content: Nid (url)" and you are going to click "REWRITE RESULTS." A box
    field will open and you are going to replace Your-Website.com with your
     domain. Here is a picture example http://i.imgur.com/yQjSatJ.png.
   - To insert website name click on the last field that is called
     "Global: Custom text (site)" You will see a text that said "Your Website
      Name" replace that with your website name.


* Troubleshooting
* FAQ

* Maintainers
      Darryl Norris
      Twitter: @darol100
      Email: module@darrylnorris.com
      LinkedIn: www.linkedin.com/in/darrylnorris/
      Github: https://github.com/darol100
      Website: www.darrylnorris.com
