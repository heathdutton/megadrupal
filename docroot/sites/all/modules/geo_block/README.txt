About
------------
Creating blocks, the information to be displayed based on the current 
location of the user.

For example, for users in London can display the same information, 
but for visitors from New York is totally different.

Available geoservices:

ipgeobase.ru - for Russian cities. Return cyrillic names.
hostip.info - for EU.
Tell me if you know of a similar free service to the provision of the API, 
the feature will be added to the module.

It's important! Name cities are sensitive to the fact that you are using and 
the fact that the service returns.

Installation
------------

1. Copy the folder with the module in the directory /sites/all/modules/
2. Enable module Geo Block

How to
------------
To configure the module, go to Aders admin/structure/block/gb_configure. 
To work directly with a functional module, you have to go to the page 
dmin/structure/block and go to the desired power setting, or create a new one. 
(Geo block only works with blocks created with the help of a standard module 
block, for example, blocks of views will not have the required functions) 
The visibility settings there is a tab Geo Settings, where you can add content 
to the map of different cities.
* City - a city for which to display customized content below.
* Block title and block body identical to the original settings.
* Add one more - adds settings to another city.
* Remove one - removes the last town in the list.

If the city will not be identified or is not configured for it, then display 
the information from the default settings of the block.

Examples of results
--------------------
You can see the form in which gives information about the city you vybravnny 
service.
ipgeobase:
* http://ipgeobase.ru:7020/geo?ip=IP_ADDRESS
hostip:
* http://api.hostip.info/get_json.php?ip=IP_ADDRESS

Additional information
-------------------------
I would be very grateful for any help in the translation module to the "
correct" and understandable English.

Author
------
Nikita Malyshev
niklanrus@gmail.com
http://niklan.net/
