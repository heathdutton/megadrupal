-- SUMMARY --

A very minimal D7 Postal Code field with validation for one country
(listed below) or a combination of countries.

Countries with validation:

    USA
    Canada
    UK
    Germany
    France
    Italy
    Australia
    Netherlands
    Spain
    Denmark
    Sweden
    Belgium
    India

To configure this module, navigate to admin/structure/postal_code.
Selecting countries in the 'Valid "Any" Countries" list will validate the
submitted postal code against regexes for those countries using
the "Any Country" widget type. If you want submissions validated,
make sure to check the "Validate" checkbox.

To configure content types to add this field type,
navigate to admin/structure/types. Select "manage fields"
beside the content type (eg: blog, page, article...) and follow
the normal procedure to add a new field, choosing "Postal Code"
under field type. Beneath "Widget" a number of selections will appear
for each country type, as well as an "Any" country which is configurable
(see above) to validate any included country's postal code.

Thanks to Pixel Envision and Geeks With Blogs for the list of
countries and Classic Graphics for the time to complete it.

Difference from postal_code_validation Module
My module - postal_code provides a feature to create a field of field type
postal code with different Field API widgets such as 'Postal Code:Any Format,
Postal Code:Canada Format etc'. And there is a
configuration form(admin/structure/postal_code) where we select countries to be
validated if selected widget type is 'Postal Code: Any Format'.
Whereas postal_code_validation Module provides a validation function which need
to be called on #validate attribute of a custom field when building a custom
module with country code and data as parameters.

-- REQUIREMENTS --

field_ui,field,options Module (in drupal core) must be enabled.


-- INSTALLATION --

See the installation guide here - http://drupal.org/node/70151
