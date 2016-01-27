********************************************************************
           S T A T I S T I C S    A J A X    M O D U L E
********************************************************************
Original Author: Sean Hamlin
Current Maintainers: Sean Hamlin

********************************************************************
DESCRIPTION:

   This module provides a way to programmatically update the {node_counter}
   table (provided by the core statistics module).

   This module listens to a URL in the format of '/statistics/ajax/[nid]'
   where [nid] is the id of the node you are looking to update the statistics
   for

   The response is return in the JSON format:

   Example success message for a get:
   {"status":"success","totalcount":"3","daycount":"3"}

   Example success message for a update:
   { "status": "success", "data": "node updated" }

   Example error message:
   { "status": "error", "data": "POST is not allowed" }

********************************************************************
SAMPLE CODE TO GET THE CURRENT STATISTICS:

   Sample javascript code (using jQuery):

   // TODO, remove the hardcoded nid
   var nid = 52;
   // fire AJAX
   jQuery.ajax({
     url: "/statistics_ajax/get/" + nid,
     type: "POST",
     dataTypeString: "text"
   });

SAMPLE CODE TO UPDATE THE CURRENT STATISTICS:

   Sample javascript code (using jQuery):

   // TODO, remove the hardcoded nid
   var nid = 52;
   // fire AJAX
   jQuery.ajax({
     url: "/statistics_ajax/update/" + nid,
     type: "POST",
     dataTypeString: "text"
   });

********************************************************************
PERMISSIONS:

   This module defines the "use statistics_ajax" and "administer statistics_ajax"
   permissions. The "use statistics_ajax" permission determines whether a user will
   be able to update the database when the appropriate URL is called.  The
   "administer statistics_ajax" permission determines whether a user will be able to
   edit the "Statistics AJAX" administration pages.

********************************************************************
INSTALLATION:

1. Place the entire statistics_ajax directory into your Drupal modules/
   directory or the sites modules directory (eg site/default/modules)


2. Enable this module by navigating to:

     Administer > Build > Modules


3. Configure this module by navigating to:

     Administer > Site Configuration > Statistics AJAX settings

     Here there are options to:

     a) Allow ONLY xmlhttprequest method (AJAX)

         Restricts the node counter statistics update to AJAX
         xmlhttprequests (through javascript)

     b) Allow HTTP GET method

         Allows HTTP GET requests to update the node counter statistics

     c) Allow HTTP POST method

         Allows HTTP POST requests to update the node counter statistics


    Note: you will need to enable at least one of GET or POST HTTP requests
    in order to use this module
