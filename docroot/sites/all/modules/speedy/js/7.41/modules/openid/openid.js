(function($){Drupal.behaviors.openid={attach:function(context){var loginElements=$(".form-item-name, .form-item-pass, li.openid-link");var openidElements=$(".form-item-openid-identifier, li.user-link");var cookie=$.cookie("Drupal.visitor.openid_identifier");if(!$("#edit-openid-identifier.openid-processed").length){if(cookie){$("#edit-openid-identifier").val(cookie)}if($("#edit-openid-identifier").val()||location.hash=="#openid-login"){$("#edit-openid-identifier").addClass("openid-processed");loginElements.hide();openidElements.css("display","block")}}$("li.openid-link:not(.openid-processed)",context).addClass("openid-processed").click(function(){loginElements.hide();openidElements.css("display","block");$("#edit-name, #edit-pass").removeClass("error");$("div.messages.error").hide();$("#edit-openid-identifier")[0].focus();return false});$("li.user-link:not(.openid-processed)",context).addClass("openid-processed").click(function(){openidElements.hide();loginElements.css("display","block");$("#edit-openid-identifier").val("").removeClass("error");$("div.messages.error").css("display","block");$("#edit-name")[0].focus();return false})}}})(jQuery);