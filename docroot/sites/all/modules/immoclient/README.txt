INTRODUCTION
------------
This module creates a content type called immoclient. Immoclient is built after 
the official German "openimmo" xml-standard to create real estates.
This is used in German speaking countries (Germany, Austria, Swiss) to create 
and transfer these so called "objects".

What this module does:
It creates about 300 fields per click and gives an easy to use UI to manage real 
estates, used by real-estate agents in German speaking countries.


REQUIREMENTS
------------
This module requires the following modules:
 * Entity API (https://www.drupal.org/project/entity)
 * Inline Entity Form (https://www.drupal.org/project/inline_entity_form)
 * Date (https://www.drupal.org/project/date)
 * Format Number API (https://www.drupal.org/project/format_number)
 * Views (https://drupal.org/project/views)
 * Entity Translation (https://www.drupal.org/project/entity_translation)
 * Entity reference (https://www.drupal.org/project/entityreference)
 * Field Group (https://www.drupal.org/project/field_group)
 * Location (https://www.drupal.org/project/location)
 * GMap Module (https://www.drupal.org/project/gmap)
 * Title (https://www.drupal.org/project/title)



RECOMMENDED MODULES
-------------------
 * Administration menu (https://www.drupal.org/project/admin_menu)


INSTALLATION
------------
Best practice is to work with drush! Another issue is that a plain installation 
in default English makes no sense for this module. For serious practicing you 
should have an installation with German language pack as default. 
The reason is to support the ability to transfer contents written in German, 
although you can test this in English too.

In drush you only need to download and enable just immoclient, then because of 
the dependencies the rest is token.

Without drush only enable immoclient.

Configure the modules as written in configuration.
 
After you configured and inserted some real estate objects you have to build 
your views, because of the huge amount of fields, and some of them are only for 
the agencies itself, there is no way to show these nodes any guests.


CONFIGURATION
-------------
 * Activate clean urls

 * Configure immoclient (admin/config/content/immoclient/refnumb):
   - The object reference numbers should be created automatically to be sure 
     they are unique. 
 * Configure entity translation (admin/config/regional/entity_translation)
   - Be sure to activate all submodules of immoclient.
    
 * Configure the language (admin/config/regional/language/configure) as follows: 
   - URL, USER, SESSION
 
 * You should replace the title with the title module
    (de/admin/structure/types/manage/immoclient/fields)
    This is only needed if you want a multi language site as recommended.
    
 * Configure gmap (de/admin/config/services/gmap)
   - In combination with location gmap is used to create the address.

 * Configure location (de/admin/config/content/location)

 * Configure Format number API (de/admin/config/regional/format_number)
   - Dependant on your default language chose the appropriate signs.
    
 * Create your imprint (admin/de/impressum) 
   - There are a lot of fields that are demanded by openimmo.xml.
 
 * Create at least one contact. 
   - Salutation, name and telefone can be enough!

 * Configure user permissions in Administration » People » Permissions:
   - Immoclient is a node, so give the roles the appropriate permissions.   
   - If you have a complex role model, choose which role can administer contacts,
     customers and the imprint. To create nodes of type "immoclient" they have 
     to have this right. 
   - The same is for the submodules.
   - To create nodes of immoclient, you have to give roles the administer 
     permissions for immo price and immo customer. 
   - To view these nodes they need 'view price' permission.
   - If, as recommended, you have the admin_menu and the toolbar activated, the 
     roles with access to toolbar and the admin_menu will only see the admin 
     menu, because it's more powerfull. 

MAINTAINERS
-----------
Current maintainers:
 * Marc Endres (maen) - https://drupal.org/user/559328
