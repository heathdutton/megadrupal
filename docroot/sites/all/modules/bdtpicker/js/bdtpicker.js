/**
 * @file
 * Bootstrap Date & Time picker integration.
 */

(function ($, Drupal) {
  'use strict';
  Drupal.behaviors.bdtpicker = {
    attach: function (context, settings) {
      if (typeof $().datetimepicker === 'undefined') {
        return;
      }

      var languageCode = Drupal.moment.getInterfaceLanguageCode();
      var settingsExists = settings.hasOwnProperty('bdtpicker') && settings.bdtpicker.hasOwnProperty('instances');

      $('input.bdtpicker[type="text"][data-date-format][data-date-format!=""]').once('bdtpicker', function () {
        var id = $(this).attr('id');
        var iSettingsExists = settingsExists && settings.bdtpicker.instances.hasOwnProperty(id);
        var options = (iSettingsExists && settings.bdtpicker.instances[id].hasOwnProperty('options') ?
          settings.bdtpicker.instances[id].options : {}
        );
        var $fieldWrapper = $(this).parents('.field-type-datetime');

        options.language = languageCode;

        $(this)
          .attr('autocomplete', 'off')
          .datetimepicker(options);

        if (iSettingsExists) {
          if (settings.bdtpicker.instances[id].hasOwnProperty('chain')) {
            $(this).on(
              'dp.change',
              null,
              {'instance': settings.bdtpicker.instances[id]},
              Drupal.bdtpicker.picker.onChange.chain
            );
          }

          if (settings.bdtpicker.instances[id].hasOwnProperty('next')) {
            $(this).on(
              'dp.hide',
              null,
              {'instance': settings.bdtpicker.instances[id]},
              Drupal.bdtpicker.picker.onHide.next
            );
          }

          if (settings.bdtpicker.instances[id].hasOwnProperty('defaultDate')) {
            $(this).on(
              'dp.show',
              null,
              {'instance': settings.bdtpicker.instances[id]},
              Drupal.bdtpicker.picker.onShow.defaultDate
            );
          }
        }

        if ($fieldWrapper.length) {
          var name = $(this).attr('name').replace(/\[(value|value2)\]\[date\]$/, '[all_day]');
          var $allDay = $('input[type="checkbox"][name="' + name + '"]', $fieldWrapper);

          $allDay.once('bdtpicker-allDay', function () {
            $(this).change(Drupal.bdtpicker.allDay.onChange.refreshPicker);

            if ($(this).is(':checked')) {
              Drupal.bdtpicker.allDay.refreshPicker(true, $fieldWrapper);
            }
          });
        }
      });
    }
  };

  Drupal.bdtpicker = Drupal.bdtpicker || {};

  Drupal.bdtpicker.picker = Drupal.bdtpicker.picker || {};
  Drupal.bdtpicker.picker.onHide = Drupal.bdtpicker.picker.onHide || {};
  Drupal.bdtpicker.picker.onChange = Drupal.bdtpicker.picker.onChange || {};
  Drupal.bdtpicker.picker.onShow = Drupal.bdtpicker.picker.onShow || {};

  Drupal.bdtpicker.picker.onHide.next = function (e) {
    var $next = $('input[name="' + e.data.instance.next.name + '"]:visible', $(this).parents('form'));

    if ($next.length === 0) {
      return;
    }

    var nextPicker = $next.data('DateTimePicker');

    if (typeof nextPicker === 'undefined') {
      return;
    }

    if ($(this).val() !== '' && ($next.val() === '' || nextPicker.bdtpickerChanged)) {
      nextPicker.bdtpickerChanged = false;
      $next.focus();
    }
  };

  Drupal.bdtpicker.picker.onChange.chain = function (e) {
    var $form = $(this).parents('form');
    var name;
    var $pairData;

    for (name in e.data.instance.chain) {
      if (e.data.instance.chain.hasOwnProperty(name)) {
        $pairData = $('input[name="' + name + '"]', $form).data('DateTimePicker');

        if (e.data.instance.chain[name] === 'previous') {
          $pairData.setMaxDate(e.date);
          if ($pairData.getDate() > e.date) {
            $pairData.bdtpickerChanged = true;
            $pairData.setDate(e.date);
          }
        }
        else if (e.data.instance.chain[name] === 'next') {
          $pairData.setMinDate(e.date);
          if ($pairData.getDate() < e.date) {
            $pairData.bdtpickerChanged = true;
            $pairData.setDate(e.date);
          }
        }
      }
    }
  };

  Drupal.bdtpicker.picker.onShow.defaultDate = function (e) {
    if ($(this).val() === '') {
      $(this).data('DateTimePicker').setDate(e.data.instance.defaultDate);
    }
  };

  Drupal.bdtpicker.picker.instanceProperties = function (id) {
    var settingsExists = (
      Drupal.settings.hasOwnProperty('bdtpicker')
      && Drupal.settings.bdtpicker.hasOwnProperty('instances')
      && Drupal.settings.bdtpicker.instances.hasOwnProperty(id)
    );

    return settingsExists ? Drupal.settings.bdtpicker.instances[id] : {};
  };

  Drupal.bdtpicker.allDay = Drupal.bdtpicker.allDay || {};
  Drupal.bdtpicker.allDay.onChange = Drupal.bdtpicker.allDay.onChange || {};

  Drupal.bdtpicker.allDay.onChange.refreshPicker = function () {
    Drupal.bdtpicker.allDay.refreshPicker(
      $(this).is(':checked'),
      $(this).parents('.field-type-datetime')
    );
  };

  Drupal.bdtpicker.allDay.refreshPicker = function (isAllDay, $fieldWrapper) {
    $('input[type="text"].bdtpicker', $fieldWrapper).each(function () {
      var properties = Drupal.bdtpicker.picker.instanceProperties($(this).attr('id'));
      var picker = $(this).data('DateTimePicker');
      var format = $(this).attr('data-date-format');

      properties.options = properties.options || {};

      if (isAllDay) {
        format = Drupal.moment.dateLimitFormatDate(format);
      }

      if (typeof picker === 'undefined') {
        properties.options.format = format;
      }
      else {
        picker.options.format = format;
        picker.format = picker.options.format;
        picker.setValue(picker.getDate());
      }
    });
  };

}(jQuery, Drupal));
