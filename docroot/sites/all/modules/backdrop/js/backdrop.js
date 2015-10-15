(function ($) {
  $(document).ready(function() {
    //hide form
    $('#backdrop-upload-form').hide();

    $('#lnk-add-backdrop').bind('click', function() {
      $('#backdrop-upload-form').toggle(400);
      return false;
    });
  });

}(jQuery));