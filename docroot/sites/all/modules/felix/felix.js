(function ($) {
  Drupal.behaviors.felix = {
    attach: function(context, settings) {
      if ($('#felix-search-node', context).length == 0) {
        return;
      }
  
      Drupal.felix = {};
      Drupal.felix.$searchInput = $('#felix-search-node #edit-node', context);
  
      // Create a "Better Autocomplete" object, see betterautocomplete.js
      Drupal.felix.$searchInput.betterAutocomplete('init',
        Drupal.settings.basePath + 'felix/autocomplete',
        {},
        { // Callbacks
        select: function(result) {
          // Only change the link text if it is empty
          if (typeof result.disabled != 'undefined' && result.disabled) {
            return false;
          }
          Drupal.felix.$searchInput.val(result.value);
          $('#edit-node').blur();
        },
        constructURL: function(path, search) {
          return path + "/" + Drupal.settings.felix.nodetype + "?s=" + encodeURIComponent(search);
        },
        insertSuggestionList: function($results, $input) {
          $results.width($input.outerWidth() - 2) // Subtract border width.
            .css({
              position: 'absolute',
              left: $input.offset().left - $('#page').offset().left,
              top: $input.offset().top + $input.outerHeight() - $('#page').offset().top,
              zIndex: 211000,
              maxHeight: '330px',
            })
            .hide()
            .insertAfter($('#felix-search-node', context).parent());
          }
      });
    }
  };
})(jQuery);
