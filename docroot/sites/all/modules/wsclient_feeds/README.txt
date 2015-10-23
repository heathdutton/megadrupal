This module provides a Feeds Fetcher that consumes web services, and a 
mapper+processor that helps translate the responses into Feeds-compatible 
Drupal data.


Usage
=====

You must first set up and establish a connection to your web service using 
WSClient. 
Enable "Web service client UI" and find it at admin/config/services/wsclient
See WSClient docs.

Indentify or prepare the content type or entity you will be creating data into.
You should add fields to match the data schema of the content you are 
interested in.

Create a Feeds Importer ( /admin/structure/feeds/ ) 
and set import or processor options as needed.

Choose "Web Service Fetcher" as the Fetcher. Adjust its 'Settings'

Choose your services operation, and optionally the default values for its 
expected parameters if any. 
The parameters are defined in the WSCLient definition, and may have been 
defined by the Web Service Definition (WSDL)

Choose "Web Service Parser" as the Parser. Adjust its 'Settings'

>You can, if you prefer, use XPath Parser as the parser, but if you use the
"Web Service Parser", the data mappings of the response can be better
automated.

If the expected response packet is structured or deep, you may have to specify
the element that will be parsed into data. This will usually be the element
that is one of a list, eg 'item's in an RSS feed.

Ensure your "Processor" is the correct entity type (eg "Node") 
and the settings are set to the right content type.

In the 'Mappings', each of the expected values in the expected response
can be listed as a 'source.

Demo
----

A fully configured demo configuration is supplied as a 'Feature'.
If you enable 'Features' and the 'wsclient_feeds_demo' (Football games)
then several Content Types and Feeds Importers will be added to your site.
( Only do this on a throw-away tester site )

It has a number of common requirements (entityreference, date), and a rare one: 
  feeds_tamper_string2id

The three Feeds importers will create, populate or link data from a public
web service that returns World Cup Football results.
http://footballpool.dataaccess.eu/data/info.wso?WSDL

It's sort of important to run the imports in the following order.

Simple:
"Get Football Teams" will retrieve a list of "Teams" and their logos,
creating a node for each. It will not include the team roster.

Linked:
"Get Football Games" will create an entry for each match, and demonstrates 
how entityreference (team vs team) and taxonomy can be used on the imported 
data.

Parameterized:
"Get Full Team Info" demonstrates the use of an argument.
Running an import on this webservice call requires you to enter the name
of an individual team (Country Name). The response is just one data entry
at a time, and it uses feeds to update individual team nodes with additional 
data.


Reference
---------

Some sample web services this has been tested against are listed here:
http://www.xmethods.net/ve2/index.po
