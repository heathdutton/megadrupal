/**
 * @file
 * Shows/hides the conditions textarea depending the state of is_conditional radio button.
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
var XCSearch = {};
(function ($) {
  $(document).ready(function() {
    if ($('input[name=is_conditional]').attr('checked')) {
      $('#conditions').show();
    }
    $('input[name=is_conditional]').click(function() {
      if (this.value == 0) {
        $('#conditions').hide('slow');
      }
      else {
        $('#conditions').show('slow');
      }
    });
  });
}(jQuery));