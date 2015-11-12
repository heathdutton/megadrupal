********************************************************************
D R U P A L    M O D U L E
********************************************************************
Name: Feeds BrightTALK Module
Author: the greenman
Sponsor: Code Positive [www.codepositive.com]
Drupal: 7.0.x
********************************************************************
DESCRIPTION:

The Feeds BrightTALK module provides a custom feed parser. This
parser makes it very simple to map BrightTALK feed data onto Drupal
fields.

You can set up your own content type and fields to map feeds onto,
or more simply enable the BrightTALK Channel module which will
automatically create and map these for you.


********************************************************************
DEPENDENCIES:

Feeds module
http://drupal.org/project/feeds

Views module
http://drupal.org/project/views



********************************************************************
INSTALLATION:

    Note: It is assumed that you have Drupal up and running.  Be sure to
    check the Drupal web site if you need assistance.  If you run into
    problems, you should always read the INSTALL.txt that comes with the
    Drupal package and read the online documentation.


	1. Place the entire module directory into your Drupal
            modules/directory.

	2. Enable the module by navigating to:

	    Administer > Site building > Modules

            Optionally, also enable the BrightTALK Channel module.

	   Click the 'Save configuration' button at the bottom to commit your
           changes.



********************************************************************
CONFIGURATION:

If you've enabled the BrightTALK Channel module you can skip these configuration steps.

1. Create a content type to use for channels and add fields.

2. Create a content type for webcasts and add fields.

3. Create a feed importer at admin/structure/feeds

    Under 'Basic settings' - set 'Attach to content type' to your channel content type.

    Under 'Processor' - 'Settings' - set 'Content type' to your webcast content type.

    Under 'Processor' - 'Mapping' - match Source fields to Target fields of your webcast content type.

    Webcast ID               GUID

     Author                      Author

     Channel ID               Channel ID

     Webcast ID               Communication ID

      Duration                   Duration

      Summary                  Summary

      Tags                          Tags

      Start date and time   Start: Start

      Webcast image         Image URL

      Webcast status         Status

      Webcast title            Title

      Webcast url              Url URL



********************************************************************
USAGE:

Create a BrightTALK Channel node with these settings

     Title
     ---------------------------------------------------------------
     Set to title of the channel.

     Overview
     ---------------------------------------------------------------
     A description of the channel.

     Feed URL
     ---------------------------------------------------------------
     The URL of the channel's feed.

     Example: http://www.brighttalk.com/channel/43/feed

     Channel ID
     ---------------------------------------------------------------
     Will be filled in automatically.

     Channel Homepage URL
     ---------------------------------------------------------------
     Will be filled in automatically.

     Channel Description
     ---------------------------------------------------------------
     Will be filled in automatically.

     Channel Title
     ---------------------------------------------------------------
     Will be filled in automatically.


Once a Channel node has been created Webcast nodes featuring
BrightTALK webcasts will be created automatically.



