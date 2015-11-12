This module provides a field formatter that will display the contents of
an uploaded file as a table. This allows you easily update table data
displayed on a site without importing as nodes or another entity. This
module currently only supports CSV-formatted files.

This module can optionally be used with the DataTables module
(https://drupal.org/project/datatables) to provide javascript client-side
sorting of the generated tables.

## Installation

1. Copy the file_table_formatter module to sites/all/modules directory.
2. Enable the module at Administer >> Modules.

## Usage

1. To configure the table formatter on a content type, go to
   Administer >> Structure >> Content Types.
2. Add a file field to the content type, if necessary, under Manage
   Fields next to a specific content type.
3. Click Manage Display next to the content type with your file field.
4. Select "Display file contents as a table" from the Format box next
   to the file field.
5. If your files have a header row, enable this feature by clicking
   the gear icon and checking the box.
6. Click Save at the bottom of the page.
7. Create a node of the selected type and upload a CSV-formatted file
   to the field you set the formatter on. When viewing the node, you
   should see tables displayed in place of file links.
8. To add titles to the tables, use the file Description field.

To enable javascript sorting of tables:

1. Download and install the DataTables module.
2. Go to Manage Display for the content type with the file field you
   want to format.
3. Click the gear next the field and check the "Use javascript sortable
   data tables" box.
4. Click Save at the bottom of the page.

NOTE: DataTables tables with this formatter default to disabling pagination
and a default sorting column. To enable these, override the
theme_file_table_formatter_table() function in your theme.
