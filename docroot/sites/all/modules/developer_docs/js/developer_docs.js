/**
 * @file
 * Script to filter modules in module list.
 */

(function ($) {
  Drupal.behaviors.developerDocs = {
    attach: function (context) {
      /* cache td elements to improve search performance*/
      var $rows = $(".module-list tbody>tr"), $cells = $rows.children();
      $("#element-class", context).once(function() {
        $(this).keyup(function(){
          var term = $(this).val();
          if (term != "") {
            $rows.hide();
            $cells.filter(function() {
              return jQuery(this).text().toLowerCase().indexOf(term) > -1;
            }).parent("tr").show();
          }
          else {
            $rows.show();
          }
        });
      });
    }
  };
})(jQuery);
