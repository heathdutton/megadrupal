Belgian eID Mellon integration modules
======================================

beidmellon_fedid:
  Defines a FedID field (to be used for users).
  
  Additionally, if users with a FedID need to be imported from a .csv file, the
  beidmellon_fedict module includes field processor hooks for the user_import
  module (requires versions 2.2 or higher of this module).

beidmellon_auth:
  Login and registration functionality. Expects Mellon to return a FedID, given
  name and surname in order to authenticate a user. The server environment
  variables that contain this information can be set up through the following
  Drupal variables:

  - beidmellon_environment_givenname
  - beidmellon_environment_surname
  - beidmellon_environment_fedid
