// Global killswitch: only run if we are in a supported browser.
(function ($) {
  $(document).ready(function(){

  // this is where the selector goes, and then attach a behaviour like on p.218 of learning jQuery
  var emailLabel = $('#whatcounts-block-form-1 label').remove().text().replace(/: $/,"");

  var emailDefault = $('#edit-email').val();
  var emailField = $('#edit-email');
  if (emailDefault == '') {
    $('#edit-email').addClass('placeholder').val(emailLabel);
  }
  $('#edit-email').focus(function() {
    if (this.value == emailLabel) {
      $(this).removeClass('placeholder').val('');
    };
  })
  .blur(function() {
    if (this.value == '') {
      $(this).addClass('placeholder').val(emailLabel);
    }
  });

  //  Empty the placeholder value if it's still there on submit
  /*
  $('#whatcounts-block-form-1').submit(function() { 
    if ($('#edit-email').val() == emailLabel) {
      $('#edit-email').val('');
    }
  });
  */
})(jQuery);
