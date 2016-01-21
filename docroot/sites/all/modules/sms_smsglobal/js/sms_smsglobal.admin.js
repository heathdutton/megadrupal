(function ($) {

  /**
   * @file
   * Javascript tasks for the admin interface.
   */

  Drupal.behaviors.smsSmsGlobalAdmin = {
    attach: function (context, settings) {
      // Show/hide the allowed IPs field depending on the IP restrict checkbox.
      $('#edit-sms-smsglobal-ip-restrict', context).once('ip-restrict', function () {
        var $this = $(this);
        var $allowed_ips_field = $('.form-item-sms-smsglobal-allowed-ips', context);

        var toggle_allowed_ip_field = function (on_load) {
          on_load = typeof on_load !== 'undefined' ? on_load : false;
          var is_restricted = $this.is(':checked');

          // Show if the restricted field is checked, hide otherwise.
          if (!is_restricted) {
            // Do a nice slide-up when not on load.
            if (on_load) {
              $allowed_ips_field.hide();
            }
            else {
              $allowed_ips_field.slideUp('normal');
            }
          }
          else {
            $allowed_ips_field.slideDown('normal');
          }
        };

        // Handle the change event.
        $this.change(function (e) {
          toggle_allowed_ip_field();
        });

        // Toggle on load.
        toggle_allowed_ip_field(true);
      });

      // Change the sender ID field title and description depending on the chosen
      // sender ID type.
      $('#edit-sms-smsglobal-sender-id-type', context).once('senderid-type', function () {
        var $this = $(this);
        var $sender_id_field = $('div.form-item-sms-smsglobal-sender-id', context);
        var sender_id_type, clean_sender_id_type;

        // This matches
        // sms_smsglobal.module#_sms_smsglobal_get_sender_id_types() and has
        // transformation details.
        var sender_id_transformations = {
          '1': {
            'title': 'Custom word',
            'description': 'The custom word that will be used as the sender ID. Should be <strong>between 4 and 11 characters</strong> in length.'
          },
          '2': {
            'title': 'Dedicated number',
            'description': 'The dedicated number that will be used as the sender ID.<br />Please prefix with country code (but without the plus) and do not enter spaces or punctuation, e.g. "61431234567".'
          }
        };

        // Handles the change event and does replacement of the title &
        // description.
        var handle_sender_id_change = function () {
          sender_id_type = $this.val();

          // If we have a transform details for this type, transform!
          if (sender_id_transformations.hasOwnProperty(sender_id_type)) {
            var transformation = sender_id_transformations[sender_id_type];

            // Change title.
            $sender_id_field.find('label').html(Drupal.t('Sender ID (@type)', {
              '@type': transformation.title
            }, context) + ' <span class="form-required" title="This field is required.">*</span>');

            // Change description.
            $sender_id_field.find('div.description').html(Drupal.t(transformation.description));
          }
        };

        // Handle the change event.
        $this.change(function (e) {
          handle_sender_id_change();
        });

        // Toggle on load.
        handle_sender_id_change();
      });
    }
  };

})(jQuery);
