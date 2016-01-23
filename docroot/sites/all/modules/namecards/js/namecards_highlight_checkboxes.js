/**
 * 
 */
(function ($) {
  $(document).ready(function() {
    $('input[type="checkbox"]', '.namecards-create-email-list-table tbody').each(function(index, ele) {
      $element = $(this);
      $row = $element.parents('tr');
      
      var checkbox = $element[0];
      var row = $row[0];
      //alert(boxChecked);
      $element.bind('change', function() {
          if (checkbox.checked) {
            $row.addClass('namecards-selected');
          }
          else {
            $row.removeClass('namecards-selected');
          }
      });
    });
  });
}(jQuery));
