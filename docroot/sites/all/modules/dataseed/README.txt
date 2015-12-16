Dataseed
--------

Dataseed is an open platform for interactive data visualisation and analysis.
This module allows you to use Drupal entities as the data source for Dataseed
visualisations through the provided Search API service class. You can setup
visualisations of nodes, users, taxonomies and more - anything that can be
indexed with Search API can be visualised with Dataseed.

For more information about the technical details of Dataseed, see the online
developer documentation: https://getdataseed.com/documentation

Requirements
------------

* Search API - https://drupal.org/project/search_api
* Entity API - https://www.drupal.org/project/entity
* A dataseed account - https://getdataseed.com


Steps to visualise Drupal content within Dataseed
-------------------------------------------------

1. Enable the "Dataseed" and "Dataseed Search API" modules

2. Go to "Configuration" > "System" > "Dataseed"

3. Enter your Dataseed email address and password under "Authentication
   Credentials". You can leave the other details as they are unless you have a
   privately hosted custom version of Dataseed.

4. Go to "Configuration" > "Search and metadata" > "Search API"

5. Create a new server by clicking "Add server"

6. Name the server "Dataseed" and set "Service class" to "Dataseed service".
   NB:  It's really important you name the server "Dataseed".  

7. After creating the server, create an index (a dataset in Dataseed) by
   clicking "Add index"

8. Setup the index as you would any other, ensuring that "Server" is set to
   "Dataseed".  Do not change the contents of the "Default Visualisations" field 
   unless you know what you're doing - this is there to create an empty visualisation 
   in Dataseed for you. 

9. Select the fields that you want to use in your visualisation. 

10. Go go the newly created index's "Status" tab

11. Click "Index now" to create the observations in Dataseed. You must have some
    content in your Drupal site for the index button to appear! To ensure that
    content added in the future gets indexed in Dataseed make sure cron is
    working.

12. The index should show up as a Dataset within your Dataseed account.  You should
   be able to click on "Visualise" to see an empty visualisation that you can start
   adding elements to.


Troubleshooting
---------------
Here are some issues you may encounter, along with workarounds:

1.  "No data appears to be indexed and there are errors when deleting the index."
   Ensure that there are no special characters in the index name as Dataseed will slugify
   them and then the Dataseed dataset id will not match the Drupal index id.  This includes
   underscores - avoid them as a workaround.

2.  "My dataset in Dataseed was deleted".  Ensure you choose a name for your index that does 
   not match an existing dataset in your Dataseed account - in case there is a match this dataset
   will be deleted and recreated every time Drupal re-creates the index.

3.  "No visualisation is created in Dataseed".  Ensure that you have not edited the 
    json in the "Default Visualisations" textarea on the index add/edit page.

