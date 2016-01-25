$Id: README.txt,v 1.2 2011/03/18 14:58:34 2dareis2do$

Description:
------------
This module extends the webform module to easily allow submitting a SalesForce web-to-lead form. This is important for Salesforce users who are not currently subscribed to either their Enterprise, Developer or Unlimited editions.

Dependencies:
------------
Options Element v1.0 +
Webform v3.x +

Installation:
------------
This module is installed like any other drupal module. Firstly make sure your dependenencies are installed correctly. This modules currently works and has been tested with v7.x-1.0-rc1 and Webform v3.x.

Configuration:
------------
At first you will need to login to your SalesForce Account and create a web-to-lead form. This is accessible from Setup>Customize>Leads>Web-to-Lead.

You should now be on the Web-to-Lead Setup page. Simply click on the 'Create web-to-lead button' and select the fields that you would like to have available on your website. Once you have finished hit the 'Generate' button and you should be taken to a page where you can copy the form that salesforce has generated to your webpage. You can determine your Salesforce OID and the names of the fields by looking at the HTML that has been generated.

In order to be able to select the option to have a Webform piece of content submitted to SalesForce, you will need to configure the module to submit to salesforce using your account settings that can be found in the SalesForce generated web-to-lead form. A link to the configuration page can be found under Administration>Site Configuration or you can access it directly using the path 'admin/config/services/salesforce_webform'.

You must enter your Salesforce OID. This can be found in your web-to-lead form you generated earlier. You can also enter your 'Salesforce URL' (if it is different from the default). If your Webform fields have the same names as the SalesForce fields, you do not need to enter any mapping information; those fields will be picked up automatically. For any of your Webform fields that have different names, you will need to enter the Webform field name in the "Key" column and the SalesForce field name in the "Value" column. You can return to this page at any time to add field mappings.

Now you are ready to create your form using Webform as you would normally. Add a new piece of content of type Webform (node/add/webform). After saving the webform, at node/%webform_menu/webform/sfweb2lead_webform you will see a SalesForce Settings fieldset in which you can choose whether to submit this piece of content to SalesForce. If you leave the Lead Source for this Webform blank, it will default to the Title when the form is submitted to SalesForce. Once you click the Save button, you can proceed to create all your fields in the normal fashion for the Webform.

For any previously created Webforms, all you need to do is edit the Webform piece of content and select the option to submit to salesforce. You can do any necessary field mapping through the module configuration form. You don't need to rename any of your existing field names in the Webform if you don't want to.

Thats it. Once you are done, you can submit your form and see if it populates SalesForce.

One tip is to enable the SalesForce debugEmail field in your Webform to allow you to be emailed once the data has been submitted.

Options Elements (Multi-Select):
--------------
This module implements this feature. Be careful to ensure that the values in the SaleForce multivalue fields and Webform multivalue fields are the same.
