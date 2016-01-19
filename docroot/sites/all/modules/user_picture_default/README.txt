User Picture Default

This module forces the picture field on the user, to be set to the default
image, configured in the account settings.

By using this module, a user picture is always available to any
module/function that handles entities, because the file reference is set as a
property on the entity.

Drupal's default behavior is that the code which handles a user entity must
check the user_picture_default variable but this is not as flexible, since it
does not populate a field with an image etc.

All you have to to is enable the module, set the path to any default user
picture, in admin/config/people/accounts and enjoy.

The picture can be included in a theme/module or just be uploaded in a file
field.
