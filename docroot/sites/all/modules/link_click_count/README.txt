
----------------------------------------------------------------------------------------
                               Link Click Count
----------------------------------------------------------------------------------------

The Link Click Count module allows users to find the number of clicks happend on the 
particular link.


Installation 
-------------

 * Link Click Count module depends on Link module, Views module and Views Distinct
   module.
 * Copy the whole Link click count directory to your modules directory
   (e.g. DRUPAL_ROOT/sites/all/modules) and activate the Link Click Count 
   module


Configuration
--------------

 * Visit any link field setting page.
   [admin/structure/types/manage/<content-type>/fields/<any-link-field>]
   (Link Clicks Count Configuration - Save the clicks happened on this link.)

 * Visit that Manage Display page.
   [admin/structure/types/manage/(content-type)/fields/(any-link-field)/display]
   Goto to the appropiate field, for which you have saved the configuration,
   Then select the field formatter settings to "Counts the click happened on this link."

 * Getting statistics, You can view statistics on tracked links using the views module.
   By default, the link click count module creates a view for you to use named Link Click Count Stats,
   which can be displayed at this link http://www.yourdomain.com/link-click-count-stats.
