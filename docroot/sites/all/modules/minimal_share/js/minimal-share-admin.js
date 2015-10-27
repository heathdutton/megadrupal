(function ($) {

  Drupal.behaviors.minimalShareAdmin = {
    attach: function (context, settings) {
      $('#minimal-share-config-form', context).once('minimal-share', function() {
        var $preview = $('#minimal-share-preview > .minimal-share', this);
        var $selectedLabelType = $('input[name$="[label_type]"]', this);

        $selectedLabelType.filter(':checked').once('minimal-share', function() {
          var $this = $(this);
          var $label = $this.siblings();
          var $wrapper = $label.children('span');
          var $fakeLink = $wrapper.children('span');
          var fakeLinkLabel = $fakeLink.html();
          var fakeLinkType = $fakeLink.attr('class');

          $preview.append('<span class="' + fakeLinkType + '">' + fakeLinkLabel + '</span>');
        });

        $selectedLabelType.bind('change', function() {
          var $this = $(this);
          var $label = $this.siblings();
          var $wrapper = $label.children('span');
          var $fakeLink = $wrapper.children('span');
          var fakeLinkLabel = $fakeLink.html();
          var fakeLinkType = $fakeLink.attr('class').split(' ');
          var $fakeLinkPreview = $preview.children('.' + fakeLinkType[0]);

          // Add or remove the icon class base on the new selection.
          if (typeof fakeLinkType[1] !== 'undefined') {
            $fakeLinkPreview.addClass(fakeLinkType[1]);
          }
          else {
            $fakeLinkPreview.attr('class', fakeLinkType);
          }

          $fakeLinkPreview.text(fakeLinkLabel);
        });

        var $enableCheckboxes = $('input[name$="[enabled]"]');

        $enableCheckboxes.change(function() {
          var $this = $(this);
          var enabled = $this.is(':checked');
          var nameParts = $this.attr('name').split('][')
          var serviceType = nameParts[nameParts.length - 2];

          if (!enabled) {
            $preview.children('.' + serviceType).addClass('hide');
          }
          else {
            $preview.append($preview.find('> .' + serviceType).removeClass('hide')).trigger('sortupdate');
          }

          $preview.children('.last').removeClass('last');
          $preview.children(':visible:last').addClass('last');
        }).trigger('change');

        $preview.sortable().bind('sortstart', function(e, ui) {
          var $dragging = $(e.target);
          var $placeholder = $('.sortable-placeholder');
          var $style = $preview.children('style');
          $dragging.removeClass('first');
          var css = '.ui-sortable-placeholder { background-color: ' + $dragging.css('background-color') + '; visibility: visible; }';

          if (!$style.length) {
            $style = $('<style />').appendTo($preview);
          }

          $style.html(css);
        }).bind('sortupdate', function(e, ui) {
          var $this = $(this);
          var $weightFields = $('input[name$="[weight]"]');
          var $enabledServices = $preview.children(':not(.hide)');
          var enabledServicesLength = $enabledServices.length - 1;

          $weightFields.each(function() {
            var $this = $(this);
            var nameParts = $this.attr('name').split('][')
            var serviceType = nameParts[nameParts.length - 2];
            var $fakeLink = $preview.children('.' + serviceType);
            var index = $enabledServices.index($fakeLink);

            if (index === -1) {
              index = enabledServicesLength + 1;
              enabledServicesLength++;
            }

            $this.val(index);

            $preview.children('.last').removeClass('last');
            $preview.children(':visible:last').addClass('last');
          });
        }).trigger('sortupdate');

        var $customLabels = $('input[name$="[custom]"]', this);

        // Override text of "Custom" radio label when custom label is used.
        $customLabels.each(function() {
          var $this = $(this);
          var value = $this.val();
          var $customRadio = $this.parents('.fieldset-wrapper').find('[value="custom"]');
          var $customRadioLabel = $customRadio.siblings().find('span > span');
          var nameParts = $customRadio.attr('name').split('][');
          var serviceType = nameParts[nameParts.length - 2];
          var $previewFakeLink = $preview.children('.' + serviceType);

          value = Drupal.behaviors.minimalShareAdmin.replaceCountToken(value, $this);
          value = Drupal.behaviors.minimalShareAdmin.replaceIconToken(value, $this);

          if ($customRadio.attr('checked') && value) {
            $previewFakeLink.html(value);
            $customRadioLabel.data('orig-title', $customRadioLabel.text());
            $customRadioLabel.html(value);
          }
        });

        // Update "Custom" radio label when custom label is changed.
        $customLabels.keyup(function(e) {
          var $this = $(this);
          var value = $this.val();
          var $customRadio = $this.parents('.fieldset-wrapper').find('[value="custom"]');
          var $customRadioLabel = $customRadio.siblings().find('span > span');
          var nameParts = $customRadio.attr('name').split('][');
          var serviceType = nameParts[nameParts.length - 2];
          var $previewFakeLink = $preview.children('.' + serviceType);

          value = Drupal.behaviors.minimalShareAdmin.replaceCountToken(value, $this);
          value = Drupal.behaviors.minimalShareAdmin.replaceIconToken(value, $this);

          if (value && !$customRadioLabel.data('orig-title')) {
            $customRadioLabel.data('orig-title', $customRadioLabel.text());
          }

          if (value) {
            $customRadioLabel.html(value);
            $previewFakeLink.html(value);
          }
          else {
            $customRadioLabel.text($customRadioLabel.data('orig-title'));
            $previewFakeLink.text($customRadioLabel.data('orig-title'));
          }
        });
      });
    },
    /**
     * Replace [count] token for label types.
     */
    replaceCountToken: function(value, $elm) {
      if (value.indexOf('[count]') !== -1) {
        var $countRadio = $elm.parents('.fieldset-wrapper').find('[value="name_count"]');

        if (!$countRadio.length) {
          return value;
        }

        var $countRadioLabel = $countRadio.siblings().children('span');
        var count = $countRadioLabel.text().match(/\d+/);

        value = value.replace(/\[count\]/g, count);

      }

      return value;
    },
    /**
     * Replace [icon] token for label types.
     */
    replaceIconToken: function(value, $elm) {
      if (value.indexOf('[icon]') !== -1) {
        var service = $elm.attr('id').replace('edit-minimal-share-services-', '').replace('-custom', '');

        if (typeof Drupal.settings.minimalShare.unicodes[service] === 'undefined') {
          return value;
        }

        value = value.replace(/\[icon\]/g, '<span class="icon">' + Drupal.settings.minimalShare.unicodes[service]) + '</span>';
      }

      return value;
    }
  };

})(jQuery);
