  (function ($) {
  function handleFocus() {
    if($('#edit-name').length > 0) {
      $("#edit-name")[0].focus();
    } else if($('#edit-pass').length > 0 && $('#edit-yubikey-otp').length > 0) {
      if($('#edit-pass')[0].offsetTop < $('#edit-yubikey-otp')[0].offsetTop) {
        $('#edit-pass')[0].focus();
      } else {
        $('#edit-yubikey-otp')[0].focus();
        $('#edit-yubikey-otp').keypress(function(event) {
          var evt = event || window.event;
          var code = evt.which || evt.keyCode || evt.charCode;
          if (13 == code) {
            if(evt.which){
              evt.preventDefault();
            } else if (evt.keyCode) {
              evt.keyCode = 9;
            } else {
              evt.charCode = 9;
            }
            $('#edit-pass').focus();
          }
        });
      }
    } else if($('#edit-yubikey-otp').length > 0) {
      $('#edit-yubikey-otp')[0].focus();
    } else if($('#edit-pass').length > 0) {
      $('#edit-pass')[0].focus();
    }
  }
  
  $(document).ready(function() {
    if($('#edit-yubikey-otp').length > 0) {
      $('#edit-yubikey-otp')[0].value = "";
    }
    if($('#edit-yubikey-forgot-pass').length > 0) {
      if($('#edit-yubikey-forgot-pass').is(':checked')) {
        $('#yk_pass_div').hide();
      }
      $('#edit-yubikey-forgot-pass').click(function() {
        if($(this).is(':checked')) {
          $('#yk_pass_div').slideUp('fast');
        } else {
          $('#yk_pass_div').slideDown('fast');
        }
      });
    }
  });
  
  Drupal.behaviors.yubikey = {
    attach: function(context) {
      var $yubikeyElements = $("#edit-yubikey-otp-wrapper");
    
      if ($("#edit-openid-identifier.openid-processed").size()) {
        // Hide YubiKey OTP block
          $yubikeyElements.hide();
      } else {
        handleFocus();
      }
     
      $("li.openid-link:not(.yubikey-openid-processed)", context).addClass('yubikey-openid-processed').click(function() {
          // Hide YubiKey OTP block
          $yubikeyElements.hide();
          // Remove possible error message.
          $("#edit-yubikey-otp").removeClass("error");
          return false;
        });
      $("li.user-link:not(.yubikey-openid-processed)", context).addClass('yubikey-openid-processed').click(function() {
          // Show YubiKey OTP block
          $yubikeyElements.show();
          handleFocus();
          return false;
        });
    }
  };
  
  Drupal.behaviors.openid = {
    attach: function(context) {
      var $loginElements = $("#edit-name-wrapper, #edit-pass-wrapper, li.openid-link");
      var $openidElements = $("#edit-openid-identifier-wrapper, li.user-link");
    
      // This behavior attaches by ID, so is only valid once on a page.
      if (!$("#edit-openid-identifier.openid-processed").size() && $("#edit-openid-identifier").val()) {
        $("#edit-openid-identifier").addClass('openid-processed');
        $loginElements.hide();
        // Use .css("display", "block") instead of .show() to be Konqueror friendly.
        $openidElements.css("display", "block");
      }
      $("li.openid-link:not(.openid-processed)", context)
        .addClass('openid-processed')
        .click( function() {
           $loginElements.hide();
           $openidElements.css("display", "block");
          // Remove possible error message.
          $("#edit-name, #edit-pass").removeClass("error");
          $("div.messages.error").hide();
          // Set focus on OpenID Identifier field.
          $("#edit-openid-identifier")[0].focus();
          return false;
        });
      $("li.user-link:not(.openid-processed)", context)
        .addClass('openid-processed')
        .click(function() {
           $openidElements.hide();
           $loginElements.css("display", "block");
          // Clear OpenID Identifier field and remove possible error message.
          $("#edit-openid-identifier").val('').removeClass("error");
          $("div.messages.error").css("display", "block");
          /*
          // Set focus on username field.
          //$("#edit-name")[0].focus();
          */
          return false;
        });
    }
  };
})(jQuery);
