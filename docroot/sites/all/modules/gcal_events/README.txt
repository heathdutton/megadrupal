GCal Events Documentation

To install:

1. Download this module
2. Extract this module
3. Upload this module to your website
4. Enable this module

See https://www.drupal.org/documentation/install/modules-themes/modules-7 if you need more help.

To configure:

Get a Google Developer ID key
1. Navigate to https://code.google.com/apis/console.
2. Create a project and turn on the Calendar API (agree to the TOS).
3. Navigate to "Credentials" in the left menu under "APIs & auth".
4. Create a new key (server key) if you don't already have one.
5 .Once it's created, it'll display on that page. Copy the "API KEY" and save it for later.

Get the Google API Client Library for PHP
1. Navigate to https://github.com/google/google-api-php-client and clone the master branch.
2. Rename that cloned directory "google-api-php-client" and move it to sites/all/libraries.

Configure your GCal Events blocks
1. In the module configuration page, set the number of blocks to display.
2. In the block configuration screen, configure block 0.
3. Get the Calendar ID from google calendar and input as the calendar ID.
4. Put the API Key you created into the "Developer ID" field.
5. Still in the block configuration screen, set the block to an active region and save.
6. Also check to make sure you have block caching enabled on the performance settings for your Drupal site.
   This will help keep you below the quota limits for the free API account.

DONE!

You should now see a few upcoming events in the block you configured. You can tweak the title of the block and how the events are displayed in 
the configuration screens.


How to get Google Calendar ID:

1). Log into your Google Calendar.
2). Click on the "Settings" tab.
3). Select "Calendars"
4). Click on the calendar you want to use.
5). In the section labeled "Calendar Address", you should see "Calendar ID". Copy and paste that ID. into the GCal Events configuration.

If you just fill in the Calendar ID, GCal Events will default to using your public calendar. Events marked as private will not show up. If you want to display your private 
calendar (not advised):

1). Under the section marked "Private Address", click on XML
2). You will see a URL that looks something like this:
    http://www.google.com/calendar/feeds/somename%40gmail.com/private-0562849527c0abe35379accccb4ecfd4/basic
3). http://www.google.com/calendar/feeds/[ Celendar ID      ]/private-[PRIVATE ID                    ]/basic
4). For Private ID, you are looking for the section between  "private-"                and           "/basic"



Template Configuration:

One of the exciting features that GCal Events has is templates. Instead of having every event block display the same information about the events, it can display a configurable 
block with a two-level template.

Here's how it works. GCal Events provides you with some simple variables for each event. The title, description, date, time, location, and a URL. (#TITLE#, #DESC#, #DATE#, 
#TIME#, #LOC#, #LOCURL#, #URL#). #LOCURL# is a url-escaped version of the location, for use in making URLs.

The first thing you can do is edit the "Event Template". This is what will show up for each event. Maybe you just want the date and location? Or just the date and title?

The second template level is where the real power and flexibility comes in. You can actually modify each of the variables (Title, Description, Date, Time, Location) BEFORE they 
get filled into the event template. Want to add a <B> tag around the title? Perhaps you want to include a line break? Maybe you want to make the title link to the URL? Or make 
the location be a link to google maps? By default, each of the second-level templates will contain only the variable they represent.

Example Template Configurations:

To get the title to be a link to the event in google calendar:
Title Template: <b><a href="#URL#">#TITLE#</A></b><br>

To get the location to be a link to an MIT map
Location Template: <a href="http://whereis.mit.edu/map-jpg?mapterms=#LOCURL#&mapsearch=go">#LOC#</A><br>

To get the location to be a google map:
Location Template: <a href="http://maps.google.com/?q=#LOCURL#">#LOC#</A>



Timezone Configutation:

On most systems, timezone configuration will not be needed. The "local time" according to the machine hosting your page will be what is used. If the machine is set up using UTC 
or a different timezone, you may need to select an alternate time zone to display the events correctly.


