INTRODUCTION
-------------
* The module name (Sentiment Analysis) itself describes everything 
  about this module.

* This module provides a field type ("Sensitivity")

* This module needs an additional API key(3rd Party) to check 
  user inputted text and returns the result of sentiment(If negative).

REQUIREMENTS
-------------

* Needs External API key(3rd Party) from https://www.idolondemand.com.
* Internet connection to hit API with inputted value and get output.

INSTALLATION
-------------

* Install as you would normally install a contributed Drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.

CONFIGURATION
-----------------------

* This module requires API key(3rd Party) from https://www.idolondemand.com.
* Login / SignUp in https://www.idolondemand.com
* After successful login goto 
  https://www.idolondemand.com/developer/apis/analyzesentiment/#try
* Copy API key (Check project page for example images).
* Before Proceeding please read this 
  https://www.idolondemand.com/docs/eula.html and then generate key.
* However when the module is enabled, if you go to the manage fields page of 
  the content types at admin/structure/types/manage/fields, 
  you will see a new field type "Sensitivity" in the 
  Field type selection drop down.
  Once you select the field type as "Sensitivity", in the Widget drop down
  field, you will get the "Word Sentiment"(selected by default).
* After selecting Field Type as "Sensitivity" thats it, 
  it will take care of sentiment description of User input.
* Also this module provides "Sentiment" in views to show custom table data. 

USAGE
------
 * As most of the site(s), they are having user inputted value(s) 
   by their user, again to evaluate those value(s), admin/other permitted
   role(s) will check the content and then post in their site(s).
 * After enabling this module, there is no need to check manually for 
   the sentiment description of user inputted value(s).

MAINTAINER
-----------
Current maintainers:
 * A AjayKumar Reddy (ajaykumarreddy1392) - https://www.drupal.org/user/3261994
