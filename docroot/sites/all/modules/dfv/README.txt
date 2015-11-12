
-- SUMMARY --

This modules enables site admins and developers to make the allowed values
of a field depend on another proviously selected field's value.

You can use views with arguments to let views return the allowed values
for a dependent field.

-- INSTALLATION --

Install as usual. After installation you will have additional field
settings: parent field and views which return allowed values.

-- CONFIGURATION --

Create view with display of type "Dependent field value". Add contextual
filter there by parent field value, for example: if parent field is
entityreference to users, filter will by user_id.
Enable dfv settings for field and select parent field and view.
