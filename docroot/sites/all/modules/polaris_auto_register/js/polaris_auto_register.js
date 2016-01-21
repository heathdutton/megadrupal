/**
 * @file polaris_auto_register.js
 * Behaviors for new online registration form
 */
(function($) {
  Drupal.behaviors.polaris_auto_register = {
    attach: function(context) {

      initial_setup();

      // Validate the date of birth (must be 13 or older)
      // ...as long as we're not looking at the "lost card number" version of the form.
      $('body:not(.page-polaris-auto-register-lost-card-number, .page-connected) #edit-polaris-auto-register-dob-year').focusout(function() {
        validate_birthdate();
      });

      // check email to see if already registered
      $('#edit-polaris-auto-register-email').focusout(function() {
        validate_email();
      });

      // format phone number 1
      $('#edit-polaris-auto-register-phone-voice-1').focusout(function() {
        format_phone1();
      });

      // format phone number 2
      $('#edit-polaris-auto-register-phone-voice-2').focusout(function() {
        format_phone2();
      });

      $('#polaris-auto-register-lost-card-lookup').click(function() {
        validate_email();
        return false; // Don't submit the form like you normally would.
      });

      // Telephone Lookup
      $('.page-polaris-auto-register-telephone-lookup #edit-polaris-auto-register-phone-voice-1').focusout(function() {
        telephone_lookup();
      });
      $('#polaris-auto-register-staff-telephone-lookup').click(function() {
        telephone_lookup();
        return false;
      });

    } // end of attach
  }; // end of Drupal.behaviors.polaris_auto_register

  function initial_setup() {
    // initially hide the alt phone number field and ajax spinner
    $('.phone2').hide();
    $('.progress').hide();

    // show the alt phone number, hide the link
    $('.alt_phone').click(function() {
      $('.phone2').show();
      $('.alt_phone').hide();
      return false;
    })
  }

  function validate_birthdate() {
    // get values in month, day, and year
    var m = $('#edit-polaris-auto-register-dob-month').val();
    var d = $('#edit-polaris-auto-register-dob-day').val();
    var y = $('#edit-polaris-auto-register-dob-year').val();
    var start = new Date(m + '-' + d + '-' + y);

    var full_date = new Date();

    var days   = (full_date - start)/1000/60/60/24/365;

    if (days < 13) {
      // add error class to the fields
      $('#edit-polaris-auto-register-dob-month').addClass("error");
      $('#edit-polaris-auto-register-dob-day').addClass("error");
      $('#edit-polaris-auto-register-dob-year').addClass("error");
      $('.dob_error').show();
      return false;
    }
    else {
      // remove error class from fields
      $('#edit-polaris-auto-register-dob-month').removeClass("error");
      $('#edit-polaris-auto-register-dob-day').removeClass("error");
      $('#edit-polaris-auto-register-dob-year').removeClass("error");
      $('.dob_error').hide();
    }
  }

  function validate_email() {
    // Some text that we may use in a few instances...
    var lost_card_warnings = '<h3>Protect Your Account</h3>' +
                              '<p>Your library card is an important personal item that needs protection. If you’ve lost your physical card, you can replace your ' +
                              'card at any Richland Library location. Bring a photo ID and we will give you a new card right away. There is a $1 replacement fee. ' +
                              'If you would like to protect your account, contact us with your full name and we can block your account from use until you can get by ' +
                              'to replace it.</p>' +
                              '<h3>Reprint Your Card</h3>' +
                              '<p>In order to reprint your library card, <a href="/my-account">sign in to your account</a> on our website. Once signed in, go back up ' +
                              'to the orange "My Account" button and click to reveal the drop-down menu. Select "Reprint my Library card" and then print the page ' +
                              'that appears. Cut out or fold up your card and use it to check out just as you would your plastic card. Don\'t toss your paper card ' +
                              'in the trash without shredding it. We have plastic sleeves to hold a paper ' +
                              'card, just ask for one at the checkout desk.</p>';
    // get the entered name and email address
    var fname = $('#edit-polaris-auto-register-name-first').val();
    var mname = $('#edit-polaris-auto-register-name-middle').val();
    var lname = $('#edit-polaris-auto-register-name-last').val();
    var phone = $('#edit-polaris-auto-register-phone-voice-1').val();
    var email = $('#edit-polaris-auto-register-email').val();

    // test the email
    var testEmail = /^[A-Z0-9._%+-]+@([A-Z0-9-]+\.)+[A-Z]{2,4}$/i;

    var m = $('#edit-polaris-auto-register-dob-month').val();
    var d = $('#edit-polaris-auto-register-dob-day').val();
    var y = $('#edit-polaris-auto-register-dob-year').val();
    var dob = m + '-' + d + '-' + y;

    if (testEmail.test(email)) {
      // show the spinner so user knows something is going on
      $('.progress').show();

      $('#edit-polaris-auto-register-email').removeClass("error");

      $('.email_msg').html("").slideUp();

      $.ajax({
        url: "/polaris-auto-register/check-email",
        type: 'POST',
        data: {email: email, fname: fname, mname: mname, lname: lname, phone: phone, dob: dob},
        success: function(data) {
          // hide the spinner again
          $('.progress').hide();

          if (data.return == 0) {
            // set first and last values in a cookie
            $.cookie('polaris_auto_register_ity_name', fname);
            var barcode = data.barcode;
            // set barcode value in a cookie
            $.cookie('polaris_auto_register_ity_barcode', barcode);
            var email2 = data.email;
            // set email value in a cookie
            $.cookie('polaris_auto_register_ity_email', email2);
            var email_split = email2.split("@");
            var email_first_char = email2.slice(0, 1);
            email2 = email_first_char + '******@' + email_split[1];
            barcode = '***** ***** ' + barcode.slice(-4);

            if (data.polaris_name === null) {
              data.polaris_name = '';
            }

            if (data.email != '') {
              newMsg = '<h3>Is This You?</h3>'
              + '<p>It appears that you may already have a library card at '
              + 'Richland Library.</p>'
              + '<ul style="list-style-type:none"><li><div class="registrant">Name: ' + data.polaris_name + '</div></li>'
              + '<li><div class="registrant">Library Card: ' + barcode + '</div></li>'
              + '<li><div class="registrant">Email: ' + email2 + '</div></li></ul>'
              + '<input class="emailBtn form-submit" type="button" value="Email My Library Card Number to Me" />'
              + '<p>If you have any difficulties, please <a href="/ask-us">Ask a Librarian</a>.</p>'
              + lost_card_warnings;
             }
             else {
              newMsg = '<h3>Is This You?</h3>'
              + '<p>It appears that you may already have a library card at '
              + 'Richland Library.</p>'
              + '<ul style="list-style-type:none"><li><div class="registrant">Name: ' + data.polaris_name + '</div></li>'
              + '<li><div class="registrant">Library Card: ' + barcode + '</div></li></ul>'
              + '<p>It appears that you may already have a Richland Library customer account, '
              + 'but the account may not have an associated email address. You will need an email '
              + 'on file for automatic retrieval of your library card. If you don’t have one, '
              + 'please visit one of our branch locations with a valid photo ID to get your library '
              + 'card number.</p>'
              + '<p>If you have any difficulties, please <a href="/ask-us">Ask a Librarian</a>.</p>'
              + lost_card_warnings;
             }

            $('#edit-polaris-auto-register-email').addClass("error");
            $('.email_msg').html(newMsg).slideDown();
          }
        }
      }).always(function() {
        // AJAX has finished.

        // For "lost library card" version of the form...
        // Check to see if the email markup is empty/hidden and display a NOT FOUND message.
        if ($('body').hasClass('page-polaris-auto-register-lost-card-number')) {
          validate_lost_card_message(lost_card_warnings);
        }
      }); // end of ajax

      return false;
    }
    else {
      $('#edit-polaris-auto-register-email').addClass("error");

      $('.email_msg').html("Please enter a valid email address").slideDown();

      return false;
    }
  }

  function telephone_lookup() {
    var phone = $('#edit-polaris-auto-register-phone-voice-1').val();
    // show the spinner so user knows something is going on
    $('.progress').show();
    $('#edit-polaris-auto-register-email').removeClass("error");
    $('.email_msg').html("").slideUp();$.ajax({
      url: "/polaris-auto-register/telephone-lookup-ajax",
      type: 'POST',
      data: {phone: phone},
      success: function(data) {
        // hide the spinner again
        $('.progress').hide();
        if (data.return == 0) {

          if (data.polaris_name === null) {
            data.polaris_name = '';
          }

          newMsg = '<h3>Matching Customers:</h3>'
          + '<ul style="list-style-type:none"><li><div class="registrant">Name: ' + data.name + '</div></li>'
          + '<li><div class="registrant">Phone Number: ' + data.phone + '</div></li>'
          + '<li><div class="registrant">Library Card:<br><b>' + data.barcode + '</b></div></li></ul>';
        }
        else {
          newMsg = '<h3>No Matches Found.</h3>'
        }
        $('.email_msg').html(newMsg).slideDown();
      }
    }); // end of ajax
    return false;
  }

  // this will be called when the button to email barcode is clicked
  $(".email_msg").on("click", ".emailBtn", function() {
    send_email();
  });

  function send_email() {
    var m_barcode = $.cookie('polaris_auto_register_ity_barcode');
    var m_email = $.cookie('polaris_auto_register_ity_email');
    var m_name = $.cookie('polaris_auto_register_ity_name');

    $.ajax({
        url: "/polaris-auto-register/send-email",
        type: 'POST',
        data: {f_name: m_name, x_barcode: m_barcode, x_email: m_email},
        success: function(data, context) {
          if (data == 1) {

            // get rid of the cookies, since we don't need them
            $.cookie('polaris_auto_register_ity_name', '');
            $.cookie('polaris_auto_register_ity_barcode', '');
            $.cookie('polaris_auto_register_ity_email', '');

            newMsg2 = '<h3>Your Email Has Been Sent</h3>'
                     + '<p>An email has been sent to your email address on file  '
                     + 'containing your barcode.</p>'
                     + '<input class="closeBtn form-submit" type="button" value="Close This Page" />'
                     + '<p>If you have any difficulties, please <a href="/ask-us">Ask a Librarian</a>.</p>';

                     $('#edit-polaris-auto-register-email').removeClass("error");
                     $('.email_msg').html(newMsg2).slideDown();
          }

        }
      }); // end of ajax

    return false;
  }

   // this will be called when the button to close is clicked
  $(".email_msg").on("click", ".closeBtn", function() {
    location.href = window.location.origin;
  });


  function format_phone1() {
    var phone1 = $('#edit-polaris-auto-register-phone-voice-1').val();

    // make sure it's a string
    phone1 = phone1.toString();

    // get rid of spaces and none numeric characters
    phone1 = phone1.replace(/\D/g,"");

    // format the phone number in 555-555-55555 format
    phone1 = phone1.replace(/(\d{3})(\d{3})(\d{4})/, "$1-$2-$3");

    if (phone1.length != 12) {
      $('#edit-polaris-auto-register-phone-voice-1').addClass("error");
      $('#edit-polaris-auto-register-phone-voice-1').focus();
      $('.phone1-error').show();
      $('.phone1_error').html('Please enter a 10 digit phone number (e.g., 555-555-55555).').slideDown();
    }
    else {
      // remove error class if it exists
      $('.phone1-error').hide();
      $('.phone1_error').html('').slideUp();
      $('#edit-polaris-auto-register-phone-voice-1').removeClass("error");

      // replace the value
      $('#edit-polaris-auto-register-phone-voice-1').val(phone1);
    }
  }

  function format_phone2() {
    var phone2 = $('#edit-polaris-auto-register-phone-voice-2').val();

    // make sure it's a string
    phone2 = phone2.toString();

    // get rid of spaces and none numeric characters
    phone2 = phone2.replace(/\D/g,"");

    // format the phone number in 555-555-55555 format
    phone2 = phone2.replace(/(\d{3})(\d{3})(\d{4})/, "$1-$2-$3");

    if (phone2.length >= 1 && phone2.length != 12) {
      $('#edit-polaris-auto-register-phone-voice-2').addClass("error");
      $('#edit-polaris-auto-register-phone-voice-2').focus();
      $('.phone2-error').show();
      $('.phone2_error').html('Please enter a 10 digit phone number (e.g., 555-555-55555).').slideDown();
    }
    else {
      // remove error class if it exists
      $('.phone2-error').hide();
      $('.phone2_error').html('').slideUp();
      $('#edit-polaris-auto-register-phone-voice-2').removeClass("error");

      // replace the value
      $('#edit-polaris-auto-register-phone-voice-2').val(phone2);
    }
  }

  /**
   * Checks to make sure that the email message container shows something when the person searches for his/her lost library card.
   */
  function validate_lost_card_message(lost_card_warnings) {
    if ($('.email_msg').is(':empty')){
      var new_message = $('#polaris_markup2').html();
      $('.email_msg').html('Sorry, we were not able to find any matching information in our system. ' + new_message + lost_card_warnings).slideDown();
    }
  }

})(jQuery);
