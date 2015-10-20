Overview
================================================================================

This module assists in making Services responses comply with the [White House
API Standards](https://github.com/WhiteHouse/api-standards).

* By default, it will modify all responses to include metadata in accordance 
  with said standards
* It also supplies services_govformat_format(), which can be used by services
  resource callbacks to return data in a compliant format.
  
For example:
````
  return services_govformat_format(array(
    'results' => $nodes,
    'pagesize' => $page_size,
    'count' => $count,
  ));
````

#### Dependencies

* services
