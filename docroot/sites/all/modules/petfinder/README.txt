Petfinder Module

Description:
Provides integration points for Petfinder.com API service
http://www.petfinder.com/developers/api-docs
======================================================================

INSTALLATION

1. Install as you would a normal module, there are no external dependencies 
   other than PHP 5.2 minimum due to the use of JSON handling functions 
   (enforced by the module).
2. Configure the API keys required to make calls to the Petfinder API service, 
   located under Configuration > System > Petfinder 
   (admin/config/system/petfinder).  If you don't already have API keys, they 
   can be created by going to http://www.petfinder.com/developers and 
   registering with the service. 

======================================================================

PAGES

There are currently two menu paths defined, one for shelters and one for pets:

petfinder/shelter/<shelter_id>
  This will retrieve a shelter record from the Petfinder service and render it 
  into a page.  There is a template that can be overridden at the theme level 
  called petfinder_shelter.tpl.php, and a preprocess hook defined at 
  template_preprocess_petfinder_shelter().

petfinder/pet/<pet_id>
  This will retrieve a pet record from the Petfinder service and render it 
  into a page.  There is a template that can be overridden at the theme level 
  called petfinder_pet.tpl.php, and a preprocess hook defined at 
  template_preprocess_petfinder_pet().

petfinder/search/shelters
  This will provide a search form, configurable from the module settings page, 
  for searching shelters in the Petfinder.com database.  Override 
  theme_petfinder_shelter_list() to customize the list output, and the same 
  templates and preprocess hooks as used for the shelter record page, where the 
  'page' template variable will be set to FALSE. 

petfinder/search/pets
  This will provide a search form, configurable from the module settings page, 
  for searching pets in the Petfinder.com database.  Override 
  theme_petfinder_pet_list() to customize the list output, and the same 
  templates and preprocess hooks as used for the pet record page, where the 
  'page' template variable will be set to FALSE. 

======================================================================

PUBLIC API

There are also abstracted API calls that allows developers to query the service:

function petfinder_query($method, $args)
  $method     Method from the API docs (e.g. shelter.get, pet.getRandom, etc) 
              found at http://www.petfinder.com/developers/api-docs
  $args       Array of arguments for the API method, in key-val pairs

function petfinder_pet_load($id)
  $id         Petfinder ID for pet record

function petfinder_shelter_load($id)
  $id         Petfinder ID for shelter record

Also see petfinder.api.php for hooks that the module supports.

======================================================================

THEMING

There are also theme hooks defined to allow overrides as well as direct calls:

theme('petfinder_shelter', array('shelter' => $shelter, 'page' => TRUE));
  shelter     Shelter record loaded by petfinder_shelter_load()
  page        TRUE if rendering as a standalone page, FALSE if embedding.  
              Primarily affects how the page title is handled.

theme('petfinder_pet', array('pet' => $pet, 'page' => TRUE));
  pet         Pet record loaded by petfinder_pet_load()
  page        TRUE if rendering as a standalone page, FALSE if embedding.  
              Primarily affects how the page title is handled.

theme('petfinder_shelter_list', array('shelters' => $shelters, 'offset' => 0, 'count' => NULL)
  shelters    Array of shelter records
  offset      Offset into array of shelter records to start rendering
  count       Number of records to render

theme('petfinder_pet_list', array('pets' => $pets, 'offset' => 0, 'count' => NULL)
  pets        Array of pet records
  offset      Offset into array of pet records to start rendering
  count       Number of records to render

======================================================================

PERMISSIONS

In order to prevent abuse of the Petfinder API, the following permissions are 
available and are disabled for all roles by default, except user 1:

administer petfinder
  Allows users to configure the Petfinder module's system settings

access petfinder shelter search
  Allows users to view and search the Petfinder shelter database

access petfinder pet search
  Allows users to view and search the Petfinder pet database

======================================================================

TODO

* Build a companion module using Boxes (http://drupal.org/project/boxes) that 
  can pull in animal and shelter records for use in pages
* Work with Petfinder.com to see if I can add push capabilities to allow sites 
  to upload new animal adoption listings to the service
* Anything else that people suggest!
