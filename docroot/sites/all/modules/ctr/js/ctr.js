(function ($) {
  Drupal.behaviors.ctr = {
    attach: function (context, settings) {
      $('.context-blockform-theme-selector select', context).bind('change', function (e) {
        e.preventDefault();
        // Get theme user choose
        var _theme_key = $(this).val(), $blocks = $('#context-blockform .blocks', context);
        // Show region of that theme
        if (_theme_key !== '_none') {
          // Hide all element
          $blocks.find('>div, >table, >div>span.theme-list').hide();
          $.each(settings.ctr[_theme_key], function (id, item) {
            _ctr_show_region(item, $blocks)
          });
        }
        // With all theme select, show all region
        else {
          // Show all region
          $blocks.find('>div, >table, >div>span.theme-list').show();
        }
      }).trigger('change');
    }
  };
  
  
  // Function hide region from select
  function _ctr_show_region(region, $blocks) {
    // Find table
    var $table = $blocks.find('table#context-blockform-region-' + region);
    // Show it and the toggle weight table
    $table.show().prev().show().prev().show();
  }
})(jQuery);