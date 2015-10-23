/**
 * @file
 * Attaches javascript for Droogle Hangout.
 */
(function($) {
  Drupal.behaviors.droogle_hangout = {
    attach: function (context, settings) {
      /** Add User to Hangout Popup List **/
      $('.jabber-hangout-btn').click(function() {
        $('#hangout-invite-wrapper').show();
        if ($('#hangout-invite-wrapper').length) {
          $('#hangout-invite-wrapper').draggable({
              containment: 'window',
              scroll: false,
              cancel: '#hangout-email-list, .schedule-wrapper',
          });
        }
            if ($('#jabber-hangout-invites-btn').is(':hidden')){
              $('#jabber-hangout-invites-btn').show().html('Start Hangout');
            }
            var count = $('#hangout-email-list').children('.hangout-email-invitee').length;
            var email_address = $('#jabber-hangout-invites-btn').attr('name');
            if (count < 14) {
              if (email_address === undefined) {
                email_address = '';
              }
              if (email_address.search($(this).attr('name')) == -1) {
                if ($('#jabber-hangout-invites-btn').attr('name') == '') {
                  var email_addresses = $(this).attr('name');
                  var fullname = $(this).children('span.button-fullname').html();

                  var invite = '<li class="hangout-email-invitee"><span class="remove" name="' + email_addresses + '">x</span><span class="invitee-fullname">' + fullname + '</span></li>';
                  $('#hangout-email-list').html(invite);
                  $('#jabber-hangout-invites-btn').attr('name', email_addresses);
                }
                else {
                  var next_email = $(this).attr('name');
                  var fullname = $(this).children('span.button-fullname').html();
                  var next_invite = '<li class="hangout-email-invitee"><span class="remove" name="' + next_email + '">x</span><span class="invitee-fullname">' + fullname + '</span></li>';

                  $('#hangout-email-list').append(next_invite);
                  if (email_address == '') {
                    var email_addresses = next_email;
                  }
                  else {
                    var email_addresses = email_address + ',' + next_email;
                  }

                  $('#jabber-hangout-invites-btn').attr('name', email_addresses);
                }
              }
              var count1 = $('#hangout-email-list').children('.hangout-email-invitee').length;
              var open_spots = 13 - count1;
              var count_message = 'You can invite <span class="open-spots-count">' + open_spots + '</span> more people.<br /><span class="second-line">Click any <span class="open-spots-message">Hangout</span> link to invite more people.</span>';
              $('#count-message').html(count_message);
            }
            else {

              var font_count = $('#hangout-email-list').children().length;
              var font_size = font_count + 'px';
              if (font_count == 13) {
                var error_message = '<li class="error-message" style="font-size:' + font_size + ';">You have reached the Google maximum allowed number invitations, remove an existing invitation to add more invitations or submit current hangout invitation list.</li>';
                $('#hangout-email-list').append(error_message);
              }
              else {
                font_size = $('#hangout-email-list').children('.error-message').css('font-size');
                font_size = font_size.replace('px','');
                font_size = parseInt(font_size);
                if (font_size < 20) {
                  font_size = font_size + 1;
                  font_size = font_size + 'px';
                  $('#hangout-email-list').children('.error-message').css('font-size', font_size);
                }
                else {
                  var error_message = '<h3 class="error-message">STOP!!! Not Funny Anymore!</h3>';
                  $('#hangout-email-list').children('.error-message').html(error_message);
                }
              }
            }
      });

      /** Remove invites created in popup hangouts list. **/
      $(document).on('click', '.remove', function() {
          var count = $('#hangout-email-list').children('.hangout-email-invitee').length;
          if (count <= 13) {

            var open_spots = 14 - count;
            var count_message = 'You can invite <span class="open-spots-count">' + open_spots + '</span> more people.<br /><span class="second-line">Click any <span class="open-spots-message">Hangout</span> link to invite more people.</span>';
            $('#count-message').html(count_message);
            $('#hangout-email-list').children('.error-message').remove();
          }
          var email_addresses = $('#jabber-hangout-invites-btn').attr('name');
          $(this).parent('li.hangout-email-invitee').remove();
          email_addresses = email_addresses.replace($(this).attr('name'), '');
          email_addresses = email_addresses.replace(',,',',');
          // Remove errant commas at beginning or end of email string.
          email_addresses = email_addresses.replace(/^,|,$/g,'');
          $('#jabber-hangout-invites-btn').attr('name', email_addresses);
      });

      /** Close Hangout List Popup **/
      $('#close-hangout-invite-popup').click(function() {
         $('#hangout-invite-wrapper').hide();
         $('#jabber-hangout-invites-btn').removeAttr('name');
         $('#hangout-email-list').children('li.hangout-email-invitee').remove();
      });

       /** Sent Hangout Request(s). **/
      $(document).on('click', '#jabber-hangout-invites-btn', function() {
              $(this).html('Invites Sent').delay(300).fadeOut(900);
              var url = '/droogle_hangout_create_hangout';
              //Uid info stored in name attribute.
              var ajax_email = $(this).attr('name');
              var caldate = $('#edit-droogle-hangout-calendar').val();
                $.ajax({
                  type: "POST",
                  url: url,
                  data: {
                    'ajaxemail': ajax_email,
                    'caldate': caldate,
                  },
                  dataType: 'json',
                  success: function(outputme) {
                     var hangout_success = outputme.successful;
                     var hangoutlink = outputme.hangoutlink;
                     var invitee_email = outputme.invitee_email;
                     if (caldate == '') {
                       window.open(hangoutlink, '_blank');
                     }
                     var msg = 'You are invited to a Google Hangout Session: ' + hangoutlink;

                     var msg2 = '<div class="hangout-sent"><div class="close-hangout-sent">x</div><div class="hangout-sent-inner"><span class="hangout-sent-bold">A Google Hangout Session invite was sent to:</span> ' + invitee_email + '.<br /><span class="hangout-sent-bold">Go to the hangout by clicking: </span><br /><a href="' + hangoutlink + '" target="_blank">Go To Hangout</a></div></div>';

                     $('#hangout-invite-wrapper').after(msg2);

                     setTimeout(function () {
                        $('.hangout-sent').fadeOut(function(){
                          $(this).remove();
                        });
                     }, 1800000);

                     $('#jabber-hangout-invites-btn').removeAttr('name');
                     $('#hangout-email-list').children('li.hangout-email-invitee').remove();
                     $('#hangout-invite-wrapper').hide();
                  }
                });
        return false;
      });

        /* Remove Hangout Sent popup message */
        $(document).on('click', '.close-hangout-sent', function() {
          $(this).parent('.hangout-sent').remove();
        });
    }
  };
}(jQuery));
