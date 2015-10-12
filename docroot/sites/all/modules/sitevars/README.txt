Module: 'sitevars'
kris@o3world.com
nov 26 '12

Edit a key-value store, accessible from:
Configuration > System > Site variables.

Requires 'administer site configuration' permission.
Variable name converted to lowercase, using only [a-z], [0-9] and [_].
Key-value pairs stored in variable 'sitevars_array' as associative array.

Pass a name to sitevars_get() to retrieve one value; an array to
retrieve several. Call it with no parameters to fetch your entire
key-value store. Pass a string and TRUE to retrieve all variables
whose name begins with a prefix.
