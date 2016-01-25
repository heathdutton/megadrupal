Search API location
-------------------

This module adds support for indexing location values provided by the geofield
module and subsequently filtering and/or sorting on them, provided the service
class supports this.

Information for users
---------------------

This module adds a new data type, "Latitude/longitude" to the "Fields" form of
Search API indexes. You can use this to index the "LatLong Pair" property of
fields of type "geofield". (You will first have to add the field under "Add
related fields" at the bottom of the page.)

For this to have any effect, the service class of the index's server has to
support the search_api_data_type_location feature. Currently, only the Search
API Solr module [1] is known to support this feature. There is also a table at
[2] which lists all known features and which service classes support them, but
it might be out of date. When in doubt, consult the documentation of your
service class.

[1] http://drupal.org/project/search_api_solr
[2] http://drupal.org/node/1254698

To make real use of this module, and of data indexes as "Latitude/longitude",
you will need to install additional modules. Two of them are included in this
project: "Search API location views" and "Search API location pages". These add
location search capabilities to the "Search API views" and the "Search API
pages" module, respectively. Please refer to their own README.txt files for
details.

Information for developers
--------------------------

All provided hooks are listed in the search_api_location.api.php file.
Documentation on the CTools plugin defined by this module, location_input, is
also available there.

This module defines a feature, "search_api_data_type_location". By supporting
the feature with your service class, you indicate that you can index the
"location" data type in a useful manner. The data type is defined as a latitude
and longitude, in decimal degrees, separated by a comma, and has string format.
E.g., Dries' place of birth would be represented as: "51.16831,4.394287".

In addition to being able to index locations, you should also recognize the
"search_api_location" search query option, which is defined as follows:

The option is an array, where each value is an array that defines one set of
location data for the query and has the following structure:
- field: The Search API field identifier of the location field. Must be indexed
  as type "location".
- lat: The latitude of the point on which this location parameter is centered.
- lon: The longitude of that point.
- radius: (optional) If results should be filtered according to their distance
  to the point, the maximum distance at which a point should be included (in
  kilometers).
- method: (optional) The method to use for filtering. This is backend-specific
  and not guaranteed to have an effect. Service classes should ignore values of
  this option which they don't recognize.
- distance: (optional) If set to TRUE, the distance to the given point is
  returned in an additional field in the result. The name of the additional
  field consists of the location field's identifier and the suffix "_distance".
  Defaults to FALSE.
Even when no radius is set, service classes can choose to use the location data,
e.g., for sorting results or adapting facets on the field.
