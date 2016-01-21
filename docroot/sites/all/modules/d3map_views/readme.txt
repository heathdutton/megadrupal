-- SUMMARY --

The D3Map Views module was created to take content output by a view which contains a state field and allow the content to highlight specific states that have content and makes those states clickable to allow them to filter Views content that is being displayed on the page.

This module is fully code documented and can be enhanced in various ways. Specifically it can be used to filter any content as long as that content has a state field it can use to determine state allocation of content. It is also fully theme-able so the display and colors can be altered if desired.

I created this because I wanted to allow data to be filtered by clicking on corresponding shape regions (in this case states) and wanted a D3 solution to display the SVG map file as it is highly documented and used. My intention was to not make the code dependent on a US shape SVG file only. The module itself is able to be used with any file but due to my current D3 JS code it will need to be altered in the future to fully realize this.

More Technical Notes:
- This module creates a new views display that can be used to render the content of a view
- The map display is using D3 to build an SVG representation of a GeoJSON map file of the US
- Custom territories are added to the map for their representation as well as smaller state areas on the east coast (D3 manipulation)
- This is intended to be added to a page which already contains the desired view of the content and that has a state field that uses "state" as the URL variable to represent the state to filter the content by
- There is a hook built in place to translate non-zip code value fields into zip code fields so that the D3 map can make the association between the desired state field and an actual state on the map
- Clicking a state generates a page click that changes the "state" value in the URL to the desired state representation value of the selected state

For a full description of the module, visit the project page:


To submit bug reports and feature suggestions, or to track changes:



-- REQUIREMENTS --

This module provides integration with Views and as a result Views is required of course.

D3 Library is also required which will include one of the versions of D3.js


-- INSTALLATION --

* Install as usual, see:
http://drupal.org/documentation/install/modules-themes/modules-7

-- CONFIGURATION --

Most of the configuration and settings are done in the view itself.

-- WHAT DOES IT ADD --

  - A new Views display format called "D3 Map" that displays a US States map that corresponds to the data the View is displaying

-- BASIC INSTRUCTIONS --

  - Install the module

  - Add the D3 Library to your site if it's not installed yet:
  your_site_root/sites/all/libraries/d3 (or your sites custom path)

  The d3.js (or other standard D3.js naming conventions) file should be located at:
  your_site_root/sites/all/libraries/d3/d3.js

  See Installing External Libraries documentation:
  https://www.drupal.org/node/1440066

  D3.js can be downloaded here:
  http://d3js.org/

  - (The Rest is done through the Drupal UI)

  - In order to map content to a state a field needs to be created that allows a 
  state to be added to your desired content type. An easy way to do this is add a 
  "List (text)" field type to your desired content type using the "Select list" 
  widget in the "Manage Fields" tab of your desired content type.

  The allowed values setting of this field should be:

  AL|Alabama
  AK|Alaska
  AS|American Samoa
  AZ|Arizona
  AR|Arkansas
  CA|California
  CO|Colorado
  CT|Connecticut
  DE|Delaware
  DC|District Of Columbia
  FM|Federated States Of Micronesia
  FL|Florida
  GA|Georgia
  GU|Guam GU
  HI|Hawaii
  ID|Idaho
  IL|Illinois
  IN|Indiana
  IA|Iowa
  KS|Kansas
  KY|Kentucky
  LA|Louisiana
  ME|Maine
  MH|Marshall Islands
  MD|Maryland
  MA|Massachusetts
  MI|Michigan
  MN|Minnesota
  MS|Mississippi
  MO|Missouri
  MT|Montana
  NE|Nebraska
  NV|Nevada
  NH|New Hampshire
  NJ|New Jersey
  NM|New Mexico
  NY|New York
  NC|North Carolina
  ND|North Dakota
  MP|Northern Mariana Islands
  OH|Ohio
  OK|Oklahoma
  OR|Oregon
  PW|Palau
  PA|Pennsylvania
  PR|Puerto Rico
  RI|Rhode Island
  SC|South Carolina
  SD|South Dakota
  TN|Tennessee
  TX|Texas
  UT|Utah
  VT|Vermont
  VI|Virgin Islands
  VA|Virginia
  WA|Washington
  WV|West Virginia
  WI|Wisconsin
  WY|Wyoming

  You can also use an existing field to force mapping through an alter hook. 
  See hook_d3map_views_alter_shape_data_alter in d3map_views.api.php for an example.

  - Now we will create our view. Navigate to the add new View UI. Create a new View
  using this same content type you added the state field to. This view should use 
  the "D3 map" display format.

  - There is a "Data Source" setting for this display format that is the most important.
  It needs to be set to the state field you added (or you intend to use to alter if 
  using the alter hook method). Be sure the state field is added as a field to the 
  View to make it available for this setting. The other display format settings can 
  be left as is if desired.

  - Be sure the state field is added to the view as indicated in the previous step and 
  revisit that step if needed.

  - This is all that is required to see the map. As long as the mapping works correctly 
  with the state field you should see the states highlighted on the map that have content 
  mapping to them. The next steps to complete this would be to stack this view on a 
  page with another view that displays the actual content.

  IMPORTANT NOTE: The state field should be an exposed filter with an 
  "Filter identifier" of "state" to allow the map to control the exposed filter of 
  other views on the page. This is what allows the map to actual filter the content 
  of another view on the page.

  - Format/Customize the maps display to your liking via CSS and you're done.


-- TROUBLESHOOTING --

* I don't see my map:

  Ensure you have followed all the steps above. Ensure the D3.js path is 
  correct and being included. Ensure your view is actually displaying data 
  by using a different display format temporarily.

* I see the map but no states are highlighted for existing content:

  Ensure the state mapping field setting is set for the views display. If 
  altering an existing field ensure you are setting the state zip code 
  correctly for translating your field's value.

* Selecting a state is not filtering my other view on the page:

  Ensure the other view has the same exposed state field and the field has 
  it's "Filter identifier" set to "state" as well. This gives all views 
  the same exposed filter value to allow them to filter each other.

-- FAQ --

Q: Why should I use this module?

A: There is no module I have found that allows you to quickly add a Map filter 
for content. There is the leaflet module which is great but maps content to 
points and not areas/shapes. It also uses D3.js which is very flexible and 
supported by any browser that understands SVG shapes.


Q: Can I use a different map?

A: Not yet but I built the module to be flexible to allow for this as a future 
enhancement. The map used is in maps/us.json but since the D3.js uses selectors 
that are specific to this map I need to re-structure the D3 to support general 
maps. The module itself is already written to support any map.


Q: How do I customize the look of my Map?

A: Custom CSS is the best way to do this. See the basic CSS in css/d3map.css for 
a baseline and expand from there.


Q: Why do I have to add a state field?

A: It is the most light weight way to collect a state field value without bloating 
the module by requiring a GeoField which wouldn't be utilized effectively.


Q: What browsers are supported by this map?

A: D3 supports so-called “modern” browsers, which generally means everything except IE8 and below. D3 is tested against Firefox, Chrome, Safari, Opera, IE9+, Android and iOS. Parts of D3 may work in older browsers, as the core D3 library has minimal requirements: JavaScript and the W3C DOM API. D3 uses the Selectors API Level 1, but you can preload Sizzle for compatibility. You'll need a modern browser to use SVG and CSS3 Transitions. D3 is not a compatibility layer, so if your browser doesn't support standards, you're out of luck. Sorry!

-- CONTACT --

Current maintainers: * Keenan Holloway (Forum One) -
http://drupal.org/user/1322066

This project has been sponsored by: * Forum One Communicate. Collaborate. 
Change the world. Visit http://forumone.com/ for more information.