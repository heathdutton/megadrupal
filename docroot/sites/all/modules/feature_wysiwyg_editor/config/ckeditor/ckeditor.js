CKEDITOR.on( 'dialogDefinition', function( ev ) {
    // Take the dialog name and its definition from the event data.
    var dialogName = ev.data.name;
    var dialogDefinition = ev.data.definition;

    // Check if the definition is from the dialog we're
    // interested in (the 'image' dialog). This dialog name found using DevTools plugin
    if ( dialogName == 'image' ) {
       // Remove the'Advanced' tab from the 'Image' dialog.
       dialogDefinition.removeContents( 'advanced' );

       // Get a reference to the 'Image Info' tab.
       var infoTab = dialogDefinition.getContents( 'info' );

       // Remove unnecessary widgets/elements from the 'Image Info' tab.
       infoTab.remove( 'txtBorder');
       infoTab.remove( 'txtHSpace');
       infoTab.remove( 'txtVSpace');
    }
});