/*
  Custom CKEditor configurations
*/

CKEDITOR.editorConfig = function( config )
{
  // config.styleSet is an array of objects that define each style available
  // in the font styles tool in the ckeditor toolbar
  config.stylesSet =
  [
    /* Block Styles */

    // Each style is an object whose properties define how it is displayed
    // in the dropdown, as well as what it outputs as html into the editor
    // text area.
    { name : 'Paragraph'   , element : 'p' },
    { name : 'Heading 2'   , element : 'h2' },
    { name : 'Heading 3'   , element : 'h3' },
    { name : 'Heading 4'   , element : 'h4' },
    { name : 'Float Right', element : 'div', attributes : { 'style' : 'float:right;' } },
    { name : 'Float Left', element : 'div', attributes : { 'style' : 'float:left;' } },
    { name : 'Preformatted Text', element : 'pre' },
  ];

}

// When opening a dialog, a "definition" is created for it, for
// each editor instance. The "dialogDefinition" event is then
// fired. We can use this event to make customizations to the
// definition of existing dialogs.
CKEDITOR.on( 'dialogDefinition', function( event )
  {
    // Take the dialog name and its definition from the event
    // data.
    var dialogName = event.data.name;
    var dialogDefinition = event.data.definition;

    // Check if the definition is from the dialog we're
    // interested on (the "table" dialog).
    if ( dialogName == 'table' )
    {
      // Get a reference to the "Table Properties" tab.
      var infoTab = dialogDefinition.getContents( 'info' );
      // Get a reference to the border size field
      var borderSizeField = infoTab.get("txtBorder");
      // Set the border to 0 (who uses html table borders anyway?)
      borderSizeField['default'] = 0;
    }
  });
