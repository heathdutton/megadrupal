(function($) {
      
  Drupal.behaviors.fileFieldSourcesScald = {};

  Drupal.behaviors.fileFieldSourcesScald.attach = function(context, settings) {

    // Open the sidebar when the Scald File soure link is clicked if it is not already open.
    $('a.filefield-source').click(function() {
      if ($(this).hasClass('filefield-source-scald') && !$('.dnd-library-wrapper').hasClass('library-on')) {
        $('.scald-anchor').click();
        $(this).closest('.form-managed-file').find('input[name*="filefield_scald"]').focus();
      }
    });

    var clearHint = function() {
      // Ensure the FileField Sources hint text is cleared out when someone clicks the
      // Insert link in the sidebar or double-clicks a thumbnail to insert the SAS markup.
      if ($('input[type="text"]:focus').length) {
        Drupal.fileFieldSources.removeHintText();
        var sid = $(this).data('atom-id');
        Drupal.dnd.insertAtom(sid);
      }
    };

    $('.dnd-library-wrapper ._insert a').click(clearHint);
    $('.dnd-library-wrapper img').dblclick(clearHint);
    $('.dnd-library-wrapper img').bind('dragstart', function() {
      Drupal.fileFieldSources.removeHintText();
    });

  };

})(jQuery);