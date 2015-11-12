CMIS Views
------------------
by Ian Norton, ian.norton@alfresco.com
Ajax file browser added by Jeremy French, jeremy.french@alfresco.com

The CMIS Views project aims to build on the drupal CMIS API by allowing Drupal 
admins to save a list of folder contents as a block and then display this at 
runtime.

In addition to creating blocks based on the folder view, it is also possible 
to write your own specific CMIS queries to pull items back by their tags, 
title or any other meta data that is exposed via CMIS.

There are currently 4 themes to choose from to format your list, 
these are bulleted list, paragraphs, table and thumbnail with description, 
last modified and file size.

How To Use
----------
CMIS Views are created from the blocks administration screen (admin/structure/block), choose the 'Add a CMIS view' option to launch the new cmis view dialog.

Using with non Alfresco CMIS repositories
-----------------------------------------
It is my goal to continue committing to both CMIS Views and the CMIS API, to allow the use of CMIS extensions in CMIS views natively in the CMIS API, at present this is achieved using a join query on the titled aspect which will break other repositories. I'm keen to hear from people using other CMIS repositories (Nuxeo, Filenet, Sharepoint etc) as to how these extensions are handled (if they're present at all), then I hope to add the relevant code to the CMIS API module and make CMIS Views vendor agnostic as I believe it should be.

Tested with
-----------
Alfresco Community 4.0.a, 4.0.b & 4.0.c*
Alfresco Enterprise 3.3 & 3.4
Alfresco Community 3.3 & 3.4

*I had problems using the SOLR search subsystem in Alfresco with CMIS views, it seems to choke on the join, so for community 4.0.a through 4.0.c change the line 'index.subsystem.name=solr' in tomcat/shared/classes/alfresco-global.properties to 'index.subsystem.name=lucene' and add 'index.recovery.mode=FULL' restart the alfresco server, check it's working and then remove 'index.recovery.mode=FULL'

The latest version of Alfresco Community can be download for free from http://wiki.alfresco.com/wiki/Download_and_Install_Alfresco 3.4 can be found at http://wiki.alfresco.com/wiki/Community_file_list_3.4.d 

Permissions
-----------
There are two permissions with this module, 'Administer CMIS views' and 
'View CMIS views', these will need to be enable for the relevant user roles.

Settings
--------
You can choose a default theme for CMIS views so that the radio button 
is automatically selected for your chosen theme to help to prevent 
administration errors, these options can be found at admin/config/cmis/views

Credits
-------
Back arrow for browser icon from:
www.iconfinder.com/icondetails/6093/128/arrow_back_green_left_next_return_icon
