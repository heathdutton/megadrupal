(function ($) {
  Drupal.behaviors.voipnumber_verification = {};
  Drupal.behaviors.voipnumber_verification.attach = function(context, settings) {
    //Check which radio button was selected to display appropriate fields
    if ($("#edit-field-settings-voipnumber-verification-voipnumber-verification-verify").attr("checked")) {
      voipnumber_verification_show_form_elements();
    }
    else {
      voipnumber_verification_hide_form_elements();
    }

    $("#edit-field-settings-voipnumber-verification-voipnumber-verification-verify").change(function () {
      if($(this).is(':checked')) {
        voipnumber_verification_show_form_elements();
      }
      else {
        voipnumber_verification_hide_form_elements();
      }
    });

    //http://stackoverflow.com/questions/2024389/jquery-click-event-is-firing-multiple-times-when-using-class-selector
    $(".voipnumber-verification-wrapper .verify-number").unbind('click').click(function(event) {
      $(".voipnumber-verification-msg").remove();
      var field_name = $(this).parent().data('field_name');
      var css_field_name = voipnumber_verification_replaceAll("_","-", field_name);
      var delta = $(this).parent().data('delta');
      var phone_number = $(this).parent().siblings('.form-item').children('input').val();
      var verify_number = $(this);

      $(this).parent().siblings('.form-item').children('input').removeClass('error');
      $(this).siblings(".verify-code-wrapper").hide();
      if(!phone_number) {
        $(this).parent().siblings('.form-item').children('input').addClass('error');
        $("#edit-"+css_field_name).prepend("<div class='voipnumber-verification-msg messages error'>Phone number is empty.</div>");
      }
      else {
        $.ajax({
          type: "GET",
          url: Drupal.settings.basePath +"?q=voipnumber_verification/send-code",
          data: "phone_number=" + encodeURIComponent(phone_number) + "&field_name=" + field_name + "&delta=" + delta,
          dataType: 'json',
          success: function (response) {
            if(response.status == 200) {
              verify_number.siblings(".verify-code-wrapper").show();
              verify_number.text("Resend code");
            }
            else {
              $(this).parent().siblings('.form-item').children('input').addClass('error');
              $("#edit-"+css_field_name).prepend("<div class='voipnumber-verification-msg messages error'>"+response.message+"</div>");
            }
          }
        });
      }
    });

    $(".voipnumber-verification-wrapper .verify-code").unbind('click').click(function(event) {
      var field_name = $(this).parents('.voipnumber-verification-wrapper').data('field_name');
      var css_field_name = voipnumber_verification_replaceAll("_","-", field_name);
      var delta = $(this).parents('.voipnumber-verification-wrapper').data('delta');
      var phone_number = $(this).parents('.voipnumber-verification-wrapper').siblings('.form-item').children('input').val();
      var code = $(this).siblings('.code').val();
      $(".voipnumber-verification-msg").remove();
      $(this).parent().siblings('.verified').hide();
      $(this).parents('.voipnumber-verification-wrapper').siblings('.form-item').children('input').removeClass('error');
      var verify_code = $(this);

      $.ajax({
        type: "GET",
        url: Drupal.settings.basePath +"?q=voipnumber_verification/verify-code",
        data: "phone_number=" + encodeURIComponent(phone_number) + "&field_name=" + field_name + "&delta=" + delta+ "&code=" + code,
        dataType: 'json',
        success: function (response) {
          if(response.verified) {
            verify_code.parent().siblings('.verified').show();
            verify_code.parent().hide();
          }
          else {
            verify_code.siblings('.code').addClass('error');
            $("#edit-"+css_field_name).prepend("<div class='voipnumber-verification-msg messages error'>" +
              "Invalid phone verification code. Please try again or contact your system administrator.</div>");
          }
        }
      });
    });

    function voipnumber_verification_replaceAll(find, replace, str) {
      return str.replace(new RegExp(find, 'g'), replace);
    }

    function voipnumber_verification_hide_form_elements() {
      $(".form-item-field-settings-voipnumber-verification-voipnumber-verification-method").hide();
      $(".form-item-field-settings-voipnumber-verification-voipnumber-verification-text").hide();
    }

    function voipnumber_verification_show_form_elements() {
      $(".form-item-field-settings-voipnumber-verification-voipnumber-verification-method").show();
      $(".form-item-field-settings-voipnumber-verification-voipnumber-verification-text").show();
    }
  };
})(jQuery);



