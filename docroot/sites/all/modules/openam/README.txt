The OpenAM project aims to provide a bridge between Drupal and OpenAM.

How does it work ?
If the OpenAM access is activated, the user connection block is hidden to avoid drupal user connection.
When a user wants to connect and access to the "/user" page, he is automatically redirected to the OpenAM server URL to login.

Once logged, the user automatically returned in the website and the module create the user (and define it as "external") or connect the user if exists.
User informations are update according to the mapping made on the configuration page, between the Drupal user fields and the OpenAM attributes.

Finally, the user is placed in roles, if roles have been created on the Drupal platform.

How install the module ?
  - Copy the module into the sites/[SITE]/modules/contrib directory
  - Activate the module
  - Configure access and attributes mapping on admin/config/services/openam