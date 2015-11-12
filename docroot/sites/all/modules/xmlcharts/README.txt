INTRODUCTION
------------
XML Charts is a Drupal module that consumes, caches and produces blocks 
to display industrial and precious metal prices from the XMLCharts 
free XML price feeds service. 


INSTALLATION
------------

1) Install and activate this module

2) Navigate to Structure -> Blocks and enable the blocks that you
   would like to use in your site.

3) Visit the configuration page for each block to adjust the title,
   units of measurement, and link to XML Charts visibility.

4) Set a crontab schedule to run cron just after 10.30 a.m. and after 
   03.00 p.m. GMT in order to get the most up to date pricing data.


Once installed, you will be able to place one or more of the precious or 
industrial metal prices on your site in any of the supported currencies.
At this time there are 9 metals and 13 currencies. 

The feed is acquired and cached within the Drupal host site and refreshed 
twice a day; after 10.30 a.m. and after 03.00 p.m. GMT. Set a crontab schedule 
to run cron just after 10.30 a.m. and after 03.00 p.m. GMT in order to get the 
most up to date pricing data.


REQUIREMENTS
------------
The XML Charts service is free. However, if you are using 
it you have to set the links to their service. According to their terms of 
service, they "don't care where or how visible your link is but it has to 
be set". 

The warning in their terms of service states that if they catch people using 
their service without linking to them, that website's IP address will be 
permanently banned. In order to comply with the XML Charts terms of service 
this XML Charts module provides hyperlinks to the appropriate XMLS Charts 
feed page. The visibility of the links can be set in the block configuration 
page.

XML Charts terms of service: http://www.xmlcharts.com/terms-of-service.html


THEMING
-------
The blocks that are produced by the XML Charts module contain CSS classes to 
assist front end developers in theming the data to suit their website needs. 
The classes are as follows:
 * xml-chart-item - wraps the entire text string
 * xml-chart-type - wraps the name of the commodity
 * xml-chart-currency - wraps the currency
 * xml-chart-units - wraps the units of measure
 * xml-chart-value - warps the value including the currency symbol


MAINTAINERS
-----------
Current maintainer:
 * Andrew Wasson (awasson) - https://drupal.org/user/127091


DISCLAIMER
----------
I don't know the owners of the XML Charts Free feed service 
and have only developed this module because I have found it useful. 

I hope others find this module useful and easy to use.
