
--------------------------------------------------------------------------------
Smart Import
--------------------------------------------------------------------------------

Maintainers:
* Shamsh Tabrij (SSE, CyberNetikz), tabrij@cybernetikz.com

This modules is designed to help import node data from xls file

Installation
-------------

* Copy the whole smart_import directory to your modules directory and
activate the module.

Usage
-----

* Go to /admin/content/smart-import for import data.

hooks
-----

#1 [MODULE_NAME]_smart_import_row_alter hook is defined for any incovenient
field prepare, this module used entity_metadata_wrapper function to create
a node.

implementation:
function my_module_smart_import_row_alter($data, &$ewrapper)
