Energy Dashboards w/ Drupal & BACnet
====================================

https://www.drupal.org/sandbox/dbt102/2237027

-- SUMMARY --

This BACnet module for Drupal turns web sites created by Drupal into "e-boards" 
(short for Energy Dash Boards).  
It is an energy management tool, 
allowing easy real-time monitoring of energy related data 
across a team within an intranet or other Drupal website.  

BACnet provides external library handling for Drupal modules.

For a full description visit the project page. 
( https://www.drupal.org/sandbox/dbt102/2237027 )

Bug reports, feature suggestions and latest developments:
( https://www.drupal.org/project/issues/2237027?categories=All )

Visit the demonstration site to see what this module does  
and to get involved 
with this BACnet-Drupal User Group. 
( http://bg-drupal.org )

-- REQUIREMENTS --

* Drupal 7.x
* Block

AND access to a ...

* A BACnet Advanced Operator Workstation (B-AWS) 
(http://www.bacnetinternational.net/btl/)

that implements BACnet Web Services-1 
(see http://www.bacnet.org/WG/XML/index.html) 

as specified in 
Standard 135-2012
 -- BACnet®-- 
 A Data Communication Protocol for Building Automation 
 and Control Networks 
(see https://www.ashrae.org/resources--publications/bookstore/bacnet)

Following are in dev and will also be required:
* Chaos tool suite (ctools)
* Views
* Libraries 2.0
* Flots 0.7 library 
http://www.flotcharts.org



-- INSTALLATION --

* Install as usual, see 
https://www.drupal.org/documentation/install/modules-themes/modules-7 
for further information.
  
*  Note that installing external libraries is 
separate from installing this module 
and should happen in the sites/all/libraries directory. 
See http://drupal.org/node/1440066 for more information.


-- Libraries Installation --
Copy each of the following libraries 
into their respective folder  
(i.e. flot and transform) 
in the libraries folder for the module to detect the libraries correctly
* Flots 0.7 library  
(see https://code.google.com/p/flot/downloads/detail?name=flot-0.7.zip&can=2&q=)
* Jquery transform 0.9.3 library 
( see http://wiki.github.com/heygrady/transform/ ) 


-- CONTACT --

Current maintainers:

* David Thompson (dbt102)
https://www.drupal.org/user/338300



-- SPONSORED BY -- 

* The BACnet Group - Drupal. We are a non-profit organization encouraging 
the successful interactions between BACnet building automation systems 
and Drupal content management systems.

* Reformed Energy - We offers products and services to simplify 
the management of energy usage for all types of facilities.  
Our core products we embed as Energy Dashboards into your existing website.  
We offer these free - at no cost - to our customers.  
Our core services are driven by 
Embedded Audits performed real time on your facilities.  
We develop and implement projects for our customers 
to assist them with reducing the cost to operate their facilities.  
While the bottom line is reforming energy usage, 
we track and coordinate the path there 
thru secure web based energy management tools. 

* Function1 - We believe that technology exists to 
help people put their ideas into action. 
Our consultants work closely with customers to understand their needs 
and augment industry leading software - 
rounding the rough edges, 
improving efficiency, 
and ultimately making it easy and enjoyable to use. 
Our solutions deploy faster, cost less,  
and are far more usable than standard deployments because of this.
