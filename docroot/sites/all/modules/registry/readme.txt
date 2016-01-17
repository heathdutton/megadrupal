Registry

  The registry is a key-value store which loads on each request and contains 
  information about the request and other context. It allows modules to set 
  and request information along the execution chain.

  It was inspired by the CMI and WSCCI initiatives for Drupal 8, but attempts 
  to provide a useful feature to Drupal 6. The registry is only for 
  aggregating information about the current request, and is not used for 
  storing information across requests (for this, use Drupal's variable 
  functions, or consider the Object Cache module).