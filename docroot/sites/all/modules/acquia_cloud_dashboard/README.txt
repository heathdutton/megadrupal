Acquia Cloud Dashboard allows you to manage your Acquia Hosting from any Drupal 
Site.

You can use the Drupal Site hosted on Acquia, or a separate Drupal Site hosted 
elsewhere, to manage your Acquia Hosting.

**INSTALLATION**
1) Once Enabled, proceed to admin/config/cloud-api/configure and enter your
Acquia Cloud API Credentials
2) Access Cloud Dashboard from admin/config/cloud-api/view
3) Links on Cloud Dashboard are contextual based on the section and self-
explanatory

**FAQ**
How does it work?
Once the module is enabled, an "Acquia CLoud Report" page is made available 
which gives complete details about the Sites you have got, the various 
environments on your subscription, the databases, the servers etc. The report 
also allows you to manage various aspects of your Acquia Hosting (ex Domains) 
by providing appropriate links and forms.

What can be done through this module?
As of today, the module supports the below, besides providing a comprehensive 
report showing various aspects of your cloud hosting

Add / Delete Domain Names
Purge Varnish Caches
Add / Delete SSH Keys
I can do all that from my Acquia Dashboard, why do I need this module?
In most of the cases, you would not require this module. This module will help 
you if you would like some of your employees or users to be able to do specific 
tasks. If you work with a Vendor or a Contractor and if you do not wish to add 
the contractor as a Technical Contact on your Acquia Hosting (because it gives 
him access to pretty much everything), you can setup a small portal with this 
module, giving him permissions to do specific tasks. (Ex: You would like the 
contractor to be able to add new domains and clear Varnish Caches but you would 
not like him to be able to delete domains accidentally or intentionally).

Access Control is going to be added to Acquia Hosting Dashboard pretty soon. 
And this module might be useless then. But till it happens, this module can help 
you accomplish certain level of access control on your Acquia Hosting.

What do I need to be able to use this module?
You would require your Acquia Cloud API username and password. You can get 
this from your Acquia Cloud Dashboard.

So this module works through Cloud API? Isn't there a cap on the number of API 
calls that can be made to Cloud API?
Yes. This module works using Cloud API. The module caches the report generated 
for 60 mins by default. This duration can be changed from the module settings. 
The module shows when the report was last updated. And any user with the 
permission to manually update the report can do so as and when required.

Is there a way that I can add new databases or manage my SVN users, or backup 
my databases through this module?
As of today, the module is limited to managing domains, varnish caches and 
SVN users and the report. If you would like new things to be added, log it in 
the issue queue. I would be glad to take that up.
