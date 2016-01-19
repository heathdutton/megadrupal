
ABOUT OSM Route
---------------

OSM (OpenStreetMap) Route enables routing calculation from a list of 
georeferenced entities using the YOURS REST APIs 
(http://wiki.openstreetmap.org/wiki/YOURS).

A typical usage scenario for this module is building automatically a path 
for a node itinerary through its list of referenced POIs. 
The retrieved path is written on a desired geofield and can be used to display 
the itinerary as a map layer (through e.g., the openlayers module).

The OSM Route Module also provides the automatic driving directions retrieval,
both as a long-text multi field autofill, and in form of an on-fly rendered 
block.

As reported in OpenStreetMap Wiki, the Gosmore Routing Engine is not designed 
for generating very long routes (> 200km).
It is suggested to use the module for shortest path not to experience too heavy 
computational lags.

USAGE
-----

To work with OSM Route you need to have this configuration:
- A content type (e.g., POI) containing a field of type geofield 
  (e.g., field_waypoints), which can represent the waypoints on the path.
- A content type (e.g., Itinerary) containing (1) an entity reference to the POI
  and (2) a field of type geofield (e.g., field_path) where to store the 
  retrieved path (3, optional) a long text field where to save the driving 
  instructions.

These are the configuration steps:
1) Create/configure the POI content type/entity (e.g., POI):
   Create a field of type geofield (e.g., field_waypoints) to host the 
   waypoints of the path. The field can be multi-value; in this case OSM Route 
   will connect the waypoints inside the entity.
2) Create/configure the Itinerary content type (e.g., Itinerary).
  2.a) Add a field reference (e.g., field_ref_poi) to the POI entities.
       This field must be multi-value if you want to create a route among
       more POIs. Note that field_waypoints can also be multi-value itself.
  2.b) Add the output path field (e.g., field_path) to the itinerary.
       It must be of type geofield, and multi-value to store segments between 
       waypoints. To directly visualize the route (and waypoints if needed)
       you can use a Openlayers widget.
  2.c) Optionally, you can add a long text field (e.g., field_instructions)
       where the module can store the driving instructions.
       This field must be multi-value, since different segments of the path are
       saved into multiple field values, and of type filter text/full html
       since output are html rendered blocks of text.
3) Configure "OSM route" settings (/admin/config/services/osm_route) to let
   the module use these entities and fields, e.g.,
   - Content type with the itinerary: Itinerary
   - Georeferenced content field: field_ref_poi
   - Coordinates field: field_waypoints
   - Name of the field where to save the calculated path: field_path
   - Name of the field where to save driving instructions: field_instructions
   - ...

If everything is configured correctly the path will be built when saving the 
itinerary, ready to be shown on a map. 

NOTES
-----
- If the Itinerary node will be edited, the path and the driving instructions 
    will be updated.
- If a referenced POI is removed, the calculated itinerary and the driving
    instruction fields are kept, being them stored after saving, but 
    the driving instruction block will be updated, being it calculated at 
    runtime. This allows you to choose the behavior you need the most.

CREDITS
-------

This project is sponsored by Infora (http://www.infora.it).
