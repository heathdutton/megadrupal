
Eventbrite API for Drupal 7.x
----------------------------
Eventbrite API allows Drupal to integrate with the Eventbrite event booking
service. The module allows the user to pull data from Eventbrite to be displayed
on their Drupal site and allows new events, tickets, organizers and venues to be
uploaded to Eventbrite directly from Drupal.

Installation
------------
Please note, these instructions to not contain full instructions on how to
setup your Eventbrite account. Setting up an eventbrite account is fairly
straight forward, check the eventbrite website, http://www.eventbrite.com/ for
more information.

Eventbrite API can be installed like any other Drupal module -- place it
in the modules directory for your site and enable it on the
'admin/build/modules' page. Ensure you also have these dependent modules
installed: libraries, date_popup, entity and views.

You will also need to download the eventbrite php library from GitHub. This
can be done with Drush using the command "drush eventbrite-php". For manual install
download it from https://github.com/eventbrite/eventbrite.php and unpack it in 
the sites/all/libraries folder. Rename the unpacked folder eventbrite. You should
delete the examples folder in the eventbrite library. Remember that libraries 
are case sensitive, so ensure the file is named in lower case "eventbrite.php".

Create a new account on eventbrite by clicking the signup link in the top right
corner and following the steps. Next, get your API by clicking on your account
name in the top right corner and selecting Account. Next find API User Key in
the left hand menu. Copy your user key and and enter it here
admin/eventbrite/config. Next click on Request an API Key near the bottom of the
screen. Type in all of the required information and click Request Key. Copy the
key and enter it in to the same page as the user key within the module config
page.

Once you're here you can create some test events to start using the module, or
if you already have an eventbrite acount, you're already done.

Using Eventbrite API
-------------------
From the config page, admin/eventbrite/config, once you have entered your keys,
you can click on the "Import all Eventbrite event data" button to import your
events into the Eventbrite API entities. Once you've imported your eventbrite
data, you can view the data by going to the relevant item within the eventbrite
menu. Events can be edited by going to the event list page,
admin/eventbrite/event and clicking the edit link.


Maintainers
-----------

- jamiehollern
- kafmil
