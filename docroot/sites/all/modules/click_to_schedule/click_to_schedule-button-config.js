(function ($) {

  Drupal.behaviors.timeTradeButtonConfig = {
    attach: function (context, settings) {
      // Zenbox feedback tab. Values will be the same for all users and sites.
      if (typeof(Zenbox) !== "undefined") {
        Zenbox.init({
          dropboxID:   "20032782",
          url:         "https://timetrade.zendesk.com",
          tabID:       "feedback",
          tabColor:    "gray",
          tabPosition: "Right",
          hide_tab:    true
        });
      }

      // For each color option, make clickable color tile
      var html = '';
      $('select#edit-button-color option', context).each(function () {
        var v = this.value;
        html +='<div class="color-tile" id="tile_'+v+'"></div>';
      });
      $('#color-picker').html(html);

      // Set color and onclick for each color tile
      $('#color-picker .color-tile').each(function() {
        var code = this.id.split('_')[1];
        $(this).css('background-color', '#'+code);
        $(this).click(function() {
          // Change select value
          var code = this.id.split('_')[1];
          $('select#edit-button-color').val(code);
          var v = $('select#edit-button-color').val();
          // Mark selected color
          markSelected(code);
          // Trigger change in select
          $('select#edit-button-color').change();
        });
      });

      // Make first tile selected
      $('.color-tile:first').addClass('selected-color-tile');

      // Show button with all the initial styles
      updateButton();

      // When size or color is changed, change the button
      $('#edit-button-size,#edit-button-color').change(updateButton);


      // Advanced options
      var apptLink = Drupal.settings.click_to_schedule.appt_link;

      if (Drupal.settings.click_to_schedule.subscription_type == 'paid') {
        $('#edit-advanced').css('color', '#000');
      }

      // Set custom target url, if applicable
      // Set custom target url & type element to disabled, if appropriate
      if ($('#edit-button-action:checked').length) {
        if ($('#edit-landing-page-url').val()) {
          $('#button-preview a').bind('click', newWindowHandler);
        }
      } else {
        $('#edit-landing-page-url').each(function() {
          this.disabled = true;
        });
        $('#edit-landing-page-url').parent().addClass('form-disabled');
        $('#edit-link-location-internal').each(function() {
          this.disabled = true;
        });
        $('#edit-link-location-external').each(function() {
          this.disabled = true;
        });
        $('#edit-link-location-internal').parent().addClass('form-disabled');
        $('#edit-link-location-external').parent().addClass('form-disabled');
      }

      // Checking and unchecking redirect url
      $('#edit-button-action').change(function() {
        if ($('#edit-button-action:checked').length) {
          $('#edit-landing-page-url').each(function() {
            this.disabled = false;
          });
          $('#edit-landing-page-url').parent().removeClass('form-disabled');
          $('#edit-link-location-internal').each(function() {
            this.disabled = false;
          });
          $('#edit-link-location-external').each(function() {
            this.disabled = false;
          });
          $('#edit-link-location-internal').parent().removeClass('form-disabled');
          $('#edit-link-location-external').parent().removeClass('form-disabled');
          if ($('#edit-landing-page-url').val()) {
            var href = $('#edit-landing-page-url').val();
            if ($('input:radio[name=link_location]:checked').val() == 'external') {
              if (href.substring(0, 7) != 'http://' && href.substring(0, 8) != 'https://') {
                href = 'http://' + $('#edit-landing-page-url').val();
              }
            } else {
              href = '/' + href;
            }
            $('input[name="href"]').val(href);
            $('input[name="popup"]').val(0);
            $('#button-preview a').attr('href', href);
            $('#button-preview a').removeData('fancybox');
            $('#button-preview a').unbind('click.fb');
            $('#button-preview a').bind('click', newWindowHandler);
            $('#button-preview a').removeClass('iframe');
            $('#button-preview a').removeClass('appt-popup');
          }
        } else {
          $('#edit-landing-page-url').each(function() {
            this.disabled = true;
          });
          $('#edit-landing-page-url').parent().addClass('form-disabled');
          $('#edit-link-location-internal').each(function() {
            this.disabled = true;
          });
          $('#edit-link-location-external').each(function() {
            this.disabled = true;
          });
          $('#edit-link-location-internal').parent().addClass('form-disabled');
          $('#edit-link-location-external').parent().addClass('form-disabled');
          $('#button-preview a').fancybox({'height': 500, 'width': 750});
          $('#button-preview a').unbind('click', newWindowHandler);
          $('#button-preview a').attr('href', apptLink);
          $('input[name="href"]').val(apptLink);
          $('input[name="popup"]').val(1);
          $('#button-preview a').addClass('iframe');
          $('#button-preview a').addClass('appt-popup');
        }
      });

      // Changing button target url
      $('#edit-landing-page-url').change(function() {
        if ($('#edit-landing-page-url').val()) {
          // href used in preview but not saved as button property
          var ahref;
          var href = $('#edit-landing-page-url').val();
          if ($('input:radio[name=link_location]:checked').val() == 'external') {
            if (href.substring(0, 7) != 'http://' && href.substring(0, 8) != 'https://') {
              ahref = href = 'http://' + $('#edit-landing-page-url').val();
            }
          } else {
            ahref = '/' + href;
          }
          $('input[name="href"]').val(href);
          $('input[name="popup"]').val(0);
          $('#button-preview a').attr('href', ahref);
          $('#button-preview a').unbind('click.fb');
          $('#button-preview a').removeData('fancybox');
          $('#button-preview a').bind('click', newWindowHandler);
          $('#button-preview a').removeClass('iframe');
          $('#button-preview a').removeClass('appt-popup');
        } else {
          $('input[name="href"]').val(apptLink);
          $('#button-preview a').fancybox({'height': 500, 'width': 750});
          $('#button-preview a').unbind('click', newWindowHandler);
          $('#button-preview a').attr('href', apptLink);
          $('input[name="popup"]').val(1);
          $('#button-preview a').addClass('iframe');
          $('#button-preview a').addClass('appt-popup');
        }
      });

      // Change internal/external site radio
      $('input:radio[name=link_location]').change(function() {
        if ($('#edit-landing-page-url').val()) {
          // href used in preview but not saved as button property
          var ahref;
          var href = $('#edit-landing-page-url').val();
          if ($('input:radio[name=link_location]:checked').val() == 'external') {
            if (href.substring(0, 7) != 'http://' && href.substring(0, 8) != 'https://') {
              ahref = href = 'http://' + $('#edit-landing-page-url').val();
            }
          } else {
            ahref = '/' + href;
          }
          $('#button-preview a').attr('href', ahref);
          $('input[name="href"]').val(href);
        }
      });

    }
  };


  function updateButton() {
    var size = $('#edit-button-size').val();
    var color = $('#edit-button-color').val();
    var modulePath = Drupal.settings.click_to_schedule.click_to_schedule_path;
    $('#button-preview img').attr('src', '/'+modulePath+'/buttons/button-'+color+'-'+size+'.png');
    var x = 'whatever';
  }

  /* Mark color tile with color code = val as selected; unmark all others
  */
  function markSelected(val) {
    $('#color-picker .color-tile').each(function() {
      $(this).removeClass('selected-color-tile');
    });
    $('#tile_'+val).addClass('selected-color-tile');
  }

  function newWindowHandler() {
    window.open(this.href);
    return false;
  }

})(jQuery);
