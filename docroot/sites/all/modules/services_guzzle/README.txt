Goals
==============
- Simple, human-readable, service description that can be read by Guzzle Client Builder
- Easy way to create GuzzleClient instance, just provide .json file or array with guzzle service description
- Drupal 7 backport of this feature http://drupal.org/node/1447736

Installation
==============
- Service module with REST server is required
- Create your endpoint for REST server and go to Guzzle tab

Documentation
==============
- Guzzle documentation: http://guzzlephp.org/
- Guzzle service description: http://guzzlephp.org/guide/service/service_descriptions.html
- Guzzle Client instance building from description: http://guzzlephp.org/tour/building_services.html

Usage
==============
This module just provide valid service description that can used to create Guzzle Client instance.
Try service_guzzle_client module. It shows how you can create you Guzzle client from service description
and provide UI debug tools.