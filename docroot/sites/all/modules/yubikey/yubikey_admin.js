(function ($) {
  $(document).ready(function() {
    if(!$('#edit-yubikey-enabled').is(':checked')) {
      $('#yk_fields_div').hide();
    }
    $('#edit-yubikey-enabled').click(function() {
      if($(this).is(':checked')) {
        $('#yk_fields_div').slideDown('fast');
      } else {
        $('#yk_fields_div').slideUp('fast');
      }
    });
   
    if(!$('#edit-yubikey-authscheme-un-pwd-yk').is(':checked')) {
      $('#yk_optional_div').hide();
    }
    $('#edit-yubikey-authscheme-un-pwd-yk').click(function() {
      if($(this).is(':checked')) {
        $('#yk_optional_div').slideDown('fast');
      }
    });
    $('#edit-yubikey-authscheme-pwd-yk').click(function() {
      if($(this).is(':checked')) {
        $('#yk_optional_div').slideUp('fast');
      }
    });
    $('#edit-yubikey-authscheme-un-or-yk-pwd').click(function() {
      if($(this).is(':checked')) {
        $('#yk_optional_div').slideUp('fast');
      }
    });
    $('#edit-yubikey-authscheme-yk-only').click(function() {
      if($(this).is(':checked')) {
        $('#yk_optional_div').slideUp('fast');
      }
    });
    
    if($('#edit-yubikey-valserver-urls-type-online').is(':checked')) {
      $('#yk_valserver_urls_div').hide();
    }
    $('#edit-yubikey-valserver-urls-type-online').click(function() {
      if($(this).is(':checked')) {
        $('#yk_valserver_urls_div').slideUp('fast');
      }
    });
    $('#edit-yubikey-valserver-urls-type-custom').click(function() {
      if($(this).is(':checked')) {
        $('#yk_valserver_urls_div').slideDown('fast');
      }
    });
  });
})(jQuery);
