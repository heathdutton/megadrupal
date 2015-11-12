(function ($) {
  $(document).ready(function() {
    if ($('#edit-should-expire').attr('checked'))
      {
        $('.form-item-expiration').show();
      }
      else
      {
        $('.form-item-expiration').hide();
      }
    $('.form-item-should-expire input').click(function() {
      if ($('#edit-should-expire').attr('checked'))
      {
        $('.form-item-expiration').slideDown();
      }
      else
      {
        $('.form-item-expiration').slideUp();
      }
    });
    $('.form-item-should-expire label').click(function() {
      if ($('#edit-should-expire').attr('checked'))
      {
        $('.form-item-expiration').slideDown();
      }
      else
      {
        $('.form-item-expiration').slideUp();
      }
    });
  });
})(jQuery);
