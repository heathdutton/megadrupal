*** Purpose ***

A tiny module for localized web sites, allowing users to access content
in another languages than current user's by path alias.

*** How it works ***

By setting newly created and updated path for nodes, taxonomy and users to
language neutral.
Previously created paths can be bulk updated at module's configuration page.
It is also possible to revert changes to nodes paths made by this module.

*** Use case ***

Different users may have different language preferences.
For example, not everyone likes translated administration pages.
However, this creates an inconvenience for these users as all automatically
generated path aliases are invisible for them, they see regular /node/xxx.

*** Installation & configuration ***

Install module as usual. 

After installation go to the configuration page
(admin/config/search/path/language_settings).

Statistics for path alias languages are displayed.
One can also reset all the aliases referring to nodes, taxonomy terms and
users to be language neutral. The language of the node path aliases can be set
to match the language of the node (not implemented for other entity types).
The module can be configured to automatically reset language settings
for newly created nodes, terms and users.
