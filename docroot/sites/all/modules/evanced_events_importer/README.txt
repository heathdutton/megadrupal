=======================
Evanced Events Importer
=======================
The Evanced Events Importer module integrates features from Evanced 
Solutions' Events** calendar product in to your Drupal site.

This module imports data from Evanced Events' built-in EXML feed to 
create nodes for each published event.  When the Evanced Events 
calendar is updated in Evanced Events and cron runs in Drupal, 
the module keeps the events in sync using the ID assigned to each event. 
The module also deletes nodes that are unpublished or deleted in the 
Evanced Events calendar, provided they fall within the date range 
covered by the EXML feed.

With this module installed you can continue to use Evanced Events as 
a stand-alone product to create events, manage event registration, 
record attendance statistics, run reports and manage room reservations 
(in conjunction with Room Reserve), but you can also take advantage 
of all the power of Drupal and the Views module to create multiple 
displays of your events across your website.

This module was originally created in 2009 by Worthington Libraries 
for their website (http://www.worthingtonlibraries.org).


==================
Installation notes
==================
Note: This module requires a subscription to Events** by Evanced 
Solutions (http://evancedsolutions.com).

Required modules
----------------
-	Date

Basic configuration
-------------------
This module assumes basic familiarity with Drupal and Views.  For 
information on creating a content type or configuring views, please 
refer to the documentation for those modules.

1.	Administration > Structure > Content types 
	Create a content type for your events.  Required fields: Date, 
	Evanced ID
	-	Add a new field of type Date for the event's start date/time 
		and end date/time (Note: The Date field type has been more thoroughly
		tested than either the Date (ISO format) or Date (Unix timestamp)
		field types.)
		-	Collect an end date: Required
		-	Date attributes to collect: Year + Month + Day + Hour + Minute
		-	Time zone handling: No time zone conversion
	-	Add a new field of type Integer to accommodate the Evanced 
		Events ID
		-	Number of values: 1

2.	Administration > Structure > Taxonomy 
	Create vocabularies that correspond with the Branch Specific Lists 
	and System Wide Lists in Evanced Events.  Suggested vocabularies: 
	Location, Event Type, Age Group
	-	Branches + Locations
		-	Include all of the branches from your System 
			Configuration & Settings
		-	Arranged hierarchically beneath each branch, include all 
			of the rooms from the Location List
	-	Event Type
		Include all the event types from the Event Type List
	-	Age Group
		Include all the age groups from the Age Group List
	Associate each vocabulary with the event content type you created 
	in step 1.

3.	Administration > Configuration > Evanced Event Importer 
	Configure the Evanced Events Importer module.
	-	In the "Main configuration" tab:
		-	Supply the EXML Feed URL from your Evanced Events 
			installation.
		-	Under content type, select the content type you created
			in Step 1.
		-	Under User ID of Publishing Author, enter the Drupal
			user ID of the account that will be credited with 
			authoring the imported content. (Note: Ensure the user ID 
			you select has sufficient permissions to manage your event 
			content type, e.g., create, edit any, delete any.)
		-	Under input format, select the input format that best 
			matches your needs.
		-	Check the checkbox if you wish to strip links from the 
			event description in Evanced Events.
	-	The "XML Mapper" tab allows you to map elements from the 
		Evanced Events EXML feed to the fields of your event content 
		type in Drupal.
		-	Select the Evanced Events element at left and click the 
			button with the right arrow (-->) next to the field you 
			want to map it to.
		-	To see an example of a mapping configuration 
			typically used for event content types, click 
			on the "Suggested Settings" link. 

*IMPORTANT!* Importing events for the very first time
-----------------------------------------------------
When cron is running, the Evanced Events Importer checks to see if 
an existing event in Drupal needs to be updated -- and quickly moves 
past it if it doesn't. The most time-consuming thing the module does 
is create new event nodes. 

With this in mind: the first time you use the Evanced Events Importer, 
you may have *a lot* of new event data to import, which will be 
a time-consuming process. Although Drupal can handle it, most likely, 
if you import all that data at once, you'll hit PHP's Max Execution 
Time limit (i.e., 'max_execution_time' in your php.ini file) before 
cron can finish executing. 

To avoid overtaxing the system during your inaugural import, we 
recommend that you configure your EXML Feed so that it lists only 
a month's worth of data the first time (see "The EXML Feed" below). 
After cron has run successfully, expand the time period to two months 
(or three, if you don't have that many events scheduled in the future). 
Manually run cron again.  Continue to do this until you've imported 
all your desired data. From that point on, you should be able to set 
the Evanced Events Importer to import events up to 365 days in the 
future without causing any problems. NOTE: This may vary depending 
on how many other tasks are hooked into cron on your Drupal 
installation. 


=============
The EXML Feed
=============
Evanced provides various XML feeds for exporting data from its 
Events software. Their EXML feed includes the most robust event 
data, which is why it is the feed of choice for this module. The 
other feeds offered by Evanced (xml, rss2, atom1, ical) are more 
compact and don't include certain fields, such as event end 
times and/or specific categories (Event Types, Age Groups, etc).

Where is the EXML Feed located?
-------------------------------
Based on your installation of Evanced Events, you should be able 
to find the EXML feed here: 

<url_of_your_evanced_installation>/eventsxml.asp?dm=exml&<additional_parameters>
Example: http://www.example.com/evanced/lib/eventsxml.asp?lib=all&nd=30&alltime=1&dm=exml 

EXML Feed: Parameters
---------------------
You can add parameters to the end of the feed URL to control how 
much event data should be displayed. For example, one important 
parameter (nd) limits the number of days included in the EXML feed 
("nd=30" means "show any events coming up in the next 30 days").

You can find a description of all the parameters for configuring the 
feed in the Evanced Knowledgebase. 

-	For Events, visit the page "Creating XML-RSS Feeds," 
	located at http://kb.evanced.info/article.php?id=77 
	(as of December 2015).

-	For SignUp, visit the page "SignUp - Events XML Feed URL Variables," 
	located at http://kb.evanced.info/article.php?id=482 
	(as of December 2015).

Consult your Evanced representative if you need help with accessing or
configuring the EXML feed or Events software. 


============
Registration
============
This module does not include functionality related
to patron registration. You may be interested in the Evanced Registration
module: https://www.drupal.org/project/evanced_registration


=====================
** What about SignUp?
=====================
Evanced Solutions has released SignUp as a successor to its Events product.
Despite the differences between SignUp and Events, the EXML feed remains the 
same.  The Evanced Events Importer is compatible with both products, as tested 
and confirmed by the Drupal community: 
http://drupal.org/node/2188063#comment-9702515


=======
Contact
=======
-	Stefan Langer (slanger) - slanger@worthingtonlibraries.org
-	Kara Reuter (kittysunshine) - kreuter@worthingtonlibraries.org

This module was originally created in 2009 by Worthington Libraries 
for their website (http://www.worthingtonlibraries.org).