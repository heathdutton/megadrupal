Search API location views
-------------------------

This module adds location search capabilities to Search API views. It adds both
filters and contextual filters for filtering by proximity to a specified
location.

* Filters

When you're creating a filter on a field that is indexed as "Latitude/longitude"
(and the server's service class supports location searches) the filter will
change to allow filtering based on proximity to a specified location. The
location can be input as text and will be converted to a physical location using
the Geocoder module. As the operator, a radius can be selected, which can also
be exposed to the user like for normal filters. In addition, you have the option
to make the radius a select box instead of a text field.

* Contextual filters

There are three contextual filters:
- LonLat: Here you can fix the radius for the search distance filter. The
  central point argument is then expected as: "LAT,LON", where LAT and LON are
  the decimal latitude and longitude, respectively, of the location. If you have
  the geoPHP module installed, additional accepted formats are: WKT, geoJSON or
  any other format geoPHP accepts.
- LonLat Point: This filter accepts the point for the centre of the search
  distance filter. The same formats as above can be used. If you use this filter
  you must also use the radius filter.
- LonLat Radius: This filter accepts a radius (in km) for the search distance
  filter. This must be used with the point filter.
